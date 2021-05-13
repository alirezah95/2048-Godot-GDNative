#include "Global.hpp"

using namespace godot;


Global* Global::g = nullptr;


void Global::_register_methods() 
{
    register_method("_ready", &Global::_ready);

    return;
}

void Global::_init()
{
}

void Global::_ready() 
{
    if (g == nullptr) {
        g = this;
    }

    m_loader = ResourceLoader::get_singleton();

    String path = "res://Assets/Gfx/{0} Tile.png";
    int texture_number = 2;
    for (Ref<Texture> &ref_tex: m_number_textures){
        ref_tex = Ref<Texture>(Object::cast_to<Texture>(*m_loader->load(
            path.format(Array::make(
                String::num_int64(texture_number))))));
        texture_number *= 2;
    }

    return;
}

