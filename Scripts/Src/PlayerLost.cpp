#ifndef __PLYAER_LOST_H_
#define __PLAYER_LOST_H_

#include <Godot.hpp>
#include <CenterContainer.hpp>
#include <SceneTree.hpp>


namespace godot
{
    class PlayerLost : public CenterContainer
    {
        GODOT_CLASS(PlayerLost, CenterContainer);
    
    public:
        static void _register_methods()
        {
            register_method("_on_Buttons_pressed",
                &PlayerLost::_on_Buttons_pressed);
        };

        void _init()
        {
        };

        void _on_Buttons_pressed(int _close_or_restart)
        {
            if (_close_or_restart == 0) {
                get_tree()->quit();
            } else if (_close_or_restart == 1) {
                if (get_tree()->is_paused()) {
                    get_tree()->set_pause(false);
                }
                get_tree()->reload_current_scene();
            }
            return;
        }
    };
};

#endif // __PLAYER_LOST_H_