/*
 * Copyright (c) Andre 'Espyo' Silva 2013-2016.
 * The following source file belongs to the open-source project
 * Pikmin fangame engine. Please read the included
 * README and LICENSE files for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Leader finite state machine logic.
 */

#include "../functions.h"
#include "leader.h"
#include "leader_fsm.h"
#include "leader_type.h"
#include "mob_fsm.h"
#include "../vars.h"

void leader_fsm::create_fsm(mob_type* typ) {
    easy_fsm_creator efc;
    efc.new_state("idle", LEADER_STATE_IDLE); {
        efc.new_event(MOB_EVENT_ON_ENTER); {
            efc.run_function(leader_fsm::enter_idle);
        }
        efc.new_event(MOB_EVENT_WHISTLED); {
            efc.run_function(leader_fsm::join_group);
            efc.change_state("in_group_chasing");
        }
        efc.new_event(LEADER_EVENT_FOCUSED); {
            efc.run_function(leader_fsm::focus);
            efc.change_state("active");
        }
        efc.new_event(MOB_EVENT_HITBOX_TOUCH_N_A); {
            efc.run_function(leader_fsm::inactive_lose_health);
        }
        efc.new_event(MOB_EVENT_DEATH); {
            efc.change_state("inactive_dying");
        }
        efc.new_event(LEADER_EVENT_INACTIVE_SEARCH_SEED); {
            efc.run_function(leader_fsm::inactive_search_seed);
        }
        efc.new_event(LEADER_EVENT_GO_PLUCK); {
            efc.run_function(leader_fsm::go_pluck);
            efc.change_state("inactive_going_to_pluck");
        }
        efc.new_event(MOB_EVENT_BOTTOMLESS_PIT); {
            efc.run_function(leader_fsm::fall_down_pit);
        }
    }
    
    efc.new_state("active", LEADER_STATE_ACTIVE); {
        efc.new_event(MOB_EVENT_ON_ENTER); {
            efc.run_function(leader_fsm::enter_active);
        }
        efc.new_event(LEADER_EVENT_UNFOCUSED); {
            efc.run_function(leader_fsm::stop);
            efc.run_function(leader_fsm::unfocus);
            efc.change_state("idle");
        }
        efc.new_event(LEADER_EVENT_MOVE_START); {
            efc.run_function(leader_fsm::move);
            efc.run_function(leader_fsm::set_walk_anim);
        }
        efc.new_event(LEADER_EVENT_MOVE_END); {
            efc.run_function(leader_fsm::stop);
            efc.run_function(leader_fsm::set_stop_anim);
        }
        efc.new_event(LEADER_EVENT_HOLDING); {
            efc.run_function(leader_fsm::grab_mob);
            efc.change_state("holding");
        }
        efc.new_event(LEADER_EVENT_START_WHISTLE); {
            efc.change_state("whistling");
        }
        efc.new_event(LEADER_EVENT_DISMISS); {
            efc.change_state("dismissing");
        }
        efc.new_event(LEADER_EVENT_SPRAY); {
            efc.change_state("spraying");
        }
        efc.new_event(LEADER_EVENT_LIE_DOWN); {
            efc.run_function(leader_fsm::fall_asleep);
            efc.change_state("sleeping_waiting");
        }
        efc.new_event(MOB_EVENT_HITBOX_TOUCH_N_A); {
            efc.run_function(leader_fsm::lose_health);
        }
        efc.new_event(MOB_EVENT_DEATH); {
            efc.change_state("dying");
        }
        efc.new_event(LEADER_EVENT_GO_PLUCK); {
            efc.run_function(leader_fsm::go_pluck);
            efc.change_state("going_to_pluck");
        }
        efc.new_event(MOB_EVENT_BOTTOMLESS_PIT); {
            efc.run_function(leader_fsm::fall_down_pit);
        }
    }
    
    efc.new_state("whistling", LEADER_STATE_WHISTLING); {
        efc.new_event(MOB_EVENT_ON_ENTER); {
            efc.run_function(leader_fsm::whistle);
        }
        efc.new_event(MOB_EVENT_ON_LEAVE); {
            efc.run_function(leader_fsm::stop_whistle);
        }
        efc.new_event(LEADER_EVENT_STOP_WHISTLE); {
            efc.change_state("active");
        }
        efc.new_event(MOB_EVENT_TIMER); {
            efc.change_state("active");
        }
        efc.new_event(LEADER_EVENT_MOVE_START); {
            efc.run_function(leader_fsm::move);
        }
        efc.new_event(LEADER_EVENT_MOVE_END); {
            efc.run_function(leader_fsm::stop);
        }
        efc.new_event(MOB_EVENT_HITBOX_TOUCH_N_A); {
            efc.run_function(leader_fsm::lose_health);
        }
        efc.new_event(MOB_EVENT_DEATH); {
            efc.change_state("dying");
        }
        efc.new_event(MOB_EVENT_BOTTOMLESS_PIT); {
            efc.run_function(leader_fsm::fall_down_pit);
        }
    }
    
    efc.new_state("holding", LEADER_STATE_HOLDING); {
        efc.new_event(LEADER_EVENT_THROW); {
            efc.run_function(leader_fsm::do_throw);
            efc.change_state("active");
        }
        efc.new_event(LEADER_EVENT_RELEASE); {
            efc.run_function(leader_fsm::release);
            efc.change_state("active");
        }
        efc.new_event(LEADER_EVENT_MOVE_START); {
            efc.run_function(leader_fsm::move);
            efc.run_function(leader_fsm::set_walk_anim);
        }
        efc.new_event(LEADER_EVENT_MOVE_END); {
            efc.run_function(leader_fsm::stop);
            efc.run_function(leader_fsm::set_stop_anim);
        }
        efc.new_event(MOB_EVENT_HITBOX_TOUCH_N_A); {
            efc.run_function(leader_fsm::release);
            efc.run_function(leader_fsm::lose_health);
        }
        efc.new_event(MOB_EVENT_DEATH); {
            efc.change_state("dying");
        }
        efc.new_event(MOB_EVENT_BOTTOMLESS_PIT); {
            efc.run_function(leader_fsm::fall_down_pit);
        }
    }
    
    efc.new_state("dismissing", LEADER_STATE_DISMISSING); {
        efc.new_event(MOB_EVENT_ON_ENTER); {
            efc.run_function(leader_fsm::dismiss);
        }
        efc.new_event(MOB_EVENT_ANIMATION_END); {
            efc.change_state("active");
        }
        efc.new_event(LEADER_EVENT_MOVE_START); {
            efc.run_function(leader_fsm::move);
        }
        efc.new_event(LEADER_EVENT_MOVE_END); {
            efc.run_function(leader_fsm::stop);
        }
        efc.new_event(MOB_EVENT_HITBOX_TOUCH_N_A); {
            efc.run_function(leader_fsm::lose_health);
        }
        efc.new_event(MOB_EVENT_DEATH); {
            efc.change_state("dying");
        }
    }
    
    efc.new_state("spraying", LEADER_STATE_SPRAYING); {
        efc.new_event(MOB_EVENT_ON_ENTER); {
            efc.run_function(leader_fsm::spray);
        }
        efc.new_event(MOB_EVENT_ANIMATION_END); {
            efc.change_state("active");
        }
        efc.new_event(MOB_EVENT_HITBOX_TOUCH_N_A); {
            efc.run_function(leader_fsm::lose_health);
        }
        efc.new_event(MOB_EVENT_DEATH); {
            efc.change_state("dying");
        }
    }
    
    efc.new_state("pain", LEADER_STATE_PAIN); {
        efc.new_event(MOB_EVENT_ON_ENTER); {
            efc.run_function(leader_fsm::suffer_pain);
        }
        efc.new_event(MOB_EVENT_ANIMATION_END); {
            efc.change_state("active");
        }
    }
    
    efc.new_state("inactive_pain", LEADER_STATE_INACTIVE_PAIN); {
        efc.new_event(MOB_EVENT_ON_ENTER); {
            efc.run_function(leader_fsm::suffer_pain);
        }
        efc.new_event(MOB_EVENT_ANIMATION_END); {
            efc.run_function(leader_fsm::be_dismissed);
            efc.change_state("idle");
        }
    }
    
    efc.new_state("knocked_back", LEADER_STATE_KNOCKED_BACK); {
        efc.new_event(MOB_EVENT_ON_ENTER); {
            efc.run_function(leader_fsm::get_knocked_back);
        }
        efc.new_event(MOB_EVENT_ANIMATION_END); {
            efc.change_state("active");
        }
        efc.new_event(MOB_EVENT_BOTTOMLESS_PIT); {
            efc.run_function(leader_fsm::fall_down_pit);
        }
    }
    
    efc.new_state("inactive_knocked_back", LEADER_STATE_INACTIVE_KNOCKED_BACK); {
        efc.new_event(MOB_EVENT_ON_ENTER); {
            efc.run_function(leader_fsm::get_knocked_back);
        }
        efc.new_event(MOB_EVENT_ANIMATION_END); {
            efc.change_state("idle");
        }
        efc.new_event(MOB_EVENT_BOTTOMLESS_PIT); {
            efc.run_function(leader_fsm::fall_down_pit);
        }
    }
    
    efc.new_state("dying", LEADER_STATE_DYING); {
        efc.new_event(MOB_EVENT_ON_ENTER); {
            efc.run_function(leader_fsm::die);
        }
    }
    
    efc.new_state("inactive_dying", LEADER_STATE_INACTIVE_DYING); {
        efc.new_event(MOB_EVENT_ON_ENTER); {
            efc.run_function(leader_fsm::inactive_die);
        }
    }
    
    efc.new_state("in_group_chasing", LEADER_STATE_IN_GROUP_CHASING); {
        efc.new_event(MOB_EVENT_ON_ENTER); {
            efc.run_function(leader_fsm::chase_leader);
        }
        efc.new_event(MOB_EVENT_REACHED_DESTINATION); {
            efc.change_state("in_group_stopped");
        }
        efc.new_event(MOB_EVENT_DISMISSED); {
            efc.run_function(leader_fsm::be_dismissed);
            efc.change_state("idle");
        }
        efc.new_event(MOB_EVENT_GRABBED_BY_FRIEND); {
            efc.run_function(leader_fsm::be_grabbed_by_friend);
            efc.change_state("held_by_leader");
        }
        efc.new_event(LEADER_EVENT_INACTIVE_SEARCH_SEED); {
            efc.run_function(leader_fsm::inactive_search_seed);
        }
        efc.new_event(LEADER_EVENT_GO_PLUCK); {
            efc.run_function(leader_fsm::go_pluck);
            efc.change_state("inactive_going_to_pluck");
        }
        efc.new_event(MOB_EVENT_HITBOX_TOUCH_N_A); {
            efc.run_function(leader_fsm::inactive_lose_health);
        }
        efc.new_event(MOB_EVENT_DEATH); {
            efc.change_state("inactive_dying");
        }
        efc.new_event(MOB_EVENT_BOTTOMLESS_PIT); {
            efc.run_function(leader_fsm::fall_down_pit);
        }
    }
    
    efc.new_state("in_group_stopped", LEADER_STATE_IN_GROUP_STOPPED); {
        efc.new_event(MOB_EVENT_ON_ENTER); {
            efc.run_function(leader_fsm::stop_in_group);
        }
        efc.new_event(MOB_EVENT_SPOT_IS_FAR); {
            efc.change_state("in_group_chasing");
        }
        efc.new_event(MOB_EVENT_DISMISSED); {
            efc.run_function(leader_fsm::be_dismissed);
            efc.change_state("idle");
        }
        efc.new_event(MOB_EVENT_GRABBED_BY_FRIEND); {
            efc.run_function(leader_fsm::be_grabbed_by_friend);
            efc.change_state("held_by_leader");
        }
        efc.new_event(LEADER_EVENT_INACTIVE_SEARCH_SEED); {
            efc.run_function(leader_fsm::inactive_search_seed);
        }
        efc.new_event(LEADER_EVENT_GO_PLUCK); {
            efc.run_function(leader_fsm::go_pluck);
            efc.change_state("inactive_going_to_pluck");
        }
        efc.new_event(MOB_EVENT_HITBOX_TOUCH_N_A); {
            efc.run_function(leader_fsm::inactive_lose_health);
        }
        efc.new_event(MOB_EVENT_DEATH); {
            efc.change_state("inactive_dying");
        }
        efc.new_event(MOB_EVENT_BOTTOMLESS_PIT); {
            efc.run_function(leader_fsm::fall_down_pit);
        }
    }
    
    efc.new_state("going_to_pluck", LEADER_STATE_GOING_TO_PLUCK); {
        efc.new_event(MOB_EVENT_REACHED_DESTINATION); {
            efc.run_function(leader_fsm::start_pluck);
            efc.change_state("plucking");
        }
        efc.new_event(LEADER_EVENT_CANCEL); {
            efc.run_function(leader_fsm::stop_pluck);
            efc.change_state("active");
        }
        efc.new_event(MOB_EVENT_HITBOX_TOUCH_N_A); {
            efc.run_function(leader_fsm::stop_pluck);
            efc.run_function(leader_fsm::lose_health);
        }
        efc.new_event(MOB_EVENT_DEATH); {
            efc.run_function(leader_fsm::stop_pluck);
            efc.change_state("dying");
        }
        efc.new_event(LEADER_EVENT_UNFOCUSED); {
            efc.run_function(leader_fsm::unfocus);
            efc.change_state("inactive_going_to_pluck");
        }
        efc.new_event(MOB_EVENT_BOTTOMLESS_PIT); {
            efc.run_function(leader_fsm::fall_down_pit);
        }
    }
    
    efc.new_state("plucking", LEADER_STATE_PLUCKING); {
        efc.new_event(MOB_EVENT_ANIMATION_END); {
            efc.run_function(leader_fsm::stop_pluck);
            efc.run_function(leader_fsm::search_seed);
        }
        efc.new_event(LEADER_EVENT_CANCEL); {
            efc.run_function(leader_fsm::stop_pluck);
            efc.change_state("active");
        }
        efc.new_event(LEADER_EVENT_UNFOCUSED); {
            efc.run_function(leader_fsm::unfocus);
            efc.change_state("inactive_plucking");
        }
    }
    
    efc.new_state("inactive_going_to_pluck", LEADER_STATE_INACTIVE_GOING_TO_PLUCK); {
        efc.new_event(MOB_EVENT_REACHED_DESTINATION); {
            efc.run_function(leader_fsm::start_pluck);
            efc.change_state("inactive_plucking");
        }
        efc.new_event(MOB_EVENT_WHISTLED); {
            efc.run_function(leader_fsm::stop_pluck);
            efc.change_state("in_group_chasing");
        }
        efc.new_event(MOB_EVENT_HITBOX_TOUCH_N_A); {
            efc.run_function(leader_fsm::stop_pluck);
            efc.run_function(leader_fsm::lose_health);
        }
        efc.new_event(MOB_EVENT_DEATH); {
            efc.run_function(leader_fsm::stop_pluck);
            efc.change_state("inactive_dying");
        }
        efc.new_event(LEADER_EVENT_FOCUSED); {
            efc.run_function(leader_fsm::focus);
            efc.change_state("going_to_pluck");
        }
        efc.new_event(MOB_EVENT_BOTTOMLESS_PIT); {
            efc.run_function(leader_fsm::fall_down_pit);
        }
    }
    
    efc.new_state("inactive_plucking", LEADER_STATE_INACTIVE_PLUCKING); {
        efc.new_event(MOB_EVENT_ANIMATION_END); {
            efc.run_function(leader_fsm::stop_pluck);
            efc.run_function(leader_fsm::inactive_search_seed);
        }
        efc.new_event(MOB_EVENT_WHISTLED); {
            efc.run_function(leader_fsm::stop_pluck);
            efc.change_state("in_group_chasing");
        }
        efc.new_event(LEADER_EVENT_FOCUSED); {
            efc.run_function(leader_fsm::focus);
            efc.change_state("plucking");
        }
    }
    
    efc.new_state("sleeping_waiting", LEADER_STATE_SLEEPING_WAITING); {
        efc.new_event(MOB_EVENT_ON_ENTER); {
            efc.run_function(gen_mob_fsm::carry_stop_move);
        }
        efc.new_event(MOB_EVENT_CARRY_KEEP_GOING); {
            efc.run_function(gen_mob_fsm::check_carry_begin);
        }
        efc.new_event(MOB_EVENT_CARRIER_ADDED); {
            efc.run_function(gen_mob_fsm::handle_carrier_added);
            efc.run_function(gen_mob_fsm::check_carry_begin);
        }
        efc.new_event(MOB_EVENT_CARRIER_REMOVED); {
            efc.run_function(gen_mob_fsm::handle_carrier_removed);
        }
        efc.new_event(MOB_EVENT_CARRY_BEGIN_MOVE); {
            efc.change_state("sleeping_moving");
        }
        efc.new_event(LEADER_EVENT_CANCEL); {
            efc.run_function(leader_fsm::start_waking_up);
            efc.change_state("waking_up");
        }
        efc.new_event(LEADER_EVENT_UNFOCUSED); {
            efc.run_function(leader_fsm::unfocus);
            efc.change_state("inactive_sleeping_waiting");
        }
        efc.new_event(MOB_EVENT_HITBOX_TOUCH_N_A); {
            efc.run_function(leader_fsm::lose_health);
            efc.run_function(leader_fsm::start_waking_up);
        }
        efc.new_event(MOB_EVENT_DEATH); {
            efc.run_function(leader_fsm::start_waking_up);
            efc.change_state("dying");
        }
    }
    
    efc.new_state("sleeping_moving", LEADER_STATE_SLEEPING_MOVING); {
        efc.new_event(MOB_EVENT_ON_ENTER); {
            efc.run_function(gen_mob_fsm::carry_begin_move);
            efc.run_function(gen_mob_fsm::set_next_target);
        }
        efc.new_event(MOB_EVENT_CARRIER_REMOVED); {
            efc.run_function(gen_mob_fsm::handle_carrier_removed);
            efc.run_function(gen_mob_fsm::check_carry_stop);
        }
        efc.new_event(MOB_EVENT_CARRY_WAIT_UP); {
            efc.change_state("sleeping_waiting");
        }
        efc.new_event(MOB_EVENT_CARRY_STOP_MOVE); {
            efc.change_state("sleeping_waiting");
        }
        efc.new_event(MOB_EVENT_CARRY_BEGIN_MOVE); {
            efc.run_function(gen_mob_fsm::carry_begin_move);
            efc.run_function(gen_mob_fsm::set_next_target);
        }
        efc.new_event(MOB_EVENT_REACHED_DESTINATION); {
            efc.run_function(gen_mob_fsm::set_next_target);
        }
        efc.new_event(MOB_EVENT_CARRY_DELIVERED); {
            efc.run_function(leader_fsm::start_waking_up);
            efc.change_state("waking_up");
        }
        efc.new_event(LEADER_EVENT_CANCEL); {
            efc.run_function(leader_fsm::start_waking_up);
            efc.change_state("waking_up");
        }
        efc.new_event(LEADER_EVENT_UNFOCUSED); {
            efc.run_function(leader_fsm::unfocus);
            efc.change_state("inactive_sleeping_moving");
        }
        efc.new_event(MOB_EVENT_HITBOX_TOUCH_N_A); {
            efc.run_function(leader_fsm::lose_health);
            efc.run_function(leader_fsm::start_waking_up);
        }
        efc.new_event(MOB_EVENT_DEATH); {
            efc.run_function(leader_fsm::start_waking_up);
            efc.change_state("dying");
        }
        efc.new_event(MOB_EVENT_BOTTOMLESS_PIT); {
            efc.run_function(leader_fsm::fall_down_pit);
        }
    }
    
    efc.new_state("inactive_sleeping_waiting", LEADER_STATE_INACTIVE_SLEEPING_WAITING); {
        efc.new_event(MOB_EVENT_ON_ENTER); {
            efc.run_function(gen_mob_fsm::carry_stop_move);
        }
        efc.new_event(MOB_EVENT_CARRY_KEEP_GOING); {
            efc.run_function(gen_mob_fsm::check_carry_begin);
        }
        efc.new_event(MOB_EVENT_CARRIER_ADDED); {
            efc.run_function(gen_mob_fsm::handle_carrier_added);
            efc.run_function(gen_mob_fsm::check_carry_begin);
        }
        efc.new_event(MOB_EVENT_CARRIER_REMOVED); {
            efc.run_function(gen_mob_fsm::handle_carrier_removed);
        }
        efc.new_event(MOB_EVENT_CARRY_BEGIN_MOVE); {
            efc.change_state("inactive_sleeping_moving");
        }
        efc.new_event(LEADER_EVENT_CANCEL); {
            efc.run_function(leader_fsm::start_waking_up);
            efc.change_state("inactive_waking_up");
        }
        efc.new_event(LEADER_EVENT_FOCUSED); {
            efc.run_function(leader_fsm::focus);
            efc.change_state("sleeping_waiting");
        }
        efc.new_event(MOB_EVENT_HITBOX_TOUCH_N_A); {
            efc.run_function(leader_fsm::lose_health);
            efc.run_function(leader_fsm::start_waking_up);
        }
        efc.new_event(MOB_EVENT_DEATH); {
            efc.run_function(leader_fsm::start_waking_up);
            efc.change_state("dying");
        }
    }
    
    efc.new_state("inactive_sleeping_moving", LEADER_STATE_INACTIVE_SLEEPING_MOVING); {
        efc.new_event(MOB_EVENT_ON_ENTER); {
            efc.run_function(gen_mob_fsm::carry_begin_move);
            efc.run_function(gen_mob_fsm::set_next_target);
        }
        efc.new_event(MOB_EVENT_CARRIER_REMOVED); {
            efc.run_function(gen_mob_fsm::handle_carrier_removed);
            efc.run_function(gen_mob_fsm::check_carry_stop);
        }
        efc.new_event(MOB_EVENT_CARRY_WAIT_UP); {
            efc.change_state("inactive_sleeping_waiting");
        }
        efc.new_event(MOB_EVENT_CARRY_STOP_MOVE); {
            efc.change_state("inactive_sleeping_waiting");
        }
        efc.new_event(MOB_EVENT_CARRY_BEGIN_MOVE); {
            efc.run_function(gen_mob_fsm::carry_begin_move);
            efc.run_function(gen_mob_fsm::set_next_target);
        }
        efc.new_event(MOB_EVENT_REACHED_DESTINATION); {
            efc.run_function(gen_mob_fsm::set_next_target);
        }
        efc.new_event(MOB_EVENT_CARRY_DELIVERED); {
            efc.run_function(leader_fsm::start_waking_up);
            efc.change_state("inactive_waking_up");
        }
        efc.new_event(LEADER_EVENT_CANCEL); {
            efc.run_function(leader_fsm::start_waking_up);
            efc.change_state("inactive_waking_up");
        }
        efc.new_event(LEADER_EVENT_FOCUSED); {
            efc.run_function(leader_fsm::focus);
            efc.change_state("sleeping_moving");
        }
        efc.new_event(MOB_EVENT_HITBOX_TOUCH_N_A); {
            efc.run_function(leader_fsm::lose_health);
            efc.run_function(leader_fsm::start_waking_up);
        }
        efc.new_event(MOB_EVENT_DEATH); {
            efc.run_function(leader_fsm::start_waking_up);
            efc.change_state("dying");
        }
        efc.new_event(MOB_EVENT_BOTTOMLESS_PIT); {
            efc.run_function(leader_fsm::fall_down_pit);
        }
    }
    
    efc.new_state("waking_up", LEADER_STATE_WAKING_UP); {
        efc.new_event(MOB_EVENT_ANIMATION_END); {
            efc.change_state("active");
        }
    }
    
    efc.new_state("inactive_waking_up", LEADER_STATE_INACTIVE_WAKING_UP); {
        efc.new_event(MOB_EVENT_ANIMATION_END); {
            efc.change_state("idle");
        }
    }
    
    efc.new_state("held_by_leader", LEADER_STATE_HELD); {
        efc.new_event(MOB_EVENT_ON_LEAVE); {
            efc.run_function(leader_fsm::be_released);
        }
        efc.new_event(MOB_EVENT_THROWN); {
            efc.run_function(leader_fsm::be_thrown);
            efc.change_state("thrown");
        }
        efc.new_event(MOB_EVENT_RELEASED); {
            efc.change_state("in_group_chasing");
        }
        efc.new_event(MOB_EVENT_HITBOX_TOUCH_N_A); {
            efc.run_function(leader_fsm::inactive_lose_health);
        }
        efc.new_event(MOB_EVENT_DEATH); {
            efc.change_state("inactive_dying");
        }
        efc.new_event(MOB_EVENT_BOTTOMLESS_PIT); {
            efc.run_function(leader_fsm::fall_down_pit);
        }
    }
    
    efc.new_state("thrown", LEADER_STATE_THROWN); {
        efc.new_event(MOB_EVENT_LANDED); {
            efc.run_function(leader_fsm::land);
            efc.change_state("idle");
        }
        efc.new_event(MOB_EVENT_BOTTOMLESS_PIT); {
            efc.run_function(leader_fsm::fall_down_pit);
        }
    }
    
    typ->states = efc.finish();
    typ->first_state_nr = fix_states(typ->states, "idle");
    
    if(typ->states.size() != N_LEADER_STATES) {
        error_log(
            "ENGINE WARNING: Number of leader states on the FSM (" + i2s(typ->states.size()) +
            ") and the enum (" + i2s(N_LEADER_STATES) + ") do not match."
        );
    }
}


