/****************************************************************************
*                                                                           *
*   TUIM : a text-based user interface for command lines                    *
*                                                                           *
*   Made by Xorrad <monsieurs.aymeric@gmail.com>                            *
*   This code is licensed under MIT license (see LICENSE for details)       *                                            *
*                                                                           *
****************************************************************************/

#pragma once

#ifndef TUIM_CPP
#define TUIM_CPP

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

#ifdef __linux__
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#elif _WIN32
#include <windows.h>
#else

#endif

#define TUIM_STYLE_CODE '&'
#define TUIM_COLOR_BACKGROUND '_'
#define TUIM_COLOR_CUSTOM '#'

#define TUIM_MAKE_KEY2(a, b) (b<<8) + c
#define TUIM_MAKE_KEY3(a, b, c) (a<<16) + (b<<8) + c
#define TUIM_MAKE_KEY4(a, b, c, d) (a<<24) + (b<<16) + (c<<8) + d

namespace tuim {

    /* Forward declarations for ui */
    typedef uint32_t item_id;

    typedef uint32_t item_flags;
    typedef uint32_t text_flags;
    typedef uint32_t button_flags;
    typedef uint32_t input_text_flags;

    /* User Interface related structures */
    struct context;
    struct container;
    struct item;

    /* Math related structures */
    struct vec2 {
        int x, y;

        vec2() : vec2(0, 0) {}
        vec2(int x, int y) : x(x), y(y) {}

        bool operator==(const vec2 &other) {
            return x == other.x && y == other.y;
        }
    };

    /* Keyboard related functions are defined here */
    namespace keyboard {

        typedef uint32_t keycode;

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
            /* To complete */
        };

