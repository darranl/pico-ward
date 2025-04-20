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

#include <stdlib.h>
#include <stdbool.h>

#include "otp_context.h"

struct otp_display_context
{

};

void* otp_display_init()
{
    struct otp_display_context *context = malloc(sizeof(struct otp_display_context));

    return context;

}

bool otp_display_begin(otp_context_t *otp_context)
{
    struct otp_display_context *context = (struct otp_display_context*)otp_context->primary_display_context;

    return true;
}

void otp_display_run(otp_context_t *otp_context)
{
    struct otp_display_context *context = (struct otp_display_context*)otp_context->primary_display_context;

}