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
#include "otp_admin.h"
#include "otp_context.h"
#include "otp_display.h"
#include "otp_input.h"
#include "otp_main.h"
#include "otp_mgr.h"
#include "otp_status.h"
#include "otp_storage.h"
#include "pico_otp.h"
#include "bsp/board.h"
#include "storage.h"
#include "tusb.h"
#include "pico/stdlib.h"

#include "flash/flash.h"
#include "term/vt102.h"
#include "term/terminal_buffer.h"
#include "term/terminal_handler.h"
#include "util/hexutil.h"

int main()
{
    board_init();

    // init device stack on configured roothub port
    tud_init(BOARD_TUD_RHPORT);

    printf("\n\n\nHello World, welcome to pico-ward !!!\n\n");
    otp_context_t otp_context;

    // Phase 1 - Initialise the contexts for the individual components.
    //           This is a first pass initialisation of components independent
    //           of each other.
    // 1. OTP Admin (USB Admin Interface)
    otp_context.otp_admin_context = otp_admin_init();
    // 2. Status Indicator (LED(s))
    otp_context.otp_status_context = otp_status_init();
    // 3. Storage (Flash)
    otp_context.storage_context = otp_storage_init();
    // 4. Primary Display (LED displays, 7-segment displays, etc)
    otp_context.primary_display_context = otp_display_init();
    // 5. User Input (Buttons / Rotary Encoders)
    otp_context.user_input_context = otp_input_init();
    // 6. Pico OTP (Main OTP Core)
    otp_context.otp_core_context = otp_main_init();

    // Phase 2 - Begin each component.
    //           At this stage the components may interact to complete their
    //           initialisation routines.
    //
    //           Components should not assume other components are completly
    //           ready until we reach the run stage but this stage can be
    //           used to check default state / register callbacks etc...
    bool success = true;
    // 1. OTP Admin (USB Admin Interface)
    success = success && otp_admin_begin(&otp_context);
    // 2. Status Indicator (LED(s))
    success = success && otp_status_begin(&otp_context);
    // 3. Storage (Flash)
    success = success && otp_storage_begin(&otp_context);
    // 4. Primary Display (LED displays, 7-segment displays, etc)
    success = success && otp_display_begin(&otp_context);
    // 5. User Input (Buttons / Rotary Encoders)
    success = success && otp_input_begin(&otp_context);
    // 6. Pico OTP (Main OTP Core)
    success = success && otp_main_begin(&otp_context);

    if (!success)
    {
        printf("Failed to initialise all components\n");
        return -1;
    }

    // Phase 3 - Begin the main loop.
    while (true)
    {
        // 1. OTP Admin (USB Admin Interface)
        otp_admin_run(&otp_context);
        // 2. Status Indicator (LED(s))
        otp_status_run(&otp_context);
        // 3. Storage (Flash)
        otp_storage_run(&otp_context);
        // 4. Primary Display (LED displays, 7-segment displays, etc)
        otp_display_run(&otp_context);
        // 5. User Input (Buttons / Rotary Encoders)
        otp_input_run(&otp_context);
        // 6. Pico OTP (Main OTP Core)
        otp_main_run(&otp_context);
    }

    while(true)
    {
        tud_task(); // This is always needed in the outer loop so we can detect a connect event.

        if (tud_cdc_n_connected(CDC_INTF))
        {
            // For now assume CDC mode is a dedicated setup mode.
            //gpio_put(led, true);
            //otp_mgr_begin(&otp_core);
            //gpio_put(led, false);
        }
    }
}






