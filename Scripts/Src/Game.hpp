#ifndef GAME_HPP
#define GAME_HPP

#include <Node2D.hpp>
#include <Godot.hpp>

namespace godot
{
    class Label;
    class Game: public Node2D
    {
        GODOT_CLASS(Game, Node2D)

        private:
            Label *m_points_lbl;
            Label *m_best_lbl;
            Label *m_time_lbl;
            long m_player_points = 0;
            long m_player_best = 0;
            uint8_t m_game_minute = 0;
            uint8_t m_game_seconds = 0;

        public:
            static void _register_methods();
            /* init method */
            void _init();

            void _ready();

            void increase_points_by(uint _points);

            void update_points_lbl();

            /* Game timer funtion that connects to time_out signal */
            void _on_GameTimer_timeout();
    };
}; // namespace godot

#endif