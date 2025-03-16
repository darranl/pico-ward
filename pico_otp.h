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

#ifndef PICO_OTP_H
#define PICO_OTP_H

#include <stdbool.h>
#include <stdint.h>

#include "storage.h"
#include "flash/flash.h"

struct otp_core
{
    char pin[9]; // 8 characters plus null terminator.
    // OTP Data
    uint8_t hotp_secret[20];
    uint8_t hotp_secret_length;
    uint64_t hotp_counter;
    flash_context_t *flash_context;  // Maybe later we will make the storage more abstract.
    storage_context_t *storage_context; // This should become the primary mechanism to interact with storage.
};

typedef struct otp_core otp_core_t;

bool pico_otp_validate_pin(otp_core_t *otp_core, char *pin);

void pico_otp_set_pin(otp_core_t *otp_core, char *pin);

void pico_otp_set_hotp_secret(otp_core_t *otp_core, uint8_t *hotp_secret, uint8_t hotp_secret_length);

bool pico_otp_configured(otp_core_t *otp_core);

void pico_otp_calculate(otp_core_t *otp_core, char *otp);

void pico_otp_flash_device_info(otp_core_t *otp_core, flash_device_info_t *device_info);

#endif // OTP_H