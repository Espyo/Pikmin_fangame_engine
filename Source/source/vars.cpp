/*
 * Copyright (c) Andre 'Espyo' Silva 2013.
 * The following source file belongs to the open-source project Pikifen.
 * Please read the included README and LICENSE files for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Globally accessible variables.
 */

#include <limits.h>

#include "vars.h"

#include "const.h"

using std::map;
using std::size_t;
using std::string;
using std::vector;

ALLEGRO_BITMAP* bmp_checkbox_check = NULL;
ALLEGRO_BITMAP* bmp_cursor = NULL;
ALLEGRO_BITMAP* bmp_cursor_invalid = NULL;
ALLEGRO_BITMAP* bmp_enemy_spirit = NULL;
ALLEGRO_BITMAP* bmp_icon = NULL;
ALLEGRO_BITMAP* bmp_idle_glow = NULL;
ALLEGRO_BITMAP* bmp_message_box = NULL;
ALLEGRO_BITMAP* bmp_mouse_cursor = NULL;
ALLEGRO_BITMAP* bmp_mouse_button_icon[3] = {NULL, NULL, NULL};
ALLEGRO_BITMAP* bmp_mouse_wd_icon = NULL;
ALLEGRO_BITMAP* bmp_mouse_wu_icon = NULL;
ALLEGRO_BITMAP* bmp_notification = NULL;
ALLEGRO_BITMAP* bmp_pikmin_silhouette = NULL;
ALLEGRO_BITMAP* bmp_pikmin_spirit = NULL;
ALLEGRO_BITMAP* bmp_rock = NULL;
ALLEGRO_BITMAP* bmp_shadow = NULL;
ALLEGRO_BITMAP* bmp_smack = NULL;
ALLEGRO_BITMAP* bmp_smoke = NULL;
ALLEGRO_BITMAP* bmp_sparkle = NULL;
ALLEGRO_BITMAP* bmp_spotlight = NULL;
ALLEGRO_BITMAP* bmp_swarm_arrow = NULL;
ALLEGRO_BITMAP* bmp_wave_ring = NULL;

sample_struct sfx_attack;
sample_struct sfx_camera;
sample_struct sfx_pikmin_attack;
sample_struct sfx_pikmin_called;
sample_struct sfx_pikmin_carrying;
sample_struct sfx_pikmin_carrying_grab;
sample_struct sfx_pikmin_caught;
sample_struct sfx_pikmin_dying;
sample_struct sfx_pikmin_held;
sample_struct sfx_pikmin_idle;
sample_struct sfx_pluck;
sample_struct sfx_pikmin_plucked;
sample_struct sfx_pikmin_thrown;
sample_struct sfx_switch_pikmin;
sample_struct sfx_throw;

asset_file_names_struct asset_file_names;

