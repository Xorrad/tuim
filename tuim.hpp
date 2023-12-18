/****************************************************************************
*                                                                           *
*   TUIM : a text-based user interface for command lines                    *
*                                                                           *
*   Made by Xorrad <monsieurs.aymeric@gmail.com>                            *
*   This code is licensed under MIT license (see LICENSE for details)       *                                            *
*                                                                           *
****************************************************************************/

#pragma once

namespace tuim {
    struct context;
    extern context* ctx;
}

#ifndef TUIM_CPP
#define TUIM_CPP

/***********************************************************
*                         HEADERS                          *
***********************************************************/

#include <iostream>
#include <string>
#include <string.h>
#include <filesystem>
#include <map>
#include <vector>
#include <stdint.h>
#include <memory>
#include <sstream>
#include <regex>
#include <math.h>
#include <chrono>
#include <fstream>

#ifdef __linux__
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#elif _WIN32
#include <windows.h>
#else

#endif

/***********************************************************
*                    MACROS/DEFINES                        *
***********************************************************/

#define TUIM_DEBUG(n) printf("step %d\n", n)

#define TUIM_STYLE_CODE '&'
#define TUIM_STYLE_ESCAPE '/'
#define TUIM_COLOR_CODE '#'
#define TUIM_COLOR_BACKGROUND '_'

#define TUIM_MAKE_KEY2(a, b) (b<<8) + c
#define TUIM_MAKE_KEY3(a, b, c) (a<<16) + (b<<8) + c
#define TUIM_MAKE_KEY4(a, b, c, d) (a<<24) + (b<<16) + (c<<8) + d

namespace tuim {

    /***********************************************************
    *                  FORWARD DECLARATIONS                    *
    ***********************************************************/

    // Types
    typedef uint32_t item_id;
    typedef uint32_t item_flags;
    typedef uint32_t text_flags;
    typedef uint32_t button_flags;
    typedef uint32_t input_text_flags;
    namespace keyboard {
        typedef uint32_t keycode;
    }

    // Structures
    struct context;
    struct container;
    struct item;
    struct vec2;
    namespace color {
        struct color;
    }
    namespace font {
        struct style;
    }

    /***********************************************************
    *                    MATH UTILITIES                        *
    ***********************************************************/

    struct vec2 {
        int x, y;

        vec2() : vec2(0, 0) {}
        vec2(int x, int y) : x(x), y(y) {}

        bool operator==(const vec2 &other) {
            return x == other.x && y == other.y;
        }
    };

    /***********************************************************
    *                  KEYBOARD UTILITIES                      *
    ***********************************************************/

    namespace keyboard {
        enum key {
            NONE = 0,
            ENTER = 10,
            ESCAPE = 27,
            UP = TUIM_MAKE_KEY3(27, 91, 'A'),
            DOWN = TUIM_MAKE_KEY3(27, 91, 'B'),
            RIGHT = TUIM_MAKE_KEY3(27, 91, 'C'),
            LEFT = TUIM_MAKE_KEY3(27, 91, 'D'),
            END = TUIM_MAKE_KEY3(27, 79, 'F'),
            HOME = TUIM_MAKE_KEY3(27, 79, 'H'),
            F1 = TUIM_MAKE_KEY3(27, 79, 80),
            F2 = TUIM_MAKE_KEY3(27, 79, 81),
            F3 = TUIM_MAKE_KEY3(27, 79, 82),
            F4 = TUIM_MAKE_KEY3(27, 79, 83),
            F5 = TUIM_MAKE_KEY3(27, 79, 84),
            F6 = TUIM_MAKE_KEY3(27, 79, 85),
            F7 = TUIM_MAKE_KEY3(27, 79, 86),
            F8 = TUIM_MAKE_KEY3(27, 79, 87),
            F9 = TUIM_MAKE_KEY3(27, 79, 89),
            F10 = TUIM_MAKE_KEY3(27, 79, 90),
            F11 = TUIM_MAKE_KEY3(27, 79, 91),
            F12 = TUIM_MAKE_KEY3(27, 79, 92),
            DELETE = TUIM_MAKE_KEY4(27, 91, 51, 126),
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
            Z = 122,
            BACKSPACE = 127
            // To complete
        };
        keycode get_pressed(); // Get pressed key as unsigned int
        bool is_pressed(); // Check if a key has been pressed
    }

    /***********************************************************
    *                        COLORS                            *
    ***********************************************************/

    namespace color {
        // Structure to store rgb color codes
        struct color {
            int r;
            int g;
            int b;
            bool background;

            bool operator==(const color& other) {
                return r == other.r && g == other.g && b == other.b && background == other.background;
            }
            
            bool operator!=(const color& other) {
                return !(*this == other);
            }
        };

        color from_code(std::string str); // Convert string color code to rgb
        color from_hex(std::string str);  // Convert string hexadecimal color code to rgb
        color from_hex(unsigned int hex); // Convert hexadecimal color code to rgb
        unsigned int to_hex(color color); // Convert rgb color code to hexadecimal
        std::string to_ansi(color color); // Convert rgb color to ansi color
    }

    /***********************************************************
    *                    FONT & TEXT TAGS                      *
    ***********************************************************/

    namespace font {
        enum class mode {
            RESET = 0,
            BOLD = 1,
            DIM = 2,
            ITALIC = 3,
            UNDERLINE = 4,
            BLINKING = 5,
            INVERSE = 7,
            HIDDEN = 8,
            STRIKETHROUGH = 9
        };

        enum class style_type {
            COLOR,
            MODE
        };

        struct style {
            style_type type;
            union {
                color::color style_color;
                mode style_mode;
            };
        };

        void register_style(const std::string &code, const style &style); // Bind a new character to a style
        mode from_code(const std::string &str); // Get the mode bound to a string
        std::string to_ansi(mode mode, bool enabled); // Get the ANSI Escape Sequence of a mode
        style make_style(color::color color); // Create a style var from a color
        style make_style(mode mode); // Create a style var from a mode
    }

    /***********************************************************
    *                         FLAGS                            *
    ***********************************************************/

    enum item_flags_ {
        ITEM_FLAGS_NONE = 0,
        ITEM_FLAGS_DISABLED = 1 << 0,
        ITEM_FLAGS_STAY_ACTIVE = 1 << 1,
    };

    enum button_flags_ {
        BUTTON_FLAGS_NONE = 0,
    };

    enum input_text_flags_ {
        INPUT_TEXT_FLAGS_NONE = 0,
        INPUT_TEXT_FLAGS_NUMERIC_ONLY = 1 << 0,
        INPUT_TEXT_FLAGS_ALPHANUMERIC_ONLY = 1 << 1,
        INPUT_TEXT_ALLOW_LINE_BREAKS = 1 << 2,
    };

    /***********************************************************
    *                    WINDOW FUNCTIONS                      *
    ***********************************************************/

    void init(int argc, char* argv[]); // Initialize tuim with command line arguments
    void create_context(); // Create the global gui context
    void delete_context(); // Delete the global gui context
    context* get_context(); // Get tuim user interface global context

    void set_title(std::string title); // Set title
    void set_cursor_visible(bool cursor); // Set cursor visible

    vec2 get_cursor(); // Get cursor position
    void gotoxy(const vec2& pos); // Move cursor

    color::color get_current_foreground(); // Get active foreground color
    color::color get_current_background(); // Get active background color
    std::vector<font::mode> get_current_modes(); // Get active modes
    void reset_styles(); // Reset tuim styles

    void new_line(int count = 1); // Break to a new line
    void clear(); // Clear terminal output
    void clear_line(); // Clear terminal output
    void print_to_screen(const std::string& str); // Print text to screen and reset style
    template<typename ... Args> void print(const char* fmt, Args ... args); // Format text for printing to screen
    void display(); // Display screen
    void update(tuim::keyboard::keycode key); // Update the items

    /***********************************************************
    *                    TERMINAL FUNCTIONS                    *
    ***********************************************************/

    namespace native {
        vec2 get_window_size(); // Get the terminal window size (columns,rows)

        void set_title(std::string title); // Set terminal title
        void new_line(); // Jump terminal cursor to next line
        void gotoxy(const vec2& pos); // Move terminal cursor to position
        void clear(); // Clear terminal
        void clear_line(); // Clear current terminal line
        void clear_end(); // Clear from cursor to end
        void reset_styles(); // Reset all terminal modes/colors

        void set_cursor_visible(bool cursor); // Set the terminal cursor visible
        bool is_cursor_visible(); // Get the cursor visibility
        
        void show_user_inputs(); // Enable displaying of user inputs
        void hide_user_inputs(); // Hide user inputs
    }

    /***********************************************************
    *                     ITEMS FUNCTIONS                      *
    ***********************************************************/

    item_id str_to_id(const std::string &str); // Hash a string to get an item id
    item_id get_id(); // Get the id of the current item
    uint32_t get_index(tuim::item_id id); // Get the index of a specific item
    uint32_t get_hovered_index(); // Get the index of the hovered item

