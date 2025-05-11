/****************************************************************************
*                                                                           *
*   TUIM : a text-based user interface for terminals                        *
*                                                                           *
*   Made by Xorrad <monsieurs.aymeric@gmail.com>                            *
*   This code is licensed under MIT license (see LICENSE for details)       *
*                                                                           *
****************************************************************************/

#ifndef TUIM_CPP
#define TUIM_CPP

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

    void Update(Key key); // Update the frame depending on the key pressed
    void Clear(); // Clear the current frame buffer
    void Display(); // Draw the current frame buffer to the terminal

    /***********************************************************
    *                    TERMINAL FUNCTIONS                    *
    ***********************************************************/

    //

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
*                    WINDOW FUNCTIONS                      *
***********************************************************/



#endif