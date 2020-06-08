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
#include "miranda.h"
/* default values, replaced by the values into the configuration file */
double v_game_width_window = 1280, v_game_height_window = 720, v_game_width_reference = 1280,
       v_game_height_reference = 720, v_game_scale_factor = 1.0, v_game_effects_volume = 1.0, v_game_music_volume = 1.0,
       v_game_language = 1.0;
t_boolean v_game_fullscreen = d_false, v_game_developer = d_false;
const char *v_game_save_file = "./data/saving_file.ini";
