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
#include <string.h>

#include "otp_mgr.h"
#include "term/terminal_handler.h"
#include "term/vt102.h"
#include "util/hexutil.h"


struct otp_mgr_context;

/**
 * The handle function to be implemented for each screen.
 *
 * @param event The VT102 event.
 * @param context The OTP Manager context.
 * @return true if the screen should be redrawn, false otherwise.
 */
typedef bool (*screen_handler)(vt102_event *event, struct otp_mgr_context *context);

/**
 * @brief Typedef for a function pointer that takes a struct otp_mgr_context pointer as a parameter and returns void.
 * This function pointer is used for rendering the full OTP (One-Time Password) screen.
 *
 * @param context The otp_mgr_context pointer.
 */
typedef void (*full_renderer)(struct otp_mgr_context *context);
struct base_screen_details
{
    char* program_name;
    char* screen_name;
    char* error_message;
    char* commands;
    char* footer;
    screen_handler handler;
    full_renderer renderer;
};

struct login_screen
{
    struct base_screen_details base_screen_details;
    char entered_pin[9];
};

struct generate_otp_screen
{
    struct base_screen_details base_screen_details;
    char otp[7];
};

struct change_pin_screen
{
    struct base_screen_details base_screen_details;
    bool first_pin_entered;
    char new_pin[9];
    char confirm_pin[9];
};

struct configure_screen_handler
{
    struct base_screen_details base_screen_details;
    char otp_secret_hex[40];
    uint8_t otp_secret_length;
};

union screens
{
    struct login_screen login_screen;
    struct change_pin_screen change_pin_screen;
    struct configure_screen_handler configure_screen_handler;
};

/*
 * For now this will contain a union to allocate the space for the largest screen type.
 */
struct otp_mgr_context
{
    otp_core_t *otp_core;
    void* screen;
};


static void otp_mgr_handle(vt102_event *event, void *context);

/**
 * Begins the OTP Manager by initializing the OTP Manager context and calling the terminal handler.
 *
 * @param otp_core A pointer to the OTP core structure.
 */
void otp_mgr_begin(otp_core_t *otp_core)
{
    // This context will contain the state for the OTP Manager, it will be passed back in
    // on every event.  Although this currently uses a linear call stack later the context
    // could be passed back in for independently firing events.
    union screens screens;
    struct otp_mgr_context otp_mgr_context;
    otp_mgr_context.screen = &screens;
    otp_mgr_context.otp_core = otp_core;

    terminal_handler(&otp_mgr_handle, &otp_mgr_context);
}

void handle_disconnected()
{
    printf("Disconnected\n");
}

/*
 * Screen initialisation prototype function definitions.
 */
static void init_login_screen(struct otp_mgr_context *context);
static void init_main_menu(struct otp_mgr_context *context);
static void init_generate_screen(struct otp_mgr_context *context);
static void init_configure_screen(struct otp_mgr_context *context);
static void init_system_information_screen(struct otp_mgr_context *context);
static void init_otp_information_screen(struct otp_mgr_context *context);
static void init_flash_information_screen(struct otp_mgr_context *context);
static void init_change_pin_screen(struct otp_mgr_context *context);

void otp_mgr_handle(struct vt102_event *event, void *context)
{
    struct otp_mgr_context *otp_mgr_context = (struct otp_mgr_context *)context;
    bool redraw = false;
    switch (event->event_type)
    {
    case connect:
        init_login_screen(otp_mgr_context);
        vt102_ris();
        redraw = true;
        break;
    case disconnect:
        handle_disconnected();
        break;
    default:
        // This will be the main interaction loop event handler,
        // this will need to use a state machine representaton of the current
        // screen and give the screen the optin to process the input.
        // Also this may need a generic way to refresh the screen if our of sync Ctrl+R ?
        if (event->event_type != none)
        {
            char hex_value[10];
            hex_value[0] = '0';
            hex_value[1] = 'x';
            uint32_to_hex_string(event->character, &hex_value[2]);
            printf("Event Type: %s, Character: %s\n", vt102_event_type_to_string(event->event_type),
                   event->event_type == special ? special_key_to_string(event->character) : hex_value);
        }

        if (event->event_type == control && event->character == 0x52)
        {
            redraw = true;
        } else
        {
            struct base_screen_details *screen = otp_mgr_context->screen;
            if (screen->handler != NULL)
            {
                redraw = screen->handler(event, otp_mgr_context);
            }
        }
    }

    if (redraw)
    {
        vt102_ris(); // Reset to initial state. TODO Check we really need this.
        struct base_screen_details *screen = otp_mgr_context->screen;
        screen->renderer(otp_mgr_context);
    }
}

