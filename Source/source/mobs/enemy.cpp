/*
 * Copyright (c) Andre 'Espyo' Silva 2013-2016.
 * The following source file belongs to the open-source project
 * Pikmin fangame engine. Please read the included
 * README and LICENSE files for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Enemy class and enemy-related functions.
 */

#include "../drawing.h"
#include "enemy.h"
#include "../functions.h"

/* ----------------------------------------------------------------------------
 * Creates an enemy.
 */
enemy::enemy(const float x, const float y, enemy_type* type, const float angle, const string &vars) :
    mob(x, y, type, angle, vars),
    ene_type(type),
    spawn_delay(s2f(get_var_value(vars, "spawn_delay", "0"))),
    respawn_days_left(0),
    respawns_after_x_days(0),
    appears_after_day(0),
    appears_before_day(0),
    appears_every_x_days(0) {
    
    team = MOB_TEAM_ENEMY_1; //TODO removeish.
    
    //TODO day apperance interval
}


void enemy::draw() {
    frame* f_ptr = anim.get_frame();
    if(!f_ptr) return;
    
    float draw_x, draw_y;
    float draw_w, draw_h;
    get_sprite_center(this, f_ptr, &draw_x, &draw_y);
    get_sprite_dimensions(this, f_ptr, &draw_w, &draw_h);
    
    float radius_scale = 1.0f;
    
    bool being_delivered = false;
    ALLEGRO_COLOR extra_color;
    
    if(fsm.cur_state->id == ENEMY_EXTRA_STATE_BEING_DELIVERED) {
        //If it's being delivered, do some changes to the scale and coloring.
        being_delivered = true;
        
        if(script_timer.get_ratio_left() >= 0.5) {
            //First half of the sucking in process = interpolated coloring.
            extra_color = interpolate_color(
                              script_timer.get_ratio_left(),
                              0.5, 1.0,
                              ((onion*) carrying_target)->oni_type->pik_type->main_color,
                              al_map_rgb(0, 0, 0)
                          );
        } else {
            //Second half of the sucking in process = interpolated scaling.
            extra_color = ((onion*) carrying_target)->oni_type->pik_type->main_color;
            radius_scale = (script_timer.get_ratio_left() * 2.0);
        }
    }
    
    draw_sprite(
        f_ptr->bitmap,
        draw_x, draw_y,
        draw_w * radius_scale,
        draw_h * radius_scale,
        angle,
        map_gray(get_sprite_lighting(this))
    );
    
    if(being_delivered) {
        int old_op, old_src, old_dst, old_aop, old_asrc, old_adst;
        al_get_separate_blender(&old_op, &old_src, &old_dst, &old_aop, &old_asrc, &old_adst);
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);
        
        draw_sprite(
            f_ptr->bitmap,
            draw_x, draw_y,
            draw_w * radius_scale,
            draw_h * radius_scale,
            angle,
            extra_color
        );
        
        al_set_separate_blender(old_op, old_src, old_dst, old_aop, old_asrc, old_adst);
    }
}