        keycode get_pressed(); /* Get pressed key */
        bool is_pressed(); /* Check if key has been pressed */
    }

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
    }

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

        void register_style(const std::string &code, const style &style);

        mode from_code(const std::string &str);
        std::string to_ansi(mode mode, bool enabled);
        style make_style(color::color color);
        style make_style(mode mode);
    }

    /* User interface components are defined here */
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
    };

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
        item_id active_id;
        item_id last_active_id;
        item_id hovered_id;

        vec2 cursor;
        bool cursor_visible;

        keyboard::keycode pressed_key;

        std::map<std::string, font::style> style_codes;
        std::map<font::mode, bool> style_modes;

        std::vector<item> items_stack;
        std::vector<container> containers_stack;
        std::vector<uint32_t> margins_stack;

        std::vector<printing_info> printings_stack; /* Store what has been drawn by the last item */

        context() {
            active_id = 0;
            last_active_id = 0;
            hovered_id = 0;

            cursor = {1, 1};
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
            style_modes = {};

            items_stack = std::vector<item>(0);
            containers_stack = std::vector<container>{ container{{0, 0}} };
            margins_stack = std::vector<uint32_t>(0);
            

            printings_stack = std::vector<printing_info>(0);
        }
        ~context() {}
    };

    context* ctx; /* Global context variable for the gui */

    void init(int argc, char* argv[]); /* Initialize tuim with command line arguments */
    void create_context(); /* Create the global gui context */
    void delete_context(); /* Delete the global gui context */

    void set_cursor_visible(bool cursor); /* Set the terminal cursor visible */
    bool is_cursor_visible(); /* Get the cursor visibility */
    void set_title(std::string title); /* Set terminal title */
    void set_cursor(vec2 pos); /* Change terminal cursor position */
    vec2 get_cursor(); /* Get terminal cursor position */
    void gotoxy(vec2 pos);
    void new_line(); /* Break to a new line */
    void clear(); /* Clear terminal output */
    void clear_line(); /* Clear terminal output */
    void print_to_screen(std::string str); /* Print text to screen and reset style */
    template<typename ... Args> void print(const char* fmt, Args ... args); /* Format text for printing to screen */

    context* get_context(); /* Get tuim user interface global context */

    item_id str_to_id(const std::string &str); /* Hash a string to get an item id */
    item_id get_id(); /* Get the id of the current item*/
    uint32_t get_index(tuim::item_id id); /* Get the index of a specific item */
    uint32_t get_hovered_index(); /* Get the index of the hovered item */

    bool was_item_active(); /* Return true if the last pushed item was active on previous tick */
    bool is_item_active(); /* Determine if last pushed item is active */
    bool is_item_hovered(); /* Determine if last pushed item is hovered */
    bool is_pressed(tuim::keyboard::key key); /* Determine if a specified key has been pressed */
    bool is_pressed(tuim::keyboard::keycode code); /* Determine if a specified keycode has been pressed */
    bool is_pressed(const char* c); /* Determine if a specified character has been pressed */
    bool has_hoverable(); /* Determine if an hoverable item has been pushed */

    void set_active_id(item_id id); /* Set an item as active */
    void set_hovered_id(item_id id); /* Set an item as hovered */

    void add_item(item item); /* Push a new item */
    void remove_item(item_id id); /* Remove an item */

    void update(tuim::keyboard::keycode key); /* Update the items */
    void display(); /* Display pushed items */

    int calc_text_width(const std::string &str, int padding = 0); /* Compute a text width with padding*/
    int calc_text_vector_width(const std::vector<std::string> &str, int padding = 0); /* Compute the max width of strings in a vector */
    std::vector<int> calc_columns_width(const std::vector<std::string> &columns, const std::vector<std::vector<std::string>> &rows, int padding = 0); /* Compute the max width for each columns */

    vec2 calc_relative_position(); /* Calculate the coordinates from which to start drawing an item. */

    void text(const std::string& id, const std::string& text); /* Display text */
    void text(const std::string& text); /* Display text */
    void hr(int length);
    void paragraph(const std::string& id, const std::string& text, unsigned int width);
    bool button(const std::string& id, const std::string& text, button_flags flags = BUTTON_FLAGS_NONE); /* Display a button */
    template <typename T> bool slider(const std::string& id, T* value, T min, T max, T step); /* Display a number slider */
    template <typename T> bool input_number(const std::string& id, const std::string& text, T* value, T min, T max, T step); /* Display a input for numbers */
    template <typename U> bool input_enum(const std::string& id, const std::string& text, U* value, int max, const std::map<U, std::string>& labels); /* Display a input for enums */
    bool input_bool(const std::string& id, const std::string& text, bool* value, const std::map<bool, std::string>& labels = {{false, "False"}, {true, "True"}}); /* Display a input for booleans */
    bool input_text(const std::string& id, std::string* value, const std::string& default_value, input_text_flags flags = INPUT_TEXT_FLAGS_NONE); /* Display a input for string */
    bool checkbox(const std::string& id, const std::string& text, bool* value);
    void scroll_table(const char* id, int *cursor, int *key, std::vector<std::string> &columns, std::vector<std::vector<std::string>> &rows, int height, int padding); /* Display a navigable table */

    void push_margin(uint32_t margin);
    void pop_margin();
    uint32_t get_active_margin();

    container& get_container();
    void start_container();
    void end_container();
    void update_container();

    void start_column();
    void end_column();

    void move_container_cursor(vec2 pos);

    /* Internals are defined within this namespace */
    namespace impl {
        void open_terminal(); /* Restart program in a separate terminal */
        void add_printing_info(const std::string &str); /* Log a printing into the stack */
    }

    namespace string {
        size_t length(const std::string& str);
        std::string fill(const std::string& str, size_t length);
        std::string parse_styles(const std::string& str); /* Replace style codes with ansi escape sequences */
        std::string escape_styles(const std::string& str);
        uint32_t utf8_to_int(const std::string& c);
        std::string int_to_utf8(uint32_t code);
        bool is_printable(uint32_t code);
        size_t char_length(char c);
        uint32_t to_lowercase(uint32_t code);
        bool is_alphanumeric(uint32_t code);
        bool is_vowel(uint32_t code);
    }

}

/* Inline functions definitions */

void tuim::init(int argc, char* argv[]) {
    /* Restart if not in separate terminal */
    if(argc < 2 || strcmp(argv[1], "f") != 0) {
        //tuim::impl::open_terminal();
    }

    tuim::create_context();
}

void tuim::create_context() {
    printf("\033[?1049h"); // enable alternate buffer
    tuim::ctx = new tuim::context();
}

void tuim::delete_context() {
    set_cursor_visible(true);
    printf("\033[?1049l"); // disabled alternate buffer
    delete tuim::ctx;
}

void tuim::set_cursor_visible(bool cursor) {
    context* ctx = get_context();
    ctx->cursor_visible = cursor;
    printf("\033[?25%c", (cursor ? 'h' : 'l'));
}

bool tuim::is_cursor_visible() {
    context* ctx = get_context();
    return ctx->cursor_visible;
}

