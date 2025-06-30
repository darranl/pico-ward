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

#ifndef PICO_WARD_H
#define PICO_WARD_H

struct common_context
{
    char id;
};

typedef struct common_context pico_ward_context_t;

typedef struct common_context otp_admin_context_t;
typedef struct common_context otp_status_context_t;
typedef struct common_context otp_storage_context_t;
typedef struct common_context otp_display_context_t;
typedef struct common_context otp_input_context_t;
typedef struct common_context otp_main_context_t;


otp_admin_context_t* access_otp_admin_context(pico_ward_context_t *context);
otp_status_context_t* access_otp_status_context(pico_ward_context_t *context);
otp_storage_context_t* access_otp_storage_context(pico_ward_context_t *context);
otp_display_context_t* access_otp_display_context(pico_ward_context_t *context);
otp_input_context_t* access_otp_input_context(pico_ward_context_t *context);
otp_main_context_t* access_otp_main_context(pico_ward_context_t *context);

#endif