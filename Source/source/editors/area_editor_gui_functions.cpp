/*
 * Copyright (c) Andre 'Espyo' Silva 2013-2017.
 * The following source file belongs to the open-source project
 * Pikmin fangame engine. Please read the included
 * README and LICENSE files for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Functions about the area editor's GUI.
 */

#include "area_editor.h"
#include "../LAFI/angle_picker.h"
#include "../LAFI/button.h"
#include "../LAFI/checkbox.h"
#include "../LAFI/image.h"
#include "../LAFI/radio_button.h"
#include "../LAFI/scrollbar.h"
#include "../LAFI/textbox.h"
#include "../functions.h"
#include "../vars.h"

/* ----------------------------------------------------------------------------
 * Loads the current advanced sector appearance data onto the GUI.
 */
void area_editor::asa_to_gui() {
    if(selected_sectors.empty()) {
        if(state == EDITOR_STATE_ASA) {
            sector_to_gui();
            state = EDITOR_STATE_LAYOUT;
            change_to_right_frame();
        }
        return;
    }
    
    sector* s_ptr = *selected_sectors.begin();
    
    ((lafi::textbox*) frm_asa->widgets["txt_x"])->text =
        f2s(s_ptr->texture_info.translation.x);
    ((lafi::textbox*) frm_asa->widgets["txt_y"])->text =
        f2s(s_ptr->texture_info.translation.y);
    ((lafi::textbox*) frm_asa->widgets["txt_sx"])->text =
        f2s(s_ptr->texture_info.scale.y);
    ((lafi::textbox*) frm_asa->widgets["txt_sy"])->text =
        f2s(s_ptr->texture_info.scale.y);
    ((lafi::angle_picker*) frm_asa->widgets["ang_a"])->set_angle_rads(
        s_ptr->texture_info.rot
    );
    ((lafi::textbox*) frm_asa->widgets["txt_tint"])->text =
        c2s(s_ptr->texture_info.tint);
        
    ((lafi::textbox*) frm_asa->widgets["txt_brightness"])->text =
        i2s(s_ptr->brightness);
    ((lafi::scrollbar*) frm_asa->widgets["bar_brightness"])->set_value(
        s_ptr->brightness, false
    );
    ((lafi::checkbox*) frm_asa->widgets["chk_shadow"])->set(
        s_ptr->always_cast_shadow
    );
}


/* ----------------------------------------------------------------------------
 * Loads the current advanced sector behavior data onto the GUI.
 */
void area_editor::asb_to_gui() {
    if(selected_sectors.empty()) {
        if(state == EDITOR_STATE_ASB) {
            sector_to_gui();
            state = EDITOR_STATE_LAYOUT;
            change_to_right_frame();
        }
        return;
    }
    
    sector* s_ptr = *selected_sectors.begin();
    
    ((lafi::button*) frm_asb->widgets["but_sector_type"])->text =
        sector_types.get_name(s_ptr->type);
        
    if(s_ptr->hazards_str.empty()) {
        ((lafi::label*) frm_asb->widgets["lbl_hazard"])->text = "(No hazards)";
        disable_widget(frm_asb->widgets["but_h_del"]);
        disable_widget(frm_asb->widgets["but_h_prev"]);
        disable_widget(frm_asb->widgets["but_h_next"]);
        
    } else {
        vector<string> list = split(s_ptr->hazards_str, ";");
        if(cur_hazard_nr >= list.size()) {
            cur_hazard_nr = list.size() - 1;
        }
        ((lafi::label*) frm_asb->widgets["lbl_hazard"])->text =
            i2s(cur_hazard_nr + 1) + "/" + i2s(list.size()) + ": " +
            list[cur_hazard_nr];
            
        enable_widget(frm_asb->widgets["but_h_del"]);
        enable_widget(frm_asb->widgets["but_h_prev"]);
        enable_widget(frm_asb->widgets["but_h_next"]);
    }
    
    ((lafi::checkbox*) frm_asb->widgets["chk_h_air"])->set(
        !s_ptr->hazard_floor
    );
    
    ((lafi::textbox*) frm_asb->widgets["txt_tag"])->text = s_ptr->tag;
    
}


/* ----------------------------------------------------------------------------
 * Switches to the correct frame, depending on the current editor mode.
 */