void tuim::set_title(std::string title) {
    /* http://www.faqs.org/docs/Linux-mini/Xterm-Title.html#s3 */
    printf("\033]0;%s\007", title.c_str());
}

void tuim::set_cursor(vec2 pos) {
    context* ctx = get_context();
    ctx->cursor = pos;
}

tuim::vec2 tuim::get_cursor() {
    context* ctx = get_context();
    return ctx->cursor;
}

void tuim::gotoxy(vec2 pos) {
    printf("\033[%d;%dH", pos.y, pos.x);
    tuim::set_cursor(pos);
}

void tuim::new_line() {
    print_to_screen("\n");
}

void tuim::clear() {
    printf("\033[0m\033[2J\033[3J\033[H");
    ctx->style_modes.clear();
    ctx->pressed_key = tuim::keyboard::NONE;
    set_cursor({1, 1});
}

void tuim::clear_line() {
    printf("\033[2K");
    set_cursor({1, get_cursor().y});
}

void tuim::print_to_screen(std::string str) {
    context* ctx = get_context();
    for(size_t i = 0; i < str.length();) {
        size_t l = tuim::string::char_length(str.at(i));
        if(str.at(i) == '\n') {
            ctx->cursor = { (int) get_active_margin(), ctx->cursor.y+1};
            if(ctx->cursor.x > 1) {
                std::string margin = "\033[" + std::to_string(ctx->cursor.x) + "G";
                str = str.substr(0, i) + margin + str.substr(i, str.length());
                i += margin.length();
                // printf("margin: %d", ctx->cursor.x);
                // printf("\033[%dG", ctx->cursor.x);
            }
        }
        else if(str.at(i) == '\033') {
            do {
                i++;
            } while(i < str.length() && !((str.at(i) >= 'a' && str.at(i) <= 'z') || (str.at(i) >= 'A' && str.at(i) <= 'Z')));
        }
        else ctx->cursor.x++;
        i += l;
    }
    update_container();
    printf("%s", str.c_str());
}

template<typename ... Args>
void tuim::print(const char* fmt, Args ... args) {
    int size_s = std::snprintf(nullptr, 0, fmt, args...) + 1;
    if(size_s <= 0) throw std::runtime_error( "Error during formatting.");

    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[ size ]);

    std::snprintf(buf.get(), size, fmt, args...);

    std::string parsed = std::string( buf.get(), buf.get() + size - 1);
    parsed = tuim::string::parse_styles(parsed);

    /* Print formatted text */
    print_to_screen(parsed);

    int width = calc_text_width(parsed);
    vec2 pos = get_container().cursor;
    pos.x += width;
    move_container_cursor(pos);
}

tuim::context* tuim::get_context() {
    /* TODO -> add assert for ctx 0*/
    return tuim::ctx;
}

tuim::item_id tuim::str_to_id(const std::string &str) {
    /* http://www.cse.yorku.ca/~oz/hash.html */
    unsigned long hash = 5381;
    for (size_t i = 0; i < str.size(); ++i)
        hash = 33 * hash + (unsigned char) str[i];
    return (tuim::item_id) hash;
}

tuim::item_id tuim::get_id() {
    tuim::context* ctx = tuim::get_context();
    return ctx->items_stack.at(ctx->items_stack.size() - 1).id;
}

uint32_t tuim::get_index(tuim::item_id id) {
    tuim::context *ctx = tuim::get_context();
    for(size_t i = 0; i < ctx->items_stack.size(); i++) {
        if(ctx->items_stack.at(i).id == id) {
            return i;
        }
    }
    return -1;
}

uint32_t tuim::get_hovered_index() {
    tuim::context *ctx = tuim::get_context();
    return tuim::get_index(ctx->hovered_id);
}

bool tuim::was_item_active() {
    tuim::context* ctx = tuim::get_context();
    return ctx->last_active_id == tuim::get_id();
}

bool tuim::is_item_active() {
    tuim::context* ctx = tuim::get_context();
    return ctx->active_id == tuim::get_id();
}

bool tuim::is_item_hovered() {
    tuim::context* ctx = tuim::get_context();
    return ctx->hovered_id == tuim::get_id();
}

bool tuim::is_pressed(tuim::keyboard::key key) {
    return tuim::is_pressed((tuim::keyboard::keycode) key);
}

bool tuim::is_pressed(tuim::keyboard::keycode code) {
    tuim::context* ctx = tuim::get_context();
    return ctx->pressed_key == code;
}

