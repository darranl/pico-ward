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

#define OTP_DISPLAY_CONTEXT_ID 0xAE
struct _otp_input_context
{
    struct common_context common_context;

};

otp_input_context_t* otp_input_init()
{
    struct _otp_input_context *context = malloc(sizeof(struct _otp_input_context));
    context->common_context.id = OTP_DISPLAY_CONTEXT_ID;

    return (otp_input_context_t*) context;

}

bool otp_input_begin(pico_ward_context_t *pico_ward_context_t)
{
    struct _otp_input_context *context = (struct _otp_input_context*) access_otp_input_context(pico_ward_context_t);
    if (context->common_context.id != OTP_DISPLAY_CONTEXT_ID)
    {
        printf("Invalid context passed to otp_input_begin 0x%02x\n", context->common_context.id);
        return false;
    }

    return true;
}

void otp_input_run(otp_input_context_t *input_context)
{
    if (input_context->id != OTP_DISPLAY_CONTEXT_ID)
    {
        printf("Invalid context passed to otp_input_run 0x%02x\n", input_context->id);
        return;
    }

    struct _otp_input_context *context = (struct _otp_input_context*)input_context;


}