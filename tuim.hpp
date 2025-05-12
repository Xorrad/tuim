/****************************************************************************
*                                                                           *
*   TUIM : a text-based user interface for terminals                        *
*                                                                           *
*   Made by Xorrad <monsieurs.aymeric@gmail.com>                            *
*   This code is licensed under MIT license (see LICENSE for details)       *
*                                                                           *
****************************************************************************/

#ifndef TUIM_HPP
#define TUIM_HPP

namespace tuim {
    class Context;
    extern Context* ctx;
}

/***********************************************************
*                         HEADERS                          *
***********************************************************/

#include <iostream>
#include <string>
#include <string_view>

#ifdef __linux__
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#elif _WIN32
#error "Windows is not supported yet."
#else
#error "This operating system is not supported."
#endif

/***********************************************************
*                    MACROS/DEFINES                        *
***********************************************************/

#define TUIM_DEBUG(n) printf("step %d\n", n)

namespace tuim {

    /***********************************************************
    *                  FORWARD DECLARATIONS                    *
    ***********************************************************/

    class Item;
    class Frame;
    class Context;

    /***********************************************************
    *                           MATH                           *
    ***********************************************************/

    struct vec2 {
        int x, y;

        vec2() : vec2(0, 0) {}
        vec2(int x, int y) : x(x), y(y) {}
        ~vec2() = default;

        bool operator==(const vec2 &other) const {
            return x == other.x && y == other.y;
        }

        bool operator!=(const vec2 &other) const {
            return x != other.x || y != other.y;
        }

        vec2 operator+(const vec2 &other) const {
            return vec2(x + other.x, y + other.y);
        }
        
        vec2 operator-(const vec2 &other) const {
            return vec2(x - other.x, y - other.y);
        }
        
        vec2& operator+=(const vec2 &other) {
            x += other.x;
            y += other.y;
            return *this;
        }
        
        vec2& operator-=(const vec2 &other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }
    };

    /***********************************************************
    *                         INPUTS                           *
    ***********************************************************/

    enum class Key {};

    //

    /***********************************************************
    *                         COLORS                           *
    ***********************************************************/

    //

    /***********************************************************
    *                         FLAGS                            *
    ***********************************************************/

    //

    /***********************************************************
    *                   CONTEXT FUNCTIONS                      *
    ***********************************************************/

    void CreateContext(); // Initialize the global context
    void CreateContext(int argc, char* argv[]); // Initialize the global context with arguments
    void DeleteContext(); // Delete the global context
    Context* GetCtx(); // Get tuim global context

    void SetTitle(std::string_view title); // Change the terminal title
    void SetCursorVisibility(bool visible); // Change the cursor visibility
    void SetFullscreen(bool fullscreen); // Change the terminal to full screen

    void Update(Key key); // Update the frame depending on the key pressed
    void Clear(); // Clear the current frame buffer
    void Display(); // Draw the current frame buffer to the terminal

    /***********************************************************
    *                TERMINAL/NATIVE FUNCTIONS                 *
    ***********************************************************/

    namespace Terminal {
        bool IsUserInputsVisible(); // Determine if user inputs are visible
        vec2 GetTerminalSize(); // Determine the size in cells of the terminal window.

        void SetTitle(std::string_view title); // Change the terminal title
        void SetCursorVisibility(bool visible); // Change the terminal visibility
        void SetUserInputsVisibility(bool visible); // Change the user inputs visibility
        void SetAlternateBuffer(bool enabled); // Toggle the terminal alternate buffer
        void SetCursorPos(const vec2& pos); // Change the cursor position

        void Clear(); // Clear the whole terminal
        void ClearLine(); // Clear the terminal current line
        void ClearEnd(); // Clear the terminal from the cursor to the end
        void ClearStyles(); // Remove any active ANSI style
    }

    /***********************************************************
    *                   CONTAINERS FUNCTIONS                   *
    ***********************************************************/

    //

    /***********************************************************
    *                         ITEMS                            *
    ***********************************************************/

    //

    /***********************************************************
    *                    COMPONENTS/ITEMS                      *
    ***********************************************************/

    //

    /***********************************************************
    *                    STRING FUNCTIONS                      *
    ***********************************************************/

    //

    /***********************************************************
    *                          FRAME                           *
    ***********************************************************/
    
    class Frame {
        Frame();
        ~Frame() = default;
    };
    
   /***********************************************************
    *                        CONTEXT                          *
    ***********************************************************/

    class Context {
        Context() {}
        ~Context() = default;
    };

    /***********************************************************
    *                    GLOBAL VARIABLES                      *
    ***********************************************************/

    inline Context* ctx; // Global context variable for the gui
}

/***********************************************************
*                   INLINE DEFINITIONS                     *
***********************************************************/

/***********************************************************
*                    CONTEXT FUNCTIONS                     *
***********************************************************/

void tuim::CreateContext() {

}

void tuim::CreateContext(int argc, char* argv[]) {
    tuim::CreateContext();
}

void tuim::DeleteContext() {

}

tuim::Context* tuim::GetCtx() {
    
}

void tuim::SetTitle(std::string_view title) {

}

void tuim::SetCursorVisibility(bool visible) {

}

void tuim::SetFullscreen(bool fullscreen) {

}

void tuim::Update(tuim::Key key) {

}

void tuim::Clear() {

}

void tuim::Display() {

}

/***********************************************************
*                 TERMINAL/NATIVE FUNCTIONS                *
***********************************************************/

bool tuim::Terminal::IsUserInputsVisible() {
    // TODO: keep track of the cursor visibility in the context.
    return false;
}

tuim::vec2 tuim::Terminal::GetTerminalSize() {
    winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return vec2(size.ws_col, size.ws_row);
}

void tuim::Terminal::SetTitle(std::string_view title) {
    printf("\033]0;%s\007", title.data());
}

void tuim::Terminal::SetCursorVisibility(bool visible) {
    printf("\033[?25%c", (visible ? 'h' : 'l'));
}

void tuim::Terminal::SetUserInputsVisibility(bool visible) {
    termios term;
    tcgetattr(0, &term);
    if (visible) term.c_lflag |= ECHO;
    else term.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &term);
}

void tuim::Terminal::SetAlternateBuffer(bool enabled) {
    printf("\033[?1049%c", (enabled ? 'h' : 'l'));
}

void tuim::Terminal::SetCursorPos(const tuim::vec2& pos) {
    printf("\033[%d;%dH", pos.y+1, pos.x+1);
}

void tuim::Terminal::Clear() {
    printf("\033[2J\033[0m\033[H");
}

void tuim::Terminal::ClearLine() {
    printf("\033[2K");
}

void tuim::Terminal::ClearEnd() {
    printf("\033[0J");
}

void tuim::Terminal::ClearStyles() {
    printf("\033[0m");
}

#endif // TUIM_HPP