bool tuim::is_pressed(const char* c) {
    tuim::context* ctx = tuim::get_context();
    return ctx->pressed_key == tuim::string::utf8_to_int(c);
}

bool tuim::has_hoverable() {
    tuim::context* ctx = tuim::get_context();
    for(size_t i = 0; i < ctx->items_stack.size(); i++) {
        if(!(ctx->items_stack.at(i).flags & tuim::item_flags_::ITEM_FLAGS_DISABLED))
            return true;
    }
    return false;
}

void tuim::set_active_id(item_id id) {
    tuim::context *ctx = tuim::get_context();
    ctx->active_id = id;
}

void tuim::set_hovered_id(item_id id) {
    tuim::context *ctx = tuim::get_context();
    ctx->hovered_id = id;
}

void tuim::add_item(item item) {
    tuim::context *ctx = tuim::get_context();
    ctx->items_stack.push_back(item);
    ctx->printings_stack.clear();
}

void tuim::remove_item(item_id id) {
    tuim::context *ctx = tuim::get_context();
    for(size_t i = 0; i < ctx->items_stack.size(); i++) {
        if(ctx->items_stack.at(i).id == i)
            ctx->items_stack.erase(ctx->items_stack.begin() + i);
    }
}

void tuim::update(tuim::keyboard::keycode key) {
    tuim::context* ctx = tuim::get_context();
    ctx->pressed_key = key;

    if(ctx->active_id == 0 || !(ctx->items_stack.at(tuim::get_index(ctx->active_id)).flags & tuim::item_flags_::ITEM_FLAGS_STAY_ACTIVE)) {
        ctx->last_active_id = ctx->active_id;
        ctx->active_id = 0;

        int hovered_index = tuim::get_hovered_index();

        /*std::cerr << "has hoverable: " << tuim::has_hoverable() << std::endl;
        std::cerr << "index: " << hovered_index << std::endl;
        std::cerr << "hovered: " << ctx->hovered_id << std::endl;*/

        /* Set the first hoverable item hovered if no item is */
        if(hovered_index == -1 && tuim::has_hoverable()) {
            for(size_t i = 0; i < ctx->items_stack.size(); i++) {
                if(!(ctx->items_stack.at(i).flags & tuim::item_flags_::ITEM_FLAGS_DISABLED)) {
                    ctx->hovered_id = ctx->items_stack.at(i).id;
                    hovered_index = i;
                    break;
                }
            }
        }

        /* Move cursor to previous hoverable item */
        // if(key == tuim::keyboard::UP) {
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

        /* Move cursor to next hoverable item */
        // if(key == tuim::keyboard::DOWN) {
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

void tuim::display() {

}

int tuim::calc_text_width(const std::string &str, int padding) {

    // https://github.com/chalk/ansi-regex/blob/main/index.js
    static const std::string pattern = "[\\u001B\\u009B][[\\]()#;?]*(?:(?:(?:(?:;[-a-zA-Z\\d\\/#&.:=?%@~_]+)*|[a-zA-Z\\d]+(?:;[-a-zA-Z\\d\\/#&.:=?%@~_]*)*)?\\u0007)|(?:(?:\\d{1,4}(?:;\\d{0,4})*)?[\\dA-PR-TZcf-nq-uy=><~]))";
    static const std::regex regex(pattern);
    std::string stripped = std::regex_replace(tuim::string::parse_styles(str), regex, "");

    int width = tuim::string::length(stripped) + padding*2;

    return width;
}

int tuim::calc_text_vector_width(const std::vector<std::string> &str, int padding) {
    int max_width = 0;
    for(size_t i = 0; i < str.size(); i++) {
        int width = tuim::calc_text_width(str[i], padding);
        if(width > max_width) max_width = width;
    }
    return max_width + padding*2;
}

std::vector<int> tuim::calc_columns_width(const std::vector<std::string> &columns, const std::vector<std::vector<std::string>> &rows, int padding) {
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

tuim::vec2 tuim::calc_relative_position() {
    context* ctx = get_context();
    container ctn = ctx->containers_stack.back();
    vec2 pos = ctn.cursor;

    return pos;
}

void tuim::text(const std::string& id, const std::string& text) {
    tuim::item item = tuim::item{ tuim::str_to_id(id), tuim::item_flags_::ITEM_FLAGS_DISABLED };
    tuim::add_item(item);

    // vec2 pos = calc_relative_position();

    tuim::print(text.c_str());
    // tuim::print("\n");
}

void tuim::text(const std::string& text) {
    tuim::item item = tuim::item{ tuim::str_to_id(text), tuim::item_flags_::ITEM_FLAGS_DISABLED };
    tuim::add_item(item);
    tuim::print(text.c_str());
}

void tuim::hr(int length) {
    std::string str = "\n&s" + tuim::string::fill("―", length) + "&r\n";
    tuim::text(str);
}

void tuim::paragraph(const std::string& id, const std::string& text, unsigned int width) {
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
            cursor = {margin, cursor.y+1 - (is_eol && line_break)};
            tuim::gotoxy(cursor);

            line = "";
            line_ch_len = 0;
            line_blank_count = 0;
            line_break = false;
        }

        //Add space between words.
        if(line_ch_len > 0) {
            line += " ";
            line_ch_len++;
            line_blank_count++;
        }

        line += word;
        line_ch_len += word_ch_len;

        if(skip_next_line)
            line_break = true;

        // Print line without extra blank if it is the last word of the text.
        // if(word_pos + 1 > text.length()) {
        // }

        i = word_pos + 1;
    }

    if(line_ch_len > 0) {
        tuim::print(line.c_str());
    }

    tuim::print("&r");
    for(int i = 0; i < 1 + line_break; i++) tuim::print("\n");

    tuim::pop_margin();
}

bool tuim::button(const std::string& id, const std::string& text, button_flags flags) {
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
bool tuim::slider(const std::string& id, T* value, T min, T max, T step) {
    tuim::item item = tuim::item{ tuim::str_to_id(id), tuim::item_flags_::ITEM_FLAGS_NONE };
    tuim::add_item(item);

    if(tuim::is_item_hovered()) {
        if(tuim::is_pressed(keyboard::ENTER)) {
            tuim::print("[*] ");
            tuim::set_active_id(item.id);
        }
        else if(tuim::is_pressed(keyboard::LEFT)) {
            *value = std::max(min, *value - step);
        }
        else if(tuim::is_pressed(keyboard::RIGHT)) {
            *value = std::min(max, *value + step);
        }
        else tuim::print("[x] ");
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
        std::cin >> *value;
    }
    tuim::print("&r %s", std::to_string(*value).c_str());

    return tuim::is_item_active();
}

template <typename T>
bool tuim::input_number(const std::string& id, const std::string& text, T* value, T min, T max, T step) {
    tuim::item item = tuim::item{ tuim::str_to_id(id), tuim::item_flags_::ITEM_FLAGS_NONE };
    tuim::add_item(item);

    if(tuim::is_item_hovered()) {
        if(tuim::is_pressed(keyboard::ENTER)) {
            tuim::print("[*] ");
            std::cin >> *value;
            tuim::set_active_id(item.id);
        }
        else if(tuim::is_pressed(keyboard::LEFT)) {
            *value = std::max(min, *value - step);
            tuim::set_active_id(item.id);
        }
        else if(tuim::is_pressed(keyboard::RIGHT)) {
            *value = std::min(max, *value + step);
            tuim::set_active_id(item.id);
        }
        else tuim::print("[x] ");
    }
    else tuim::print("[ ] ");

    tuim::print(text.c_str(), *value);

    return tuim::is_item_active();
}

template <typename U>
bool tuim::input_enum(const std::string& id, const std::string& text, U* value, int max, const std::map<U, std::string>& labels) {
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

bool tuim::input_bool(const std::string& id, const std::string& text, bool* value, const std::map<bool, std::string>& labels) {
    return input_enum<bool>(id, text, value, 2, labels);
}

bool tuim::input_text(const std::string& id, std::string* value, const std::string& default_value, input_text_flags flags) {
    context* ctx = get_context();
    tuim::item item = tuim::item{ tuim::str_to_id(id), tuim::item_flags_::ITEM_FLAGS_STAY_ACTIVE };
    tuim::add_item(item);

    static size_t cursor = value->length();

    if(tuim::is_item_active()) {
        if(tuim::is_pressed(keyboard::LEFT)) {
            if(cursor > 0) {
                do {
                    cursor--;
                } while(cursor > 0 && (0b00000011 & (value->at(cursor) >> 6)) == 0b10);
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
        else if(tuim::is_pressed(keyboard::ESCAPE)) tuim::set_active_id(0);
        else {
            uint32_t code = ctx->pressed_key;
            if(tuim::string::is_printable(code)) {
                if((flags & INPUT_TEXT_FLAGS_NUMERIC_ONLY && isdigit(code))
                || (flags & INPUT_TEXT_FLAGS_ALPHANUMERIC_ONLY && tuim::string::is_alphanumeric(code))
                || (flags == INPUT_TEXT_FLAGS_NONE)) {
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
            if(tuim::is_item_active()) tuim::set_active_id(0);
            else {
                tuim::set_active_id(item.id);
                cursor = value->length();
            }
        }
        if(tuim::is_item_active()) tuim::print("[*] ");
        else tuim::print("[x] ");
    }
    else tuim::print("[ ] ");

    tuim::print("#_555555");
    size_t i = 0;
    while(i < value->length()) {
        size_t l = tuim::string::char_length(value->at(i));
        std::string ch = value->substr(i, l);

        if(tuim::is_item_active() && (ch.at(0) == TUIM_STYLE_CODE || ch.at(0) == TUIM_COLOR_CUSTOM))
            ch = "\\" + ch;

        if(tuim::is_item_active() && !tuim::keyboard::is_pressed() && cursor == i) {
            tuim::print("#_ffffff#555555%s&r#_555555", ch.c_str());
        }
        else tuim::print("%s", ch.c_str());
        
        i += l;
    }
    if(tuim::is_item_active() && !tuim::keyboard::is_pressed() && cursor == i) tuim::print("#_ffffff ");
    tuim::print("&r");

    return tuim::is_item_active();
}

bool tuim::checkbox(const std::string& id, const std::string& text, bool* value) {
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

void tuim::scroll_table(const char* id, int *cursor, int *key, std::vector<std::string> &columns, std::vector<std::vector<std::string>> &rows, int height, int padding) {
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

    /* Calculate the max width for each columns */
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

void tuim::push_margin(uint32_t margin) {
    context* ctx = get_context();
    ctx->margins_stack.push_back(margin);
}

void tuim::pop_margin() {
    context* ctx = get_context();
    if(ctx->margins_stack.size() > 0)
        ctx->margins_stack.pop_back();
}

uint32_t tuim::get_active_margin() {
    context* ctx = get_context();
    if(ctx->margins_stack.size() == 0)
        return 1;
    return ctx->margins_stack.back();
}

tuim::container& tuim::get_container() {
    tuim::context* ctx = tuim::get_context();
    return ctx->containers_stack.back();
}

void tuim::start_container() {
    tuim::context* ctx = tuim::get_context();
    tuim::container current_ctn = tuim::get_container();
    tuim::container ctn = tuim::container{ current_ctn.cursor, {0, 0} };

    ctx->containers_stack.push_back(ctn);
}

void tuim::end_container() {

}

void tuim::update_container() {
    tuim::context* ctx = tuim::get_context();
    tuim::container& current_ctn = tuim::get_container();
    current_ctn.cursor = ctx->cursor;
    current_ctn.size.x = std::max(current_ctn.size.x, std::abs(current_ctn.cursor.x - current_ctn.pos.x));
    current_ctn.size.y = std::max(current_ctn.size.y, std::abs(current_ctn.cursor.y - current_ctn.pos.y));
}

void tuim::start_column() {

}

void tuim::end_column() {

}

void tuim::move_container_cursor(vec2 pos) {
    container& ctn = get_container();
    ctn.cursor = pos;
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

void tuim::impl::add_printing_info(const std::string &str) {
    tuim::context *ctx = tuim::get_context();

    tuim::vec2 pos = tuim::get_cursor();
    uint32_t width = tuim::calc_text_width(str, 0);

    ctx->printings_stack.push_back(tuim::printing_info{pos, width});
}

tuim::keyboard::keycode tuim::keyboard::get_pressed() {
    #ifdef __linux__
        int count = 0;
        unsigned char codes[4] = { 0, 0, 0, 0 };

        /* Loop for escaped characters */
        do {
            char buf;
            termios term;
            fflush(stdout);
            tcgetattr(0, &term);
            term.c_lflag &= ~(ICANON | ECHO);
            term.c_cc[VMIN] = 1;
            term.c_cc[VTIME] = 0;
            tcsetattr(0, TCSANOW, &term);

            read(0, &buf, 1);

            term.c_lflag |= ICANON | ECHO;
            tcsetattr(0, TCSADRAIN, &term);

            codes[count] = (unsigned char) buf;
            // printf("[%d] %d\n", count, codes[count]);
            count++;
        } while(tuim::keyboard::is_pressed() && count < 4);

        /* Get the final key code */
        tuim::keyboard::keycode code = (codes[0] << (8*(count-1))) + (codes[1] << (8*(count-2))) + (codes[2] << (8*(count-3))) + codes[3];
        return code;

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

size_t tuim::string::length(const std::string &str) {
    /* https://stackoverflow.com/a/61692729 */
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

std::string tuim::string::fill(const std::string &str, size_t length) {
    std::string res = "";
    for(size_t i = 0; i < length; i++) res += str;
    return res;
}

std::string tuim::string::parse_styles(const std::string &str) {
    context* ctx = get_context();
    bool escaped = false;
    std::string parsed = "";

    for(size_t i = 0; i < str.length(); i++) {

        /* Skip if character is escaped */
        if(escaped) {
            /* Add an anti-slash there was not any color code to escape */
            if(str[i] != TUIM_STYLE_CODE && str[i] != TUIM_COLOR_CUSTOM && str[i] != '\\') parsed += "\\";

            parsed += str[i];
            escaped = false;
            continue;
        }

        if(str[i] == '\\') {
            escaped = true;
            continue;
        }

        if(str[i] == TUIM_STYLE_CODE) {
            bool background = (str.length() - i > 2 && str[i+1] == TUIM_COLOR_BACKGROUND);
            size_t code_length = background ? 2 : 1;
            if(str.length() - i < code_length)
                goto end;

            std::string raw_code = str.substr(i + 1 + background, 1);
            std::string code = str.substr(i + 1, code_length);

            if(ctx->style_codes.count(raw_code) == 0)
                goto end;

            font::style style = ctx->style_codes.at(raw_code);

            switch(style.type) {
                case font::style_type::COLOR: {
                    color::color color = color::from_code(code);
                    parsed += color::to_ansi(color);
                }
                    break;
                case font::style_type::MODE:
                    code = raw_code;
                    bool enabled = true;
                    font::mode mode = font::from_code(code);
                    if(ctx->style_modes.count(mode))
                    {
                        enabled = false;
                        ctx->style_modes.erase(mode);
                    }
                    else
                    {
                        ctx->style_modes.emplace(mode, true);
                    }
                    if(mode == font::mode::RESET)
                        ctx->style_modes.clear(); // TODO: move this to print
                    parsed += font::to_ansi(mode, enabled);
                    break;
            }
            
            i += code_length;
            continue;
        }

        if(str[i] == TUIM_COLOR_CUSTOM) {
            size_t code_length = (str.length() - i > 6 && str[i+1] == TUIM_COLOR_BACKGROUND) ? 7 : 6;
            if(str.length() - i < code_length)
                goto end;

            std::string code = str.substr(i + 1, code_length);
            bool is_hex = true;

            for(size_t i = 0; i < code.length(); i += tuim::string::char_length(code.at(i))) {
                if(i == 0 && code.at(i) == '_') continue;
                if((code.at(i) < '0' || code.at(i) > '9') && (code.at(i) < 'a' || code.at(i) > 'f')) {
                    is_hex = false;
                    break;
                }
            }
            if(!is_hex)
                goto end;

            tuim::color::color color = tuim::color::from_hex(code);
            parsed += tuim::color::to_ansi(color);

            i += code_length;
            continue;
        }

        end:
        parsed += str[i];
    }

    return parsed;
}

std::string tuim::string::escape_styles(const std::string& str) {
    bool escaped = false;
    std::string parsed = "";

    for(size_t i = 0; i < str.length(); i++) {

        if(escaped) {
            /* Add an anti-slash if there was not any color code to escape */
            if(str[i] != TUIM_STYLE_CODE && str[i] != TUIM_COLOR_CUSTOM && str[i] != '\\') parsed += "\\";
            parsed += str[i];
            escaped = false;
            continue;
        }

        if(str[i] == '\\') {
            escaped = true;
            continue;
        }

        if(str[i] == TUIM_STYLE_CODE) {
            bool background = (str.length() - i > 2 && str[i+1] == TUIM_COLOR_BACKGROUND);
            size_t code_length = background ? 3 : 2;
            if(str.length() - i < code_length) {
                parsed += str[i];
                continue;
            }

            std::string raw_code = str.substr(i + 1 + background, 1);
            std::string code = str.substr(i, code_length);

            if(ctx->style_codes.count(raw_code) == 0)
            {
                parsed += str[i];
                continue;
            }

            parsed += "\\" + code;
            continue;
        }

        if(str[i] == TUIM_COLOR_CUSTOM) {
            size_t code_length = (str.length() - i > 6 && str[i+1] == TUIM_COLOR_BACKGROUND) ? 8 : 7;
            if(str.length() - i < code_length) {
                parsed += str[i];
                continue;
            }

            std::string code = str.substr(i, code_length);
            parsed += "\\" + str[i] + code;

            i += code_length;
            continue;
        }

        parsed += str[i];
    }

    return parsed;
}

uint32_t tuim::string::utf8_to_int(const std::string& c) {
    uint32_t code = 0;
    for(size_t i = 0; i < c.length(); i++)
        code += ((unsigned char) c[i] << (8 * (c.length()-i-1)));
    return code;
}

std::string tuim::string::int_to_utf8(uint32_t code) {
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

bool tuim::string::is_printable(uint32_t code) {
    if((code & 0x1B5B00) == 0x1B5B00)
        return false;
    if(tuim::string::int_to_utf8(code)[0] == 0x1B)
        return false;
    if(code < 127)
        return std::isprint(code & 0xFF);
    return true;
}

size_t tuim::string::char_length(char c) {
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

uint32_t tuim::string::to_lowercase(uint32_t code) {
    //ß capital letter is ẞ
    if(code == 0xC39F)
        return 0xE1BA9E;
    
    //ÿ returns Ÿ which is not in order.
    if(code == 0xC3BF)
        return 0xC29F;

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

bool tuim::string::is_alphanumeric(uint32_t code) {
    #define BETWEEN(a, b) (code >= a && code <= b)
    if(BETWEEN('0', '9') || BETWEEN('a', 'z') || BETWEEN('A', 'Z'))
        return true;
    if(BETWEEN(50048, 50070) || BETWEEN(50072, 50102) || BETWEEN(50104, 50111)) //À-Ö && Ø-ö && ø-ÿ
        return true;

    return false;
}

bool tuim::string::is_vowel(uint32_t code) {
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

void tuim::font::register_style(const std::string &code, const style &style) {
    context* ctx = get_context();
    ctx->style_codes.emplace(code, style);
}

tuim::font::mode tuim::font::from_code(const std::string &str) {
    context* ctx = get_context();
    return ctx->style_codes.at(str).style_mode;
}

std::string tuim::font::to_ansi(mode mode, bool enabled) {
    int code = (int) mode;
    if(!enabled && mode != mode::RESET)
        code += 21;
    return "\033[" + std::to_string(code) + "m";
}

tuim::font::style tuim::font::make_style(color::color color) {
    style style;
    style.type = style_type::COLOR;
    style.style_color = color;
    return style;
}

tuim::font::style tuim::font::make_style(mode mode) {
    style style;
    style.type = style_type::MODE;
    style.style_mode = mode;
    return style;
}

tuim::color::color tuim::color::from_code(std::string str) {
    context* ctx = get_context();
    
    /* Check if there is the background character */
    bool background = (str[0] == TUIM_COLOR_BACKGROUND);

    /* Remove background character if there is one */
    if(background) str = str.substr(1, str.length() - 1);

    color color = ctx->style_codes.at(str).style_color;
    color.background = background;

    return color;
}

tuim::color::color tuim::color::from_hex(std::string str) {
    /* Check if there is the background character */
    bool background = (str[0] == TUIM_COLOR_BACKGROUND);

    /* Remove background character if there is one */
    if(background) str = str.substr(1, str.length() - 1);

    /* Convert string to int */
    unsigned int hex;
    std::stringstream ss;
    ss << std::hex << str;
    ss >> hex;

    color color = from_hex(hex);
    color.background = background;

    return color;
}

tuim::color::color tuim::color::from_hex(unsigned int hex) {
    color color;
    color.r = (hex >> 16) & 0xFF;
    color.g = (hex >> 8) & 0xFF;
    color.b = (hex) & 0xFF;
    color.background = false;
    return color;
}

unsigned int tuim::color::to_hex(color color) {
    return (color.r << 16) + (color.g << 8) + color.b;
}

std::string tuim::color::to_ansi(color color) {
    /* https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797 */

    std::string ansi = "\33[" + std::string(color.background ? "48" : "38") + ";2;";
    ansi += std::to_string(color.r) + ";";
    ansi += std::to_string(color.g) + ";";
    ansi += std::to_string(color.b) + "m";

    return ansi;
}
#endif