void leader_fsm::whistle(mob* m, void* info1, void* info2) {
    leader* l_ptr = (leader*) m;
    
    l_ptr->lea_type->sfx_whistle.play(0, false);
    
    for(unsigned char d = 0; d < 6; ++d) whistle_dot_radius[d] = -1;
    whistle_fade_timer.start();
    whistle_fade_radius = 0;
    whistling = true;
    l_ptr->lea_type->sfx_whistle.play(0, false);
    l_ptr->set_animation(LEADER_ANIM_WHISTLING);
    l_ptr->script_timer.start(2.5f);
}

void leader_fsm::stop_whistle(mob* m, void* info1, void* info2) {
    if(!whistling) return;
    
    ((leader*) m)->lea_type->sfx_whistle.stop();
    
    whistle_fade_timer.start();
    whistle_fade_radius = whistle_radius;
    
    whistling = false;
    whistle_radius = 0;
    
}

void leader_fsm::join_group(mob* m, void* info1, void* info2) {
    leader* l_ptr = (leader*) m;
    
    add_to_group(cur_leader_ptr, l_ptr);
    size_t n_group_members = l_ptr->group->members.size();
    for(size_t m = 0; m < n_group_members; ++m) {
        mob* member = l_ptr->group->members[0];
        remove_from_group(member);
        add_to_group(cur_leader_ptr, member);
    }
}