/*
 *  Common Screen Handling
 */

static void render_screen(struct base_screen_details* screen_details)
{
    vt102_erase_display();
    _vt102_write_str("== ");
    _vt102_write_str(screen_details->program_name);
    vt102_cup("2", "0");
    _vt102_write_str("-- ");
    _vt102_write_str(screen_details->screen_name);

    if (screen_details->error_message != NULL)
    {
        vt102_cup("4", "10");
        _vt102_write_str(screen_details->error_message);
    }

    vt102_cup("34", "0");
    _vt102_write_str("-- ");
    _vt102_write_str(screen_details->commands);
    vt102_cup("35", "0");
    _vt102_write_str("== ");
    _vt102_write_str(screen_details->footer);
    _vt102_write_flush();
}

/*
 * Login Screen
 */

void render_login_screen(struct otp_mgr_context *context)
{
    render_screen(context->screen);

    struct login_screen *login_screen = context->screen;

    vt102_cup("10", "10");
    _vt102_write_str("Please enter your PIN and press <ENTER>");
    vt102_cup("11", "10");

    int entered = 0;
    for (int i = 0; i < 8; i++) // The 9th is the null terminator.
    {
        if (login_screen->entered_pin[i] != 0x00)
        {
            _vt102_write_char('*');
            entered++;
        }
        else
        {
            _vt102_write_char('-');
        }
    }
    int cursor = 10 + entered;
    char cursor_string[3];
    sprintf(cursor_string, "%d", cursor);

    vt102_cup("11", cursor_string);
    _vt102_write_flush();


    printf("Login screen sent.\n");
}

bool login_screen_handler(vt102_event *event, struct otp_mgr_context *context)
{
    if (event->event_type == character && event->character >= 0x30 && event->character <= 0x39)
    {
        struct login_screen *login_screen = context->screen;
        for (int i = 0; i < 8; i++)
        {
            if (login_screen->entered_pin[i] == 0x00)
            {
                login_screen->entered_pin[i] = event->character;
                _vt102_write_char('*');
                _vt102_write_flush();
                break;
            }
        }
    }
    else if (event->event_type == control && event->character == 0x4D)
    {
        // Process the entered pin.
        printf("Processing entered pin.\n");
        struct login_screen *login_screen = context->screen;
        if (pico_otp_validate_pin(context->otp_core, login_screen->entered_pin))
        {
            printf("PIN Valid\n");
            init_main_menu(context);
        }
        else
        {
            printf("PIN Invalid\n");
            struct base_screen_details *screen = context->screen;
            screen->error_message = "Invalid PIN";
        }

        // Clear the entered pin good or bad.
        for (int i = 0; i < 9; i++)
        {
            login_screen->entered_pin[i] = 0x00;
        }

        return true;
    }
    return false;
}

/*
 * A base init function for screens, if we add new fields the base can ensure
 * they are initialised to NULL even by init functions that do not know about them.
 */
static void init_screen(struct base_screen_details *screen)
{
    screen->program_name = NULL;
    screen->screen_name = NULL;
    screen->commands = NULL;
    screen->error_message = NULL;
    screen->footer = NULL;
    screen->handler = NULL;
    screen->renderer = NULL;
}

