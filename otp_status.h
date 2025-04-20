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

// OTP Status is responsible for indicating status using available LEDs.
// This could be a single LED or multiple LEDs.

#ifndef OTP_STATUS_H
#define OTP_STATUS_H

#include "otp_context.h"

#include <stdbool.h>

/*
 * This function initialises the OTP status indicator.
 * It should be called once at the start of the program.
 *
 * @returns A pointer to the OTP status context.
*/
void* otp_status_init();

/*
 * This function begins the OTP status indicator, it may rely on
 * other components being initialised first.
 *
 * @returns true if the status component was successfully started, false otherwise.
*/
bool otp_status_begin(otp_context_t *otp_context);

/**
 * This is the main "run" handler for the status component, it will be
 * called in the main loop of the program.
 */
void otp_status_run(otp_context_t *otp_context);

#endif // OTP_STATUS_H