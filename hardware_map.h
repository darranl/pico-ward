/* Copyright 2025, Darran A Lofthouse
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

#ifndef HARDWARE_MAP_H
#define HARDWARE_MAP_H

#include "hardware/spi.h"

/*
 * This file is used to map the hardware to the specific pins used in the application.
 *
 * This will allow different profiles to be defined but controlled centrally.
 */


#ifndef HARDWARE_MAPPED
// This is a final default mapping, if a prior mapping has occured it should have defined
// HARDWARE_MAPPED to prevent this default mapping.



#endif // HARDWARE_MAPPED

// Flash Hardware Settings
#define FLASH_SPI_BANK spi0
#define FLASH_HOLD_GPIO 21
#define FLASH_WP_GPIO 20
#define FLASH_TX_GPIO 19
#define FLASH_CLK_GPIO 18
#define FLASH_CS_GPIO 17
#define FLASH_RX_GPIO 16

#endif // HARDWARE_MAP_H