    bool was_item_active(); // Return true if the last pushed item was active on previous tick
    bool is_item_active(); // Determine if last pushed item is active
    bool is_item_hovered(); // Determine if last pushed item is hovered
    bool is_pressed(tuim::keyboard::key key); // Determine if a specified key has been pressed
    bool is_pressed(tuim::keyboard::keycode code); // Determine if a specified keycode has been pressed
    bool is_pressed(const char* c); // Determine if a specified character has been pressed
    bool has_hoverable(); // Determine if an hoverable item has been pushed

    void set_active_id(item_id id); // Set an item as active
    void set_hovered_id(item_id id); // Set an item as hovered

    void add_item(item item); // Push a new item
    void remove_item(item_id id); // Remove an item

    int calc_text_width(const std::string &str, int padding = 0); // Compute a text width with padding
    int calc_text_vector_width(const std::vector<std::string> &str, int padding = 0); // Compute the max width of strings in a vector
    std::vector<int> calc_columns_width(const std::vector<std::string> &columns, const std::vector<std::vector<std::string>> &rows, int padding = 0); // Compute the max width for each columns

    vec2 calc_relative_position(); // Calculate the coordinates from which to start drawing an item

    /***********************************************************
    *                    COMPONENTS/ITEMS                      *
    ***********************************************************/

    void text(const std::string& id, const std::string& text); // Display text
    void text(const std::string& text); // Display text
    void hr(int length); // Display horizontal bar
    void paragraph(const std::string& id, const std::string& text, unsigned int width); // Display paragraph with automatic line breaks and word spacing
    bool button(const std::string& id, const std::string& text, button_flags flags = BUTTON_FLAGS_NONE); // Display a button
    template <typename T> bool slider(const std::string& id, T* value, T min, T max, T step); // Display a number slider
    template <typename T> bool input_number(const std::string& id, const std::string& text, T* value, T min, T max, T step); // Display a input for numbers
    template <typename U> bool input_enum(const std::string& id, const std::string& text, U* value, int max, const std::map<U, std::string>& labels); // Display a input for enums
    bool input_bool(const std::string& id, const std::string& text, bool* value, const std::map<bool, std::string>& labels = {{false, "False"}, {true, "True"}}); // Display a input for booleans
    bool input_text(const std::string& id, std::string* value, const std::string& default_value, input_text_flags flags = INPUT_TEXT_FLAGS_NONE); // Display a input for string
    bool checkbox(const std::string& id, const std::string& text, bool* value); // Display checkbox
    void animation(const std::string& id, int* current, float speed, const std::vector<std::string>& frames); // Display animated frames
    void scroll_table(const char* id, int *cursor, int *key, std::vector<std::string> &columns, std::vector<std::vector<std::string>> &rows, int height, int padding); // Display a navigable table

    /***********************************************************
    *                    STYLE FUNCTIONS                       *
    ***********************************************************/

    void push_margin(uint32_t margin); // Push a new horizontal margin
    void pop_margin(); // Pop current horizontal margin
    uint32_t get_active_margin(); // Get current horizontal margin

    /***********************************************************
    *                   CONTAINERS FUNCTIONS                   *
    ***********************************************************/

    container& get_container();
    void update_container();
    void move_container_cursor(vec2 pos);
    
    void start_container();
    void end_container();

    void start_column();
    void end_column();

    /***********************************************************
    *                   INTERNAL FUNCTIONS                     *
    ***********************************************************/

    namespace impl {
        void open_terminal(); // Restart program in a separate terminal
        void add_printing_info(const std::string &str); // Log a printing into the stack
    }

    /***********************************************************
    *                    STRING FUNCTIONS                      *
    ***********************************************************/

    namespace string {
        size_t length(const std::string& str); // Get number of characters in string
        std::string fill(const std::string& str, size_t length); // Get a duplicate of a string
        bool is_style(const std::string& str, size_t pos); // Check if there is a style tag in a string
        std::pair<font::style, size_t> extract_style(const std::string& str, size_t pos); // Extract a style from a string
        std::string strip_styles(const std::string& str); // Remove all style tags from string
        std::string escape_styles(const std::string& str); // Escape style tags in string
        uint32_t utf8_to_int(const std::string& c); // Get the codepoint of a utf8 character (as string)
        std::string int_to_utf8(uint32_t code); // Convert a utf8 codepoint to a string
        bool is_printable(uint32_t code); // Check if a utf8 character is printable
        size_t char_length(char c); // Get length (in bytes) of a utf8 character
        uint32_t to_lowercase(uint32_t code); // Get lowercase of utf8 chararcter
        bool is_alphanumeric(uint32_t code); // Check if utf8 character is a letter or number
        bool is_vowel(uint32_t code); // Check if utf8 character is a vowel
    }

    /***********************************************************
    *                    SCREEN BUFFER                         *
    ***********************************************************/
    
    namespace screen {
        struct character {
            std::string str;
            color::color foreground;
            color::color background;
            std::vector<font::mode> modes;

            bool operator==(const character& other) {
                return str == other.str && foreground == other.foreground && background == other.background && modes == other.modes;
            }
        };

        struct buffer {
            vec2 size;
            std::map<uint32_t, character> buffer;

            vec2 get_position(uint32_t index); // Return the position of an index
            uint32_t get_index(const vec2& pos); // Return the index of a position

            bool is_in_buffer(uint32_t index); // Return false if the index is out of bounds
            bool is_character(uint32_t index, const character& ch); // Check if character at index
            character& get_character(uint32_t index); // Get the character at an index
            void set_character(uint32_t index, character ch); // Change the character at an index

            void clear_line(uint32_t y);
            void resize(const vec2& new_size); // Resize the buffer
            void print(); // Print the buffer
        };
    }
    
   /***********************************************************
    *                    UI STRUCTURES                         *
    ***********************************************************/

    // This structure is not really used yet
    struct printing_info {
        vec2 pos;
        uint32_t size;
    };

    struct container {
        vec2 pos;
        vec2 size;
        vec2 cursor;

        container(vec2 pos) : container(pos, {0, 0}) {}
        container(vec2 pos, vec2 size) : pos(pos), size(size) {}
    };

    struct item {
        item_id id;
        item_flags flags;
    };

    struct context {
        item_id active_id; // Id of active item during current frame
        item_id last_active_id; // Id of active item during last frame
        item_id hovered_id; // Id of hovered item during current frame

        vec2 cursor; // Current position of the cursor
        bool cursor_visible; // Is the terminal (blinking) cursor enabled

        keyboard::keycode pressed_key; // Value of pressed key during frame

        std::map<std::string, font::style> style_codes; // Dictionary of registered styles
        std::map<font::mode, bool> style_modes; // Dictionary of active modes (BOLD, ITALIC...)
        color::color foreground_color;
        color::color background_color;

        std::vector<item> items_stack; // Items during last frame (?)
        std::vector<container> containers_stack; // Containers during current frame
        std::vector<uint32_t> margins_stack; // Current margins

        screen::buffer last_frame; // The screen buffer of the last frame (already drawn)
        screen::buffer current_frame; // The screen buffer of current frame (being drawn)

        std::vector<printing_info> printings_stack; // Store what has been drawn by the last item (not used)

        context() {
            active_id = 0;
            last_active_id = 0;
            hovered_id = 0;

            cursor = {1, 1}; // For whatever reasons, (0,0) and (1,1) seems to be the same pos
            cursor_visible = true;

            pressed_key = tuim::keyboard::NONE;

            style_codes = {
                { "r", font::make_style(font::mode::RESET) },
                { "b", font::make_style(font::mode::BOLD) },
                { "d", font::make_style(font::mode::DIM) },
                { "i", font::make_style(font::mode::ITALIC) },
                { "u", font::make_style(font::mode::UNDERLINE) },
                { "g", font::make_style(font::mode::BLINKING) },
                { "n", font::make_style(font::mode::INVERSE) },
                { "h", font::make_style(font::mode::HIDDEN) },
                { "s", font::make_style(font::mode::STRIKETHROUGH) }
            };

            items_stack = std::vector<item>(0);
            containers_stack = std::vector<container>{ container{{0, 0}} };
            margins_stack = std::vector<uint32_t>(0);

            vec2 window_size = native::get_window_size();
            last_frame = screen::buffer{window_size};
            current_frame = screen::buffer{window_size};

            printings_stack = std::vector<printing_info>(0);
        }
        ~context() {}
    };

    /***********************************************************
    *                    GLOBAL VARIABLES                      *
    ***********************************************************/

    inline context* ctx; // Global context variable for the gui
}

/***********************************************************
*                   INLINE DEFINITIONS                     *
***********************************************************/

/***********************************************************
*                    WINDOW FUNCTIONS                      *
***********************************************************/

inline void tuim::init(int argc, char* argv[]) {
    // Restart if not in separate terminal
    if(argc < 2 || strcmp(argv[1], "f") != 0) {
        //tuim::impl::open_terminal();
    }

    tuim::create_context();
}