void leader_fsm::fall_down_pit(mob* m, void* info1, void* info2) {
    m->health -= m->type->max_health * 0.2;
    m->x = m->home_x;
    m->y = m->home_y;
    m->center_sector = get_sector(m->x, m->y, NULL, true);
    m->ground_sector = m->center_sector;
    m->z = m->center_sector->z + 100;
}

void leader_fsm::focus(mob* m, void* info1, void* info2) {
    switch_to_leader((leader*) m);
}

void leader_fsm::enter_idle(mob* m, void* info1, void* info2) {
    m->set_animation(LEADER_ANIM_IDLE);
}

void leader_fsm::enter_active(mob* m, void* info1, void* info2) {
    ((leader*) m)->is_in_walking_anim = false;
    m->set_animation(LEADER_ANIM_IDLE);
}

void leader_fsm::unfocus(mob* m, void* info1, void* info2) {

}

void leader_fsm::move(mob* m, void* info1, void* info2) {
    leader* l_ptr = (leader*) m;
    movement_struct* mov = (movement_struct*) info1;
    l_ptr->chase(
        l_ptr->x + mov->get_x() * l_ptr->type->move_speed,
        l_ptr->y + mov->get_y() * l_ptr->type->move_speed,
        NULL, NULL, false, NULL, true
    );
}

