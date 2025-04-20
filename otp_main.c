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
#include <string.h>

#include "otp_context.h"
#include "otp_storage.h"
#include "pico_otp.h" // TODO Will this component replase pico_otp?

struct otp_main_context
{
    otp_core_t otp_core;

};

void* otp_main_init()
{
    struct otp_main_context *context = malloc(sizeof(struct otp_main_context));

    uint32_t i;

    // This struct contains data such as the management pin and the HOTP secret / counter,
    // for now these are initialised as the program starts but later this data will be retrieved
    // from storage possibly on demand.
    otp_core_t *otp_core = &context->otp_core;

    // Clear the pin - TODO - This is a temporary measure to ensure the pin is cleared.
    for (i = 0; i < 9; i++)
    {
        otp_core->pin[i] = 0x00;
    }
    otp_core->hotp_secret_length = 0;

    strncpy(otp_core->pin, "123456", 6);

    return context;
}

bool otp_main_begin(otp_context_t *otp_context)
{
    struct otp_main_context *context = (struct otp_main_context*)otp_context->otp_core_context;

    otp_core_t *otp_core = &context->otp_core;
    // Further OTP Core Initialisation
    otp_core->flash_context = otp_storage_get_flash_context(otp_context);

    return true;
}

void otp_main_run(otp_context_t *otp_context)
{
    struct otp_main_context *context = (struct otp_main_context*)otp_context->otp_core_context;

}

otp_core_t* otp_main_get_otp_core(otp_context_t *otp_context)
{
    struct otp_main_context *context = (struct otp_main_context*)otp_context->otp_core_context;

    return &context->otp_core;
}