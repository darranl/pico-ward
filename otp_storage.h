/* Copyright 2025, Darran A Lofthouse
 *
 * This file is part of pico-ward.
 *
 * pico-sha-test is free software: you can redistribute it and/or modify it under the terms
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

// OTP Storage is responsible for managing the storage requirements of the project.

#ifndef OTP_STORAGE_H
#define OTP_STORAGE_H

#include <stdbool.h>

#include "flash/flash.h" // TODO TEMP Remove
#include "pico_ward.h"
#include "storage.h" // TODO TEMP Remove

/*
 * This function initialises the OTP storage component.
 * It should be called once at the start of the program.
 *
 * @returns A pointer to the OTP storage context.
*/
otp_storage_context_t* otp_storage_init();

/*
 * This function begins the OTP storage indicator, it may rely on
 * other components being initialised first.
 *
 * @returns true if the storage component was successfully started, false otherwise.
*/
bool otp_storage_begin(pico_ward_context_t *pico_ward_context);

/**
 * This is the main "run" handler for the storage component, it will be
 * called in the main loop of the program.
 */
void otp_storage_run(otp_storage_context_t *storage_context);

// TODO TEMP REMOVE
flash_context_t* otp_storage_get_flash_context(otp_storage_context_t *storage_context);
storage_context_t* otp_storage_get_storage_context(otp_storage_context_t *storage_context);

#endif // OTP_STORAGE_H