void leader_fsm::stop(mob* m, void* info1, void* info2) {
    m->stop_chasing();
}

void leader_fsm::set_walk_anim(mob* m, void* info1, void* info2) {
    leader* l_ptr = (leader*) m;
    if(!l_ptr->is_in_walking_anim) {
        l_ptr->set_animation(LEADER_ANIM_WALK);
        l_ptr->is_in_walking_anim = true;
    }
}

void leader_fsm::set_stop_anim(mob* m, void* info1, void* info2) {
    leader* l_ptr = (leader*) m;
    if(l_ptr->is_in_walking_anim) {
        l_ptr->set_animation(LEADER_ANIM_IDLE);
        l_ptr->is_in_walking_anim = false;
    }
}

void leader_fsm::grab_mob(mob* m, void* info1, void* info2) {
    ((leader*) m)->holding_pikmin = (mob*) info1;
    
}

void leader_fsm::do_throw(mob* m, void* info1, void* info2) {
    leader* leader_ptr = (leader*) m;
    mob* holding_ptr = leader_ptr->holding_pikmin;
    
    holding_ptr->fsm.run_event(MOB_EVENT_THROWN);
    
    holding_ptr->x = leader_ptr->x;
    holding_ptr->y = leader_ptr->y;
    holding_ptr->z = leader_ptr->z;
    
    float angle, d;
    coordinates_to_angle(cursor_x - leader_ptr->x, cursor_y - leader_ptr->y, &angle, &d);
    
    float throw_height_mult = 1.0;
    if(typeid(*holding_ptr) == typeid(pikmin)) {
        throw_height_mult = ((pikmin*) holding_ptr)->pik_type->throw_height_mult;
    }
    
    //This results in a 1.3 second throw, just like in Pikmin 2. Regular Pikmin are thrown about 288.88 units high.
    holding_ptr->speed_x =
        cos(angle) * d * THROW_DISTANCE_MULTIPLIER * (1.0 / (THROW_STRENGTH_MULTIPLIER * throw_height_mult));
    holding_ptr->speed_y =
        sin(angle) * d * THROW_DISTANCE_MULTIPLIER * (1.0 / (THROW_STRENGTH_MULTIPLIER * throw_height_mult));
    holding_ptr->speed_z =
        -(GRAVITY_ADDER) * (THROW_STRENGTH_MULTIPLIER * throw_height_mult);
        
    holding_ptr->angle = angle;
    holding_ptr->face(angle);
    
    holding_ptr->was_thrown = true;
    
    remove_from_group(holding_ptr);
    leader_ptr->holding_pikmin = NULL;
    
    sfx_throw.stop();
    sfx_throw.play(0, false);
    leader_ptr->set_animation(LEADER_ANIM_THROW);
}

