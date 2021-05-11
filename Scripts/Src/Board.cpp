#include "Board.hpp"
#include <ResourceLoader.hpp>
#include <Position2D.hpp>
#include <Panel.hpp>

using namespace godot;

void Board::_register_methods()
{
    register_method("_ready", &Board::_ready);
}


void Board::_init()
{
}


void Board::_ready()
{
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
                add_child(new_bg);
                tile_current_positino.x += 128 + 3;
            }
            tile_current_positino.x = m_tile_start_pos.x + 3;
            tile_current_positino.y += 128 + 4;
        }
    }

    return;
}