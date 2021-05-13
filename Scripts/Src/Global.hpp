#ifndef __GLOBAL_H__
#define __GLOBAL_H__


#include <Node.hpp>
#include <Godot.hpp>
#include <Texture.hpp>
#include <ResourceLoader.hpp>
#include <array>


using std::array;


namespace godot
{
    class Board;

    class Global : public Node
    {
        GODOT_CLASS(Global, Node)

    private:
        ResourceLoader* m_loader;
    
    public:
        array<Ref<Texture>, 16> m_number_textures;
        Board* m_curr_board = nullptr;

    public:
        static Global* g;

    public:
        static void _register_methods();

        void _init();

        void _ready();

    };
    
};


#endif // __GLOBAL_H__