void leader_fsm::release(mob* m, void* info1, void* info2) {
    ((leader*) m)->holding_pikmin = NULL;
}

void leader_fsm::dismiss(mob* m, void* info1, void* info2) {
    ((leader*) m)->dismiss();
}

void leader_fsm::spray(mob* m, void* info1, void* info2) {
    m->stop_chasing();
    size_t spray_nr = *((size_t*) info1);
    
    if(spray_amounts[spray_nr] == 0) {
        m->fsm.set_state(LEADER_STATE_ACTIVE);
        return;
    }
    
    float shoot_angle = cursor_angle + ((spray_types[spray_nr].burpable) ? M_PI : 0);
    
    random_particle_spray(
        PARTICLE_TYPE_BITMAP,
        bmp_smoke,
        m->x + cos(shoot_angle) * m->type->radius,
        m->y + sin(shoot_angle) * m->type->radius,
        shoot_angle,
        spray_types[spray_nr].main_color
    );
    
    spray_amounts[spray_nr]--;
    
    m->set_animation(LEADER_ANIM_SPRAYING);
}

void leader_fsm::lose_health(mob* m, void* info1, void* info2) {
    //TODO
    
    if(m->invuln_period.time_left > 0.0f) return;
    m->invuln_period.start();
    
    hitbox_touch_info* info = (hitbox_touch_info*) info1;
    float damage = 0;
    float knockback = 0;
    float knockback_angle = 0;
    
    damage = calculate_damage(info->mob2, m, info->hi2, info->hi1);
    calculate_knockback(info->mob2, m, info->hi2, info->hi1, &knockback, &knockback_angle);
    
    m->health -= damage;
    apply_knockback(m, knockback, knockback_angle);
    
    //If info2 has a value, then this leader is inactive.
    if(knockback > 0 && damage == 0) {
        if(info2)
            m->fsm.set_state(LEADER_STATE_INACTIVE_KNOCKED_BACK);
        else
            m->fsm.set_state(LEADER_STATE_KNOCKED_BACK);
    } else {
        if(info2)
            m->fsm.set_state(LEADER_STATE_INACTIVE_PAIN);
        else
            m->fsm.set_state(LEADER_STATE_PAIN);
    }
}

