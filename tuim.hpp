/****************************************************************************
*                                                                           *
*   TUIM : a text-based user interface for command lines                    *
*                                                                           *
*   Made by Xorrad <monsieurs.aymeric@gmail.com>                            *
*   This code is licensed under MIT license (see LICENSE for details)       *                                            *
*                                                                           *
****************************************************************************/

#ifndef TUIM_CPP
#define TUIM_CPP

#include <iostream>
#include <string.h>
#include <filesystem>
#include <map>

#ifdef __linux__

#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#elif _WIN32

#include <windows.h>

#else

#endif

#define TUIM_COLOR '&'
#define TUIM_CUSTOM_COLOR '#'
#define TUIM_BACKGROUND_COLOR '_'

namespace tuim {

    void init(int argc, char* argv[]); /* Initialize tuim with command line arguments */
    void set_title(std::string title); /* Set terminal title */
    void move(int x, int y); /* Change terminal cursor position */
    void clear(); /* Clear terminal output */
    void clear_line(); /* Clear terminal output */

    template<typename ... Args> void print(const char* fmt, Args ... args); /* Print formatted text */

    /* Internals are defined within this namespace */
    namespace impl {

        void open_terminal(); /* Restart program in new separate terminal */
        std::string format(std::string str); /* Format a string for printing */

    };

    /* Keyboard related functions are defined here */
    namespace keyboard {

        enum key {
            ESCAPE = 27,
            BACKSPACE = 127,
            ENTER = 10,
            UP = (27<<16) + (91<<8) + 65,
            DOWN = (27<<16) + (91<<8) + 66,
            RIGHT = (27<<16) + (91<<8) + 67,
            LEFT = (27<<16) + (91<<8) + 68,
            A = 97,
            B = 98,
            C = 99,
            D = 100,
            E = 101,
            F = 102,
            G = 103,
            H = 104,
            I = 105,
            J = 106,
            K = 107,
            L = 108,
            M = 109,
            N = 110,
            O = 111,
            P = 112,
            Q = 113,
            R = 114,
            S = 115,
            T = 116,
            U = 117,
            V = 118,
            W = 119,
            X = 120,
            Y = 121,
            Z = 122
        };

        key get_pressed(); /* Get pressed key */
        bool is_pressed(); /* Check if key has been pressed */

    };

    namespace color {

        /* Structure to store rgb color codes */
        struct color {
            int r;
            int g;
            int b;
            bool background;
        };

        color from_code(std::string str); /* Convert string color code to rgb */
        color from_hex(std::string str); /* Convert string hexadecimal color code to rgb */
        color from_hex(unsigned int hex); /* Convert hexadecimal color code to rgb */
        unsigned int to_hex(color color); /* Convert rgb color code to hexadecimal*/
        std::string to_ansi(color color); /* Convert rgb color to ansi color */

        /**
         * Using minecraft color codes
         * https://htmlcolorcodes.com/minecraft-color-codes/
        */
        const std::map<std::string, color> codes = {
            { "0", from_hex(0x000000) }, /* BLACK */
            { "1", from_hex(0x0000AA) }, /* DARK_BLUE */
            { "2", from_hex(0x00AA00) }, /* DARK_GREEN */
            { "3", from_hex(0x00AAAA) }, /* DARK_AQUA */
            { "4", from_hex(0xAA0000) }, /* DARK_RED */
            { "5", from_hex(0xAA00AA) }, /* DARK_PURPLE */
            { "6", from_hex(0xFFAA00) }, /* GOLD */
            { "7", from_hex(0xAAAAAA) }, /* GREY */
            { "8", from_hex(0x555555) }, /* DARK_GREY */
            { "9", from_hex(0x5555FF) }, /* BLUE*/
            { "a", from_hex(0x55FF55) }, /* GREEN */
            { "b", from_hex(0x55FFFF) }, /* AQUA */
            { "c", from_hex(0xFF5555) }, /* RED */
            { "d", from_hex(0xFF55FF) }, /* LIGHT_PURPLE */
            { "e", from_hex(0xFFFF55) }, /* YELLOW */
            { "f", from_hex(0xFFFFFF) }, /* WHITE */

            { "l", from_hex(0x000001) }, /* BOLD */
            { "o", from_hex(0x000002) }, /* ITALIC */
            { "n", from_hex(0x000003) }, /* UNDERLINED */

            { "r", from_hex(0x000000) } /* RESET */
        };
    };

};

