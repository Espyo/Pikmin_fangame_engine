/*
 * Copyright (c) Andre 'Espyo' Silva 2013.
 * The following source file belongs to the open-source project Pikifen.
 * Please read the included README and LICENSE files for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Gate finite state machine logic.
 */

#include "gate_fsm.h"

#include "../functions.h"
#include "../mobs/gate.h"
#include "../utils/string_utils.h"
#include "../vars.h"
#include "gen_mob_fsm.h"

/* ----------------------------------------------------------------------------
 * Creates the finite state machine for the gate's logic.
 */
void gate_fsm::create_fsm(mob_type* typ) {
    easy_fsm_creator efc;
    efc.new_state("idling", GATE_STATE_IDLING); {
        efc.new_event(MOB_EVENT_ON_ENTER); {
            efc.run(gate_fsm::set_anim);
        }
        efc.new_event(MOB_EVENT_HITBOX_TOUCH_N_A); {
            efc.run(gen_mob_fsm::be_attacked);
        }
        efc.new_event(MOB_EVENT_DEATH); {
            efc.run(gate_fsm::open);
            efc.change_state("destroyed");
        }
    }
    
    efc.new_state("destroyed", GATE_STATE_DESTROYED); {
    
    }
    
    
    typ->states = efc.finish();
    typ->first_state_nr = fix_states(typ->states, "idling");
    
    //Check if the number in the enum and the total match up.
    engine_assert(
        typ->states.size() == N_GATE_STATES,
        i2s(typ->states.size()) + " registered, " +
        i2s(N_GATE_STATES) + " in enum."
    );
}


/* ----------------------------------------------------------------------------
 * When a gate is opened. This creates the particle explosion too.
 */
void gate_fsm::open(mob* m, void* info1, void* info2) {
    m->set_animation(GATE_ANIM_DESTROYED);
    m->start_dying();
    m->finish_dying();
    m->tangible = false;
    
    particle p(
        PARTICLE_TYPE_BITMAP, m->pos, m->z + m->type->height,
        80, 2.75, PARTICLE_PRIORITY_MEDIUM
    );
    p.bitmap = bmp_smoke;
    p.color = al_map_rgb(238, 204, 170);
    particle_generator pg(0, p, 11);
    pg.number_deviation = 1;
    pg.size_deviation = 16;
    pg.angle = 0;
    pg.angle_deviation = TAU / 2;
    pg.total_speed = 75;
    pg.total_speed_deviation = 15;
    pg.duration_deviation = 0.25;
    pg.emit(particles);
}


/* ----------------------------------------------------------------------------
 * When a gate needs to enter its default "idling" animation.
 */
void gate_fsm::set_anim(mob* m, void* info1, void* info2) {
    m->set_animation(GATE_ANIM_IDLING);
}