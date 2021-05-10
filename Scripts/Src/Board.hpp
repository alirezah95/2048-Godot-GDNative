#ifndef BOARD_HPP
#define BOARD_HPP

#include <Node2D.hpp>
#include <Godot.hpp>

namespace godot
{
    class Board: public Node2D
    {
        GODOT_CLASS(Board, Node2D)
        
        private:

        public:
            static void _register_methods();

            void _init();

            void _ready();
    };
};

#endif