void leader_fsm::inactive_lose_health(mob* m, void* info1, void* info2) {
    int a = 0;
    leader_fsm::lose_health(m, info1, &a);
    //We need to send the function a value so it knows
    //it's an inactive leader.
}

void leader_fsm::die(mob* m, void* info1, void* info2) {
    //TODO
}

void leader_fsm::inactive_die(mob* m, void* info1, void* info2) {
    //TODO
}

void leader_fsm::suffer_pain(mob* m, void* info1, void* info2) {
    m->set_animation(LEADER_ANIM_PAIN);
    m->stop_chasing();
}

void leader_fsm::get_knocked_back(mob* m, void* info1, void* info2) {
    m->set_animation(LEADER_ANIM_KNOCKED_DOWN);
}

void leader_fsm::fall_asleep(mob* m, void* info1, void* info2) {
    leader_fsm::dismiss(m, info1, info2);
    m->stop_chasing();
    
    m->become_carriable(false);
    
    m->set_animation(LEADER_ANIM_LIE);
}

void leader_fsm::start_waking_up(mob* m, void* info1, void* info2) {
    m->become_uncarriable();
    m->set_animation(LEADER_ANIM_GET_UP);
}

void leader_fsm::chase_leader(mob* m, void* info1, void* info2) {
    m->chase(0, 0, &m->following_group->x, &m->following_group->y, false);
    m->set_animation(LEADER_ANIM_WALK);
    focus_mob(m, m->following_group);
}

