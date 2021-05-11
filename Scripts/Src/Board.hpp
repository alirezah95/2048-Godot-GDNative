#ifndef BOARD_HPP
#define BOARD_HPP

#include <Node2D.hpp>
#include <Godot.hpp>
#include <PackedScene.hpp>

namespace godot
{
    class PackedScene;
    class Panel;
    class Board: public Node2D
    {
        GODOT_CLASS(Board, Node2D)
        
        private:
            Ref<PackedScene> m_bg_tile_scn;
            Vector2 m_tile_start_pos;
        public:
            static void _register_methods();

            void _init();

            void _ready();
    };
};

#endif