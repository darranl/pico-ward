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

/*
 * The OTP Manager provides a simple interface to manage the Pico OTP runtime.
 */

#ifndef OTP_MGR_H
#define OTP_MGR_H

#include <stdbool.h>

#include "pico_otp.h"
#include "term/vt102.h"

void* otp_mgr_init();
bool otp_mgr_begin(void *otp_mgr_context, otp_core_t *otp_core);
void otp_mgr_run(void *otp_mgr_context);

#endif // OTP_MGR_H

