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

// OTP input is responsible for managing the input requirements of the project.

#ifndef OTP_INPUT_H
#define OTP_INPUT_H

#include <stdbool.h>

#include "pico_ward.h"

/*
 * This function initialises the OTP input component.
 * It should be called once at the start of the program.
 *
 * @returns A pointer to the OTP input context.
*/
otp_input_context_t* otp_input_init();

/*
 * This function begins the OTP input indicator, it may rely on
 * other components being initialised first.
 *
 * @returns true if the input component was successfully started, false otherwise.
*/
bool otp_input_begin(pico_ward_context_t *pico_ward_context);

/**
 * This is the main "run" handler for the input component, it will be
 * called in the main loop of the program.
 */
void otp_input_run(otp_input_context_t *input_context);

#endif // OTP_INPUT_H