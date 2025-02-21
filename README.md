# pico-ward

Personal security tools running on a Raspberry Pi Pico

Initially this project is focussed on one time password support
in the form of HOTP and TOTP token generation providing additional
protection for a users online identity.  This may be expanded upon
later to also support newer standards such as WebAuthn and
potentially additional utilities such encryption and decryption.

## Checkout and Build

This process assumes a working pico-sdk is already available with
the required tools already installed.



## Branches

### main

This is the main development branch of the project and has minimal
hardware dependencies.

Required components:

 - W25Q64JV Serial Flash Memory

## Modules

This project uses the following Git repositories as sub modules to
aggregate the required utilities.

 = [pico-flash](https://github.com/darranl/pico-flash)
 = [pico-security](https://github.com/darranl/pico-security)
 = [pico-term](https://github.com/darranl/pico-term)
 = [pico-util](https://github.com/darranl/pico-util)
 = [pico-vscode](https://github.com/darranl/pico-vscode)
