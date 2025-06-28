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

// Definition of the common context struct that will be used by all components.

#ifndef OTP_CONTEXT_H
#define OTP_CONTEXT_H

// The otp_context struct is passed to the individual components during normal operation,
// components should only access their own context and should not access the context of other components.
// This is to ensure that the components are decoupled and can be modified independently.
// Components can however use the otp_context struct to call the other components.

struct otp_context
{
    void *otp_admin_context;
    void *otp_status_context;
    void *storage_context;
    void *primary_display_context;
    void *user_input_context;
    void *otp_main_context;
};

typedef struct otp_context otp_context_t;

#endif // OTP_CONTEXT_H