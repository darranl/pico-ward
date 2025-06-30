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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "flash/flash.h"
#include "hardware_map.h"
#include "pico_ward.h"
#include "storage.h" // TODO Should merge here.

#define OTP_STORAGE_CONTEXT_ID 0xB1
struct _otp_storage_context
{
    struct common_context common_context;
    flash_context_t flash_context;
    bool flash_initialised;
    storage_context_t storage_context;
};

static void _configure_flash_context(flash_context_t *flash_context);

otp_storage_context_t* otp_storage_init()
{
    struct _otp_storage_context *context = malloc(sizeof(struct _otp_storage_context));
    context->common_context.id = OTP_STORAGE_CONTEXT_ID;
    context->storage_context.id = STORAGE_CONTEXT_ID;

    _configure_flash_context(&context->flash_context);
    flash_spi_init(&context->flash_context);
    flash_reset(&context->flash_context);

    context->flash_initialised = flash_post_reset_test(&context->flash_context);

    if (context->flash_initialised)
    {
        storage_begin(&context->storage_context, &context->flash_context);
        printf("Storage Initialised = %d\n", &context->storage_context.initialised);
    }

    return (otp_storage_context_t*) context;
}

bool otp_storage_begin(pico_ward_context_t *pico_ward_context)
{
    struct _otp_storage_context *context = (struct _otp_storage_context*) access_otp_storage_context(pico_ward_context);
    if (context->common_context.id != OTP_STORAGE_CONTEXT_ID)
    {
        printf("Invalid context passed to otp_storage_begin 0x%02x\n", context->common_context.id);
        return false;
    }

    if (!context->flash_initialised)
    {
        printf("Flash not initialised\n");
        return false;
    }

    return true;
}

void otp_storage_run(otp_storage_context_t *storage_context)
{
    if (storage_context->id != OTP_STORAGE_CONTEXT_ID)
    {
        printf("Invalid context passed to otp_storage_run 0x%02x\n", storage_context->id);
        return;
    }

    struct _otp_storage_context *context = (struct _otp_storage_context*)storage_context;
}

flash_context_t* otp_storage_get_flash_context(otp_storage_context_t *storage_context)
{
    if (storage_context->id != OTP_STORAGE_CONTEXT_ID)
    {
        printf("Invalid context passed to otp_storage_get_flash_context 0x%02x\n", storage_context->id);
        return NULL;
    }

    struct _otp_storage_context *context = (struct _otp_storage_context*) storage_context;


    return &context->flash_context;
}

storage_context_t* otp_storage_get_storage_context(otp_storage_context_t *storage_context)
{
    if (storage_context->id != OTP_STORAGE_CONTEXT_ID)
    {
        printf("Invalid context passed to otp_storage_get_storage_context 0x%02x\n", storage_context->id);
        return NULL;
    }

    struct _otp_storage_context *context = (struct _otp_storage_context*) storage_context;

    return &context->storage_context;
}

static void _configure_flash_context(flash_context_t *flash_context)
{
    flash_context->spi = FLASH_SPI_BANK;
    flash_context->tx_pin = FLASH_TX_GPIO;
    flash_context->clk_pin = FLASH_CLK_GPIO;
    flash_context->rx_pin = FLASH_RX_GPIO;
    flash_context->hold_pin = FLASH_HOLD_GPIO;
    flash_context->wp_pin = FLASH_WP_GPIO;
    flash_context->cs_pin = FLASH_CS_GPIO;
}