inline void tuim::create_context() {
    printf("\033[?1049h"); // enable alternate buffer
    tuim::ctx = new tuim::context();
    tuim::native::hide_user_inputs();
    tuim::native::set_cursor_visible(false);
    tuim::reset_styles();
}

inline void tuim::delete_context() {
    printf("\033[?1049l"); // disabled alternate buffer
    tuim::native::show_user_inputs();
    tuim::native::set_cursor_visible(true);
    delete tuim::ctx;
}

inline tuim::context* tuim::get_context() {
    if(tuim::ctx == nullptr) throw std::runtime_error("error context undefined");
    return tuim::ctx;
}

inline void tuim::set_title(std::string title) {
    tuim::native::set_title(title);
}

inline void tuim::set_cursor_visible(bool cursor) {
    context* ctx = get_context();
    ctx->cursor_visible = cursor;
    tuim::native::set_cursor_visible(cursor);
}

inline tuim::vec2 tuim::get_cursor() {
    context* ctx = get_context();
    return ctx->cursor;
}

inline void tuim::gotoxy(const vec2& pos) {
    context* ctx = get_context();
    ctx->cursor = pos;
}

inline tuim::color::color tuim::get_current_foreground() {
    context* ctx = get_context();
    return ctx->foreground_color;
}

inline tuim::color::color tuim::get_current_background() {
    context* ctx = get_context();
    return ctx->background_color;
}

inline std::vector<tuim::font::mode> tuim::get_current_modes() {
    context* ctx = get_context();
    std::vector<font::mode> modes;
    for(auto[mode, b] : ctx->style_modes) {
        if(b) modes.push_back(mode);
    }
    return modes;
}

inline void tuim::reset_styles() {
    context* context = get_context();
    ctx->style_modes.clear();
    ctx->foreground_color = {255, 255, 255, false};
    ctx->background_color = {0, 0, 0, true};
}

inline void tuim::new_line(int count) {
    context* ctx = get_context();
    ctx->cursor = {(int) get_active_margin(), ctx->cursor.y+count};
}

inline void tuim::clear() {
    ctx->margins_stack.clear();
    ctx->pressed_key = tuim::keyboard::NONE;
    ctx->current_frame = {ctx->current_frame.size};
    tuim::gotoxy({1, 1});
    tuim::reset_styles();
}

inline void tuim::clear_line() {
    context* ctx = get_context();
    ctx->cursor = {1, ctx->cursor.y};
    ctx->current_frame.clear_line(ctx->cursor.y);
}

inline void tuim::print_to_screen(const std::string& str) {
    context* ctx = get_context();

    // We read the string and move the global cursor accordingly
    for(size_t i = 0; i < str.length();) {
        size_t l = tuim::string::char_length(str.at(i));
        
        // Check if there is a style tag at this index
        font::style style;
        size_t style_length = 0;
        std::tie(style, style_length) = string::extract_style(str, i);
        if(style_length > 0) {
            switch(style.type) {
                case font::style_type::COLOR:
                    if(style.style_color.background)
                        ctx->background_color = style.style_color;
                    else
                        ctx->foreground_color = style.style_color;
                    break;
                case font::style_type::MODE:
                    if(ctx->style_modes.count(style.style_mode))
                        ctx->style_modes.erase(style.style_mode);
                    else
                        ctx->style_modes[style.style_mode] = true;
                    if(style.style_mode == font::mode::RESET) {
                        reset_styles();
                        uint32_t index = ctx->current_frame.get_index(ctx->cursor);
                        if(index >= 0) {
                            if(ctx->current_frame.buffer.count(index) == 0) {
                                screen::character ch = {" ", get_current_foreground(), get_current_background(), {font::mode::RESET}};
                                ctx->current_frame.set_character(index, ch);
                            }
                            else {
                                screen::character ch = ctx->current_frame.get_character(index);
                                ch.modes.push_back(font::mode::RESET);
                                ctx->current_frame.set_character(index, ch);
                            }
                        }
                    }
                    break;
            }
            i += style_length;
            continue;
        }

        // Move global cursor to next line when line break
        // and take into account the current margin
        if(str.at(i) == '\n') {
            tuim::new_line();
        }
        // ANSI Escape sequences are not displayed in the terminal
        // So we don't move the cursor when changing color or mode
        // Note: Escape sequences related to the cursor are not checked yet
        else if(str.at(i) == '\033') {
            do {
                i++;
            } while(i < str.length() && !((str.at(i) >= 'a' && str.at(i) <= 'z') || (str.at(i) >= 'A' && str.at(i) <= 'Z')));
        }
        // Other character only take one "space" in the buffer
        else {
            uint32_t index = ctx->current_frame.get_index(ctx->cursor);
            screen::character ch = {str.substr(i, l), get_current_foreground(), get_current_background(), get_current_modes()};
            if(ctx->current_frame.buffer.count(index) == 1 && ctx->current_frame.buffer.at(index).str == " ") {
                const std::vector<font::mode>& modes = ctx->current_frame.buffer.at(index).modes;
                if(modes.size() > 0 && modes.at(0) == font::mode::RESET)
                    ch.modes.push_back(font::mode::RESET);
            }
            ctx->current_frame.set_character(index, ch);
            ctx->cursor.x++;
        }

        i += l;
    }
    update_container();
}

template<typename ... Args>
inline void tuim::print(const char* fmt, Args ... args) {
    // Replace the arguments in the format string
    int size_s = std::snprintf(nullptr, 0, fmt, args...) + 1;
    if(size_s <= 0) throw std::runtime_error("error during formatting");
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[ size ]);
    std::snprintf(buf.get(), size, fmt, args...);
    std::string parsed = std::string( buf.get(), buf.get() + size - 1);

    // Replace the style tags by their ANSI equivalent
    // parsed = tuim::string::parse_styles(parsed);

    print_to_screen(parsed);

    int width = calc_text_width(parsed);
    vec2 pos = get_container().cursor;
    pos.x += width;
    move_container_cursor(pos);
}

inline void tuim::display() {
    context* ctx = get_context();
    screen::buffer& last_frame = ctx->last_frame;
    screen::buffer& current_frame = ctx->current_frame;

    color::color foreground = {255,255,255,false};
    color::color background = {0,0,0,true};
    std::map<font::mode, bool> modes;

    vec2 cursor = {0,0};
    vec2 last_cursor = {-1,0};
    uint32_t max_index = current_frame.size.x + current_frame.size.x*current_frame.size.y;

    for(uint32_t index = 0; index < max_index; index++) {
        if(current_frame.buffer.count(index)) {
            screen::character& ch = current_frame.get_character(index);

            // Styles have to be taken into account every frame
            // because they are not stored "in the screen" like characters
            // but are more like toggles
            auto reset_pos = std::find(ch.modes.begin(), ch.modes.end(), font::mode::RESET);
            if(reset_pos != ch.modes.end()) {
                printf(font::to_ansi(font::mode::RESET, true).c_str());
                foreground = {255,255,255,false};
                background = {0,0,0,true};
                ch.modes.erase(reset_pos);
            }
            for(font::mode mode : ch.modes) {
                if(modes.count(mode) == 0) {
                    printf(font::to_ansi(mode, true).c_str());
                }
                modes[mode] = true;
            }
            for(auto it = modes.begin(); it != modes.end();) {
                if(!it->second) {
                    printf(font::to_ansi(it->first, false).c_str());
                    it = modes.erase(it);
                }
                else {
                    modes[it->first] = false;
                    it++;
                }
            }
            if(foreground != ch.foreground) {
                printf(color::to_ansi(ch.foreground).c_str());
                foreground = ch.foreground;
            }
            if(background != ch.background) {
                printf(color::to_ansi(ch.background).c_str());
                background = ch.background;
            }

            // If the character was not present during last frame, or
            // if it has been changed, 
            if(last_frame.buffer.count(index) == 0 || !last_frame.is_character(index, ch)) {
                if(last_cursor.x != cursor.x-1 || last_cursor.y != cursor.y)
                    native::gotoxy(cursor);
                printf(ch.str.c_str());
                last_cursor = cursor;
            }    
        }
        else if(last_frame.buffer.count(index)) {
            native::gotoxy(cursor);
            native::reset_styles();
            printf(" ");
        }

        cursor.x++;
        if(cursor.x >= current_frame.size.x) {
            cursor.x = 0;
            cursor.y++;
        }
    }
    // Reset modes and colors
    // Clear from the last line to end of screen
    native::reset_styles();
    native::gotoxy({0, cursor.y+1});
    native::clear_end();
    ctx->last_frame = ctx->current_frame;
}

/***********************************************************
*                     TERMINAL FUNCTIONS                   *
***********************************************************/

inline tuim::vec2 tuim::native::get_window_size() {
    winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return vec2{size.ws_col, size.ws_row};
}

