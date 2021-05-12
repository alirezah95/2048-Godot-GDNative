#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Node.hpp>
#include <Godot.hpp>
#include <Texture.hpp>
#include <ResourceLoader.hpp>
#include <String.hpp>
#include <array>


using std::array;


namespace godot
{
    class Global : public Node
    {
        GODOT_CLASS(Global, Node);

    private:
        ResourceLoader* m_loader;
    
    public:
        array<Ref<Texture>, 16> m_number_textures;

    public:
        static void _register_methods()
        {
            register_method("_ready", &Global::_ready);

            return;
        }

        void _init() {};

        void _ready()
        {
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

    };
    
};



#endif // __GLOBAL_H__