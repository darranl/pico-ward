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
#include "otp_main.h"
#include "otp_mgr.h"

struct otp_admin_context
{
    void *otp_mgr_context;

};

void* otp_admin_init()
{
    struct otp_admin_context *context = malloc(sizeof(struct otp_admin_context));

    context->otp_mgr_context = otp_mgr_init();
    return context;

}

bool otp_admin_begin(otp_context_t *otp_context)
{
    struct otp_admin_context *context = (struct otp_admin_context*)otp_context->otp_admin_context;

    otp_core_t *otp_core = otp_main_get_otp_core(otp_context);
    otp_mgr_beginII(context->otp_mgr_context, otp_core);

    return true;
}

void otp_admin_run(otp_context_t *otp_context)
{
    struct otp_admin_context *context = (struct otp_admin_context*)otp_context->otp_admin_context;
    otp_mgr_run(context->otp_mgr_context);
}