float area_editor_backup_interval = DEF_AREA_EDITOR_BACKUP_INTERVAL;
float area_editor_grid_interval = DEF_AREA_EDITOR_GRID_INTERVAL;
size_t area_editor_snap_threshold = DEF_AREA_EDITOR_SNAP_THRESHOLD;
bool area_editor_show_edge_length = true;
bool area_editor_show_territory = false;
size_t area_editor_undo_limit = DEF_AREA_EDITOR_UNDO_LIMIT;
unsigned char area_editor_view_mode = 0;
timer area_title_fade_timer(AREA_TITLE_FADE_DURATION);
bmp_manager bitmaps("");
ALLEGRO_BITMAP* bmp_error = NULL;
map<string, bouncer_type*> bouncer_types;
vector<bouncer*> bouncers;
vector<bridge*> bridges;
map<string, bridge_type*> bridge_types;
button_manager buttons;
point cam_box[2];
point cam_final_pos;
float cam_final_zoom = 1.0;
point cam_pos;
float cam_zoom = 1.0;
bool can_throw_leaders = true;
ALLEGRO_COLOR carrying_color_move = al_map_rgb(255, 255, 255);
ALLEGRO_COLOR carrying_color_stop = al_map_rgb(96, 192, 192);
float carrying_speed_base_mult = 0.5;
float carrying_speed_max_mult = 0.8;
float carrying_speed_weight_mult = 0.0004;
mob* closest_group_member = NULL;
bool closest_group_member_distant = false;
vector<vector<control_info> > controls;
vector<converter*> converters;
map<string, converter_type*> converter_types;
bool creator_tool_area_image_shadows = true;
int creator_tool_area_image_size = 2048;
bool creator_tool_area_image_mobs = true;
string creator_tool_auto_start_option;
string creator_tool_auto_start_mode;
bool creator_tool_change_speed = false;
float creator_tool_change_speed_mult = 2.0;
bool creator_tool_geometry_info = false;
bool creator_tool_hitboxes = false;
mob* creator_tool_info_lock = NULL;
float creator_tool_mob_hurting_ratio = 0.5;
pikmin_type* creator_tool_last_pikmin_type = NULL;
unsigned char creator_tool_keys[20];
bool creator_tools_enabled = true;
string cur_message;
size_t cur_message_char = 0;
timer cur_message_char_timer(0);
size_t cur_message_section = 0;
ALLEGRO_BITMAP* cur_message_speaker = NULL;
vector<size_t> cur_message_stopping_chars;
float cursor_max_dist = 200.0;
float cursor_spin_speed = 180;
map<string, mob_type*> custom_mob_types;
map<string, particle_generator> custom_particle_generators;
float day_minutes_end = 60 * 19;
float day_minutes_per_irl_sec = 2;
float day_minutes_start = 60 * 7;
map<string, decoration_type*> decoration_types;
vector<decoration*> decorations;
bool draw_cursor_trail = true;
vector<drop*> drops;
map<string, drop_type*> drop_types;
bool editor_mmb_pan = false;
float editor_mouse_drag_threshold = DEF_EDITOR_MOUSE_DRAG_THRESHOLD;
map<string, enemy_type*> enemy_types;
vector<enemy*> enemies;
ALLEGRO_FONT* font_area_name = NULL;
ALLEGRO_FONT* font_builtin = NULL;
ALLEGRO_FONT* font_counter = NULL;
ALLEGRO_FONT* font_main = NULL;
unsigned int font_counter_h = 0;
unsigned int font_main_h = 0;
ALLEGRO_FONT* font_value = NULL;
vector<group_task*> group_tasks;
map<string, group_task_type*> group_task_types;
map<string, hazard> hazards;
hud_item_manager hud_items(N_HUD_ITEMS);
float hud_coords[N_HUD_ITEMS][4];
float idle_task_range = 50;
float info_print_duration = 5.0f;
float info_print_fade_duration = 3.0f;
string info_print_text;
timer info_print_timer =
    timer(
        1.0f,
[] () { info_print_text.clear(); }
    );
