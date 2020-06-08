/*
 * miranda_sandbox
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
#include "camera_crafter.obj.h"
struct s_object *master_camera_crafter;
struct s_camera_crafter_attributes *p_camera_crafter_alloc(struct s_object *self, struct s_object *environment) {
  struct s_camera_crafter_attributes *result = d_prepare(self, camera_crafter);
  f_game_object_new(self, environment);
  return result;
}
struct s_object *f_camera_crafter_new(struct s_object *self, struct s_object *environment) {
  p_camera_crafter_alloc(self, environment);
  return self;
}
d_define_method(camera_crafter, add_view)(struct s_object *self, const char *key, const char *position, double scale_resolution_x,
    double scale_resolution_y, double center_x, double center_y, double zoom, e_environment_surfaces surface) {
  d_using(camera_crafter);
  d_using(game_object);
  struct s_scoped_local_positioning {
    char *description;
    enum e_camera_spots spot;
  } scoped_positioning_elements[] = {
    {"fullscreen",              e_camera_spot_fullscreen},
    {"half_top",                e_camera_spot_half_top},
    {"half_bottom",             e_camera_spot_half_bottom},
    {"quarter_top_left",        e_camera_spot_top_left_quarter},
    {"quarter_top_right",       e_camera_spot_top_right_quarter},
    {"quarter_bottom_left",     e_camera_spot_bottom_left_quarter},
    {"quarter_bottom_right",    e_camera_spot_bottom_right_quarter},
    {NULL}
  };
  struct s_environment_attributes *environment_attributes = d_cast(game_object_attributes->environment, environment);
  struct s_camera_crafter_view *camera_view;
  enum e_camera_spots current_spot = e_camera_spot_fullscreen;
  int index_positioning;
  d_game_lock(self) {
    if ((camera_view = (struct s_camera_crafter_view *)d_malloc(sizeof(struct s_camera_crafter_view)))) {
      strncpy(camera_view->label, key, d_entity_label_size);
      if ((camera_view->string_label = f_string_new_constant(d_new(string),(char *)key))) {
        camera_view->surface = surface;
        camera_view->zoom = zoom;
        camera_view->scale_resolution_x = scale_resolution_x;
        camera_view->scale_resolution_y = scale_resolution_y;
        camera_view->center_x = center_x;
        camera_view->center_y = center_y;
        camera_view->position_x = 0.0;
        camera_view->position_y = 0.0;
        camera_view->dimension_w = environment_attributes->current_w;
        camera_view->dimension_h = environment_attributes->current_h;
        for (index_positioning = 0; scoped_positioning_elements[index_positioning].description; ++index_positioning)
          if (f_string_strcmp(scoped_positioning_elements[index_positioning].description, position) == 0) {
            current_spot = scoped_positioning_elements[index_positioning].spot;
            break;
          }
        switch (current_spot) {
          case e_camera_spot_half_bottom:
            camera_view->position_y = (environment_attributes->current_h / 2.0);
          case e_camera_spot_half_top:
            camera_view->dimension_h = (environment_attributes->current_h /2.0);
            break;
          case e_camera_spot_top_left_quarter:
            camera_view->dimension_w = (environment_attributes->current_w / 2.0);
            camera_view->dimension_h = (environment_attributes->current_h / 2.0);
            break;
          case e_camera_spot_top_right_quarter:
            camera_view->position_x = (environment_attributes->current_w / 2.0);
            camera_view->dimension_w = (environment_attributes->current_w / 2.0);
            camera_view->dimension_h = (environment_attributes->current_h / 2.0);
            break;
          case e_camera_spot_bottom_left_quarter:
            camera_view->position_y = (environment_attributes->current_h / 2.0);
            camera_view->dimension_w = (environment_attributes->current_w / 2.0);
            camera_view->dimension_h = (environment_attributes->current_h / 2.0);
            break;
          case e_camera_spot_bottom_right_quarter:
            camera_view->position_x = (environment_attributes->current_w / 2.0);
            camera_view->position_y = (environment_attributes->current_h / 2.0);
            camera_view->dimension_w = (environment_attributes->current_w / 2.0);
            camera_view->dimension_h = (environment_attributes->current_h / 2.0);
          default: 
            break;
        }
        if ((camera_view->camera = f_camera_new(d_new(camera), camera_view->position_x, camera_view->position_y, camera_view->dimension_w,
                camera_view->dimension_h, camera_view->surface, game_object_attributes->environment))) {
          d_call(camera_view->camera, m_camera_set_reference, camera_view->scale_resolution_x, camera_view->scale_resolution_y);
          d_call(camera_view->camera, m_camera_set_center, camera_view->center_x, camera_view->center_y);
          d_call(camera_view->camera, m_camera_set_zoom, camera_view->zoom);
          d_call(game_object_attributes->environment, m_environment_add_camera, camera_view->string_label, 
              camera_view->camera);
          f_list_append(&(camera_crafter_attributes->views), (struct s_list_node *)camera_view, e_list_insert_tail);
        } else
          d_die(d_error_malloc);
      } else
        d_die(d_error_malloc);
    } else 
      d_die(d_error_malloc);
  } d_game_unlock(self);
  return self;
}
d_define_method(camera_crafter, get_view)(struct s_object *self, const char *key) {
  d_using(camera_crafter);
  struct s_camera_crafter_view *current_camera_view = NULL;
  d_game_lock(self) {
    d_foreach(&(camera_crafter_attributes->views), current_camera_view, struct s_camera_crafter_view)
      if (f_string_strcmp(current_camera_view->label, key) == 0)
        break;
  } d_game_unlock(self);
  return (current_camera_view)?current_camera_view->camera:NULL;
}
d_define_method(camera_crafter, get_view_struct)(struct s_object *self, const char *key) {
  d_using(camera_crafter);
  struct s_camera_crafter_view *current_camera_view = NULL;
  d_game_lock(self) {
    d_foreach(&(camera_crafter_attributes->views), current_camera_view, struct s_camera_crafter_view)
      if (f_string_strcmp(current_camera_view->label, key) == 0)
        break;
  } d_game_unlock(self);
  d_cast_return(current_camera_view);
}
d_define_method(camera_crafter, set_view_zoom)(struct s_object *self, const char *key, double zoom) {
  struct s_camera_crafter_view *current_camera_view;
  d_game_lock(self) {
    if ((current_camera_view = (struct s_camera_crafter_view *)d_call(self, m_camera_crafter_get_view_struct, key))) {
      d_call(current_camera_view->camera, m_camera_set_zoom, zoom);
      current_camera_view->zoom = zoom;
    }
  } d_game_unlock(self);
  return self;
}
d_define_method(camera_crafter, set_view_reference)(struct s_object *self, const char *key, double scale_resolution_x, double scale_resolution_y) {
  struct s_camera_crafter_view *current_camera_view;
  d_game_lock(self) {
    if ((current_camera_view = (struct s_camera_crafter_view *)d_call(self, m_camera_crafter_get_view_struct, key))) {
      d_call(current_camera_view->camera, m_camera_set_reference, scale_resolution_x, scale_resolution_y);
      current_camera_view->scale_resolution_x = scale_resolution_x;
      current_camera_view->scale_resolution_y = scale_resolution_y;
    }
  } d_game_unlock(self);
  return self;
}
d_define_method(camera_crafter, del_view)(struct s_object *self, const char *key) {
  d_using(camera_crafter);
  d_using(game_object);
  struct s_camera_crafter_view *current_camera_view;
  struct s_camera_crafter_modificator *current_camera_modificator;
  d_game_lock(self) {
    if ((current_camera_view = (struct s_camera_crafter_view *)d_call(self, m_camera_crafter_get_view_struct, key))) {
      f_list_delete(&(camera_crafter_attributes->views), (struct s_list_node *)current_camera_view);
      d_call(game_object_attributes->environment, m_environment_del_camera, current_camera_view->string_label);
      d_delete(current_camera_view->string_label);
      d_delete(current_camera_view->camera);
      while ((current_camera_modificator = (struct s_camera_crafter_modificator *)current_camera_view->modificators.head)) {
        f_list_delete(&(current_camera_view->modificators), (struct s_list_node *)current_camera_modificator);
        d_delete(current_camera_modificator->string_label);
        d_delete(current_camera_modificator->modificator);
        d_free(current_camera_modificator);
      }
      d_free(current_camera_view);
    }
  } d_game_unlock(self);
  return self;
}
d_define_method(camera_crafter, update)(struct s_object *self, const char *label_camera) {
  return self; 
}
d_define_method(camera_crafter, delete)(struct s_object *self, struct s_camera_crafter_attributes *attributes) {
  d_using(game_object);
  struct s_camera_crafter_view *current_camera_view;
  struct s_camera_crafter_modificator *current_camera_modificator;
  while ((current_camera_view = (struct s_camera_crafter_view *)attributes->views.head)) {
    f_list_delete(&(attributes->views), (struct s_list_node *)current_camera_view);
    d_call(game_object_attributes->environment, m_environment_del_camera, current_camera_view->string_label);
    d_delete(current_camera_view->string_label);
    d_delete(current_camera_view->camera);
    while ((current_camera_modificator = (struct s_camera_crafter_modificator *)current_camera_view->modificators.head)) {
      f_list_delete(&(current_camera_view->modificators), (struct s_list_node *)current_camera_modificator);
      d_delete(current_camera_modificator->string_label);
      d_delete(current_camera_modificator->modificator);
      d_free(current_camera_modificator);
    }
    d_free(current_camera_view);
  }
  return NULL;
}
d_define_class(camera_crafter) { d_hook_method(camera_crafter, e_flag_public, add_view),
  d_hook_method(camera_crafter, e_flag_public, get_view),
  d_hook_method(camera_crafter, e_flag_public, get_view_struct),
  d_hook_method(camera_crafter, e_flag_public, set_view_zoom),
  d_hook_method(camera_crafter, e_flag_public, set_view_reference),
  d_hook_method(camera_crafter, e_flag_public, del_view),
  d_hook_method(camera_crafter, e_flag_public, update),
  d_hook_delete(camera_crafter),
  d_hook_method_tail};
