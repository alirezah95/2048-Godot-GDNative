#include "NumberTile.hpp"
#include "Board.hpp"
#include "Global.hpp"

#include <AnimationPlayer.hpp>



namespace godot
{
    void NumberTile::_register_methods() 
    {
        register_method("_ready", &NumberTile::_ready);
        register_method("_process", &NumberTile::_process);
        return;
    }
    
    void NumberTile::_init() {}
    
    void NumberTile::_ready() 
    {
        m_update_anima = get_node<AnimationPlayer>("UpdateNumberAnima");
        set_process(false);
    }
    
    void NumberTile::_process(const float delta) 
    {
        if (m_frame_delay > 0) {
            m_frame_delay -= 1;
            return;
        }

        Vector2 new_pos = get_position() + m_move_sp * delta;
        if (((m_target_pos.x - new_pos.x) * m_move_sp.x) < 0
                || ((m_target_pos.y - new_pos.y) * m_move_sp.y) < 0) {
            set_position(m_target_pos);
            m_index = m_target_indx;
            m_frame_delay = 1;
            
            Global::g->m_curr_board->decrease_moving_tiles();
            set_process(false);
            return;
        }
        set_position(new_pos);
    }

    void NumberTile::set_num_log_2(int _num_log_2, bool _with_animation)
    {
        m_num_log_2 = _num_log_2;
        m_number = (long)pow(2, m_num_log_2);

        update_texture();
        return;
    }

    uint8_t NumberTile::get_num_log_2() const
    {
        return m_num_log_2;
    }

    void NumberTile::queue_num_log_2_update(int _num_log_2)
    {
        m_num_log_2 = _num_log_2;
        m_number = (long)pow(2, m_num_log_2);
        m_is_update_queued = true;
        return;
    }
    
    bool NumberTile::is_queue_for_update() 
    {
        return m_is_update_queued;
    }

    void NumberTile::update_texture(bool _with_animation) 
    {
        set_texture(Global::g->m_number_textures[m_num_log_2 - 1]);
        if (_with_animation)
            m_update_anima->play("Update");
        
        m_is_update_queued = false;
        return;
    }

    long NumberTile::get_number() const
    {
        return m_number;
    }

    void NumberTile::set_mat_index(const MatrixIndex &_index)
    {
        m_index = _index;
        return;
    }

    MatrixIndex NumberTile::get_mat_index() const
    {
        return m_index;
    }

    void NumberTile::move_to_index(const MatrixIndex &_to_index,
            Vector2 _target_pos)
    {
        m_target_indx = _to_index;
        m_target_pos = _target_pos;

        // Godot::print("Set item ({0}, {1}) to move to index ({2}, {3})",
        //     m_index.row, m_index.col, m_target_indx.row, m_target_indx.col);
        
        m_move_sp = (m_target_pos - get_position()) * 8;
        set_process(true);
        return;
    }
    
};