bool intended_scr_fullscreen = DEF_SCR_FULLSCREEN;
int intended_scr_h = DEF_SCR_H;
int intended_scr_w = DEF_SCR_W;
vector<interactable*> interactables;
map<string, interactable_type*> interactable_types;
bool is_input_allowed = false;
float joystick_min_deadzone = 0.2f;
float joystick_max_deadzone = 0.9f;
map<ALLEGRO_JOYSTICK*, int> joystick_numbers;
vector<leader*> leaders;
mob* leader_cursor_mob = NULL;
point leader_cursor_s;
sector* leader_cursor_sector = NULL;
point leader_cursor_w;
vector<leader_type*> leader_order;
vector<string> leader_order_strings;
movement_struct leader_movement;
map<string, leader_type*> leader_types;
ALLEGRO_BITMAP* lightmap_bmp = NULL;
ALLEGRO_BITMAP* loading_text_bmp = NULL;
ALLEGRO_BITMAP* loading_subtext_bmp = NULL;
map<string, liquid> liquids;
float maturity_power_mult = 0.1f;
float maturity_speed_mult = 0.1f;
size_t max_particles = 200;
size_t max_pikmin_in_field = 100;
float message_char_interval = 0.02f;
bool mipmaps_enabled = true;
ALLEGRO_MIXER* mixer = NULL;
vector<mob_action> mob_actions;
mob_category_manager mob_categories;
vector<mob*> mobs;
point mouse_cursor_s;
point mouse_cursor_w;
bool mouse_cursor_valid = true;
bool mouse_moves_cursor[MAX_PLAYERS] = {true, false, false, false};
float next_pluck_range = 160.0f;
float onion_open_range = 24.0f;
map<string, onion_type*> onion_types;
vector<onion*> onions;
unsigned char particle_quality = 2;
particle_manager particles(0);
bool paused = false;
map<string, pellet_type*> pellet_types;
vector<pellet*> pellets;
vector<pile*> piles;
map<string, pile_type*> pile_types;
vector<point> precipitation;
timer precipitation_timer(0);
float pikmin_chase_range = 200.0f;
float pikmin_grab_range = 64.0f;
vector<pikmin*> pikmin_list;
vector<pikmin_type*> pikmin_order;
vector<string> pikmin_order_strings;
map<string, pikmin_type*> pikmin_types;
float pluck_range = 30.0f;
bool pretty_whistle = true;
bool ready_for_input = false;
vector<resource*> resources;
map<string, resource_type*> resource_types;
map<string, scale_type*> scale_types;
vector<scale*> scales;
bool scr_fullscreen = DEF_SCR_FULLSCREEN;
int scr_h = DEF_SCR_H;
int scr_w = DEF_SCR_W;
ALLEGRO_TRANSFORM screen_to_world_transform;
sector_types_manager sector_types;
size_t selected_spray = 0;
replay session_replay;
unsigned char ship_beam_ring_color[3] = {0, 0, 0};
bool ship_beam_ring_color_up[3] = {true, true, true};
map<string, ship_type*> ship_types;
vector<ship*> ships;
bool show_system_info = false;
bool smooth_scaling = true;
single_animation_suite spark_animation;
map<string, mob_type*> spec_mob_types;
map<string, spike_damage_type> spike_damage_types;
vector<spray_stats_struct> spray_stats;
vector<spray_type> spray_types;
float standard_pikmin_height = 24;
float standard_pikmin_radius = 5;
map<string, status_type> status_types;
subgroup_type_manager subgroup_types;
float swarm_angle = 0;
vector<float> swarm_arrows;
float swarm_magnitude = 0;
timer swarm_next_arrow_timer(SWARM_ARROWS_INTERVAL);
bool swarm_cursor = false;
float swarm_task_range = 0;
movement_struct swarm_movement;
bmp_manager textures(TEXTURES_FOLDER_NAME);
bool throw_can_reach_cursor = true;
map<string, tool_type*> tool_types;
vector<tool*> tools;
map<string, track_type*> track_types;
vector<track*> tracks;
map<string, treasure_type*> treasure_types;
vector<treasure*> treasures;
ALLEGRO_VOICE* voice = NULL;
map<string, weather> weather_conditions;
float whistle_dot_radius[6] = { -1, -1, -1, -1, -1, -1 };
float whistle_fade_radius = 0;
timer whistle_fade_timer(WHISTLE_FADE_TIME);
float whistle_growth_speed = 180.0f;
timer whistle_next_dot_timer(WHISTLE_DOT_INTERVAL);
timer whistle_next_ring_timer(WHISTLE_RINGS_INTERVAL);
float whistle_radius = 0;
vector<unsigned char> whistle_ring_colors;
unsigned char whistle_ring_prev_color = 0;
vector<float> whistle_rings;
bool whistling = false;
bool window_position_hack = false;
ALLEGRO_TRANSFORM world_to_screen_transform;
float zoom_max_level = 3.0f;
float zoom_mid_level = 1.4f;
float zoom_min_level = 0.66f;