static void init_login_screen(struct otp_mgr_context *context)
{
    struct base_screen_details *screen = context->screen;
    init_screen(screen);
    struct login_screen *login_screen = context->screen;
    login_screen->base_screen_details.program_name = "Pico OATH";
    login_screen->base_screen_details.screen_name = "Login";
    login_screen->base_screen_details.commands = "Please Login";
    login_screen->base_screen_details.footer = "Taking control of your security.";
    login_screen->base_screen_details.handler = login_screen_handler;
    login_screen->base_screen_details.renderer = render_login_screen;
    // Clear the entered pin.
    for (int i = 0; i < 9; i++)
    {
        login_screen->entered_pin[i] = 0x00;
    }
}

/*
 * Main Menu
 */

bool main_menu_handler(vt102_event *event, struct otp_mgr_context *context)
{
    if (event->event_type == character)
    {
        switch (event->character)
        {
        case 0x31:
            if (pico_otp_configured(context->otp_core))
            {
                init_generate_screen(context);
            } else
            {
                struct base_screen_details *screen = context->screen;
                screen->error_message = "OTP not configured";
            }
            return true;
        case 0x32:
            init_configure_screen(context);
            return true;
        case 0x33:
            init_system_information_screen(context);
            return true;
        case 0x34:
            init_change_pin_screen(context);
            return true;
        case 0x51:
        case 0x71:
            init_login_screen(context);
            return true;
        }
    }

    return false;
}

void render_main_menu(struct otp_mgr_context *context)
{
    render_screen(context->screen);

    vt102_cup("8", "10");
    _vt102_write_str("1 - Generate OTP");

    vt102_cup("10", "10");
    _vt102_write_str("2 - Configure OTP");

    vt102_cup("12", "10");
    _vt102_write_str("3 - System Information");

    vt102_cup("14", "10");
    _vt102_write_str("4 - Change PIN");

    vt102_cup("16", "10");
    _vt102_write_str("Q - Quit");

    vt102_cup("18", "10");
    _vt102_write_str("[ ]");
    vt102_cup("18", "11");
}

static void init_main_menu(struct otp_mgr_context *context)
{
    struct base_screen_details *screen = context->screen;
    init_screen(screen);
    screen->program_name = "Pico OATH";
    screen->screen_name = "Main Menu";
    screen->commands = "Please choose an option.";
    screen->footer = "Taking control of your security.";
    screen->handler = main_menu_handler;
    screen->renderer = render_main_menu;
}

/*
 * Generate Screen
 */

bool generate_screen_handler(vt102_event *event, struct otp_mgr_context *context)
{
    if (event->event_type == character && event->character == 0x71 || event->character == 0x51)
    {
        // Quit
        init_main_menu(context);
        return true;
    }
    else if (event->event_type == character && event->character == 0x43 || event->character == 0x63)
    {
        struct generate_otp_screen *generate_screen = context->screen;
        pico_otp_calculate(context->otp_core, generate_screen->otp);

        return true;
    }

    return false;
}

void render_generate_screen(struct otp_mgr_context *context)
{
    render_screen(context->screen);

    vt102_cup("8", "10");
    _vt102_write_str("Generating OTP");

    vt102_cup("10", "10");
    _vt102_write_str("OTP - ");

    struct generate_otp_screen *generate_screen = context->screen;
    _vt102_write_str(generate_screen->otp);

    vt102_cup("12", "10");
    _vt102_write_str("Press C to calculate next OTP.");
    vt102_cup("13", "10");
    _vt102_write_str("Press Q to return to the main menu.");

    vt102_cup("15", "10");
    _vt102_write_str("[ ]");

    vt102_cup("15", "11");
}

static void init_generate_screen(struct otp_mgr_context *context)
{
    struct base_screen_details *screen = context->screen;
    init_screen(screen);
    screen->program_name = "Pico OATH";
    screen->screen_name = "Generate OTP";
    screen->commands = "C - Calculate, Q - Quit";
    screen->footer = "Taking control of your security.";
    screen->handler = generate_screen_handler;
    screen->renderer = render_generate_screen;
    struct generate_otp_screen *generate_screen = context->screen;
    pico_otp_calculate(context->otp_core, generate_screen->otp);

}

/*
 * Configure Screen
 */

