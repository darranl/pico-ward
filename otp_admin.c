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
#include "otp_main.h"
#include "otp_mgr.h"

#define OTP_ADMIN_CONTEXT_ID 0xAB
struct otp_admin_context
{
    char id;
    void *otp_mgr_context;
    bool handle_event_required;

};

void* otp_admin_init()
{
    struct otp_admin_context *context = malloc(sizeof(struct otp_admin_context));
    context->id = OTP_ADMIN_CONTEXT_ID;

    context->otp_mgr_context = otp_mgr_init();
    context->handle_event_required = false;
    return context;

}

bool otp_admin_begin(otp_context_t *otp_context)
{
    struct otp_admin_context *context = (struct otp_admin_context*)otp_context->otp_admin_context;
    if (context->id != OTP_ADMIN_CONTEXT_ID)
    {
        printf("Invalid context passed to otp_admin_begin 0x%02x\n", context->id);
        return false;
    }

    otp_core_t *otp_core = otp_main_get_otp_core(otp_context);
    otp_mgr_begin(context->otp_mgr_context, otp_core);

    return true;
}

void otp_admin_run(otp_context_t *otp_context)
{
    struct otp_admin_context *context = (struct otp_admin_context*)otp_context->otp_admin_context;
    if (context->id != OTP_ADMIN_CONTEXT_ID)
    {
        printf("Invalid context passed to otp_admin_run 0x%02x\n", context->id);
        return;
    }

    otp_mgr_run(context->otp_mgr_context, context->handle_event_required);
    context->handle_event_required = false; // Reset the flag after handling the event.
}


void otp_admin_notify(otp_context_t *otp_context)
{
    struct otp_admin_context *context = (struct otp_admin_context*)otp_context->otp_admin_context;
    if (context->id != OTP_ADMIN_CONTEXT_ID)
    {
        printf("Invalid context passed to otp_admin_notify 0x%02x\n", context->id);
        return;
    }

    // Set the flag to indicate that an event needs to be handled.
    context->handle_event_required = true;
}