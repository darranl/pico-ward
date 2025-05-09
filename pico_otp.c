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

#include <string.h>

#include "pico_otp.h"
#include "security/hotp.h"

bool pico_otp_validate_pin(otp_core_t *otp_core, char *pin)
{
    if (otp_core->id != OTP_CORE_CONTEXT_ID)
    {
        printf("Invalid context passed to pico_otp_validate_pin 0x%02x\n", otp_core->id);
        return false;
    }

    return strncmp(otp_core->pin, pin, 9) == 0;
}

void pico_otp_set_pin(otp_core_t *otp_core, char *pin)
{
    if (otp_core->id != OTP_CORE_CONTEXT_ID)
    {
        printf("Invalid context passed to pico_otp_set_pin 0x%02x\n", otp_core->id);
        return;
    }
    // Clear Existing Pin
    for (uint32_t i = 0; i < 9; i++)
    {
        otp_core->pin[i] = 0x00;
    }
    strncpy(otp_core->pin, pin, 9);
}

void pico_otp_set_hotp_secret(otp_core_t *otp_core, uint8_t *hotp_secret, uint8_t hotp_secret_length)
{
    if (otp_core->id != OTP_CORE_CONTEXT_ID)
    {
        printf("Invalid context passed to pico_otp_set_hotp_secret 0x%02x\n", otp_core->id);
        return;
    }
    // Clear Existing Secret - TODO should more clean up elsewhere.
    for (uint32_t i = 0; i < 20; i++)
    {
        otp_core->hotp_secret[i] = 0x00;
    }
    memcpy(otp_core->hotp_secret, hotp_secret, hotp_secret_length);
    otp_core->hotp_secret_length = hotp_secret_length;
    otp_core->hotp_counter = 0; // Reset Counter
}

bool pico_otp_configured(otp_core_t *otp_core)
{
    if (otp_core->id != OTP_CORE_CONTEXT_ID)
    {
        printf("Invalid context passed to pico_otp_configured 0x%02x\n", otp_core->id);
        return false;
    }
    return otp_core->hotp_secret_length > 0;
}

void pico_otp_calculate(otp_core_t *otp_core, char *otp)
{
    if (otp_core->id != OTP_CORE_CONTEXT_ID)
    {
        printf("Invalid context passed to pico_otp_calculate 0x%02x\n", otp_core->id);
        return;
    }
    calculate_hotp(otp_core->hotp_secret, otp_core->hotp_secret_length, otp_core->hotp_counter, otp);

    // Convert to printable characters.
    for (int i=0; i<6; i++)
    {
        otp[i] += 0x30;
    }
    otp[6] = 0x00;

    otp_core->hotp_counter++;
}

void pico_otp_flash_device_info(otp_core_t *otp_core, flash_device_info_t *device_info)
{
    if (otp_core->id != OTP_CORE_CONTEXT_ID)
    {
        printf("Invalid context passed to pico_otp_flash_device_info 0x%02x\n", otp_core->id);
        return;
    }

    flash_load_device_info(otp_core->flash_context, device_info);
}

bool pico_otp_storage_initialised(otp_core_t *otp_core)
{
    if (otp_core->id != OTP_CORE_CONTEXT_ID)
    {
        printf("Invalid context passed to pico_otp_storage_initialised 0x%02x\n", otp_core->id);
        return false;
    }

    return storage_initialised(otp_core->storage_context);
}

void pico_otp_reset_storage(otp_core_t *otp_core, bool initialise)
{
    if (otp_core->id != OTP_CORE_CONTEXT_ID)
    {
        printf("Invalid context passed to pico_otp_reset_storage 0x%02x\n", otp_core->id);
        return;
    }
    printf("Resetting Storage initialise=%d\n", initialise);
    flash_chip_erase(otp_core->flash_context);

    // Run storage_begin again as this will test if storage is correctly
    // initialised.
    //storage_begin(otp_core->storage_context, otp_core->flash_context);
}

void pico_otp_flash_read_data(otp_core_t *otp_core, uint32_t address, uint8_t *data, uint32_t length)
{
    if (otp_core->id != OTP_CORE_CONTEXT_ID)
    {
        printf("Invalid context passed to pico_otp_flash_read_data 0x%02x\n", otp_core->id);
        return;
    }
    printf("Reading from address 0x%08x\n", address);
    flash_read_data(otp_core->flash_context, address, data, length);
}