#include "Board.hpp"
#include "MatrixIndex.cpp"
#include "Global.hpp"

#include <time.h>

#include <Panel.hpp>
#include <Position2D.hpp>
#include <ResourceLoader.hpp>
#include <Texture.hpp>
#include <TextureRect.hpp>
#include <InputEventScreenTouch.hpp>
#include <InputEventScreenDrag.hpp>
#include <Timer.hpp>

using namespace godot;
using std::vector;



void Board::_register_methods()
{
    register_method("_ready", &Board::_ready);
    register_method("_input", &Board::_input);
    register_method("set_board_size", &Board::set_board_size);
    register_method("get_board_size", &Board::get_board_size);
    register_method("create_random_tile_with_num",
        &Board::create_random_tile_with_num);
    register_method("_on_ToIdleTmr_timeout", &Board::_on_ToIdleTmr_timeout);

    return;
}

void Board::_init()
{
    m_board_size = Vector2(4, 4);
    return;
}


void Board::_ready()
{
    m_to_idle_delay = get_node<Timer>("ToIdleTmr");

    Global::g->m_curr_board = this;
    m_rand_gen = Ref<RandomNumberGenerator>(RandomNumberGenerator::_new());

    m_board_mat.resize(m_board_size.y);
    for (auto &vec: m_board_mat) {
        vec.resize(m_board_size.x);
    }

    for (auto &vec: m_board_mat)
        for (auto &node: vec)
            node = nullptr;
    print_board_matrix();

    m_bg_tiles_root = get_node<Node>("BgTiles");
    m_num_tiles_root = get_node<Node>("NumTiles");

    m_bg_tile_scn = ResourceLoader::get_singleton()->load(
        "res://Scenes/BgTile.tscn", "PackedScene"
    );
    m_num_tile_scn = ResourceLoader::get_singleton()->load(
        "res://Scenes/NumberTile.tscn", "PackedScene"
    );
    
    if (m_bg_tile_scn.is_null()) {
        Godot::print("Null bg_tile_resource");
    } else {
        // Instancing background tiles.
        m_tile_start_pos = get_node<Position2D>(
            "TilesStartPos")->get_position();

        Vector2 tile_current_positino = m_tile_start_pos + Vector2(4, 4);
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                Panel* new_bg = Object::cast_to<Panel>(
                    m_bg_tile_scn->instance());
                new_bg->set_position(tile_current_positino);
                m_bg_tiles_root->add_child(new_bg);
                tile_current_positino.x += 128 + 3;
            }
            tile_current_positino.x = m_tile_start_pos.x + 3;
            tile_current_positino.y += 128 + 4;
        }
        create_random_tile_with_num(0);
        call_deferred("create_random_tile_with_num", 0);
    }

    return;
}



void Board::set_board_size(Vector2 __board_size)
{
    m_board_size = __board_size;
    return;
}


Vector2 Board::get_board_size() const
{
    return m_board_size;
}

void Board::decrease_moving_tiles() 
{
    m_moving_tiles -= 1;
    if (m_moving_tiles == 0) {
        change_state_to(BoardState::UPDATE_BOARD);
    }
    return;
}

void Board::create_random_tile_with_num(int which_num) 
{
    if (m_rand_gen.is_null()) {
        Godot::print_error("Rand gen is null", "create_random_tile_with_num",
            "Board.cpp", 115);
        return;
    }

    /* When a new random tile is going to be generated, the board empty indexes
     * must be found first and then a tile be generated at a random index which
     * is empty
     */
    vector<MatrixIndex> empty_cells;
    /* Reserve memory for performance sake. Maximum empty cells are 
     * (m_board_size.x * m_board_size.y)
    */
    empty_cells.reserve(m_board_size.x * m_board_size.y);
    for (int row = 0; row < m_board_size.x; ++row) {
        for (int col = 0; col < m_board_size.y; ++col) {
            if (m_board_mat[row][col] == nullptr) {
                empty_cells.push_back(MatrixIndex(row, col));
            }
        }
    }

    m_rand_gen->randomize();
    MatrixIndex new_tile_indx = 
            empty_cells[m_rand_gen->randi() % empty_cells.size()];
    // Godot::print("Create random tile at ({0}, {1})", new_tile_indx.row, new_tile_indx.col);
    create_num_tile_at_index(new_tile_indx, which_num);

    return;
}

