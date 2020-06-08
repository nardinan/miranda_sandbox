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
#include "loader.obj.h"
struct s_object *loader;
struct s_object *component_label;
struct s_lights_emitter *master_emitter;
t_boolean v_continue_execution = d_true, v_initialized = d_false, v_launched = d_false, v_script_launched = d_false;
struct timespec last_time;
unsigned int count_frames;
int game_load_call_start(struct s_object *environment) {
  struct s_loader_attributes *loader_attributes;
  if ((loader = f_loader_new(d_new(loader), environment))) {
    loader_attributes = d_cast(loader, loader);
    d_call(loader, m_runnable_run, NULL);
    d_call(loader, m_runnable_join, NULL);
    d_assert(master_camera_crafter = f_camera_crafter_new(d_new(camera_crafter), environment));
    d_assert(component_label = d_call(loader_attributes->ui_factory, m_ui_factory_new_label, d_ui_factory_default_font_id,
          d_ui_factory_default_font_style, "FPS: ?"));
    d_call(environment, m_environment_add_drawable, component_label, d_ui_factory_default_level, e_environment_surface_ui);
  }
  return d_true;
}
int game_loop_call(struct s_object *environment) {
  if (component_label) {
    struct timespec current_time;
    double delta_milliseconds;
    char buffer[d_string_buffer_size];
    clock_gettime(CLOCK_MONOTONIC_RAW, &current_time);
    delta_milliseconds = ((current_time.tv_sec - last_time.tv_sec) * 1000 + (current_time.tv_nsec - last_time.tv_nsec) / 1000000);
    if (delta_milliseconds > 1000) {
      memcpy(&last_time, &current_time, sizeof(struct timespec));
      delta_milliseconds /= (double)count_frames;
      snprintf(buffer, d_string_buffer_size, "FPS: %.02f", (1000 / delta_milliseconds));
      d_call(component_label, m_label_set_content_char, buffer, NULL, environment);
      count_frames = 1;
    } else
      ++count_frames;
  }
  return v_continue_execution;
}
int game_quit_call(struct s_object *environment) {
  if (!v_initialized)
    d_call(loader, m_runnable_join, NULL);
  d_delete(loader);
  return d_true;
}
void game_change_location(const char *application) {
  char buffer[PATH_MAX], *pointer;
  if ((pointer = strrchr(application, '/'))) {
    memset(buffer, 0, PATH_MAX);
    strncpy(buffer, application, (pointer - application));
    chdir(buffer);
    d_log(e_log_level_ever, "changing local path to: %s", buffer);
  }
}
int main(int argc, char *argv[]) {
  struct s_exception *exception;
  struct s_environment_attributes *environment_attributes;
  struct s_object *environment;
  struct s_object *stream_configuration;
  struct s_object *json_configuration;
  double scale_resolution_x, scale_resolution_y;
  d_pool_init;
  d_pool_begin("main context") {
    v_log_level = e_log_level_low;
    /* change chmod to the current location of the application */
    game_change_location(argv[0]);
    d_try{
      if ((stream_configuration = f_stream_new_file(d_new(stream), d_pkstr(d_game_resources_configuration), "r", 0777))) {
        if ((json_configuration = f_json_new_stream(d_new(json), stream_configuration))) {
          d_call(json_configuration, m_json_get_double, &v_game_width_window, "ss", "window", "width");
          d_call(json_configuration, m_json_get_double, &v_game_height_window, "ss", "window", "height");
          d_call(json_configuration, m_json_get_boolean, &v_game_fullscreen, "ss", "window", "fullscreen");
          d_call(json_configuration, m_json_get_double, &v_game_width_reference, "ss", "geometry", "width");
          d_call(json_configuration, m_json_get_double, &v_game_height_reference, "ss", "geometry", "height");
          d_call(json_configuration, m_json_get_double, &v_game_scale_factor, "ss", "geometry", "scale");
          d_call(json_configuration, m_json_get_double, &v_game_effects_volume, "s", "volume_effects");
          d_call(json_configuration, m_json_get_double, &v_game_music_volume, "s", "volume_music");
          d_call(json_configuration, m_json_get_double, &v_game_language, "s", "language");
          d_delete(json_configuration);
        }
        d_delete(stream_configuration);
      }
      scale_resolution_x = (v_game_width_reference * v_game_scale_factor);
      scale_resolution_y = (v_game_height_reference * v_game_scale_factor);
      d_assert(environment = f_environment_new_fullscreen(d_new(environment), v_game_width_window, v_game_height_window,
            v_game_fullscreen));
      d_assert((master_camera_crafter = f_camera_crafter_new(d_new(camera_crafter), environment)));
      d_call(environment, m_environment_set_methods, &game_load_call_start, &game_loop_call, &game_quit_call);
      environment_attributes = d_cast(environment, environment);
      d_call(master_camera_crafter, m_camera_crafter_add_view, "draw_view", "fullscreen", (double)scale_resolution_x, 
          (double)scale_resolution_y, 0.0, 0.0, 4.0, e_environment_surface_primary);
      d_call(master_camera_crafter, m_camera_crafter_add_view, "ui_view",   "fullscreen", (double)scale_resolution_x,
          (double)scale_resolution_y, 0.0, 0.0, 1.0, e_environment_surface_ui);
      d_call(environment, m_environment_set_title, d_game_title);
      d_call(environment, m_environment_set_channels, d_media_factory_max_channels);
      d_call(environment, m_environment_run_loop, NULL);
      /* unfortunately, due to the fact that the cameras might hold some resources, we need to 
       * remove them in order to prevent the reference counter to not being able to delete
       * the resources.
       */
      d_delete(environment);
    } d_catch(exception)
    {
      d_exception_dump(stderr, exception);
    } d_endtry;
  } d_pool_end;
  d_pool_destroy;
  if (v_memory_bucket)
    f_memory_bucket_destroy(&v_memory_bucket);
  f_memory_destroy(d_false);
  return 0;
}
