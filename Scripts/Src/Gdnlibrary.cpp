#include "Game.hpp"
#include "Board.hpp"
#include "Global.hpp"
#include "NumberTile.hpp"
#include "PlayerLost.cpp"

/** GDNative Initialize **/
extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
    godot::Godot::gdnative_init(o);
}

/** GDNative Terminate **/
extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
    godot::Godot::gdnative_terminate(o);
}

/** NativeScript Initialize **/
extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
    godot::Godot::nativescript_init(handle);

    godot::register_class<godot::Game>();
    godot::register_class<godot::Board>();
    godot::register_class<godot::Global>();
    godot::register_class<godot::NumberTile>();
    godot::register_class<godot::PlayerLost>();
}