inline void tuim::native::set_title(std::string title) {
    // http://www.faqs.org/docs/Linux-mini/Xterm-Title.html#s3
    printf("\033]0;%s\007", title.c_str());
}

inline void tuim::native::new_line() {
    printf("\n");
}

inline void tuim::native::gotoxy(const vec2& pos) {
    printf("\033[%d;%dH", pos.y, pos.x);
}

inline void tuim::native::clear() {
    printf("\033[2J\033[0m\033[H");
}

inline void tuim::native::clear_line() {
    printf("\033[2K");
}

inline void tuim::native::clear_end() {
    printf("\033[0J");
}

inline void tuim::native::reset_styles() {
    printf("\033[0m");
}

inline void tuim::native::set_cursor_visible(bool cursor) {
    printf("\033[?25%c", (cursor ? 'h' : 'l'));
}

inline bool tuim::native::is_cursor_visible() {
    context* ctx = get_context();
    return ctx->cursor_visible;
}

inline void tuim::native::show_user_inputs() {
    termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ECHO;
    tcsetattr(0, TCSANOW, &term);
}

inline void tuim::native::hide_user_inputs() {
    termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &term);
}

/***********************************************************
*                     ITEMS FUNCTIONS                      *
***********************************************************/

inline tuim::item_id tuim::str_to_id(const std::string &str) {
    // http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hash = 5381;
    for (size_t i = 0; i < str.size(); ++i)
        hash = 33 * hash + (unsigned char) str[i];
    return (tuim::item_id) hash;
}

inline tuim::item_id tuim::get_id() {
    tuim::context* ctx = tuim::get_context();
    return ctx->items_stack.at(ctx->items_stack.size() - 1).id;
}

inline uint32_t tuim::get_index(tuim::item_id id) {
    tuim::context *ctx = tuim::get_context();
    for(size_t i = 0; i < ctx->items_stack.size(); i++) {
        if(ctx->items_stack.at(i).id == id) {
            return i;
        }
    }
    return -1;
}

inline uint32_t tuim::get_hovered_index() {
    tuim::context *ctx = tuim::get_context();
    return tuim::get_index(ctx->hovered_id);
}

inline bool tuim::was_item_active() {
    tuim::context* ctx = tuim::get_context();
    return ctx->last_active_id == tuim::get_id();
}

inline bool tuim::is_item_active() {
    tuim::context* ctx = tuim::get_context();
    return ctx->active_id == tuim::get_id();
}

inline bool tuim::is_item_hovered() {
    tuim::context* ctx = tuim::get_context();
    return ctx->hovered_id == tuim::get_id();
}

inline bool tuim::is_pressed(tuim::keyboard::key key) {
    return tuim::is_pressed((tuim::keyboard::keycode) key);
}

inline bool tuim::is_pressed(tuim::keyboard::keycode code) {
    tuim::context* ctx = tuim::get_context();
    return ctx->pressed_key == code;
}

inline bool tuim::is_pressed(const char* c) {
    tuim::context* ctx = tuim::get_context();
    return ctx->pressed_key == tuim::string::utf8_to_int(c);
}

inline bool tuim::has_hoverable() {
    tuim::context* ctx = tuim::get_context();
    for(size_t i = 0; i < ctx->items_stack.size(); i++) {
        if(!(ctx->items_stack.at(i).flags & tuim::item_flags_::ITEM_FLAGS_DISABLED))
            return true;
    }
    return false;
}

inline void tuim::set_active_id(item_id id) {
    tuim::context *ctx = tuim::get_context();
    ctx->active_id = id;
}

inline void tuim::set_hovered_id(item_id id) {
    tuim::context *ctx = tuim::get_context();
    ctx->hovered_id = id;
}

inline void tuim::add_item(item item) {
    tuim::context *ctx = tuim::get_context();
    ctx->items_stack.push_back(item);
    ctx->printings_stack.clear();
}

inline void tuim::remove_item(item_id id) {
    tuim::context *ctx = tuim::get_context();
    for(size_t i = 0; i < ctx->items_stack.size(); i++) {
        if(ctx->items_stack.at(i).id == i)
            ctx->items_stack.erase(ctx->items_stack.begin() + i);
    }
}

inline void tuim::update(tuim::keyboard::keycode key) {
    tuim::context* ctx = tuim::get_context();
    ctx->pressed_key = key;

    if(ctx->active_id == 0 || !(ctx->items_stack.at(tuim::get_index(ctx->active_id)).flags & tuim::item_flags_::ITEM_FLAGS_STAY_ACTIVE)) {
        ctx->last_active_id = ctx->active_id;
        ctx->active_id = 0;

        int hovered_index = tuim::get_hovered_index();

        // std::cerr << "has hoverable: " << tuim::has_hoverable() << std::endl;
        // std::cerr << "index: " << hovered_index << std::endl;
        // std::cerr << "hovered: " << ctx->hovered_id << std::endl;

        // Set the first hoverable item hovered if no item is
        if(hovered_index == -1 && tuim::has_hoverable()) {
            for(size_t i = 0; i < ctx->items_stack.size(); i++) {
                if(!(ctx->items_stack.at(i).flags & tuim::item_flags_::ITEM_FLAGS_DISABLED)) {
                    ctx->hovered_id = ctx->items_stack.at(i).id;
                    hovered_index = i;
                    break;
                }
            }
        }

        // Move cursor to previous hoverable item
        if(key == keyboard::UP) {
            if(tuim::has_hoverable()) {
                tuim::item_id id = 0;
                if(hovered_index != -1) {
                    size_t index = std::max(0, hovered_index - 1);
                    while(index > 0 && (ctx->items_stack.at(index).flags & tuim::item_flags_::ITEM_FLAGS_DISABLED)) index--;
                    if((ctx->items_stack.at(index).flags & tuim::item_flags_::ITEM_FLAGS_DISABLED)) index = hovered_index;
                    id = ctx->items_stack.at(index).id;
                }
                ctx->hovered_id = id;
            }
        }

        // Move cursor to next hoverable item
        if(key == keyboard::DOWN) {
            if(tuim::has_hoverable()) {
                tuim::item_id id = 0;
                if(hovered_index != -1) {
                    size_t index = std::min(hovered_index + 1, (int) ctx->items_stack.size() - 1);
                    while(index < (ctx->items_stack.size() - 1) && (ctx->items_stack.at(index).flags) & tuim::item_flags_::ITEM_FLAGS_DISABLED) index++;
                    if((ctx->items_stack.at(index).flags & tuim::item_flags_::ITEM_FLAGS_DISABLED)) index = hovered_index;
                    id = ctx->items_stack.at(index).id;
                }
                ctx->hovered_id = id;
            }
        }
    }

    ctx->items_stack.clear();
}

inline int tuim::calc_text_width(const std::string &str, int padding) {
    // https://github.com/chalk/ansi-regex/blob/main/index.js
    // Remove ANSI Escape sequences because they are not displayed
    // static const std::string pattern = "[\\u001B\\u009B][[\\]()#;?]*(?:(?:(?:(?:;[-a-zA-Z\\d\\/#&.:=?%@~_]+)*|[a-zA-Z\\d]+(?:;[-a-zA-Z\\d\\/#&.:=?%@~_]*)*)?\\u0007)|(?:(?:\\d{1,4}(?:;\\d{0,4})*)?[\\dA-PR-TZcf-nq-uy=><~]))";
    // static const std::regex regex(pattern);
    // std::string stripped = std::regex_replace(str, regex, "");
    std::string stripped = string::strip_styles(str);
    int width = tuim::string::length(stripped) + padding*2;
    return width;
}

inline int tuim::calc_text_vector_width(const std::vector<std::string> &str, int padding) {
    int max_width = 0;
    for(size_t i = 0; i < str.size(); i++) {
        int width = tuim::calc_text_width(str[i], padding);
        if(width > max_width) max_width = width;
    }
    return max_width + padding*2;
}

inline std::vector<int> tuim::calc_columns_width(const std::vector<std::string> &columns, const std::vector<std::vector<std::string>> &rows, int padding) {
    std::vector<int> columns_width = std::vector<int>(columns.size(), 0);
    for(size_t i = 0; i < columns.size(); i++) {
        int max_width = 0;
        for(size_t j = 0; j < rows.size(); j++) {
            int row_width = tuim::calc_text_width(rows[j][i], padding);
            if(row_width > max_width) max_width = row_width;
        }
        columns_width[i] = std::max(tuim::calc_text_width(columns[i], padding), max_width);
    }
    return columns_width;
}

inline tuim::vec2 tuim::calc_relative_position() {
    context* ctx = get_context();
    container ctn = ctx->containers_stack.back();
    vec2 pos = ctn.cursor;

    return pos;
}

/***********************************************************
*                    COMPONENTS/ITEMS                      *
***********************************************************/

inline void tuim::text(const std::string& id, const std::string& text) {
    tuim::item item = tuim::item{ tuim::str_to_id(id), tuim::item_flags_::ITEM_FLAGS_DISABLED };
    tuim::add_item(item);
    tuim::print(text.c_str());
}

