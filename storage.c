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

#include <stdio.h> // TODO Remove Later If Not Needed

#include "storage.h"
#include "storage_address_map.h"

static char header[8] = "PICOWARD";

static void _storage_output_header(char *header)
{
    printf("Header: ");
    for (int i = 0; i < 8; i++)
    {
        printf("%02x", header[i]);
    }
    printf("\n");
}

void storage_begin(storage_context_t *context, flash_context_t *flash_context)
{
    // The two headers are only written to storage after the pre-requisite
    // minimal data has been written.
    //
    // Unless both headers are present the storage is considered uninitialised.
    char header_a[8];
    char header_b[8];

    flash_read_data(flash_context, HEADER_A, header_a, 8);
    _storage_output_header(header_a);
    flash_read_data(flash_context, HEADER_B, header_b, 8);
    _storage_output_header(header_b);

    context->flash_context = flash_context;
    context->initialised = false;
}

