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

// OTP main is responsible for managing the main requirements of the project.

#ifndef OTP_MAIN_H
#define OTP_MAIN_H

#include "pico_otp.h"

#include <stdbool.h>

#include "pico_ward.h"

/*
 * This function initialises the OTP main component.
 * It should be called once at the start of the program.
 *
 * @returns A pointer to the OTP main context.
*/
otp_main_context_t* otp_main_init();

/*
 * This function begins the OTP main indicator, it may rely on
 * other components being initialised first.
 *
 * @returns true if the main component was successfully started, false otherwise.
*/
bool otp_main_begin(pico_ward_context_t *pico_ward_context);

/**
 * This is the main "run" handler for the main component, it will be
 * called in the main loop of the program.
 */
void otp_main_run(otp_main_context_t *main_context);

// TODO - This will go - instead callers should obtain a reference to the context for OTP main.
otp_core_t* otp_main_get_otp_core(otp_main_context_t *main_context);

#endif // OTP_MAIN_H