struct cursor_position
{
    uint8_t row;
    uint8_t column;
};

static void set_cursor_position(struct cursor_position *cursor_position, struct cursor_position *origin)
{
    char row[3];
    char column[3];
    sprintf(row, "%d", cursor_position->row + origin->row);
    sprintf(column, "%d", cursor_position->column + origin->column);
    vt102_cup(row, column);
}

static void calculate_cursor_position(struct cursor_position *cursor_position, uint8_t index)
{
    cursor_position->row = index / 10;
    cursor_position->column = index % 10;
}

bool configure_screen_handler(vt102_event *event, struct otp_mgr_context *context)
{
    char current = 0x00;
    if (event->event_type == character && event->character == 0x71 || event->character == 0x51)
    {
        // Quit
        init_main_menu(context);
        return true;
    }
    else if (event->event_type == character &&
        ((event->character >= 0x30 && event->character <= 0x39) || (event->character >= 0x41 && event->character <= 0x5A))
        )
    {
        current = event->character;
    }
    else if (event->event_type == character && event->character >= 0x61 && event->character <= 0x7A)
    {
        current = event->character - 0x20; // Convert to upper case.
    }
    else if (event->event_type == control && event->character == 0x4D)
    {
        // Process the entered secret.
        struct configure_screen_handler *configure_screen = context->screen;
        if (configure_screen->otp_secret_length == 0)
{
            struct base_screen_details *screen = context->screen;
            screen->error_message = "No secret entered";
        }
        else if (configure_screen->otp_secret_length % 2 != 0)
        {
            struct base_screen_details *screen = context->screen;
            screen->error_message = "Secret must be an even number of characters";
        }
        else
        {
            uint8_t hotp_secret_length = configure_screen->otp_secret_length / 2;
            uint8_t hotp_secret[hotp_secret_length];
            for (int i = 0; i < hotp_secret_length; i++)
            {
                hotp_secret[i] = hex_to_char(&configure_screen->otp_secret_hex[i * 2]);
            }
            pico_otp_set_hotp_secret(context->otp_core, hotp_secret, hotp_secret_length);
            init_main_menu(context);
        }

        // It is either an error or we processed the secret.
        return true;
    }

    if (current != 0x00)
    {
        struct configure_screen_handler *configure_screen = context->screen;
        struct cursor_position cursor_position;
        calculate_cursor_position(&cursor_position, configure_screen->otp_secret_length);
        uint8_t current_row = cursor_position.row;

        if (configure_screen->otp_secret_length < 40)
        {
            configure_screen->otp_secret_hex[configure_screen->otp_secret_length] = current;
            configure_screen->otp_secret_length++;
            _vt102_write_char(current);

            calculate_cursor_position(&cursor_position, configure_screen->otp_secret_length);
            if (cursor_position.row != current_row)
            {
                set_cursor_position(&cursor_position, &(struct cursor_position){10, 10});
            }

            _vt102_write_flush();
        }
    }

    return false;
}

static void render_secret(char *otp_secret, uint8_t secret_length)
{
    struct cursor_position cursor_position;
    for (int i = 0; i < 40; i++)
    {
        // 0 % 0 == 0 so also sets the first position.
        if (i % 10 == 0)
        {
            calculate_cursor_position(&cursor_position, i);
            set_cursor_position(&cursor_position, &(struct cursor_position){10, 10});
        }

        if (i + 1 > secret_length)
        {
            _vt102_write_char('-');
        }
        else
        {
            _vt102_write_char(otp_secret[i]);
        }
    }
}

void render_configure_screen(struct otp_mgr_context *context)
{
    render_screen(context->screen);

    vt102_cup("8", "10");
    _vt102_write_str("Enter OTP secret and press <ENTER>");

    struct configure_screen_handler *configure_screen = context->screen;

    struct cursor_position cursor_position;

    render_secret(configure_screen->otp_secret_hex, configure_screen->otp_secret_length);

    vt102_cup("15", "10");
    _vt102_write_str("Press Q to return to the main menu.");


    calculate_cursor_position(&cursor_position, configure_screen->otp_secret_length);
    set_cursor_position(&cursor_position, &(struct cursor_position){10, 10});
}

