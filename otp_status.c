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

#include "hardware/gpio.h"
#include "pico_ward.h"

#define OTP_STATUS_CONTEXT_ID 0xB0
struct _otp_status_context
{
    struct common_context common_context;
    uint led;
};

otp_status_context_t* otp_status_init()
{
    struct _otp_status_context *context = malloc(sizeof(struct _otp_status_context));
    context->common_context.id = OTP_STATUS_CONTEXT_ID;

    context->led = PICO_DEFAULT_LED_PIN;
    gpio_init(context->led);
    gpio_set_dir(context->led, GPIO_OUT);
    gpio_put(context->led, false);

    return (otp_status_context_t*)context;
}

bool otp_status_begin(pico_ward_context_t *pico_ward_context)
{
    struct _otp_status_context *context = (struct _otp_status_context*) access_otp_status_context(pico_ward_context);
    if (context->common_context.id != OTP_STATUS_CONTEXT_ID)
    {
        printf("Invalid context passed to otp_status_begin 0x%02x\n", context->common_context.id);
        return false;
    }

    return true;
}

void otp_status_run(otp_status_context_t *status_context)
{
    if (status_context->id != OTP_STATUS_CONTEXT_ID)
    {
        printf("Invalid context passed to otp_status_run 0x%02x\n", status_context->id);
        return;
    }

    struct _otp_status_context *context = (struct _otp_status_context*)status_context;


}