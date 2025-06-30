/* Copyright 2025, Darran A Lofthouse
 *
 * This file is part of pico-ward.
 *
 * pico-term is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * pico-term is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with pico-term.
 * If  not, see <https://www.gnu.org/licenses/>.
 */

enum otp_error
{
    OTP_ERROR_NONE = 0x00,
    OTP_ERROR_INVALID_PIN = 0x01,
    OTP_ERROR_INVALID_SECRET = 0x02,
    OTP_ERROR_STORAGE_NOT_INITIALISED = 0x03,
    OTP_ERROR_STORAGE_READ_FAILED = 0x04,
    OTP_ERROR_STORAGE_WRITE_FAILED = 0x05,
    OTP_ERROR_STORAGE_ERASE_FAILED = 0x06,
    OTP_ERROR_INVALID_HOTP_SECRET_LENGTH = 0x07,
    OTP_ERROR_HOTP_CALCULATION_FAILED = 0x08,
};

static inline const char* otp_error_to_string(enum otp_error err) {
    switch (err) {
        case OTP_ERROR_NONE: return "No error";
        case OTP_ERROR_INVALID_PIN: return "Invalid PIN";
        case OTP_ERROR_INVALID_SECRET: return "Invalid secret";
        case OTP_ERROR_STORAGE_NOT_INITIALISED: return "Storage not initialised";
        case OTP_ERROR_STORAGE_READ_FAILED: return "Storage read failed";
        case OTP_ERROR_STORAGE_WRITE_FAILED: return "Storage write failed";
        case OTP_ERROR_STORAGE_ERASE_FAILED: return "Storage erase failed";
        case OTP_ERROR_INVALID_HOTP_SECRET_LENGTH: return "Invalid HOTP secret length";
        case OTP_ERROR_HOTP_CALCULATION_FAILED: return "HOTP calculation failed";
        default: return "Unknown error";
    }
}