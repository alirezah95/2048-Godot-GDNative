#ifndef BOARD_HPP
#define BOARD_HPP

#include <Godot.hpp>
#include <Node2D.hpp>
#include <PackedScene.hpp>
#include <vector>
#include <InputEvent.hpp>

using std::vector;

namespace godot
{
    class PackedScene;
    class Panel;
    class MatrixIndex;
    class Global;
    class Board : public Node2D
    {
        GODOT_CLASS(Board, Node2D);

    private:
        Ref<PackedScene> m_bg_tile_scn;
        Vector2 m_tile_start_pos;
        Vector2 m_board_size;
        vector<vector<Object *>> m_board_mat;
        Node *m_bg_tiles_root;
        Node *m_num_tiles_root;
        /* A reference to Global autoload in scene tree*/
        Global* m_global;

    private:
        void create_num_tile_at_index(const MatrixIndex &index, int which_num);
    public:
        static void _register_methods();

        void _init();

        void _ready();

        void _input(const Ref<InputEvent> event);

        /* m_board_size setter and getter*/
        void set_board_size(Vector2 __board_size);
        Vector2 get_board_size() const;
    };
}; // namespace godot

#endif