void area_editor::change_to_right_frame() {
    sub_state = EDITOR_SUB_STATE_NONE;
    
    hide_all_frames();
    
    if(state == EDITOR_STATE_MAIN) {
        frm_main->show();
        update_main_frame();
    } else if(state == EDITOR_STATE_LAYOUT) {
        frm_layout->show();
        sector_to_gui();
    } else if(state == EDITOR_STATE_ASB) {
        frm_asb->show();
        asb_to_gui();
    } else if(state == EDITOR_STATE_TEXTURE) {
        frm_texture->show();
    } else if(state == EDITOR_STATE_ASA) {
        frm_asa->show();
        asa_to_gui();
    } else if(state == EDITOR_STATE_MOBS) {
        frm_mobs->show();
        mob_to_gui();
    } else if(state == EDITOR_STATE_PATHS) {
        frm_paths->show();
        path_to_gui();
    } else if(state == EDITOR_STATE_DETAILS) {
        frm_details->show();
        details_to_gui();
    } else if(state == EDITOR_STATE_REVIEW) {
        frm_review->show();
        review_to_gui();
    } else if(state == EDITOR_STATE_INFO) {
        frm_info->show();
        info_to_gui();
    } else if(state == EDITOR_STATE_TOOLS) {
        frm_tools->show();
        tools_to_gui();
    } else if(state == EDITOR_STATE_OPTIONS) {
        frm_options->show();
    }
}


/* ----------------------------------------------------------------------------
 * GUI functions for clearing the data for the current area.
 */
void area_editor::clear_current_area_gui() {
    ((lafi::button*) frm_main->widgets["but_area"])->text = cur_area_name;
    frm_area->show();
    enable_widget(frm_bottom->widgets["but_save"]);
    frm_paths->widgets["lbl_path_dist"]->hide();
    ((lafi::checkbox*) frm_paths->widgets["chk_show_path"])->uncheck();
}


/* ----------------------------------------------------------------------------
 * Deletes the currently selected hazard from the list.
 */
void area_editor::delete_current_hazard() {
    sector* s_ptr = *selected_sectors.begin();
    
    vector<string> list = split(s_ptr->hazards_str, ";");
    s_ptr->hazards_str.clear();
    for(size_t h = 0; h < list.size(); ++h) {
        if(h == cur_hazard_nr) continue;
        s_ptr->hazards_str += list[h] + ";";
    }
    if(!s_ptr->hazards_str.empty()) {
        s_ptr->hazards_str.pop_back(); //Remove the trailing semicolon.
    }
    cur_hazard_nr = min(cur_hazard_nr, list.size() - 1);
    
    homogenize_selected_sectors();
    asb_to_gui();
}


/* ----------------------------------------------------------------------------
 * Loads the current details data onto the GUI.
 */
void area_editor::details_to_gui() {
    if(selected_shadow) {
    
        frm_shadow->show();
        ((lafi::textbox*) frm_shadow->widgets["txt_x"])->text =
            f2s(selected_shadow->center.x);
        ((lafi::textbox*) frm_shadow->widgets["txt_y"])->text =
            f2s(selected_shadow->center.y);
        ((lafi::textbox*) frm_shadow->widgets["txt_w"])->text =
            f2s(selected_shadow->size.x);
        ((lafi::textbox*) frm_shadow->widgets["txt_h"])->text =
            f2s(selected_shadow->size.y);
        ((lafi::angle_picker*) frm_shadow->widgets["ang_an"])->set_angle_rads(
            selected_shadow->angle
        );
        ((lafi::scrollbar*) frm_shadow->widgets["bar_al"])->set_value(
            selected_shadow->alpha, false
        );
        ((lafi::textbox*) frm_shadow->widgets["txt_file"])->text =
            selected_shadow->file_name;
        ((lafi::textbox*) frm_shadow->widgets["txt_sx"])->text =
            f2s(selected_shadow->sway.x);
        ((lafi::textbox*) frm_shadow->widgets["txt_sy"])->text =
            f2s(selected_shadow->sway.y);
            
    } else {
        frm_shadow->hide();
    }
}


/* ----------------------------------------------------------------------------
 * Saves the advanced sector appearance data to memory using info on the gui.
 */
void area_editor::gui_to_asa() {
    sector* s_ptr = *selected_sectors.begin();
    
    s_ptr->texture_info.translation.x =
        s2f(((lafi::textbox*) frm_asa->widgets["txt_x"])->text);
    s_ptr->texture_info.translation.y =
        s2f(((lafi::textbox*) frm_asa->widgets["txt_y"])->text);
    s_ptr->texture_info.scale.x =
        s2f(((lafi::textbox*) frm_asa->widgets["txt_sx"])->text);
    s_ptr->texture_info.scale.y =
        s2f(((lafi::textbox*) frm_asa->widgets["txt_sy"])->text);
    s_ptr->texture_info.rot =
        ((lafi::angle_picker*) frm_asa->widgets["ang_a"])->get_angle_rads();
    s_ptr->texture_info.tint =
        s2c(((lafi::textbox*) frm_asa->widgets["txt_tint"])->text);
        
    s_ptr->brightness =
        s2i(((lafi::textbox*) frm_asa->widgets["txt_brightness"])->text);
    s_ptr->always_cast_shadow =
        ((lafi::checkbox*) frm_asa->widgets["chk_shadow"])->checked;
        
    homogenize_selected_sectors();
    asa_to_gui();
}


