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

#include "pico_ward.h"

#define OTP_DISPLAY_CONTEXT_ID 0xAD
struct _otp_display_context
{
    struct common_context common_context;

};

otp_display_context_t* otp_display_init()
{
    struct _otp_display_context *context = malloc(sizeof(struct _otp_display_context));
    context->common_context.id = OTP_DISPLAY_CONTEXT_ID;

    return (otp_display_context_t*) context;

}

bool otp_display_begin(pico_ward_context_t *pico_ward_context)
{
    struct _otp_display_context *context = (struct _otp_display_context*) access_otp_display_context(pico_ward_context);
    if (context->common_context.id != OTP_DISPLAY_CONTEXT_ID)
    {
        printf("Invalid context passed to otp_display_begin 0x%02x\n", context->common_context.id);
        return false;
    }

    return true;
}

void otp_display_run(otp_display_context_t *display_context)
{
    if (display_context->id != OTP_DISPLAY_CONTEXT_ID)
    {
        printf("Invalid context passed to otp_display_run 0x%02x\n", display_context->id);
        return;
    }

    struct _otp_display_context *context = (struct _otp_display_context*)display_context;


}