/* Inline functions definitions */

void tuim::init(int argc, char* argv[]) {
    /* Restart if not in separate terminal */
    if(argc < 2 || strcmp(argv[1], "f") != 0) {
        tuim::impl::open_terminal();
    }
}

void tuim::set_title(std::string title) {
    /* http://www.faqs.org/docs/Linux-mini/Xterm-Title.html#s3 */
    printf("\033]0;%s\007", title.c_str());
}

void tuim::move(int x, int y) {
    /* https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797 */
    printf("\033[%d;%dH", y, x);
}

void tuim::clear() {
    /* https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797 */
    printf("\33[2J\33[3J\r");
}

void tuim::clear_line() {
    /* https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797 */
    printf("\33[2K\r");
}

template<typename ... Args>
void tuim::print(const char* fmt, Args ... args) {
    int size_s = std::snprintf(nullptr, 0, fmt, args...) + 1;
    if(size_s <= 0) throw std::runtime_error( "Error during formatting.");

    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[ size ]);

    std::snprintf(buf.get(), size, fmt, args...);

    std::string formatted = std::string( buf.get(), buf.get() + size - 1);
    formatted = tuim::impl::format(formatted);

    /* Print formatted text and reset colors and styles */
    printf("%s\33[0m", formatted.c_str());
}

void tuim::impl::open_terminal() {
    #ifdef __linux__ 
        /* Get the executable full path. */
        std::string exec = "\"" + std::filesystem::canonical("/proc/self/exe").string() + "\" f";

        execl("/usr/bin/gnome-terminal", "gnome-terminal", "--full-screen", "-q", "-e", exec.c_str(), (char*)0);
    #elif _WIN32
    
    #else

    #endif

    /* Close current process to avoid duplicates */
    exit(EXIT_SUCCESS);
}

std::string tuim::impl::format(std::string str) {
    bool escaped = false;
    std::string formatted = "";

    for(int i = 0; i < str.length(); i++) {

        /* Skip if character is escaped */
        if(escaped) {
            /* Add an anti-slash there was not any color code to escape */
            if(str[i] != TUIM_COLOR && str[i] != TUIM_CUSTOM_COLOR && str[i] != '\\') formatted += "\\";

            formatted += str[i];
            escaped = false;
            continue;
        }

        if(str[i] == '\\') {
            escaped = true;
            continue;
        }

        /* Check for minecraft color codes */
        if(str[i] == TUIM_COLOR) {
            int code_length = (str.length() - i > 2 && str[i+1] == TUIM_BACKGROUND_COLOR) ? 2 : 1;
            if(str.length() - i < code_length) continue;

            std::string code = str.substr(i + 1, code_length);

            if(strcmp(code.c_str(), "l") == 0) { /* Check if bold code */
                formatted += "\33[1m";
            } else if(strcmp(code.c_str(), "o") == 0) { /* Check if italic code */
                formatted += "\33[3m";
            } else if(strcmp(code.c_str(), "n") == 0) { /* Check if underline code */
                formatted += "\33[4m";
            } else if(strcmp(code.c_str(), "r") == 0) { /* Check if reset code */
                formatted += "\33[0m";
            } else {
                tuim::color::color color = tuim::color::from_code(code);
                formatted += tuim::color::to_ansi(color);
            }
            
            i += code_length;
            continue;
        }

        /* Check for custom color codes */
        if(str[i] == TUIM_CUSTOM_COLOR) {
            int code_length = (str.length() - i > 6 && str[i+1] == TUIM_BACKGROUND_COLOR) ? 7 : 6;
            if(str.length() - i < code_length) continue;

            std::string code = str.substr(i + 1, code_length);

            tuim::color::color color = tuim::color::from_hex(code);
            formatted += tuim::color::to_ansi(color);

            i += code_length;
            continue;
        }

        formatted += str[i];
    }

    return formatted;
}