/* ----------------------------------------------------------------------------
 * Saves the advanced sector behavior data to memory using info on the gui.
 */
void area_editor::gui_to_asb() {
    sector* s_ptr = *selected_sectors.begin();
    s_ptr->hazard_floor =
        !((lafi::checkbox*) frm_asb->widgets["chk_h_air"])->checked;
    s_ptr->tag =
        ((lafi::textbox*) frm_asb->widgets["txt_tag"])->text;
        
    homogenize_selected_sectors();
    asb_to_gui();
}


/* ----------------------------------------------------------------------------
 * Saves the details data to memory using info on the gui.
 */
void area_editor::gui_to_details() {
    if(!selected_shadow) return;
    
    selected_shadow->center.x =
        s2f(((lafi::textbox*) frm_shadow->widgets["txt_x"])->text);
    selected_shadow->center.y =
        s2f(((lafi::textbox*) frm_shadow->widgets["txt_y"])->text);
    selected_shadow->size.x =
        s2f(((lafi::textbox*) frm_shadow->widgets["txt_w"])->text);
    selected_shadow->size.y =
        s2f(((lafi::textbox*) frm_shadow->widgets["txt_h"])->text);
    selected_shadow->angle =
        ((lafi::angle_picker*) frm_shadow->widgets["ang_an"])->get_angle_rads();
    selected_shadow->alpha =
        ((lafi::scrollbar*) frm_shadow->widgets["bar_al"])->low_value;
    selected_shadow->sway.x =
        s2f(((lafi::textbox*) frm_shadow->widgets["txt_sx"])->text);
    selected_shadow->sway.y =
        s2f(((lafi::textbox*) frm_shadow->widgets["txt_sy"])->text);
        
    string new_file_name =
        ((lafi::textbox*) frm_shadow->widgets["txt_file"])->text;
        
    if(new_file_name != selected_shadow->file_name) {
        //New image, delete the old one.
        if(selected_shadow->bitmap != bmp_error) {
            bitmaps.detach(selected_shadow->file_name);
        }
        selected_shadow->bitmap =
            bitmaps.get(TEXTURES_FOLDER_NAME + "/" + new_file_name, NULL);
        selected_shadow->file_name = new_file_name;
    }
    
    made_changes = true;
}


/* ----------------------------------------------------------------------------
 * Saves the mob data to memory using info on the gui.
 */
void area_editor::gui_to_mob() {
    mob_gen* m_ptr = *selected_mobs.begin();
    m_ptr->angle =
        (
            (lafi::angle_picker*) frm_mob->widgets["ang_angle"]
        )->get_angle_rads();
    m_ptr->vars =
        ((lafi::textbox*) frm_mob->widgets["txt_vars"])->text;
    homogenize_selected_mobs();
}


/* ----------------------------------------------------------------------------
 * Saves the sector data to memory using info on the gui.
 */
void area_editor::gui_to_sector() {
    sector* s_ptr = *selected_sectors.begin();
    s_ptr->z =
        s2f(((lafi::textbox*) frm_sector->widgets["txt_z"])->text);
        
    s_ptr->fade =
        ((lafi::radio_button*) frm_sector->widgets["rad_fade"])->selected;
        
    update_sector_texture(
        s_ptr,
        ((lafi::button*) frm_sector->widgets["but_texture"])->text
    );
    
    homogenize_selected_sectors();
    sector_to_gui();
}


/* ----------------------------------------------------------------------------
 * Hides all menu frames.
 */
void area_editor::hide_all_frames() {
    //TODO
    frm_main->hide();
    frm_layout->hide();
    frm_asb->hide();
    frm_texture->hide();
    frm_asa->hide();
    frm_mobs->hide();
    frm_paths->hide();
    frm_details->hide();
    frm_review->hide();
    frm_info->hide();
    frm_tools->hide();
    frm_options->hide();
}


/* ----------------------------------------------------------------------------
 * Loads the current area metadata onto the GUI.
 */
void area_editor::info_to_gui() {
    //TODO
}


/* ----------------------------------------------------------------------------
 * Loads the current mob data onto the GUI.
 */
