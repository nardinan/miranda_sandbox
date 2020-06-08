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
#ifndef game_game_object_h
#define game_game_object_h
#include "miranda.h"
d_declare_class(game_object) {
  struct s_attributes head;
  struct s_object *environment;
  struct s_object *lock;
} d_declare_class_tail(game_object);
struct s_game_object_attributes *p_game_object_alloc(struct s_object *self);
extern struct s_object *f_game_object_new(struct s_object *self, struct s_object *environment);
d_declare_method(game_object, lock)(struct s_object *self, const char *file, const char *function, unsigned int line);
#define d_game_lock(_s)\
  d_call(_s, m_game_object_lock, __FILE__, __FUNCTION__, __LINE__);\
do
d_declare_method(game_object, unlock)(struct s_object *self, const char *file, const char *function, unsigned int line);
#define d_game_unlock(_s)\
  while(0);\
d_call(_s, m_game_object_unlock, __FILE__, __FUNCTION__, __LINE__)
d_declare_method(game_object, delete)(struct s_object *self, struct s_game_object_attributes *attributes);
#endif
