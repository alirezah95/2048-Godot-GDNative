#include "Game.hpp"

#include <Label.hpp>

using namespace godot;


void Game::_register_methods()
{
    register_method("_ready", &Game::_ready);
    register_method("_on_GameTimer_timeout", &Game::_on_GameTimer_timeout);
}


void Game::_init()
{
}


void Game::_ready()
{
    m_points_lbl = get_node<Label>("MainUI/TopUIHBox/ScoreLbl");
    m_best_lbl = get_node<Label>("MainUI/TopUIHBox/BestLbl");
    m_time_lbl = get_node<Label>("MainUI/TopUIHBox/TimeLbl");

    update_points_lbl();
    _on_GameTimer_timeout();

    return;
}

void Game::increase_points_by(uint _points) 
{
    m_player_points += _points;
    update_points_lbl();
    return;
}

void Game::update_points_lbl() 
{
    m_points_lbl->set_text(String::num_int64(m_player_points));
    if (m_player_points > m_player_best) {
        m_player_best = m_player_points;
        m_best_lbl->set_text(String::num_int64(m_player_best));
    }
    return;
}

void Game::_on_GameTimer_timeout() 
{
    m_game_seconds += 1;
    if (m_game_seconds == 60) {
        m_game_minute++;
        m_game_seconds = 0;
    }
    String time_str = "{0}{1}:{2}{3}";
    m_time_lbl->set_text(time_str.format(Array::make(m_game_minute / 10,
            m_game_minute % 10, m_game_seconds / 10, m_game_seconds % 10)));
}