void Board::create_num_tile_at_index(const MatrixIndex& index, int which_num)
{
    auto new_num_tile = Object::cast_to<NumberTile>(m_num_tile_scn->instance());

    m_board_mat[index.row][index.col] = new_num_tile;

    new_num_tile->set_position(
        Object::cast_to<Panel>(m_bg_tiles_root->get_child(
            index.row * m_board_size.y + index.col))
            ->get_position());
    new_num_tile->set_num_log_2(which_num);
    new_num_tile->set_index(index);

    m_num_tiles_root->add_child(new_num_tile);
    return;
}

void Board::_input(const Ref<InputEvent> event)
{
    if (Object::cast_to<InputEventScreenTouch>(*event) != nullptr) {
        auto c_event = Object::cast_to<InputEventScreenTouch>(*event);
        switch (m_curr_board_st) {
        case BoardState::IDLE:
            if (c_event->is_pressed()) {
                m_touch_indx = c_event->get_index();
                change_state_to(BoardState::TOUCH);
            }
        case BoardState::TOUCH:
            if (c_event->is_pressed() == false)
                change_state_to(BoardState::IDLE);
        }
    }

    if (Object::cast_to<InputEventScreenDrag>(*event) != nullptr) {
        auto c_event = Object::cast_to<InputEventScreenDrag>(*event);
        if (m_curr_board_st == BoardState::TOUCH) {
            m_total_drag += c_event->get_relative();
            if (m_total_drag.length_squared() > c_DRAG_THRESHOLD) {
                change_state_to(BoardState::SWIP);
            }
        }
    }
}

void Board::change_state_to(BoardState __new_state)
{
    m_curr_board_st = __new_state;

    switch (m_curr_board_st) {
    case BoardState::IDLE:
        m_touch_indx = -1;
        m_total_drag = Vector2(0, 0);
        m_moving_tiles = 0;
        m_tiles_to_update.clear();
        m_tiles_to_free.clear();
        break;
    case BoardState::TOUCH:
        break;
    case BoardState::SWIP:
        if (fabs(m_total_drag.y) > fabs(m_total_drag.x)) {
            /* Swip down or up */
            if (m_total_drag.y > 0.0)
                swipe_down();
            else
                swipe_up();
        } else {
            /* Swip right or left */
            if (m_total_drag.x > 0.0)
                swipe_right();
            else
                swipe_left();
        }
        print_board_matrix();
        break;
    case BoardState::UPDATE_BOARD:
        create_random_tile_with_num(0);

        for (auto &item: m_tiles_to_free)
            item->queue_free();
        
        if (m_tiles_to_update.size() == 0) {
            change_state_to(BoardState::IDLE);
        } else {
            for (auto &item: m_tiles_to_update)
                item->update_texture(true);
            /* Starts a delay timer to reset board to idle */
            m_to_idle_delay->start();
        }
        break;
    }

    return;
}

void Board::swipe_up() 
{
    // Godot::print("Swip up");
    for (int col = 0; col < m_board_size.x; ++col) {
        for (int row = 1; row < m_board_size.y; ++row) {
            if (m_board_mat[row][col] != nullptr) {
                int move_row = row;
                while (move_row > 0) {
                    if (m_board_mat[move_row - 1][col] == nullptr) {
                        move_row -= 1;
                    } else if (m_board_mat[row][col]->get_number()
                        == m_board_mat[move_row - 1][col]->get_number()) {
                        move_row -= 1;
                        break;
                    } else
                        break;
                } // end while

                if (move_row == row) {
                    /* This item cannot move up. */
                    continue;
                } else {
                    m_moving_tiles += 1;
                    set_item_move_to_indx(MatrixIndex(row, col),
                        MatrixIndex(move_row, col));
                }
            }
        }
    }

    if (m_moving_tiles == 0) {
        // Godot::print("No item to move");
        change_state_to(BoardState::IDLE);
    }

    return;
}

