#include "Board.hpp"
#include "MatrixIndex.cpp"
#include "Global.cpp"

#include <time.h>

#include <Panel.hpp>
#include <Position2D.hpp>
#include <ResourceLoader.hpp>
#include <Texture.hpp>
#include <TextureRect.hpp>
#include <InputEventScreenTouch.hpp>
#include <InputEventScreenDrag.hpp>

using namespace godot;
using std::vector;

void Board::_register_methods()
{
    register_method("_ready", &Board::_ready);
    register_method("_input", &Board::_input);
    register_method("set_board_size", &Board::set_board_size);
    register_method("get_board_size", &Board::get_board_size);

    return;
}

void Board::_init()
{
    m_board_size = Vector2(4, 4);
    return;
}

void Board::_ready()
{
    /* Get Global autoload reference. */
    m_global = Object::cast_to<Global>(get_node("/root/Global"));

    m_board_mat.resize(m_board_size.y);
    for (vector<Object*> &vec: m_board_mat) {
        vec.resize(m_board_size.x);
    }

    m_bg_tiles_root = get_node<Node>("BgTiles");
    m_num_tiles_root = get_node<Node>("NumTiles");

    m_bg_tile_scn = ResourceLoader::get_singleton()->load(
        "res://Scenes/BgTile.tscn", "PackedScene");
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

        MatrixIndex first_num_tile_index;
        srand(time(nullptr));
        srand(rand());
        int rnd = rand();
        first_num_tile_index.row = rnd % (int)m_board_size.y;
        srand(rnd);
        first_num_tile_index.col = rand() % (int)m_board_size.x;
        create_num_tile_at_index(first_num_tile_index, 2);
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

void Board::create_num_tile_at_index(const MatrixIndex& index, int which_num)
{
    auto new_num_tile = TextureRect::_new();
    if (m_global != nullptr)
        new_num_tile->set_texture(m_global->m_number_textures[which_num]);
    
    m_board_mat[index.row][index.col] = new_num_tile;

    new_num_tile->set_position(
        Object::cast_to<Panel>(m_bg_tiles_root->get_child(
            index.row * m_board_size.y + index.col))
            ->get_position());
    m_num_tiles_root->add_child(new_num_tile);

    return;
}

void Board::_input(const Ref<InputEvent> event)
{
    if (Object::cast_to<InputEventScreenTouch>(*event) != nullptr){
        auto c_event = Object::cast_to<InputEventScreenTouch>(*event);
        Godot::print(c_event->get_position());
    }
}