inline void tuim::text(const std::string& text) {
    tuim::item item = tuim::item{ tuim::str_to_id(text), tuim::item_flags_::ITEM_FLAGS_DISABLED };
    tuim::add_item(item);
    tuim::print(text.c_str());
}

inline void tuim::hr(int length) {
    std::string str = "\n&s" + tuim::string::fill("―", length) + "&r\n";
    tuim::print(str.c_str());
}

inline void tuim::paragraph(const std::string& id, const std::string& text, unsigned int width) {
    tuim::item item = tuim::item{ tuim::str_to_id(id), tuim::item_flags_::ITEM_FLAGS_DISABLED };
    tuim::add_item(item);

    vec2 cursor = get_cursor();
    int margin = cursor.x;
    size_t i = 0;
    
    tuim::push_margin(margin);

    std::string line = "";
    size_t line_ch_len = 0;
    size_t line_blank_count = 0;

    bool line_break = false;

    while(i < text.length()) {
        size_t word_bytes_len = 0;
        size_t word_ch_len = 0;
        size_t word_pos = i;

        // Count byte and char length of next word (until end of text or space).
        while(word_pos < text.length() && text.at(word_pos) != ' ' && text.at(word_pos) != '\n') {
            word_bytes_len += tuim::string::char_length(text.at(word_pos));
            word_ch_len++;
            word_pos = i + word_bytes_len;
        }

        bool skip_next_line = (word_pos < text.length() && text.at(word_pos) == '\n');
        bool is_eol = line_ch_len > 0 && (line_ch_len + word_ch_len) >= width;

        std::string word = text.substr(i, word_bytes_len);
        word_ch_len = tuim::calc_text_width(word);

        // Handle the end of line (need at least one word).
        if(is_eol || line_break) {

            // Count the number of spaces/words for each line and find the number of blank missing to reach EOL.
            // Distribute additional spaces between each words to fill the blank at EOL.
            if(line_ch_len > 0) {
                int diff = width - line_ch_len;
                if(diff > 0 && !line_break && line_blank_count > 0) {
                    float step = (float) diff / (float) (line_blank_count);
                    int blank_count = 0;
                    float acc = 0.f;
                    while(blank_count < diff) {
                        for(size_t k = 0; k < line.length(); k += tuim::string::char_length(line.at(k))) {
                            if(blank_count == diff)
                                break;
                            if(line.at(k) != ' ')
                                continue;
                            if(acc >= 1.f) {
                                line.insert(k, 1, ' ');
                                k++;
                                blank_count++;
                                acc -= 1.f;
                            }
                            acc += step;
                        }
                    }
                }
                tuim::print(line.c_str());
                // printf("\tdiff: %d ; step: %f ; acc: %f ; t: %d ; c: %d", diff, step, acc, line_blank_count, blank_count);
            }

            cursor = get_cursor();
            cursor = {margin, cursor.y+1};
            tuim::gotoxy(cursor);

            line = "";
            line_ch_len = 0;
            line_blank_count = 0;
            line_break = false;
        }

        //Add a space between words.
        if(line_ch_len > 0) {
            line += " ";
            line_ch_len++;
            line_blank_count++;
        }

        line += word;
        line_ch_len += word_ch_len;

        if(skip_next_line)
            line_break = true;

        i = word_pos + 1;
    }

    if(line_ch_len > 0) {
        tuim::print(line.c_str());
    }

    tuim::print("&r");
    for(int i = 0; i < 1 + line_break; i++) tuim::print("\n");

    tuim::pop_margin();
}

inline bool tuim::button(const std::string& id, const std::string& text, button_flags flags) {
    tuim::item item = tuim::item{ tuim::str_to_id(id), tuim::item_flags_::ITEM_FLAGS_NONE };
    tuim::add_item(item);

    if(tuim::is_item_hovered()) {
        if(tuim::is_pressed(keyboard::ENTER)) {
            tuim::set_active_id(item.id);
        }
        tuim::print("[x] ");
    }
    else tuim::print("[ ] ");

    tuim::print(text.c_str());

    return tuim::is_item_active();
}

template <typename T>
inline bool tuim::slider(const std::string& id, T* value, T min, T max, T step) {
    tuim::item item = tuim::item{ tuim::str_to_id(id), tuim::item_flags_::ITEM_FLAGS_NONE };
    tuim::add_item(item);

    if(tuim::is_item_hovered()) {
        if(tuim::is_pressed(keyboard::ENTER)) {
            tuim::print("[*] ");
            tuim::set_active_id(item.id);
        }
        else tuim::print("[x] ");

        if(tuim::is_pressed(keyboard::LEFT)) {
            *value = std::max(min, *value - step);
        }
        else if(tuim::is_pressed(keyboard::RIGHT)) {
            *value = std::min(max, *value + step);
        }
    }
    else tuim::print("[ ] ");

    tuim::print("#555555"); // Push grey color
    float prct = ((float) (*value - min) / (float) (max-min))*100.f;
    for(int t = 0; t < 100; t++) {
        if(t >= prct) tuim::print("&r");
        tuim::print("█");
    }
    if(tuim::is_item_hovered() && tuim::is_pressed(keyboard::ENTER)) {
        tuim::print("&r ");
        tuim::native::show_user_inputs();
        std::cin >> *value;
        tuim::native::hide_user_inputs();
    }
    tuim::print("&r %s", std::to_string(*value).c_str());

    return tuim::is_item_active();
}

template <typename T>
inline bool tuim::input_number(const std::string& id, const std::string& text, T* value, T min, T max, T step) {
    tuim::item item = tuim::item{ tuim::str_to_id(id), tuim::item_flags_::ITEM_FLAGS_NONE };
    tuim::add_item(item);

    if(tuim::is_item_hovered()) {
        if(tuim::is_pressed(keyboard::ENTER)) {
            tuim::print("[*] ");
            tuim::native::show_user_inputs();
            std::cin >> *value;
            tuim::native::hide_user_inputs();
            tuim::set_active_id(item.id);
        }
        else tuim::print("[x] ");
        
        if(tuim::is_pressed(keyboard::LEFT)) {
            *value = std::max(min, *value - step);
            tuim::set_active_id(item.id);
        }
        else if(tuim::is_pressed(keyboard::RIGHT)) {
            *value = std::min(max, *value + step);
            tuim::set_active_id(item.id);
        }
    }
    else tuim::print("[ ] ");

    tuim::print(text.c_str(), *value);

    return tuim::is_item_active();
}

template <typename U>
inline bool tuim::input_enum(const std::string& id, const std::string& text, U* value, int max, const std::map<U, std::string>& labels) {
    tuim::item item = tuim::item{ tuim::str_to_id(id), tuim::item_flags_::ITEM_FLAGS_NONE };
    tuim::add_item(item);

    if(tuim::is_item_hovered()) {
        if(tuim::is_pressed(keyboard::LEFT)) {
            *value = static_cast<U>((*value == 0) ? max - 1 : *value - 1);
            tuim::set_active_id(item.id);
        }
        else if(tuim::is_pressed(keyboard::RIGHT)) {
            *value = static_cast<U>((*value + 1) % max);
            tuim::set_active_id(item.id);
        }
        
        tuim::print("[x] ");
    }
    else tuim::print("[ ] ");

    tuim::print(text.c_str(), labels.at(*value).c_str());

    return tuim::is_item_active();
}

inline bool tuim::input_bool(const std::string& id, const std::string& text, bool* value, const std::map<bool, std::string>& labels) {
    return input_enum<bool>(id, text, value, 2, labels);
}

