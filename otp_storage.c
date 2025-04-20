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
#include "otp_context.h"
#include "storage.h" // TODO Should merge here.

struct otp_storage_context
{
    flash_context_t flash_context;
    bool flash_initialised;
    storage_context_t storage_context;
};

static void _configure_flash_context(flash_context_t *flash_context);

void* otp_storage_init()
{
    struct otp_storage_context *context = malloc(sizeof(struct otp_storage_context));

    _configure_flash_context(&context->flash_context);
    flash_spi_init(&context->flash_context);
    flash_reset(&context->flash_context);

    context->flash_initialised = flash_post_reset_test(&context->flash_context);

    if (context->flash_initialised)
    {
        storage_begin(&context->storage_context, &context->flash_context);
        printf("Storage Initialised = %d\n", &context->storage_context.initialised);
    }

    return context;
}

bool otp_storage_begin(otp_context_t *otp_context)
{
    struct otp_storage_context *context = (struct otp_storage_context*)otp_context->storage_context;
    if (!context->flash_initialised)
    {
        printf("Flash not initialised\n");
        return false;
    }

    return true;
}

void otp_storage_run(otp_context_t *otp_context)
{
    struct otp_storage_context *context = (struct otp_storage_context*)otp_context->storage_context;

}

flash_context_t* otp_storage_get_flash_context(otp_context_t *otp_context)
{
    struct otp_storage_context *context = (struct otp_storage_context*)otp_context->storage_context;
    return &context->flash_context;
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