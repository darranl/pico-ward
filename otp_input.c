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
#include <stdio.h>

#include "otp_context.h"

#define OTP_DISPLAY_CONTEXT_ID 0xAE
struct otp_input_context
{
    char id;

};

void* otp_input_init()
{
    struct otp_input_context *context = malloc(sizeof(struct otp_input_context));
    context->id = OTP_DISPLAY_CONTEXT_ID;

    return context;

}

bool otp_input_begin(otp_context_t *otp_context)
{
    struct otp_input_context *context = (struct otp_input_context*)otp_context->user_input_context;
    if (context->id != OTP_DISPLAY_CONTEXT_ID)
    {
        printf("Invalid context passed to otp_input_begin 0x%02x\n", context->id);
        return false;
    }

    return true;
}

void otp_input_run(otp_context_t *otp_context)
{
    struct otp_input_context *context = (struct otp_input_context*)otp_context->user_input_context;
    if (context->id != OTP_DISPLAY_CONTEXT_ID)
    {
        printf("Invalid context passed to otp_input_run 0x%02x\n", context->id);
        return;
    }

}