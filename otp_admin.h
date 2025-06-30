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

// OTP Admin is responsible for the administrative USB interface.

#ifndef OTP_ADMIN_H
#define OTP_ADMIN_H

#include <stdbool.h>

#include "pico_ward.h"

/*
 * This function initialises the OTP admin component.
 * It should be called once at the start of the program.
 *
 * @returns A pointer to the OTP admin context.
*/
otp_admin_context_t* otp_admin_init();

/*
 * This function begins the OTP admin indicator, it may rely on
 * other components being initialised first.
 *
 * @returns true if the admin component was successfully started, false otherwise.
*/
bool otp_admin_begin(pico_ward_context_t *pico_ward_context);

/**
 * This is the main "run" handler for the admin component, it will be
 * called in the main loop of the program.
 */
void otp_admin_run(otp_admin_context_t *admin_context);

/**
 * Notify OTP admin that some asynchronous event has occurred.
 *
 * The individual handler should check if it is waiting for something.
 * If OTP Admin is not interested it will quietly be ignored.
 */
void otp_admin_notify(otp_admin_context_t *admin_context);

/**
 * Handle a notification event.
 *
 * This function will be called by the underlying OTP manager to handle any
 * previously received notification.
 *
 * Returns true if notify has been called, false otherwise. Calling this function
 * clears the notification state.
 */
bool otp_admin_handle_notification(otp_admin_context_t *admin_context);

#endif // OTP_ADMIN_H