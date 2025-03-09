/* Copyright 2025, Darran A Lofthouse
 *
 * This file is part of pico-ward.
 *
 * pico-ward is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * pico-ward is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with pico-ward.
 * If  not, see <https://www.gnu.org/licenses/>.
*/

#ifndef STORAGE_H
#define STORAGE_H

#include <stdbool.h>

#include "flash/flash.h"


struct storage_context
{
    flash_context_t *flash_context;
    bool initialised;

};

typedef struct storage_context storage_context_t;

/*
 * Establish the storage context and detect if it has been initialised.
*/
void storage_begin(storage_context_t *context, flash_context_t *flash_context);

#endif // STORAGE_H
