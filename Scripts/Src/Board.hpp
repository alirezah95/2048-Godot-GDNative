#ifndef BOARD_HPP
#define BOARD_HPP

#include "NumberTile.hpp"

#include <Godot.hpp>
#include <Node2D.hpp>
#include <PackedScene.hpp>
#include <vector>
#include <InputEvent.hpp>
#include <RandomNumberGenerator.hpp>

using std::vector;

namespace godot
{
    class PackedScene;
    class Panel;
    class MatrixIndex;
    class Global;

    /* Enum variable showing board stat */
    enum class BoardState: int
    {
        IDLE,
        TOUCH,
        SWIP,
        MOVE_ITEMS,
        UPDATE_BOARD
    };

    class Board : public Node2D
    {
        GODOT_CLASS(Board, Node2D);

    private:
        const float c_DRAG_THRESHOLD = 64. * 64.;
        Ref<PackedScene> m_bg_tile_scn;
        Ref<PackedScene> m_num_tile_scn;
        Vector2 m_tile_start_pos;
        Vector2 m_board_size;
        vector<vector<NumberTile*>> m_board_mat;
        Node *m_bg_tiles_root;
        Node *m_num_tiles_root;
        BoardState m_curr_board_st = BoardState::IDLE;
        /* Stores total drag by user, which shows which way to swip. */
        Vector2 m_total_drag = Vector2(0, -1);
        /* Holds the number of moving tiles, so to track if moving is finished*/
        int m_moving_tiles = 0;
        /* Stores which tiles to free inf UPDATE_BOARD state */
        vector<NumberTile*> m_tiles_to_free;
        /* Stroes which tiles to set to increase number in UPDATE_BOARD state */
        vector<NumberTile*> m_tiles_to_update;
        /* Stores touch index of first touch */
        int m_touch_indx = -1;
        /* A RandomNumberGenerator object to generate random numbers */
        Ref<RandomNumberGenerator> m_rand_gen;
        

    private:
        void create_num_tile_at_index(const MatrixIndex &index, int which_num);
        void change_state_to(BoardState __new_sate);
        void swipe_up();
        void swipe_down();
        void swipe_right();
        void swipe_left();
        void set_item_move_to_indx(const MatrixIndex &indx,
                const MatrixIndex &to_indx);
        void print_board_matrix(); /* For debug purposes. */

    public:
        static void _register_methods();

        void _init();

        void _ready();

        void _input(const Ref<InputEvent> event);

        /* m_board_size setter and getter*/
        void set_board_size(Vector2 __board_size);
        Vector2 get_board_size() const;

        void create_random_tile_with_num(int which_num);

        void decrease_moving_tiles();
    };
}; // namespace godot

#endif