inline bool tuim::input_text(const std::string& id, std::string* value, const std::string& default_value, input_text_flags flags) {
    context* ctx = get_context();
    tuim::item item = tuim::item{ tuim::str_to_id(id), tuim::item_flags_::ITEM_FLAGS_STAY_ACTIVE };
    tuim::add_item(item);

    static size_t cursor = value->length();

    if(tuim::is_item_active()) {
        if(tuim::is_pressed(keyboard::LEFT)) {
            if(cursor > 0) {
                do {
                    cursor--;
                } while(cursor > 0 && ((0b00000011 & (value->at(cursor) >> 6)) == 0b10));
            }
        }
        else if(tuim::is_pressed(keyboard::RIGHT)) {
            if(cursor < value->length())
                cursor += tuim::string::char_length(value->at(cursor));
        }
        else if(tuim::is_pressed(keyboard::DELETE)) {
            if(cursor < value->length()) {
                *value = value->erase(cursor, tuim::string::char_length(value->at(cursor)));
            }
        }
        else if(tuim::is_pressed(keyboard::BACKSPACE)) {
            if(cursor > 0) {
                size_t l = 0;
                do {
                    l++;
                    cursor--;
                } while(cursor > 0 && (0b00000011 & (value->at(cursor) >> 6)) == 0b10);
                *value = value->erase(cursor, l);
            }
        }
        else if(tuim::is_pressed(keyboard::UP)) {
            if(flags & INPUT_TEXT_ALLOW_LINE_BREAKS) {
                if(cursor > 0) {
                    size_t line_pos = 0;
                    size_t prev_line_len = 0;

                    // Move cursor until end of last line.
                    // Count position in initial line.
                    while(cursor > 0) {
                        if(line_pos > 0 && value->at(cursor) == '\n') break;
                        do {
                            line_pos++;
                            cursor--;
                        } while(cursor > 0 && ((0b00000011 & (value->at(cursor) >> 6)) == 0b10));
                    }

                    // Move cursor until end of last line.
                    // Count line length.
                    while(cursor > 0) {
                        if(prev_line_len > 0 && value->at(cursor) == '\n') break;
                        do {
                            prev_line_len++;
                            cursor--;
                        } while(cursor > 0 && ((0b00000011 & (value->at(cursor) >> 6)) == 0b10));
                    }

                    // If it is the beginning of the text, we have to move one char less,
                    // because the cursor is not at end of last line, but start of current line.
                    if(cursor == 0 && value->at(cursor) != '\n') line_pos--;
                    cursor += std::min(line_pos, prev_line_len);
                }
            }
        }
        else if(tuim::is_pressed(keyboard::DOWN)) {
            if(flags & INPUT_TEXT_ALLOW_LINE_BREAKS) {
                if(cursor < value->length()) {
                    size_t line_pos = 0;
                    size_t next_line_len = 0;
                    size_t tmp_cursor = cursor;

                    // Count position in initial line.
                    while(tmp_cursor > 0) {
                        if(value->at(tmp_cursor-1) == '\n') break;
                        do {
                            line_pos++;
                            tmp_cursor--;
                        } while(tmp_cursor > 0 && ((0b00000011 & (value->at(tmp_cursor) >> 6)) == 0b10));
                    }

                    // Advance cursor until line break.
                    while(cursor < value->length()) {
                        if(value->at(cursor) == '\n') {
                            cursor++;
                            break;
                        }
                        cursor += tuim::string::char_length(value->at(cursor));
                    }

                    // Count next line length
                    tmp_cursor = cursor;
                    while(tmp_cursor < value->length()) {
                        if(value->at(tmp_cursor) == '\n') break;
                        size_t ch_len = tuim::string::char_length(value->at(tmp_cursor));
                        next_line_len += ch_len;
                        tmp_cursor += ch_len;
                    }

                    cursor += std::min(line_pos, next_line_len);
                }
            }
        }
        else if(tuim::is_pressed(keyboard::ESCAPE)) tuim::set_active_id(0);
        else {
            uint32_t code = ctx->pressed_key;
            if(tuim::string::is_printable(code)) {
                if((flags & INPUT_TEXT_FLAGS_NUMERIC_ONLY && isdigit(code))
                || (flags & INPUT_TEXT_FLAGS_ALPHANUMERIC_ONLY && tuim::string::is_alphanumeric(code))
                || !(flags & (INPUT_TEXT_FLAGS_NUMERIC_ONLY | INPUT_TEXT_FLAGS_ALPHANUMERIC_ONLY))) {
                    std::string str = tuim::string::int_to_utf8(code);
                    for(size_t i = 0; i < str.length(); i++)
                        (*value).insert(value->begin() + cursor + i, str[i]);
                    cursor += str.length();
                }
            }
        }
    }

    if(tuim::is_item_hovered()) {
        if(tuim::is_pressed(keyboard::ENTER)) {
            if(tuim::is_item_active()) {
                if(flags & INPUT_TEXT_ALLOW_LINE_BREAKS) {
                    (*value).insert(value->begin() + cursor, '\n');
                    cursor++;
                }
                else {
                    tuim::set_active_id(0);
                }
            }
            else {
                tuim::set_active_id(item.id);
                cursor = value->length();
            }
        }
        if(tuim::is_item_active()) tuim::print("[*] ");
        else tuim::print("[x] ");
    }
    else tuim::print("[ ] ");

    tuim::push_margin(tuim::get_cursor().x);

    tuim::print("#_555555");
    size_t i = 0;
    while(i < value->length()) {
        size_t l = tuim::string::char_length(value->at(i));
        std::string ch = value->substr(i, l);

        if(tuim::is_item_active() && (ch.at(0) == TUIM_STYLE_CODE || ch.at(0) == TUIM_COLOR_CODE))
            ch = TUIM_STYLE_ESCAPE + ch;

        if(tuim::is_item_active() && !tuim::keyboard::is_pressed() && cursor == i) {
            if(ch == "\n") ch = " \n";
            tuim::print("#_ffffff#555555%s&r#_555555", ch.c_str());
        }
        else tuim::print("%s", ch.c_str());
        
        i += l;
    }
    if(tuim::is_item_active() && !tuim::keyboard::is_pressed() && cursor == i) tuim::print("#_ffffff ");
    tuim::print("&r");

    tuim::pop_margin();

    return tuim::is_item_active();
}

inline bool tuim::checkbox(const std::string& id, const std::string& text, bool* value) {
    tuim::item item = tuim::item{ tuim::str_to_id(id), tuim::item_flags_::ITEM_FLAGS_NONE };
    tuim::add_item(item);

    if(tuim::is_item_hovered()) {
        if(tuim::is_pressed(keyboard::ENTER)) {
            tuim::set_active_id(item.id);
            *value = !*value;
        }
        tuim::print("[x] ");
    }
    else tuim::print("[ ] ");

    tuim::print(text.c_str(), (*value ? "✔" : "✗"));

    return tuim::is_item_active();
}

inline void tuim::animation(const std::string& id, int* current, float speed, const std::vector<std::string>& frames) {
    static std::map<tuim::item_id, uint64_t> timestamps;
    
    tuim::item_id item_id = tuim::str_to_id(id);
    tuim::item item = tuim::item{ item_id, tuim::item_flags_::ITEM_FLAGS_DISABLED };
    tuim::add_item(item);

    // Get current time in milliseconds
    uint64_t current_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    // Check if first iteration or time to change frame
    if(timestamps.count(item_id) == 0 || current_time - timestamps.at(item_id) >= speed * 1000) {
        timestamps[item_id] = current_time;
        (*current) = (*current == (int) frames.size()-1) ? 0 : *current + 1;
    }

    tuim::print(frames.at(*current).c_str());
}

inline void tuim::scroll_table(const char* id, int *cursor, int *key, std::vector<std::string> &columns, std::vector<std::vector<std::string>> &rows, int height, int padding) {
    tuim::item item = tuim::item{ tuim::str_to_id(id), tuim::item_flags_::ITEM_FLAGS_STAY_ACTIVE };
    tuim::add_item(item);

    if(tuim::is_item_active()) {
        if(tuim::is_pressed(keyboard::UP)) {
            *cursor = std::max(0, *cursor - 1);
        }
        if(tuim::is_pressed(keyboard::DOWN)) {
            *cursor = std::min(*cursor + 1, (int) rows.size()-1);
        }
        if(tuim::is_pressed(keyboard::ENTER)) {
            //row clicked
        }

        if(tuim::is_pressed(keyboard::LEFT)) {
            *key = std::max(0, *key - 1);
        }
        if(tuim::is_pressed(keyboard::RIGHT)) {
            *key = std::min(*key + 1, (int) columns.size()-1);
        }

        if(tuim::is_pressed(keyboard::ESCAPE)) tuim::set_active_id(0);
    }

    if(tuim::is_item_hovered()) {
        if(tuim::is_pressed(keyboard::ENTER)) {
            tuim::set_active_id(item.id);
        }
        if(tuim::is_item_active()) tuim::print("[*]\n");
        else tuim::print("[x]\n");
    }
    else tuim::print("[ ]\n");

    // Calculate the max width for each columns
    std::vector<int> columns_width = tuim::calc_columns_width(columns, rows, padding);

    auto print_table_border = [&](const char *c1, const char *c2, const char *c3) {
        tuim::print(c1);
        for(size_t i = 0; i < columns.size(); i++) {
            if(i != 0) tuim::print(c2);
            tuim::print("%s", tuim::string::fill(u8"─", columns_width[i]).c_str());
        }
        tuim::print(c3);
        tuim::print("\n");
    };

    print_table_border(u8"┌", u8"┬", u8"┐");

    for(size_t i = 0; i < columns.size(); i++) {
        if(i == 0) tuim::print(u8"│");

        std::string padding_text = tuim::string::fill(" ", padding);
        std::string column_name = columns[i];

        tuim::print("%s%s%s│", padding_text.c_str(), columns[i].c_str(), padding_text.c_str());
    }
    tuim::print("\n");
    
    for(size_t i = 0; i < rows.size(); i++) {
        print_table_border(u8"├", u8"┼", u8"┤");

        for(size_t j = 0; j < columns.size(); j++) {
            if(j == 0) tuim::print(u8"│");

            bool selected = tuim::is_item_active() && i == (size_t) *cursor && j == (size_t) *key;

            double column_padding = padding + (columns_width[j] - tuim::calc_text_width(rows[i][j], padding));
            std::string padding_text = tuim::string::fill(" ", padding - selected);
            std::string padding_text2 = tuim::string::fill(" ", column_padding - selected);
            std::string column_name = rows[i][j];

            if(selected) tuim::print("[");
            tuim::print("%s%s%s", padding_text.c_str(), column_name.c_str(), padding_text2.c_str());
            if(selected) tuim::print("]");
            tuim::print(u8"│");
        }
        tuim::print("\n");
    }

    print_table_border(u8"└", u8"┴", u8"┘");
}