tuim::keyboard::key tuim::keyboard::get_pressed() {
    #ifdef __linux__
        int count = 0;
        int codes[3] = { 0, 0, 0 };

        /* Loop for escaped characters */
        do {
            char buf;
            struct termios old;
            fflush(stdout);
            if(tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
            old.c_lflag &= ~ICANON;
            old.c_lflag &= ~ECHO;
            old.c_cc[VMIN] = 1;
            old.c_cc[VTIME] = 0;
            if(tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
            if(read(0, &buf, 1) < 0)
                perror("read()");
            old.c_lflag |= ICANON;
            old.c_lflag |= ECHO;
            if(tcsetattr(0, TCSADRAIN, &old) < 0)
                perror("tcsetattr ~ICANON");
            codes[count] = (int) buf;
            count++;
        } while(tuim::keyboard::is_pressed());

        /* Get the final key code */
        unsigned int code = (codes[0] << (8*(count-1))) + (codes[1] << (8*(count-2))) + codes[2];
        return (tuim::keyboard::key) code;

    #elif _WIN32

    #else

    #endif
}

bool tuim::keyboard::is_pressed() {
    #ifdef __linux__ 
        termios term;
        tcgetattr(0, &term);
        term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(0, TCSANOW, &term);

        int byteswaiting;
        ioctl(0, FIONREAD, &byteswaiting);

        tcgetattr(0, &term);
        term.c_lflag |= ICANON | ECHO;
        tcsetattr(0, TCSANOW, &term);

        return byteswaiting > 0;
    #elif _WIN32
        
    #else

    #endif
}

tuim::color::color tuim::color::from_code(std::string str) {
    /* Check if there is the background character */
    bool background = (str[0] == TUIM_BACKGROUND_COLOR);

    //printf("(%d) %s\n", background, str.c_str());
    
    /* Remove background character if there is one */
    if(background) str = str.substr(1, str.length() - 1);

    /* Raise an exception if the color code does not exist */
    //if(tuim::color::codes.count(str) == 0) throw std::invalid_argument("invalid color code");

    tuim::color::color color = tuim::color::codes.at(str);
    color.background = background;

    return color;
}

tuim::color::color tuim::color::from_hex(std::string str) {
    /* Check if there is the background character */
    bool background = (str[0] == TUIM_BACKGROUND_COLOR);

    /* Remove background character if there is one */
    if(background) str = str.substr(1, str.length() - 1);

    /* Convert string to int */
    unsigned int hex;
    std::stringstream ss;
    ss << std::hex << str;
    ss >> hex;

    tuim::color::color color = tuim::color::from_hex(hex);
    color.background = background;

    return color;
}

tuim::color::color tuim::color::from_hex(unsigned int hex) {
    tuim::color::color color;
    color.r = (hex >> 16) & 0xFF;
    color.g = (hex >> 8) & 0xFF;
    color.b = (hex) & 0xFF;
    color.background = false;
    return color;
}

unsigned int tuim::color::to_hex(tuim::color::color color) {
    return (color.r << 16) + (color.g << 8) + color.b;
}

std::string tuim::color::to_ansi(tuim::color::color color) {
    /* https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797 */

    std::string ansi = "\33[" + std::string(color.background ? "48" : "38") + ";2;";
    ansi += std::to_string(color.r) + ";";
    ansi += std::to_string(color.g) + ";";
    ansi += std::to_string(color.b) + "m";

    return ansi;
}

#endif