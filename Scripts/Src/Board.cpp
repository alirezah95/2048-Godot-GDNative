#include "Board.hpp"

#include <time.h>

#include <Panel.hpp>
#include <Position2D.hpp>
#include <ResourceLoader.hpp>
#include <Texture.hpp>
#include <TextureRect.hpp>

#include "MatrixIndex.cpp"

using namespace godot;

void Board::_register_methods()
{
    register_method("_ready", &Board::_ready);
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
    m_bg_tiles_root = get_node<Node>("BgTiles");
    m_num_tiles_root = get_node<Node>("NumTiles");

    m_bg_tile_scn = ResourceLoader::get_singleton()->load(
        "res://Scenes/BgTile.tscn", "PackedScene");
    if (m_bg_tile_scn.is_null()) {
        Godot::print("Null bg_tile_resource");
    } else {
        // Instancing background tiles.
        m_tile_start_pos = get_node<Position2D>("TilesStartPos")->get_position();

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
        first_num_tile_index.row = rand() % (int)m_board_size.y;
        srand(rand());
        first_num_tile_index.col = rand() % (int)m_board_size.x;
        create_num_tile_at_index(first_num_tile_index);
    }

    return;
}

void Board::create_num_tile_at_index(const MatrixIndex& index)
{
    auto new_num_tile = TextureRect::_new();
    new_num_tile->set_texture(
        ResourceLoader::get_singleton()->load("res://Assets/Gfx/2 Tile.png"));
    // m_board_mat[index.row][index.col] = new_num_tile;

    new_num_tile->set_position(
        Object::cast_to<Panel>(m_bg_tiles_root->get_child(
            index.row * m_board_size.y + index.col))
            ->get_position());
    m_num_tiles_root->add_child(new_num_tile);

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