/***********************************************************
*                    STYLE FUNCTIONS                       *
***********************************************************/

inline void tuim::push_margin(uint32_t margin) {
    context* ctx = get_context();
    ctx->margins_stack.push_back(margin);
}

inline void tuim::pop_margin() {
    context* ctx = get_context();
    if(ctx->margins_stack.size() > 0)
        ctx->margins_stack.pop_back();
    // Move cursor to new margin
    ctx->cursor.x = get_active_margin();
    printf("\033[%dG", ctx->cursor.x);
}

inline uint32_t tuim::get_active_margin() {
    context* ctx = get_context();
    if(ctx->margins_stack.size() == 0)
        return 1;
    return ctx->margins_stack.back();
}

/***********************************************************
*                   CONTAINERS FUNCTIONS                   *
***********************************************************/

inline tuim::container& tuim::get_container() {
    tuim::context* ctx = tuim::get_context();
    return ctx->containers_stack.back();
}

inline void tuim::update_container() {
    tuim::context* ctx = tuim::get_context();
    tuim::container& current_ctn = tuim::get_container();
    current_ctn.cursor = ctx->cursor;
    current_ctn.size.x = std::max(current_ctn.size.x, std::abs(current_ctn.cursor.x - current_ctn.pos.x));
    current_ctn.size.y = std::max(current_ctn.size.y, std::abs(current_ctn.cursor.y - current_ctn.pos.y));
}

inline void tuim::move_container_cursor(vec2 pos) {
    container& ctn = get_container();
    ctn.cursor = pos;
}

inline void tuim::start_container() {
    tuim::context* ctx = tuim::get_context();
    tuim::container current_ctn = tuim::get_container();
    tuim::container ctn = tuim::container{ current_ctn.cursor, {0, 0} };

    ctx->containers_stack.push_back(ctn);
}

inline void tuim::end_container() {

}


inline void tuim::start_column() {

}

inline void tuim::end_column() {

}

/***********************************************************
*                   INTERNAL FUNCTIONS                     *
***********************************************************/

inline void tuim::impl::open_terminal() {
    #ifdef __linux__ 
        // Get the executable full path
        std::string exec = "\"" + std::filesystem::canonical("/proc/self/exe").string() + "\" f";
        execl("/usr/bin/gnome-terminal", "gnome-terminal", "--full-screen", "-q", "-e", exec.c_str(), (char*)0);
    #elif _WIN32
    
    #else

    #endif

    // Close current process to avoid duplicates
    exit(EXIT_SUCCESS);
}

inline void tuim::impl::add_printing_info(const std::string &str) {
    tuim::context *ctx = tuim::get_context();

    tuim::vec2 pos = tuim::get_cursor();
    uint32_t width = tuim::calc_text_width(str, 0);

    ctx->printings_stack.push_back(tuim::printing_info{pos, width});
}

/***********************************************************
*                  KEYBOARD UTILITIES                      *
***********************************************************/

inline tuim::keyboard::keycode tuim::keyboard::get_pressed() {
    int count = 0;
    unsigned char codes[4] = { 0, 0, 0, 0 };

    // Loop for escaped characters
    do {
        char buf;
        termios term;
        fflush(stdout);
        tcgetattr(0, &term);
        term.c_lflag &= ~ICANON;
        term.c_cc[VMIN] = 1;
        term.c_cc[VTIME] = 0;
        tcsetattr(0, TCSANOW, &term);

        read(0, &buf, 1);

        term.c_lflag |= ICANON;
        tcsetattr(0, TCSADRAIN, &term);

        codes[count] = (unsigned char) buf;
        // printf("[%d] %d\n", count, codes[count]);
        count++;
    } while(tuim::keyboard::is_pressed() && count < 4);

    // Get the final key code
    tuim::keyboard::keycode code = (codes[0] << (8*(count-1))) + (codes[1] << (8*(count-2))) + (codes[2] << (8*(count-3))) + codes[3];
    return code;
}

inline bool tuim::keyboard::is_pressed() {
    termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    tcgetattr(0, &term);
    term.c_lflag |= ICANON;
    tcsetattr(0, TCSANOW, &term);

    return byteswaiting > 0;
}

/***********************************************************
*                    STRING FUNCTIONS                      *
***********************************************************/

inline size_t tuim::string::length(const std::string &str) {
    // https://stackoverflow.com/a/61692729
    size_t len = 0;
    unsigned char c = str[0];
    for (size_t i = 0; c != 0; len++) {
        int v0 = (c & 0x80) >> 7;
        int v1 = (c & 0x40) >> 6;
        int v2 = (c & 0x20) >> 5;
        int v3 = (c & 0x10) >> 4;
        i += 1 + v0 * v1 + v0 * v1 * v2 + v0 * v1 * v2 * v3;
        c = str[i];
    }
    return len;
}

inline std::string tuim::string::fill(const std::string &str, size_t length) {
    std::string res = "";
    for(size_t i = 0; i < length; i++) res += str;
    return res;
}

inline bool tuim::string::is_style(const std::string& str, size_t pos) {
    if(pos > 0 && str.at(pos-1) == TUIM_STYLE_ESCAPE)
        return false;
    return str.at(pos) == TUIM_STYLE_CODE || str.at(pos) == TUIM_COLOR_CODE;
}

inline std::pair<tuim::font::style, size_t> tuim::string::extract_style(const std::string& str, size_t pos) {
    context* ctx = get_context();
    font::style style;

    if(pos >= str.length() || pos == str.length()-1 || !is_style(str, pos))
        return {style, 0};
    
    std::string code = "";
    bool is_background = (str.at(pos+1) == TUIM_COLOR_BACKGROUND);

    if(str.at(pos) == TUIM_STYLE_CODE) {
        // Extract the style tag from the string
        for(size_t i = pos+1+is_background; i < str.length();) {
            size_t ch_length = tuim::string::char_length(str.at(i));
            std::string ch = str.substr(i, ch_length);
            
            // Temporarily hard limit of 7 characters for style tag
            // Later, we could keep track of the longest style tag and stop at that length
            if(ch == ";" || code.length() == 7)
                break;

            code += ch;
            i += ch_length;
        }
        // Check if the style tag exists
        // The longest tag matching with what has been extracted is kept
        // We pop_back the code until empty or the tag exists
        while(!code.empty() && ctx->style_codes.count(code) == 0) {
            code.pop_back();
        }
        if(code.empty() || ctx->style_codes.count(code) == 0)
            return {style, 0};
        style = ctx->style_codes.at(code);
    }
    else if(str.at(pos) == TUIM_COLOR_CODE) {
        // Extract the color hex from the string
        for(size_t i = pos+1+is_background; i < str.length();) {
            size_t ch_length = tuim::string::char_length(str.at(i));
            std::string ch = str.substr(i, ch_length);

            if(ch == ";" || code.length() == 6 || ch_length > 1)
                break;
            if((ch.at(0) < 'a' || ch.at(0) > 'f') && (ch.at(0) < 'A' || ch.at(0) > 'F') && (ch.at(0) < '0' || ch.at(0) > '9'))
                break;

            code += ch;
            i += ch_length;
        }
        // Skip if it is not a valid hex value
        if(code.length() != 6)
            return {style, 0};
        style.type = font::style_type::COLOR;
        style.style_color = color::from_hex(code);
    }
    if(style.type == font::style_type::COLOR)
        style.style_color.background = is_background;
    // Include identifiers (&,#,_) and the semicolon following the style tag
    size_t style_length = 1 + is_background + code.length();
    style_length += (pos+style_length < str.length() && str.at(pos+style_length) == ';');
    return {style, style_length};
}

inline std::string tuim::string::strip_styles(const std::string& str) {
    std::string stripped = "";
    for(size_t i = 0; i < str.length();) {
        size_t ch_length = char_length(str.at(i));
        size_t style_length;
        font::style style;
        std::tie(style, style_length) = extract_style(str, i);
        if(style_length > 0) {
            i += style_length;
            continue;
        }
        stripped += str.substr(i, ch_length);
        i += ch_length;
    }
    return stripped;
}

inline std::string tuim::string::escape_styles(const std::string& str) {
    std::string parsed = "";
    for(size_t i = 0; i < str.length();) {
        size_t length = extract_style(str, i).second;
        if(length > 1)
            parsed += TUIM_STYLE_ESCAPE;
        parsed += str.substr(i, length);
        i += length + string::char_length(str.at(i));
    }
    return parsed;
}