void Board::swipe_down() 
{
    // Godot::print("Swip down");
    for (int col = 0; col < m_board_size.x; ++col) {
        for (int row = m_board_size.y - 2; row >= 0; --row) {
            if (m_board_mat[row][col] != nullptr) {
                int move_row = row;
                while (move_row < m_board_size.y - 1) {
                    if (m_board_mat[move_row + 1][col] == nullptr) {
                        move_row += 1;
                    } else if (m_board_mat[row][col]->get_number()
                        == m_board_mat[move_row + 1][col]->get_number()) {
                        move_row += 1;
                        break;
                    } else
                        break;
                } // end while
                
                if (move_row == row) {
                    /* This item cannot move up. */
                    continue;
                } else {
                    m_moving_tiles += 1;
                    set_item_move_to_indx(MatrixIndex(row, col),
                        MatrixIndex(move_row, col));
                }
            }
        }
    }

    if (m_moving_tiles == 0) {
        // Godot::print("No item to move");
        change_state_to(BoardState::IDLE);
    }

    return;
}

void Board::swipe_right() 
{
    // Godot::print("Swip right");
    for (int row = 0; row < m_board_size.y; ++row) {
        for (int col = m_board_size.x - 2; col >= 0; --col) {
            if (m_board_mat[row][col] != nullptr) {
                int move_col = col;
                while (move_col < m_board_size.x - 1) {
                    if (m_board_mat[row][move_col + 1] == nullptr) {
                        move_col += 1;
                    } else if (m_board_mat[row][col]->get_number()
                        == m_board_mat[row][move_col + 1]->get_number()) {
                        move_col += 1;
                        break;
                    } else
                        break;
                } // end while

                if (move_col == col) {
                    /* This item cannot move up. */
                    continue;
                } else {
                    m_moving_tiles += 1;
                    set_item_move_to_indx(MatrixIndex(row, col),
                        MatrixIndex(row, move_col));
                }
            }
        }
    }

    if (m_moving_tiles == 0) {
        change_state_to(BoardState::IDLE);
    }

    return;
}

void Board::swipe_left()
{
    for (int row = 0; row < m_board_size.y; ++row) {
        for (int col = 1; col < m_board_size.x; ++col) {
            if (m_board_mat[row][col] != nullptr) {
                int move_col = col;
                while (move_col > 0) {
                    if (m_board_mat[row][move_col - 1] == nullptr) {
                        move_col -= 1;
                    } else if (m_board_mat[row][col]->get_number()
                        == m_board_mat[row][move_col - 1]->get_number()) {
                        move_col -= 1;
                        break;
                    } else
                        break;
                } // end while

                if (move_col == col) {
                    /* This item cannot move up. */
                    continue;
                } else {
                    m_moving_tiles += 1;
                    set_item_move_to_indx(MatrixIndex(row, col),
                        MatrixIndex(row, move_col));
                }
            }
        }
    }

    if (m_moving_tiles == 0) {
        change_state_to(BoardState::IDLE);
    }

    return;
}

void Board::set_item_move_to_indx(const MatrixIndex &indx,
        const MatrixIndex &to_indx) 
{
    NumberTile *item = m_board_mat[indx.row][indx.col];
    NumberTile *to_item = m_board_mat[to_indx.row][to_indx.col];

    m_board_mat[indx.row][indx.col] = nullptr;
    item->move_to_index(to_indx, Object::cast_to<Panel>(
        m_bg_tiles_root->get_child(
            to_indx.row * m_board_size.y + to_indx.col))
                ->get_position());
    if (to_item == nullptr) {
        m_board_mat[to_indx.row][to_indx.col] = item;
    } else if (to_item->get_number() == item->get_number()) {
        m_tiles_to_free.push_back(item);
        m_tiles_to_update.push_back(to_item);
        to_item->queue_num_log_2_update(to_item->get_num_log_2() + 1);

    } /* Else item should not be moved */

    return;
}

void Board::print_board_matrix() 
{
    return;
    String b = "";
    for (auto &vec: m_board_mat){
        for (int i = 0; i < vec.size(); i++) {
            if (i == vec.size() - 1)
                if (vec[i] == nullptr)
                    b += "NULL ]";
                else
                    b += vec[i]->get_name() + " ]";
            else if (i == 0)
                if (vec[i] == nullptr)
                    b += "[ NULL, ";
                else
                    b += "[ " + vec[i]->get_name() + ", ";
            else
                if (vec[i] == nullptr)
                    b += "NULL, ";
                else
                    b += vec[i]->get_name() + ", ";
        }
        b += "\n";
    }

    Godot::print(b);

    return;
}


void Board::_on_ToIdleTmr_timeout() 
{
    change_state_to(BoardState::IDLE);
    return;
}