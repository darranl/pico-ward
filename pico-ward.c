/* Copyright 2024, Darran A Lofthouse
 *
 * This file is part of pico-ward.
 *
 * pico-ward is free software: you can redistribute it and/or modify it under the terms
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

#include "hardware_map.h"
#include "otp_mgr.h"
#include "pico_otp.h"
#include "bsp/board.h"
#include "tusb.h"
#include "pico/stdlib.h"

#include "flash/flash.h"
#include "term/vt102.h"
#include "term/terminal_buffer.h"
#include "term/terminal_handler.h"
#include "util/hexutil.h"




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

int main()
{
    board_init();

    // init device stack on configured roothub port
    tud_init(BOARD_TUD_RHPORT);

    printf("\n\n\nHello World, welcome to pico-ward !!!\n\n");

    flash_context_t flash_context;
    _configure_flash_context(&flash_context);
    flash_spi_init(&flash_context);
    flash_reset(&flash_context);

    if (!flash_post_reset_test(&flash_context))
    {
        printf("Post Reset Test Failed\n");
        return -1;
    }

    const uint led = PICO_DEFAULT_LED_PIN;
    gpio_init(led);
    gpio_set_dir(led, GPIO_OUT);

    uint32_t i;

    // This struct contains data such as the management pin and the HOTP secret / counter,
    // for now these are initialised as the program starts but later this data will be retrieved
    // from storage possibly on demand.
    otp_core_t otp_core;
    // Clear the pin - TODO - This is a temporary measure to ensure the pin is cleared.
    for (i = 0; i < 9; i++)
    {
        otp_core.pin[i] = 0x00;
    }
    otp_core.hotp_secret_length = 0;

    strncpy(otp_core.pin, "123456", 6);
    // Further OTP Core Initialisation
    otp_core.flash_context = &flash_context;

    while(true)
    {
        tud_task(); // This is always needed in the outer loop so we can detect a connect event.

        if (tud_cdc_n_connected(CDC_INTF))
        {
            // For now assume CDC mode is a dedicated setup mode.
            gpio_put(led, true);
            otp_mgr_begin(&otp_core);
            gpio_put(led, false);
        }
    }
}






