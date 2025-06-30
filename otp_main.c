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
#include <stdio.h>

#include "otp_storage.h"
#include "pico_otp.h" // TODO Will this component replase pico_otp? 20250628 Yes I think so.
#include "pico_ward.h"

#define OTP_MAIN_CONTEXT_ID 0xAF

#define TASK_NONE = 0x00
#define TASK_CHECK_PIN = 0x01

struct _otp_main_context
{
    struct common_context common_context;
    otp_core_t otp_core;

};

struct base_task
{
    uint8_t task_id; // The task ID.
};

otp_main_context_t* otp_main_init()
{
    struct _otp_main_context *context = malloc(sizeof(struct _otp_main_context));
    context->common_context.id = OTP_MAIN_CONTEXT_ID;
    context->otp_core.id = OTP_CORE_CONTEXT_ID;

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

    return (otp_main_context_t*) context;
}

bool otp_main_begin(pico_ward_context_t *pico_ward_context)
{
    struct _otp_main_context *context = (struct _otp_main_context*) access_otp_main_context(pico_ward_context);
    if (context->common_context.id != OTP_MAIN_CONTEXT_ID)
    {
        printf("Invalid context passed to otp_main_begin 0x%02x\n", context->common_context.id);
        return false;
    }

    otp_storage_context_t *storage_context = access_otp_storage_context(pico_ward_context);

    otp_core_t *otp_core = &context->otp_core;
    // Further OTP Core Initialisation
    otp_core->flash_context = otp_storage_get_flash_context(storage_context);
    otp_core->storage_context = otp_storage_get_storage_context(storage_context);

    return true;
}

void otp_main_run(otp_main_context_t *main_context)
{
    if (main_context->id != OTP_MAIN_CONTEXT_ID)
    {
        printf("Invalid context passed to otp_main_run 0x%02x\n", main_context->id);
        return;
    }

    struct _otp_main_context *context = (struct _otp_main_context*)main_context;



}

otp_core_t* otp_main_get_otp_core(otp_main_context_t *main_context)
{
    if (main_context->id != OTP_MAIN_CONTEXT_ID)
    {
        printf("Invalid context passed to otp_main_get_otp_core 0x%02x\n", main_context->id);
        return 0x00;
    }

    struct _otp_main_context *context = (struct _otp_main_context*)main_context;

    return &context->otp_core;
}