void leader_fsm::stop_in_group(mob* m, void* info1, void* info2) {
    m->stop_chasing();
    m->set_animation(LEADER_ANIM_IDLE);
}

void leader_fsm::be_dismissed(mob* m, void* info1, void* info2) {
    m->stop_chasing();
    m->set_animation(LEADER_ANIM_IDLE);
}

void leader_fsm::go_pluck(mob* m, void* info1, void* info2) {
    leader* lea_ptr = (leader*) m;
    pikmin* pik_ptr = (pikmin*) info1;
    
    lea_ptr->auto_pluck_pikmin = pik_ptr;
    lea_ptr->chase(
        pik_ptr->x, pik_ptr->y,
        NULL, NULL,
        false, nullptr, true,
        pik_ptr->type->radius + lea_ptr->type->radius
    );
    pik_ptr->pluck_reserved = true;
    
    //Now for the leaders in the group.
    for(size_t m = 0; m < lea_ptr->group->members.size(); ++m) {
        mob* member_ptr = lea_ptr->group->members[m];
        if(typeid(*member_ptr) == typeid(leader)) {
            member_ptr->fsm.run_event(LEADER_EVENT_INACTIVE_SEARCH_SEED);
        }
    }
}

void leader_fsm::start_pluck(mob* m, void* info1, void* info2) {
    leader* l_ptr = (leader*) m;
    l_ptr->auto_pluck_pikmin->fsm.run_event(MOB_EVENT_PLUCKED, (void*) l_ptr);
    l_ptr->auto_pluck_pikmin = nullptr;
    l_ptr->set_animation(LEADER_ANIM_PLUCK);
}

