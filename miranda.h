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
#ifndef game_miranda_h
#define game_miranda_h
#include <miranda/ground.h>
#include <miranda/objects/objects.h>
#include <miranda/objects/io/io.h>
#include <miranda/objects/geometry/geometry.h>
#include <miranda/objects/media/media.h>
#include <miranda/objects/media/cameras/cameras.h>
#include <miranda/objects/media/ui/ui.h>
#include <miranda/objects/network/network.h>
#include <miranda/endian.local.h>
#include "game_object.obj.h"
#define d_game_game_configuration "game_config"
#define d_game_ui_configuration "game_ui"
extern double v_game_width_window, v_game_height_window, v_game_width_reference, v_game_height_reference,
       v_game_scale_factor, v_game_effects_volume, v_game_music_volume, v_game_language;
extern t_boolean v_game_fullscreen, v_game_developer;
extern const char *v_game_save_file;
#endif

