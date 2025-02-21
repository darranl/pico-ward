/* Copyright 2024, Darran A Lofthouse
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

#include "otp_mgr.h"
#include "pico_otp.h"
#include "bsp/board.h"
#include "tusb.h"
#include "pico/stdlib.h"

#include "term/vt102.h"
#include "term/terminal_buffer.h"
#include "term/terminal_handler.h"
#include "util/hexutil.h"


int main()
{
    board_init();

    // init device stack on configured roothub port
    tud_init(BOARD_TUD_RHPORT);

    printf("\n\n\nHello World, welcome to my terminal playground !!!\n\n");

    const uint led = PICO_DEFAULT_LED_PIN;
    gpio_init(led);
    gpio_set_dir(led, GPIO_OUT);

    uint32_t i;

    // This struct contains data such as the management pin and the HOTP secret / counter,
    // for now these are initialised as the program starts but later this data will be retrieved
    // from storage possibly on demand.
    struct otp_core otp_core;
    // Clear the pin - TODO - This is a temporary measure to ensure the pin is cleared.
    for (i = 0; i < 9; i++)
    {
        otp_core.pin[i] = 0x00;
    }
    otp_core.hotp_secret_length = 0;

    strncpy(otp_core.pin, "123456", 6);

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






