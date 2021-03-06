#ifndef __NUMBERTILE_H__
#define __NUMBERTILE_H__

#include "MatrixIndex.cpp"

#include <Godot.hpp>
#include <TextureRect.hpp>


namespace godot
{
    class AnimationPlayer;

    class NumberTile : public TextureRect
    {
        GODOT_CLASS(NumberTile, TextureRect);
    
    private:
        AnimationPlayer *m_update_anima;
        MatrixIndex m_index;
        MatrixIndex m_target_indx;
        Vector2 m_target_pos;
        Vector2 m_move_sp;
        int m_frame_delay = 1;
        long m_number;
        /* Holds the log2 of m_number variable, to avoid calculations */
        uint8_t m_num_log_2;
        /* Holds if an update texure/nubmer is queued to happen. */
        bool m_is_update_queued = false;
    
    public:
        static void _register_methods();

        void _init();

        void _ready();

        void _process(const float delta);

        void set_num_log_2(int _num_log_2, bool _with_animation = false);

        uint8_t get_num_log_2() const;

        void queue_num_log_2_update(int _num_log_2);

        /* Shows if item is queued for update so the nubmer is not correct now*/
        bool is_queue_for_update();

        void update_texture(bool _with_animation = false);

        long get_number() const;

        void set_mat_index(const MatrixIndex &_index);

        MatrixIndex get_mat_index() const;

        void move_to_index(const MatrixIndex &_to_index,
                Vector2 _target_position);

    };
};

#endif // __NUMBERTILE_H__