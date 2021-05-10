#ifndef GAME_HPP
#define GAME_HPP

#include <Node2D.hpp>
#include <Godot.hpp>

namespace godot
{
    class Game: public Node2D
    {
        GODOT_CLASS(Game, Node2D)

        private:

        public:
            static void _register_methods();
            /* init method */
            void _init();

            void _ready();
    };
}; // namespace godot

#endif