void area_editor::mob_to_gui() {
    frm_mob->hide();
    frm_mob_multi->hide();
    
    if(selected_mobs.size() == 1 || selection_homogenized) {
        frm_mob->show();
        
        mob_gen* m_ptr = *selected_mobs.begin();
        
        (
            (lafi::angle_picker*) frm_mob->widgets["ang_angle"]
        )->set_angle_rads(m_ptr->angle);
        ((lafi::textbox*) frm_mob->widgets["txt_vars"])->text = m_ptr->vars;
        
        ((lafi::button*) frm_mob->widgets["but_category"])->text =
            m_ptr->category->plural_name;
            
        lafi::button* but_type =
            (lafi::button*) frm_mob->widgets["but_type"];
        if(m_ptr->category->id == MOB_CATEGORY_NONE) {
            disable_widget(but_type);
        } else {
            enable_widget(but_type);
        }
        but_type->text = m_ptr->type ? m_ptr->type->name : "";
        
    } else if(selected_mobs.size() > 1 && !selection_homogenized) {
        frm_mob_multi->show();
        
    }
    
}


/* ----------------------------------------------------------------------------
 * Opens the frame where you pick from a list.
 * For the type of content, use area_editor_old::AREA_EDITOR_PICKER_*.
 */
void area_editor::open_picker(const unsigned char type) {
    vector<string> elements;
    bool can_create_new = false;
    
    if(type == AREA_EDITOR_PICKER_AREA) {
        elements = folder_to_vector(AREAS_FOLDER_PATH, true);
        can_create_new = true;
        
    } else if(type == AREA_EDITOR_PICKER_SECTOR_TYPE) {
    
        for(size_t t = 0; t < sector_types.get_nr_of_types(); ++t) {
            elements.push_back(sector_types.get_name(t));
        }
        
    } else if(type == AREA_EDITOR_PICKER_HAZARD) {
    
        for(auto h = hazards.begin(); h != hazards.end(); ++h) {
            elements.push_back(h->first);
        }
        
    } else if(type == AREA_EDITOR_PICKER_MOB_CATEGORY) {
    
        for(unsigned char f = 0; f < N_MOB_CATEGORIES; ++f) {
            //0 is none.
            if(f == MOB_CATEGORY_NONE) continue;
            elements.push_back(mob_categories.get(f)->plural_name);
        }
        
    } else if(type == AREA_EDITOR_PICKER_MOB_TYPE) {
    
        mob_gen* m_ptr = *selected_mobs.begin();
        if(m_ptr->category->id != MOB_CATEGORY_NONE) {
            m_ptr->category->get_type_names(elements);
        }
        
    }
    
    generate_and_open_picker(elements, type, can_create_new);
}


/* ----------------------------------------------------------------------------
 * Loads the current path data onto the GUI.
 */
void area_editor::path_to_gui() {
    //TODO
}


/* ----------------------------------------------------------------------------
 * Adds texture suggestions to the gui frame.
 */
void area_editor::populate_texture_suggestions() {
    lafi::frame* f = (lafi::frame*) frm_texture->widgets["frm_list"];
    
    while(!f->widgets.empty()) {
        f->remove(f->widgets.begin()->first);
    }
    
    if(texture_suggestions.empty()) return;
    
    f->easy_reset();
    f->easy_row();
    
    for(size_t s = 0; s < texture_suggestions.size(); ++s) {
    
        string name = texture_suggestions[s].name;
        lafi::image* i =
            new lafi::image(texture_suggestions[s].bmp);
        lafi::label* l =
            new lafi::label(name);
            
        auto lambda = [name, this] (lafi::widget*, int, int) {
            ((lafi::button*) this->frm_sector->widgets["but_texture"])->text =
                name;
            update_texture_suggestions(name);
            gui_to_sector();
            state = EDITOR_STATE_LAYOUT;
            change_to_right_frame();
        };
        i->left_mouse_click_handler = lambda;
        l->left_mouse_click_handler = lambda;
        f->easy_add("img_" + i2s(s), i, 48, 48, lafi::EASY_FLAG_WIDTH_PX);
        f->easy_add("lbl_" + i2s(s), l, 96, 48, lafi::EASY_FLAG_WIDTH_PX);
        f->easy_row(0);
    }
    
    (
        (lafi::scrollbar*) frm_texture->widgets["bar_scroll"]
    )->make_widget_scroll(f);
}


/* ----------------------------------------------------------------------------
 * Picks an item and closes the list picker frame.
 */
