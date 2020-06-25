/*
 * Copyright (c) Andre 'Espyo' Silva 2013.
 * The following source file belongs to the open-source project Pikifen.
 * Please read the included README and LICENSE files for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Header for the general editor-related functions.
 */

#ifndef EDITOR_INCLUDED
#define EDITOR_INCLUDED

#include <string>
#include <vector>

#include "../game_state.h"

using std::map;
using std::string;
using std::vector;

/*
 * A generic class for an editor.
 * It comes with some common stuff, mostly GUI stuff.
 */
class editor : public game_state {
public:

    editor();
    virtual ~editor() = default;
    
    virtual void do_drawing() = 0;
    virtual void do_logic() = 0;
    virtual void handle_allegro_event(ALLEGRO_EVENT &ev);
    virtual void load();
    virtual void unload();
    virtual void update_transformations();
    virtual string get_name() const = 0;
    
protected:

    static const int   EDITOR_ICON_BMP_PADDING;
    static const int   EDITOR_ICON_BMP_SIZE;
    static const float DOUBLE_CLICK_TIMEOUT;
    static const float KEYBOARD_CAM_ZOOM;
    static const float UNSAVED_CHANGES_WARNING_DURATION;
    static const int   UNSAVED_CHANGES_WARNING_HEIGHT;
    static const int   UNSAVED_CHANGES_WARNING_SPIKE_SIZE;
    static const int   UNSAVED_CHANGES_WARNING_WIDTH;
    
    enum EDITOR_ICONS {
        ICON_SAVE,
        ICON_LOAD,
        ICON_QUIT,
        ICON_HITBOXES,
        ICON_REFERENCE,
        ICON_INFO,
        ICON_RESIZE,
        ICON_PLAY_PAUSE,
        ICON_NEXT,
        ICON_PREVIOUS,
        ICON_ADD,
        ICON_REMOVE,
        ICON_MOVE_RIGHT,
        ICON_MOVE_LEFT,
        ICON_SELECT_NONE,
        ICON_DUPLICATE,
        ICON_ADD_STOP,
        ICON_ADD_LINK,
        ICON_ADD_1W_LINK,
        ICON_REMOVE_STOP,
        ICON_REMOVE_LINK,
        ICON_ADD_CIRCLE_SECTOR,
        ICON_VERTEXES,
        ICON_EDGES,
        ICON_SECTORS,
        ICON_MOBS,
        ICON_PATHS,
        ICON_DETAILS,
        ICON_REVIEW,
        ICON_TOOLS,
        ICON_OPTIONS,
        ICON_UNDO,
        ICON_GRID,
        ICON_MOB_RADIUS,
        ICON_PIKMIN_SILHOUETTE,
        ICON_ANIMATIONS,
        ICON_SPRITES,
        ICON_BODY_PARTS,
        ICON_PLAY,
        ICON_SNAP_GRID,
        ICON_SNAP_VERTEXES,
        ICON_SNAP_EDGES,
        ICON_SNAP_NOTHING,
        
        N_EDITOR_ICONS
    };
    
    
    struct transformation_controller {
    public:
        bool keep_aspect_ratio;
        bool allow_rotation;
        
        void draw_handles();
        bool handle_mouse_down(const point pos);
        void handle_mouse_up();
        bool handle_mouse_move(const point pos);
        point get_center() const;
        point get_size() const;
        float get_angle() const;
        void set_center(const point &center);
        void set_size(const point &size);
        void set_angle(const float angle);
        transformation_controller();
        
    private:
        static const float HANDLE_RADIUS;
        static const float ROTATION_HANDLE_THICKNESS;
        signed char moving_handle;
        point center;
        point size;
        float angle;
        ALLEGRO_TRANSFORM align_transform;
        ALLEGRO_TRANSFORM disalign_transform;
        float radius;
        point pre_move_size;
        float pre_rotation_angle;
        float pre_rotation_mouse_angle;
        point get_handle_pos(const unsigned char handle) const;
        void update();
        
    };
    
    struct picker_item {
        //Its name.
        string name;
        //What category it belongs to, or empty string for none.
        string category;
        //Bitmap, if any.
        ALLEGRO_BITMAP* bitmap;
        
