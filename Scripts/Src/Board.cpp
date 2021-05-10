#include "Board.hpp"

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
    Godot::print("_ready from Board class.");

    return;
}