static void init_configure_screen(struct otp_mgr_context *context)
{
    struct base_screen_details *screen = context->screen;
    init_screen(screen);
    screen->program_name = "Pico OATH";
    screen->screen_name = "Configure OTP";
    screen->commands = "Configuring OTP";
    screen->footer = "Taking control of your security.";
    screen->handler = configure_screen_handler;
    screen->renderer = render_configure_screen;
    struct configure_screen_handler *configure_screen = context->screen;
    configure_screen->otp_secret_length = 0;
}

/*
 * System Information Screen
 */

bool system_information_screen_handler(vt102_event *event, struct otp_mgr_context *context)
{
    if (event->event_type == character)
    {
        switch (event->character)
        {
            case 0x31:
                if (pico_otp_configured(context->otp_core))
                {
                    init_otp_information_screen(context);
                } else
                {
                    struct base_screen_details *screen = context->screen;
                    screen->error_message = "OTP not configured";
                }
                return true;
            case 0x32:
                // TODO Display Flash information.
                break;
            case 0x51:
            case 0x71:
                // Quit
                init_main_menu(context);
                return true;
        }
    }
    return false;
}

void render_system_information_screen(struct otp_mgr_context *context)
{
    render_screen(context->screen);

    vt102_cup("8", "10");
    _vt102_write_str("1 - Show OTP Information");

    vt102_cup("10", "10");
    _vt102_write_str("2 - Show Flash Information");

    vt102_cup("16", "10");
    _vt102_write_str("Q - Quit");

    vt102_cup("18", "10");
    _vt102_write_str("[ ]");
    vt102_cup("18", "11");
}

static void init_system_information_screen(struct otp_mgr_context *context)
{
    struct base_screen_details *screen = context->screen;
    init_screen(screen);
    screen->program_name = "Pico OATH";
    screen->screen_name = "System Information";
    screen->commands = "System Information";
    screen->footer = "Taking control of your security.";
    screen->handler = system_information_screen_handler;
    screen->renderer = render_system_information_screen;
}

/*
 * OTP Information Screen
 */

// Used by multiple information only screens to return.
bool return_to_system_information_screen_handler(vt102_event *event, struct otp_mgr_context *context)
{
    if (event->event_type == character && event->character == 0x71 || event->character == 0x51)
    {
        // Quit
        init_system_information_screen(context);
        return true;
    }
    return false;
}

void render_otp_information_screen(struct otp_mgr_context *context)
{
    render_screen(context->screen);

    vt102_cup("8", "10");
    _vt102_write_str("Configured HOTP Secret");

    uint8_t hotp_secret_length = context->otp_core->hotp_secret_length * 2;
    char hotp_secret_hex[hotp_secret_length];
    for (int i = 0; i < context->otp_core->hotp_secret_length; i++)
    {
        uint8_to_hex(context->otp_core->hotp_secret[i], &hotp_secret_hex[i * 2]);
    }

    render_secret(hotp_secret_hex, hotp_secret_length);

    vt102_cup("15", "10");
    char counter[20];
    sprintf(counter, "%ld", context->otp_core->hotp_counter);
    _vt102_write_str("Count - ");
    _vt102_write_str(counter);

    vt102_cup("17", "10");
    _vt102_write_str("Press Q to return to the main menu.");

    vt102_cup("19", "10");
    _vt102_write_str("[ ]");
    vt102_cup("19", "11");
}

static void init_otp_information_screen(struct otp_mgr_context *context)
{
    struct base_screen_details *screen = context->screen;
    init_screen(screen);
    screen->program_name = "Pico OATH";
    screen->screen_name = "OTP Information";
    screen->commands = "OTP Information";
    screen->footer = "Taking control of your security.";
    screen->handler = return_to_system_information_screen_handler;
    screen->renderer = render_otp_information_screen;
}

/*
 * Flash Information Screen
 */