void area_editor::pick(const string &name, const unsigned char type) {
    if(type == AREA_EDITOR_PICKER_AREA) {
        cur_area_name = name;
        area_editor::load_area(false);
        update_main_frame();
        
    } else if(type == AREA_EDITOR_PICKER_HAZARD) {
        sector* s_ptr = *selected_sectors.begin();
        vector<string> list = split(s_ptr->hazards_str, ";");
        if(!s_ptr->hazards_str.empty()) {
            s_ptr->hazards_str += ";";
        }
        s_ptr->hazards_str += name;
        homogenize_selected_sectors();
        asb_to_gui();
        cur_hazard_nr = list.size();
        
    } else if(type == AREA_EDITOR_PICKER_SECTOR_TYPE) {
        sector* s_ptr = *selected_sectors.begin();
        s_ptr->type = sector_types.get_nr(name);
        homogenize_selected_sectors();
        asb_to_gui();
        
    } else if(type == AREA_EDITOR_PICKER_MOB_CATEGORY) {
        mob_gen* m_ptr = *selected_mobs.begin();
        m_ptr->category = mob_categories.get_from_pname(name);
        m_ptr->type = NULL;
        homogenize_selected_mobs();
        mob_to_gui();
        
    } else if(type == AREA_EDITOR_PICKER_MOB_TYPE) {
        mob_gen* m_ptr = *selected_mobs.begin();
        m_ptr->type = m_ptr->category->get_type(name);
        homogenize_selected_mobs();
        mob_to_gui();
        
    }
    
    show_bottom_frame();
    change_to_right_frame();
}


/* ----------------------------------------------------------------------------
 * Loads the current review data onto the GUI.
 */
void area_editor::review_to_gui() {
    //TODO
}


/* ----------------------------------------------------------------------------
 * Loads the current sector data onto the GUI.
 */
void area_editor::sector_to_gui() {
    lafi::button* but_sel_filter =
        ((lafi::button*) frm_layout->widgets["but_sel_filter"]);
    if(selection_filter == SELECTION_FILTER_SECTORS) {
        but_sel_filter->icon = icons.get(ICON_SELECT_SECTORS);
        but_sel_filter->description =
            "Current selection filter: Sectors + edges + vertexes.";
    } else if(selection_filter == SELECTION_FILTER_EDGES) {
        but_sel_filter->icon = icons.get(ICON_SELECT_EDGES);
        but_sel_filter->description =
            "Current selection filter: Edges + vertexes.";
    } else {
        but_sel_filter->icon = icons.get(ICON_SELECT_VERTEXES);
        but_sel_filter->description =
            "Current selection filter: Vertexes only.";
    }
    
    frm_sector->hide();
    frm_sector_multi->hide();
    
    if(selected_sectors.size() == 1 || selection_homogenized) {
        frm_sector->show();
        
        sector* s_ptr = *selected_sectors.begin();
        
        ((lafi::textbox*) frm_sector->widgets["txt_z"])->text =
            f2s(s_ptr->z);
            
        if(s_ptr->fade) {
            ((lafi::radio_button*) frm_sector->widgets["rad_fade"])->select();
            ((lafi::button*) frm_sector->widgets["but_texture"])->text = "";
            disable_widget(frm_sector->widgets["but_texture"]);
            
        } else {
            (
                (lafi::radio_button*) frm_sector->widgets["rad_texture"]
            )->select();
            ((lafi::button*) frm_sector->widgets["but_texture"])->text =
                s_ptr->texture_info.file_name;
            enable_widget(frm_sector->widgets["but_texture"]);
            
        }
        
    } else if(selected_sectors.size() > 1 && !selection_homogenized) {
        frm_sector_multi->show();
        
    }
}


/* ----------------------------------------------------------------------------
 * Selects either the previous or the next hazard on the list.
 */
void area_editor::select_different_hazard(const bool next) {
    sector* s_ptr = *selected_sectors.begin();
    vector<string> list = split(s_ptr->hazards_str, ";");
    cur_hazard_nr = min(cur_hazard_nr, list.size() - 1);
    cur_hazard_nr = sum_and_wrap(cur_hazard_nr, next ? 1 : -1, list.size());
    asb_to_gui();
}


/* ----------------------------------------------------------------------------
 * Loads the current tools data onto the GUI.
 */
void area_editor::tools_to_gui() {
    //TODO
}


/* ----------------------------------------------------------------------------
 * Updates the main frame.
 */
void area_editor::update_main_frame() {
    if(cur_area_name.empty()) {
        frm_area->hide();
    } else {
        frm_area->show();
    }
    ((lafi::button*) frm_main->widgets["but_area"])->text = cur_area_name;
}
