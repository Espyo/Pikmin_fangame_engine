/*
 * Copyright (c) Andr� 'Espyo' Silva 2014.
 * The following source file belongs to the open-source project
 * Pikmin fangame engine. Please read the included README file
 * for more information.
 * Pikmin is copyright (c) Nintendo.
 *
 * === FILE DESCRIPTION ===
 * Header for the sector, linedef, etc. classes and related functions.
 */

#ifndef SECTOR_INCLUDED
#define SECTOR_INCLUDED

#include <set>
#include <vector>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "element.h"

using namespace std;


struct area_map;
struct blockmap;
struct linedef;
struct sector;
struct sector_texture;
struct triangle;
struct vertex;
typedef vector<vertex*> polygon;


//Intersection between two lines. Used to mark
//linedefs as red on the editor.
struct linedef_intersection {
    linedef* l1, *l2;
    linedef_intersection(linedef* l1, linedef* l2);
    bool contains(linedef* l);
};


/*
 * The blockmap divides the entire area
 * in a grid, so that collision detections only
 * happen between stuff in the same grid cell.
 * This is to avoid having, for instance, a Pikmin
 * on the lake part of TIS check for collisions with
 * a wall on the landing site part of TIS.
 */
struct blockmap {
    float x1, y2;
    unsigned n_cols, n_rows;
    vector<vector<linedef*> > linedefs_in_blocks;
};


/*
 * A line that delimits a sector.
 */
struct linedef {
    vertex* vertices[2];
    size_t vertex_nrs[2];
    sector* sectors[2];
    size_t sector_nrs[2];
    
    linedef(size_t v1 = string::npos, size_t v2 = string::npos);
    void fix_pointers(area_map &a);
};


/*
 * A structure with info on a texture for a sector.
 * For sectors with two of these, the texture smoothly
 * transitions from the first to the second.
 */
struct sector_texture {
    float scale;   //Texture scale...
    float trans_x; //X translation...
    float trans_y; //Y translation...
    float rot;     //And rotation.
    ALLEGRO_BITMAP* bitmap;
    
    sector_texture();
};


/*
 * A sector, like the ones in Doom.
 * It's composed of lines, so it's essentially
 * a polygon. It has a certain height, and its looks
 * is determined by its floors.
 */
struct sector {
    sector_texture textures[2];
    unsigned short type;
    unsigned int tag;
    float z; //Height.
    float brightness;
    
    vector<element*> elements;
    vector<size_t> linedef_nrs;
    vector<linedef*> linedefs;
    vector<triangle> triangles;
    
    sector();
    void connect_linedefs(area_map &a, size_t s_nr);
    void fix_pointers(area_map &a);
};


/*
 * A triangle. Sectors (polygons) are made out of triangles.
 * These are used to detect whether a point is inside a sector,
 * and to draw, seeing as OpenGL cannot draw concave polygons.
 */
struct triangle {
    vertex* points[3];
    triangle(vertex* v1, vertex* v2, vertex* v3);
};


/*
 * A vertex is a 2D point, used to determine
 * the end-points of a linedef.
 */
struct vertex {
    float x, y;
    vector<size_t> linedef_nrs;
    vector<linedef*> linedefs;
    
    vertex(float x, float y);
    void connect_linedefs(area_map &a, size_t v_nr);
    void fix_pointers(area_map &a);
};


/*
 * A structure that holds all of the
 * info about the current area, so that
 * the sectors know how to communicate with
 * the linedefs, the linedefs with the
 * vertices, etc.
 */
struct area_map {
    blockmap bmap;
    vector<vertex*> vertices;
    vector<linedef*> linedefs;
    vector<sector*> sectors;
    
    void clear();
};


void check_linedef_intersections(vertex* v);
void clean_poly(polygon* p);
void cut_poly(polygon* outer, vector<polygon>* inners);
float get_angle_dif(float a1, float a2);
void get_cce(vector<vertex> &vertices_left, vector<size_t> &ears, vector<size_t> &convex_vertices, vector<size_t> &concave_vertices);
void get_polys(sector* s, polygon* outer, vector<polygon>* inners);
vertex* get_rightmost_vertex(map<linedef*, bool> &sides_todo);
vertex* get_rightmost_vertex(polygon* p);
sector* get_sector(float x, float y, size_t* sector_nr);
bool is_vertex_convex(const vector<vertex> &vec, const size_t nr);
bool is_vertex_ear(const vector<vertex> &vec, const vector<size_t> &concaves, const size_t nr);
bool is_point_in_triangle(float px, float py, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3);
bool lines_intersect(float l1x1, float l1y1, float l1x2, float l1y2, float l2x1, float l2y1, float l2x2, float l2y2, float* ur, float* ul);
void triangulate(sector* s);


enum SECTOR_TYPES {
    SECTOR_TYPE_NORMAL,
    SECTOR_TYPE_BOTTOMLESS_PIT,
    SECTOR_TYPE_BASE,
};


enum TERRAIN_SOUNDS {
    TERRAIN_SOUND_NONE,
    TERRAIN_SOUND_DIRT,
    TERRAIN_SOUND_GRASS,
    TERRAIN_SOUND_STONE,
    TERRAIN_SOUND_WOOD,
    TERRAIN_SOUND_METAL,
    TERRAIN_SOUND_WATER,
};


#endif //ifndef SECTOR_INCLUDED