        picker_item(
            const string &name,
            const string &category = "", ALLEGRO_BITMAP* bitmap = nullptr
        );
    };
    
    
    //Bitmap with all of the editor icons.
    ALLEGRO_BITMAP* bmp_editor_icons;
    //Top-left corner of the canvas.
    point canvas_tl;
    //Bottom right corner of the canvas.
    point canvas_br;
    //X coordinate of the canvas GUI separator. -1 = undefined.
    int canvas_separator_x;
    //Callback for when it's time to process the dialog's contents.
    std::function<void()> dialog_process_callback;
    //Callback for when the user closes the dialog, if any.
    std::function<void()> dialog_close_callback;
    //Title to display on the dialog.
    string dialog_title;
    //If the next click is within this time, it's a double-click.
    float double_click_time;
    //List of every individual editor icon.
    vector<ALLEGRO_BITMAP*> editor_icons;
    //Is the Ctrl key currently pressed down?
    bool is_ctrl_pressed;
    //Is a dialog currently open?
    bool is_dialog_open;
    //Is the GUI currently in focus? False if it's the canvas.
    bool is_gui_focused;
    //Is the mouse currently hovering the gui? False if it's the canvas.
    bool is_mouse_in_gui;
    //Is the left mouse button currently pressed down?
    bool is_m1_pressed;
    //Is the right mouse button currently pressed down?
    bool is_m2_pressed;
    //Is the middle mouse button currently pressed down?
    bool is_m3_pressed;
    //Is the Shift key currently pressed down?
    bool is_shift_pressed;
    //Number of the mouse button pressed.
    size_t last_mouse_click;
    //Has the user picked any content to load yet?
    bool loaded_content_yet;
    //Has the user made any unsaved changes yet?
    bool made_new_changes;
    //Is this a real mouse drag, or just a shaky click?
    bool mouse_drag_confirmed;
    //Starting coordinates of a raw mouse drag.
    point mouse_drag_start;
    //List of picker dialog items to choose from.
    vector<picker_item> picker_items;
    //Callback for when the user picks an item from the picker dialog.
    std::function<void(const string &, const bool)> picker_pick_callback;
    //Text to display above the picker dialog list.
    string picker_list_header;
    //Can the user make a new item in the picker dialog?
    bool picker_can_make_new;
    //Only show picker dialog items matching this filter.
    string picker_filter;
    //Current state.
    size_t state;
    //Status bar text.
    string status_text;
    //Current sub-state.
    size_t sub_state;
    //When placing the unsaved changes warning, focus on these coordinates.
    point unsaved_changes_warning_pos;
    //Time left for the unsaved changes warning to be on-screen.
    timer unsaved_changes_warning_timer;
    //Maximum zoom level allowed.
    float zoom_max_level;
    //Minimum zoom level allowed.
    float zoom_min_level;
    
    
    //Standard functions.
    void center_camera(
        const point &min_coords, const point &max_coords
    );
    bool check_new_unsaved_changes(const point &pos = point());
    void do_logic_post();
    void do_logic_pre();
    void draw_grid(
        const float interval,
        const ALLEGRO_COLOR &major_color, const ALLEGRO_COLOR &minor_color
    );
    void draw_unsaved_changes_warning();
    void focus_next_special_input();
    point get_last_widget_pos();
    bool input_popup(
        const char* label, const char* prompt, string* text
    );
    bool list_popup(
        const char* label, const vector<string> &items, string* picked_item
    );
    void leave();
    void next_input_needs_special_focus();
    void open_dialog(
        const string &title,
        const std::function<void()> &process_callback
    );
    void open_picker(
        const string &title,
        const vector<picker_item> &items,
        const std::function<void(const string &, const bool)> &pick_callback,
        const string &list_header = "",
        const bool can_make_new = false,
        const string &filter = ""
    );
    void process_dialog();
    void process_mob_type_widgets(
        mob_category** cat, mob_type** typ,
        const bool only_show_area_editor_types,
        const std::function<void()> &category_change_callback = nullptr,
        const std::function<void()> &type_change_callback = nullptr
    );
    void panel_title(const char* title, const float width);
    bool saveable_tree_node(const string &category, const string &label);
    void set_tooltip(const string &explanation, const string &shortcut = "");
    void zoom_with_cursor(const float new_zoom);
    
    
    //Input handler functions.
    virtual void handle_key_char_anywhere(const ALLEGRO_EVENT &ev);
    virtual void handle_key_char_canvas(const ALLEGRO_EVENT &ev);
    virtual void handle_key_down_anywhere(const ALLEGRO_EVENT &ev);
    virtual void handle_key_down_canvas(const ALLEGRO_EVENT &ev);
    virtual void handle_key_up_anywhere(const ALLEGRO_EVENT &ev);
    virtual void handle_key_up_canvas(const ALLEGRO_EVENT &ev);
    virtual void handle_lmb_double_click(const ALLEGRO_EVENT &ev);
    virtual void handle_lmb_down(const ALLEGRO_EVENT &ev);
    virtual void handle_lmb_drag(const ALLEGRO_EVENT &ev);
    virtual void handle_lmb_up(const ALLEGRO_EVENT &ev);
    virtual void handle_mmb_double_click(const ALLEGRO_EVENT &ev);
    virtual void handle_mmb_down(const ALLEGRO_EVENT &ev);
    virtual void handle_mmb_drag(const ALLEGRO_EVENT &ev);
    virtual void handle_mmb_up(const ALLEGRO_EVENT &ev);
    virtual void handle_mouse_update(const ALLEGRO_EVENT &ev);
    virtual void handle_mouse_wheel(const ALLEGRO_EVENT &ev);
    virtual void handle_rmb_double_click(const ALLEGRO_EVENT &ev);
    virtual void handle_rmb_down(const ALLEGRO_EVENT &ev);
    virtual void handle_rmb_drag(const ALLEGRO_EVENT &ev);
    virtual void handle_rmb_up(const ALLEGRO_EVENT &ev);
    
private:

    //Controls text input widget focus, when focusing on one is necessary.
    unsigned char special_input_focus_controller;
    
    void process_picker();
};

#endif //ifndef EDITOR_INCLUDED