inline uint32_t tuim::string::utf8_to_int(const std::string& c) {
    uint32_t code = 0;
    for(size_t i = 0; i < c.length(); i++)
        code += ((unsigned char) c[i] << (8 * (c.length()-i-1)));
    return code;
}

inline std::string tuim::string::int_to_utf8(uint32_t code) {
    int length = 0;
    uint32_t code2 = code;
    while(code2 != 0) {
        code2 >>= 8;
        length++;
    }
    std::string c = std::string("", length);
    for(int i = 0; i < length; i++) {
        c[length - i - 1] = static_cast<char>(code & 0xFF);
        code >>= 8;
    }
    return c;
}

inline bool tuim::string::is_printable(uint32_t code) {
    if((code & 0x1B5B00) == 0x1B5B00)
        return false;
    if(tuim::string::int_to_utf8(code)[0] == 0x1B)
        return false;
    if(code < 127)
        return std::isprint(code & 0xFF);
    return true;
}

inline size_t tuim::string::char_length(char c) {
    if((0b00000001 & (c >> 7)) == 0b0)
        return 1;
    if((0b00000111 & (c >> 5)) == 0b110)
        return 2;
    if((0b00001111 & (c >> 4)) == 0b1110)
        return 3;
    if((0b00011111 & (c >> 3)) == 0b11110)
        return 4;
    return 1;
}

inline uint32_t tuim::string::to_lowercase(uint32_t code) {
    //ß capital letter is ẞ
    if(code == 0xC39F)
        return 0xE1BA9E;
    
    //ÿ returns Ÿ which is not in order.
    if(code == 0xC3BF)
        return 0xC29F;

    // Don't change anything if the character is already lowercase
    if(code >= 'a' && code <= 'z')
        return code;

    // For latin characters, the difference between the uppercase 
    // and lowercase codepoint is 0x20 (i.e à-À = 0x20).
    if(code <= 0xC3BF)
        return code + 0x20;

    //Ǆ and Ǳ have both two capital letters.
    if(code == 	0xC784 || code == 0xC7B1)
        return code + 0x02;

    // For extended latin characters, the difference between the uppercase 
    // and lowercase codepoint is mostly 0x01. There are some exceptions such as
    // Ǆ above.
    if(code <= 0xC8B3)
        return code + 0x01;

    // TODO: Complete with greeks and cyrillic characters.

    return code;
}   

inline bool tuim::string::is_alphanumeric(uint32_t code) {
    #define BETWEEN(a, b) (code >= a && code <= b)
    if(BETWEEN('0', '9') || BETWEEN('a', 'z') || BETWEEN('A', 'Z'))
        return true;
    if(BETWEEN(50048, 50070) || BETWEEN(50072, 50102) || BETWEEN(50104, 50111)) //À-Ö && Ø-ö && ø-ÿ
        return true;

    return false;
}

inline bool tuim::string::is_vowel(uint32_t code) {
    code = to_lowercase(code);
    #define IS(code, c) (code == static_cast<unsigned int>(c))
    #define BETWEEN(a, b) (code >= a && code <= b)

    if(IS(code, 'a') || IS(code, 'e') || IS(code, 'i') || IS(code, 'o') || IS(code, 'u'))
        return true;

    if(BETWEEN(0xC3A0, 0xC3A6)
        || BETWEEN(0xC3A8, 0xC3AF)
        || BETWEEN(0xC3B2, 0xC3B6)
        || BETWEEN(0xC3B8, 0xC3BD))
        return true;

    return false;
}

/***********************************************************
*                     SCREEN BUFFER                        *
***********************************************************/

inline tuim::vec2 tuim::screen::buffer::get_position(uint32_t index) {
    return vec2{(int) index % size.x, (int) floor((float) index / (float) size.y)};
}

inline uint32_t tuim::screen::buffer::get_index(const vec2& pos) {
    return pos.x + pos.y * size.x;
}

inline bool tuim::screen::buffer::is_in_buffer(uint32_t index) {
    return index <= (size.x+size.x*size.y);
}

inline bool tuim::screen::buffer::is_character(uint32_t index, const character& ch) {
    return buffer.at(index) == ch;
}

inline tuim::screen::character& tuim::screen::buffer::get_character(uint32_t index) {
    return buffer.at(index);
}

inline void tuim::screen::buffer::set_character(uint32_t index, character ch) {
    uint32_t max_index = size.x + size.x * size.y;
    // Resize vertically if index out of bounds (add new lines)
    // if(index >= max_index) {
    //     resize(vec2{(int) size.x, (int)(size.y + ceil((max_index - index)/size.x))});
    // }
    buffer[index] = ch;
}

inline void tuim::screen::buffer::clear_line(uint32_t y) {
    for(size_t i = size.x*y; y < size.x*y+size.x; i++) {
        buffer.erase(i);
    }
}

inline void tuim::screen::buffer::resize(const vec2& new_size) {
    uint32_t max_index = (size.x + size.x * size.y) - 1;

    // Handle height increase
    if(size.y < new_size.y) {
        size.y = new_size.y;
    }
    // Handle height decrease (cut)
    else {
        uint32_t diff = size.y - new_size.y;
        for(uint32_t i = max_index; i > max_index-(diff*size.x); i--) {
            buffer.erase(i);
        }
    }

    // Handle width increase
    if(size.x < new_size.x) {
        uint32_t diff = new_size.x - size.x;
        for(uint32_t i = max_index-1; i > size.x; i--) {
            if(buffer.count(i) == 0)
                continue;
            vec2 pos = get_position(i);
            pos.y += pos.y * diff;
            uint32_t new_index = get_index(pos);
            buffer.emplace(new_index, buffer.at(i));
            buffer.erase(i);
        }
    }
    // Handle width decrease (cut)
    else {
        uint32_t diff = size.x - new_size.x;
        for(uint32_t i = 0; i < max_index; i += size.x) {
            if(i > 0) {
                for(uint32_t j = i; j < i + new_size.x; j++) {
                    if(buffer.count(j) == 0)
                        continue;
                    buffer.emplace(j-diff, buffer.at(j));
                    buffer.erase(j);
                }
            }
            for(uint32_t j = i+new_size.x; j < i + size.x; j++) {
                buffer.erase(i);
            }
        }
    }
    size = new_size;
}

/***********************************************************
*                        COLORS                            *
***********************************************************/

inline tuim::color::color tuim::color::from_code(std::string str) {
    context* ctx = get_context();
    
    // Check if there is the background character
    bool background = (str[0] == TUIM_COLOR_BACKGROUND);

    // Remove background character if there is one
    if(background) str = str.substr(1, str.length() - 1);

    color color = ctx->style_codes.at(str).style_color;
    color.background = background;

    return color;
}

inline tuim::color::color tuim::color::from_hex(std::string str) {
    // Check if there is the background character
    bool background = (str[0] == TUIM_COLOR_BACKGROUND);

    // Remove background character if there is one
    if(background) str = str.substr(1, str.length() - 1);

    // Convert string to int
    unsigned int hex;
    std::stringstream ss;
    ss << std::hex << str;
    ss >> hex;

    color color = from_hex(hex);
    color.background = background;

    return color;
}

inline tuim::color::color tuim::color::from_hex(unsigned int hex) {
    color color;
    color.r = (hex >> 16) & 0xFF;
    color.g = (hex >> 8) & 0xFF;
    color.b = (hex) & 0xFF;
    color.background = false;
    return color;
}

inline unsigned int tuim::color::to_hex(color color) {
    return (color.r << 16) + (color.g << 8) + color.b;
}

inline std::string tuim::color::to_ansi(color color) {
    // https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797

    std::string ansi = "\33[" + std::string(color.background ? "48" : "38") + ";2;";
    ansi += std::to_string(color.r) + ";";
    ansi += std::to_string(color.g) + ";";
    ansi += std::to_string(color.b) + "m";

    return ansi;
}

/***********************************************************
*                    FONT & TEXT TAGS                      *
***********************************************************/

inline void tuim::font::register_style(const std::string &code, const style &style) {
    context* ctx = get_context();
    ctx->style_codes.emplace(code, style);
}

inline tuim::font::mode tuim::font::from_code(const std::string &str) {
    context* ctx = get_context();
    return ctx->style_codes.at(str).style_mode;
}

inline std::string tuim::font::to_ansi(mode mode, bool enabled) {
    int code = (int) mode;
    if(!enabled && mode != mode::RESET)
        code += (20 + (code == 1));
    return "\033[" + std::to_string(code) + "m";
}

inline tuim::font::style tuim::font::make_style(color::color color) {
    style style;
    style.type = style_type::COLOR;
    style.style_color = color;
    return style;
}

inline tuim::font::style tuim::font::make_style(mode mode) {
    style style;
    style.type = style_type::MODE;
    style.style_mode = mode;
    return style;
}

#endif