/*
 * Change PIN Screen
 */

bool change_pin_screen_handler(vt102_event *event, struct otp_mgr_context *context)
{
    if (event->event_type == character && event->character == 0x71 || event->character == 0x51)
    {
        // Quit
        init_main_menu(context);
        return true;
    }
    else if (event->event_type == character && event->character >= 0x30 && event->character <= 0x39)
    {
        struct change_pin_screen *change_pin_screen = context->screen;
        char *pin = change_pin_screen->first_pin_entered ?
            change_pin_screen->confirm_pin :
            change_pin_screen->new_pin;

        for (int i = 0; i < 8; i++)
        {
            if (pin[i] == 0x00)
            {
                pin[i] = event->character;
                _vt102_write_char('*');
                _vt102_write_flush();
                break;
            }
        }
    }
    else if (event->event_type == control && event->character == 0x4D)
    {
        struct change_pin_screen *change_pin_screen = context->screen;
        if (change_pin_screen->first_pin_entered)
        {
            if (strncmp(change_pin_screen->new_pin, change_pin_screen->confirm_pin, 9) == 0)
            {
                // Change the pin.
                pico_otp_set_pin(context->otp_core, change_pin_screen->new_pin);
                init_main_menu(context);
            }
            else
            {
                struct base_screen_details *screen = context->screen;
                screen->error_message = "PINs do not match";
                // Clear the entered pins.
                for (int i = 0; i < 9; i++)
                {
                    change_pin_screen->new_pin[i] = 0x00;
                    change_pin_screen->confirm_pin[i] = 0x00;
                }

                change_pin_screen->first_pin_entered = false;
            }
        }
        else
        {
            change_pin_screen->first_pin_entered = true;
            change_pin_screen->base_screen_details.error_message = NULL;

        }
        // User may have pressed enter after 1st pin - we need to refresh the screen.
        // It could be after the second pin, if they don't match we need to clear the entered pins and refresh.
        // If they do match we need to set the new pin and refresh to return to the menu.
        return true;
    }
    return false;
}

void render_change_pin_screen(struct otp_mgr_context *context)
{
    render_screen(context->screen);
    struct change_pin_screen *change_pin_screen = context->screen;

    vt102_cup("8", "10");
    change_pin_screen->first_pin_entered ?
        _vt102_write_str("Please re-enter your new PIN and press <ENTER>") :
        _vt102_write_str("Please enter your new PIN and press <ENTER>");

    char *pin = change_pin_screen->first_pin_entered ?
        change_pin_screen->confirm_pin :
        change_pin_screen->new_pin;

    vt102_cup("10", "10");
    int entered = 0;
    for (int i = 0; i < 8; i++) // The 9th is the null terminator.
    {
        if (pin[i] != 0x00)
        {
            _vt102_write_char('*');
            entered++;
        }
        else
        {
            _vt102_write_char('-');
        }
    }
    int cursor = 10 + entered;
    char cursor_string[3];
    sprintf(cursor_string, "%d", cursor);

    vt102_cup("14", "10");
    _vt102_write_str("Press Q to return to the main menu.");

    vt102_cup("10", cursor_string);
}

static void init_change_pin_screen(struct otp_mgr_context *context)
{
    struct base_screen_details *screen = context->screen;
    init_screen(screen);
    struct change_pin_screen *change_pin_screen = context->screen;
    change_pin_screen->base_screen_details.program_name = "Pico OATH";
    change_pin_screen->base_screen_details.screen_name = "Change PIN";
    change_pin_screen->base_screen_details.commands = "Q - Quit";
    change_pin_screen->base_screen_details.footer = "Taking control of your security.";
    change_pin_screen->base_screen_details.handler = change_pin_screen_handler;
    change_pin_screen->base_screen_details.renderer = render_change_pin_screen;

    change_pin_screen->first_pin_entered = false;
    // Clear the entered pins.
    for (int i = 0; i < 9; i++)
    {
        change_pin_screen->new_pin[i] = 0x00;
        change_pin_screen->confirm_pin[i] = 0x00;
    }
}

