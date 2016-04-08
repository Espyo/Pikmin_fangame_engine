/*
 * Copyright (c) Andre 'Espyo' Silva 2013-2016.
 * The following source file belongs to the open-source project
 * Pikmin fangame engine. Please read the included
 * README and LICENSE files for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Header for the mob type class and mob type-related functions.
 */

#ifndef MOB_TYPE_INCLUDED
#define MOB_TYPE_INCLUDED

#include <functional>
#include <map>

#include <allegro5/allegro.h>

#include "../animation.h"
#include "../data_file.h"
#include "../mob_script.h"

using namespace std;

enum ENEMY_EXTRA_STATES {
    ENEMY_EXTRA_STATE_CARRIABLE_WAITING,
    ENEMY_EXTRA_STATE_CARRIABLE_MOVING,
    ENEMY_EXTRA_STATE_BEING_DELIVERED,
};

enum MOB_CATEGORIES {
    MOB_CATEGORY_NONE,
    MOB_CATEGORY_PIKMIN,
    MOB_CATEGORY_ONIONS,
    MOB_CATEGORY_LEADERS,
    MOB_CATEGORY_ENEMIES,
    MOB_CATEGORY_TREASURES,
    MOB_CATEGORY_PELLETS,
    MOB_CATEGORY_SPECIAL,
    MOB_CATEGORY_SHIPS,
    MOB_CATEGORY_GATES,
    
    N_MOB_CATEGORIES,
};

const size_t ANIM_IDLE = 0;


/* ----------------------------------------------------------------------------
 * A mob type.
 * There are specific types, like Pikmin,
 * leader, etc., but these are used
 * to create more generic mob types,
 * like some teleporter pad, or a door.
 */
class mob_type {
public:
    //Technical things.
    string name;
    
    //Visual things.
    animation_pool anims;
    ALLEGRO_COLOR main_color;
    bool show_health;
    bool casts_shadow;
    
    //Space-related things.
    float radius;         //Diameter.
    float height;
    float move_speed;
    float rotation_speed;
    bool always_active; //If true, this mob is always active, even if it's off-camera.
    bool pushes; //Blocks passage of other mobs.
    bool pushable; //Can be pushed by other mobs.
    
    //Behavior things.
    float max_health;
    float sight_radius;
    float territory_radius;
    float near_radius;
    float near_angle;
    size_t max_carriers;
    float weight;          //Pikmin strength needed to carry it.
    float big_damage_interval;
    
    //Script things.
    vector<mob_state*> states;    //The states, events and actions.
    size_t first_state_nr;        //Number of the state a mob starts at.
    
    //Misc.
    bool is_obstacle;
    
    //Used by the special mob types, as it is not possible to control which type of mob to create without a list.
    function<void(float x, float y, float angle, const string &vars)> create_mob; //Creates a mob of this type.
    function<void(data_node* file, const bool load_resources, vector<pair<size_t, string> >* anim_conversions)> load_from_file_func;
    
    mob_type();
    ~mob_type();
    virtual void load_from_file(data_node* file, const bool load_resources, vector<pair<size_t, string> >* anim_conversions);
    void add_carrying_states();
};


void load_mob_types(const string &folder, const unsigned char category, bool load_resources);
void load_mob_types(bool load_resources);
void load_mob_type_from_file(mob_type* mt, data_node &file, const bool load_resources, const string &folder);

#endif //ifndef MOB_TYPE_INCLUDED
