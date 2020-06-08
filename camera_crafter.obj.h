/*
 * miranda sandbox
 * Copyright (C) 2018-2020 Andrea Nardinocchi (andrea@nardinan.it)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef game_camera_crafter_h
#define game_camera_crafter_h
#include "miranda.h"
enum e_camera_spots {
  e_camera_spot_fullscreen = 0,
  e_camera_spot_half_top,
  e_camera_spot_half_bottom,
  e_camera_spot_top_left_quarter,
  e_camera_spot_top_right_quarter,
  e_camera_spot_bottom_left_quarter,
  e_camera_spot_bottom_right_quarter
} e_camera_sports;
typedef struct s_camera_crafter_modificator { d_list_node_head;
  char label[d_entity_label_size];
  struct s_object *string_label;
  struct s_object *modificator;
  struct s_camera_crafter_view *view_reference;
} s_camera_crafter_modificator;
typedef struct s_camera_crafter_view { d_list_node_head;
  char label[d_entity_label_size];
  struct s_object *string_label;
  double position_x, position_y, dimension_w, dimension_h, scale_resolution_x, scale_resolution_y, center_x, center_y, zoom;
  struct s_list modificators;
  e_environment_surfaces surface;
  struct s_object *camera;
} s_camera_crafter_view;
extern struct s_object *master_camera_crafter;
d_declare_class(camera_crafter) {
  struct s_attributes head;
  struct s_list views;
} d_declare_class_tail(camera_crafter);
struct s_camera_crafter_attributes *p_camera_crafter_alloc(struct s_object *self, struct s_object *environment);
extern struct s_object *f_camera_crafter_new(struct s_object *self, struct s_object *environment);
d_declare_method(camera_crafter, add_view)(struct s_object *self, const char *key, const char *position, double scale_resolution_x,
    double scale_resolution_y, double center_x, double center_y, double zoom, e_environment_surfaces surface);
d_declare_method(camera_crafter, get_view)(struct s_object *self, const char *key);
d_declare_method(camera_crafter, get_view_struct)(struct s_object *self, const char *key);
d_declare_method(camera_crafter, set_view_zoom)(struct s_object *self, const char *key, double zoom);
d_declare_method(camera_crafter, set_view_reference)(struct s_object *self, const char *key, double scale_resolution_x, double scale_resolution_y);
d_declare_method(camera_crafter, del_view)(struct s_object *self, const char *key);
d_declare_method(camera_crafter, update)(struct s_object *self, const char *camera_key);
d_declare_method(camera_crafter, delete)(struct s_object *self, struct s_camera_crafter_attributes *attributes);
#endif
