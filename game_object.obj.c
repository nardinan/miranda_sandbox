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
#include "game_object.obj.h"
struct s_game_object_attributes *p_game_object_alloc(struct s_object *self) {
  struct s_game_object_attributes *result = d_prepare(self, game_object);
  f_memory_new(self);     /* inherit */
  f_mutex_new(self);      /* inherit */
  return result;
}
struct s_object *f_game_object_new(struct s_object *self, struct s_object *environment) {
  struct s_game_object_attributes *attributes = p_game_object_alloc(self);
  if ((attributes->lock = f_lock_per_thread_new(d_new(lock))))
    attributes->environment = d_retain(environment);
  else
    d_die(d_error_malloc);
  return self;
}
d_define_method(game_object, lock)(struct s_object *self, const char *file, const char *function, unsigned int line) {
  d_using(game_object);
  d_call(game_object_attributes->environment, m_environment_lock_loop, file, function, line);
  return self;
}
d_define_method(game_object, unlock)(struct s_object *self, const char *file, const char *function, unsigned int line) {
  d_using(game_object);
  d_call(game_object_attributes->environment, m_environment_unlock_loop, file, function, line);
  return self;
}
d_define_method(game_object, delete)(struct s_object *self, struct s_game_object_attributes *attributes) {
  d_delete(attributes->environment);
  d_delete(attributes->lock);
  return NULL;
}
d_define_class(game_object) {d_hook_method(game_object, e_flag_public, lock),
  d_hook_method(game_object, e_flag_public, unlock),
  d_hook_delete(game_object),
  d_hook_method_tail};