void leader_fsm::stop_pluck(mob* m, void* info1, void* info2) {
    leader* l_ptr = (leader*) m;
    if(l_ptr->auto_pluck_pikmin) {
        l_ptr->stop_chasing();
        l_ptr->auto_pluck_pikmin->pluck_reserved = false;
    }
    l_ptr->auto_pluck_pikmin = NULL;
    l_ptr->set_animation(LEADER_ANIM_IDLE);
}

void leader_fsm::search_seed(mob* m, void* info1, void* info2) {
    leader* l_ptr = (leader*) m;
    dist d;
    pikmin* new_pikmin = get_closest_buried_pikmin(l_ptr->x, l_ptr->y, &d, false);
    
    //If info1 is not void, that means this is an inactive leader.
    if(info1) {
        if(l_ptr->following_group)
            l_ptr->fsm.set_state(LEADER_STATE_IN_GROUP_CHASING);
        else
            l_ptr->fsm.set_state(LEADER_STATE_IDLE);
    } else {
        l_ptr->fsm.set_state(LEADER_STATE_ACTIVE);
    }
    
    if(new_pikmin && d <= next_pluck_range) {
        l_ptr->fsm.run_event(LEADER_EVENT_GO_PLUCK, (void*) new_pikmin);
    }
}

void leader_fsm::inactive_search_seed(mob* m, void* info1, void* info2) {
    int a = 0; //Dummy value.
    leader_fsm::search_seed(m, &a, NULL);
}

void leader_fsm::be_grabbed_by_friend(mob* m, void* info1, void* info2) {
    m->set_animation(LEADER_ANIM_IDLE);
}

void leader_fsm::be_released(mob* m, void* info1, void* info2) {

}

void leader_fsm::be_thrown(mob* m, void* info1, void* info2) {
    m->stop_chasing();
}

void leader_fsm::land(mob* m, void* info1, void* info2) {
    m->stop_chasing();
    m->speed_x = m->speed_y = 0;
}
