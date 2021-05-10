#include "Game.hpp"

using namespace godot;


void Game::_register_methods()
{
    register_method("_ready", &Game::_ready);
}


void Game::_init()
{
}


void Game::_ready()
{
    Godot::print("_ready from Game class.");

    return;
}