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
#include <vector>

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

    /* Forward declarations for ui */
    typedef unsigned long item_id;

    typedef int item_flags;
    typedef int text_flags;
    typedef int button_flags;

    struct context;
    struct item;

    /* Keyboard related functions are defined here */
    namespace keyboard {

        enum key {
            NONE = 0,
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
            /* To complete */
        };

        key get_pressed(); /* Get pressed key */
        bool is_pressed(); /* Check if key has been pressed */

    };

    /* User interface components are defined here */
    enum class item_flags_ {
        NONE = 0,
        ITEM_FLAGS_DISABLED = 1 << 0,
        ITEM_FLAGS_STAY_ACTIVE = 1 << 1,
    };

    enum class button_flags_ {
        NONE = 0,
    };

    struct context {
        item_id active_id;
        item_id last_active_id;
        item_id hovered_id;

        keyboard::key pressed_key;

        std::vector<item*> items_stack;

        context() {
            active_id = NULL;
            hovered_id = NULL;

            pressed_key = keyboard::key::NONE;

            items_stack = std::vector<item*>(0);
        }
        ~context() {}
    };

    struct item {
        item_id id;
        item_flags flags;
    };

    context* ctx; /* Global context variable for the gui */

    void init(int argc, char* argv[]); /* Initialize tuim with command line arguments */
    void create_context(); /* Create the global gui context */
    void delete_context(); /* Delete the global gui context */

    void set_title(std::string title); /* Set terminal title */
    void move(int x, int y); /* Change terminal cursor position */
    void clear(); /* Clear terminal output */
    void clear_line(); /* Clear terminal output */
    template<typename ... Args> void print(const char* fmt, Args ... args); /* Print formatted text */

    context* get_context(); /* Get tuim user interface global context */

    item_id str_to_id(const std::string &str); /* Hash a string to get an item id */
    item_id get_id(); /* Get the id of the current item*/
    int get_index(tuim::item_id id); /* Get the index of a specific item */
    int get_hovered_index(); /* Get the index of the hovered item */

    bool was_item_active(); /* Return true if the last pushed item was active on previous tick */
    bool is_item_active(); /* Determine if last pushed item is active */
    bool is_item_hovered(); /* Determine if last pushed item is hovered */
    bool is_pressed(keyboard::key key); /* Determine if a specified key has been pressed */
    bool has_hoverable(); /* Determine if an hoverable item has been pushed */

    void set_active_id(item_id id); /* Set an item as active */
    void set_hovered_id(item_id id); /* Set an item as hovered */

    void add_item(item *item); /* Push a new item */
    void remove_item(item_id id); /* Remove an item */

    void update(keyboard::key key); /* Update the items */
    void display(); /* Display pushed items */

    int calc_text_width(const std::string &str, int padding); /* Compute a text width with padding */
    int calc_text_vector_width(const std::vector<std::string> &str, int padding); /* Compute the max width of strings in a vector */
    std::vector<int> calc_columns_width(const std::vector<std::string> &columns, const std::vector<std::vector<std::string>> &rows, int padding); /* Compute the max width for each columns */

    bool button(std::string id, std::string text, button_flags flags); /* Display a button */
    void text(std::string id, std::string text); /* Display text */
    void scroll_table(const char* id, int *cursor, int key, std::vector<std::string> &columns, std::vector<std::vector<std::string>> &rows, int height, int padding); /* Display a navigable table */

    /* Internals are defined within this namespace */
    namespace impl {
        void open_terminal(); /* Restart program in a separate terminal */

        std::string format(std::string str); /* Format a string for printing */
    };

    namespace string {
        size_t length(const std::string &str);
        std::string fill(const std::string &str, int length);
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
        //tuim::impl::open_terminal();
    }

    tuim::create_context();
}

void tuim::create_context() {
    tuim::ctx = new tuim::context();
}

void tuim::delete_context() {
    delete tuim::ctx;
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
    tuim::move(0, 0);
    ctx->pressed_key = tuim::keyboard::NONE;
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

tuim::context* tuim::get_context() {
    /* TODO -> add assert for ctx NULL*/
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
    return ctx->items_stack.at(ctx->items_stack.size() - 1)->id;
}

int tuim::get_index(tuim::item_id id) {
    tuim::context *ctx = tuim::get_context();
    if(ctx->hovered_id == -1) return NULL;
    for(int i = 0; i < ctx->items_stack.size(); i++) {
        if(ctx->items_stack.at(i)->id == id) {
            return i;
        }
    }
    return -1;
}

int tuim::get_hovered_index() {
    tuim::context *ctx = tuim::get_context();
    if(ctx->hovered_id == -1) return NULL;
    for(int i = 0; i < ctx->items_stack.size(); i++) {
        if(ctx->items_stack.at(i)->id == ctx->hovered_id) {
            return i;
        }
    }
    return -1;
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

bool tuim::is_pressed(keyboard::key key) {
    tuim::context* ctx = tuim::get_context();
    return ctx->pressed_key == key;
}

bool tuim::has_hoverable() {
    tuim::context* ctx = tuim::get_context();
    for(int i = 0; i < ctx->items_stack.size(); i++) {
        if(!(ctx->items_stack.at(i)->flags & (int) tuim::item_flags_::ITEM_FLAGS_DISABLED))
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

void tuim::add_item(item *item) {
    tuim::context *ctx = tuim::get_context();
    ctx->items_stack.push_back(item);
}

void tuim::remove_item(item_id id) {
    tuim::context *ctx = tuim::get_context();
    for(int i = 0; i < ctx->items_stack.size(); i++) {
        if(ctx->items_stack.at(i)->id == i)
            ctx->items_stack.erase(ctx->items_stack.begin() + i);
    }
}

void tuim::update(keyboard::key key) {
    tuim::context* ctx = tuim::get_context();
    ctx->pressed_key = key;

    if(ctx->active_id == NULL || ctx->items_stack.at(tuim::get_index(ctx->active_id))->flags & ~(tuim::item_flags) tuim::item_flags_::ITEM_FLAGS_STAY_ACTIVE) {
        ctx->last_active_id = ctx->active_id;
        ctx->active_id = NULL;

        int hovered_index = tuim::get_hovered_index();

        /*std::cerr << "has hoverable: " << tuim::has_hoverable() << std::endl;
        std::cerr << "index: " << hovered_index << std::endl;
        std::cerr << "hovered: " << ctx->hovered_id << std::endl;*/

        if(hovered_index == -1 && tuim::has_hoverable()) {
            for(int i = 0; i < ctx->items_stack.size(); i++) {
                if(!(ctx->items_stack.at(i)->flags & (tuim::item_flags) tuim::item_flags_::ITEM_FLAGS_DISABLED)) {
                    ctx->hovered_id = ctx->items_stack.at(i)->id;
                    hovered_index = i;
                    break;
                }
            }
        }

        if(key == tuim::keyboard::UP) {
            if(tuim::has_hoverable()) {
                tuim::item_id id = NULL;
                if(hovered_index != -1) {
                    int index = std::max(0, hovered_index - 1);
                    while(index > 0 && (ctx->items_stack.at(index)->flags & (tuim::item_flags) tuim::item_flags_::ITEM_FLAGS_DISABLED)) index--;
                    if((ctx->items_stack.at(index)->flags & (tuim::item_flags) tuim::item_flags_::ITEM_FLAGS_DISABLED)) index = hovered_index;
                    id = ctx->items_stack.at(index)->id;
                }
                ctx->hovered_id = id;
            }
        }

        if(key == tuim::keyboard::DOWN) {
            if(tuim::has_hoverable()) {
                tuim::item_id id = NULL;
                if(hovered_index != -1) {
                    int index = std::min(hovered_index + 1, (int) ctx->items_stack.size() - 1);
                    while(index < (ctx->items_stack.size() - 1) && (ctx->items_stack.at(index)->flags) & (int) tuim::item_flags_::ITEM_FLAGS_DISABLED) index++;
                    if((ctx->items_stack.at(index)->flags & (tuim::item_flags) tuim::item_flags_::ITEM_FLAGS_DISABLED)) index = hovered_index;
                    id = ctx->items_stack.at(index)->id;
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
    return tuim::string::length(str) + padding*2;
}

int tuim::calc_text_vector_width(const std::vector<std::string> &str, int padding) {
    int max_width = 0;
    for(int i = 0; i < str.size(); i++) {
        if(tuim::string::length(str[i]) > max_width) max_width = tuim::string::length(str[i]);
    }
    return max_width + padding * 2;
}

std::vector<int> tuim::calc_columns_width(const std::vector<std::string> &columns, const std::vector<std::vector<std::string>> &rows, int padding) {
    std::vector<int> columns_width = std::vector<int>(columns.size(), 0);
    for(int i = 0; i < columns.size(); i++) {
        int max_width = 0;
        for(int j = 0; j < rows.size(); j++) {
            int row_width = tuim::calc_text_width(rows[j][i], padding);
            if(row_width > max_width) max_width = row_width;
        }
        columns_width[i] = std::max(tuim::calc_text_width(columns[i], padding), max_width);
    }
    return columns_width;
}

bool tuim::button(std::string id, std::string text, button_flags flags) {
    tuim::item_id button_id = tuim::str_to_id(id);
    tuim::item *item = new tuim::item{ button_id, NULL };
    tuim::add_item(item);

    if(tuim::is_item_hovered()) {
        if(tuim::is_pressed(keyboard::key::ENTER)) {
            tuim::set_active_id(button_id);
        }
        tuim::print("[x] ");
    }
    else tuim::print("[ ] ");
    tuim::print(text.c_str());

    return tuim::is_item_active();
}

void tuim::text(std::string id, std::string text) {
    tuim::item_id button_id = tuim::str_to_id(id);
    tuim::item *item = new tuim::item{ button_id, (tuim::item_flags) tuim::item_flags_::ITEM_FLAGS_DISABLED };
    tuim::add_item(item);

    tuim::print(text.c_str());
}


void tuim::scroll_table(const char* id, int *cursor, int key, std::vector<std::string> &columns, std::vector<std::vector<std::string>> &rows, int height, int padding) {
    tuim::item_id item_id = tuim::str_to_id(id);
    tuim::item *item = new tuim::item{ item_id, (tuim::item_flags) tuim::item_flags_::ITEM_FLAGS_STAY_ACTIVE };
    tuim::add_item(item);

    if(tuim::is_item_active()) {
        if(tuim::is_pressed(tuim::keyboard::UP)) {
            *cursor = std::max(0, *cursor - 1);
        }
        if(tuim::is_pressed(tuim::keyboard::DOWN)) {
            *cursor = std::min(*cursor + 1, (int) rows.size()-1);
        }
        if(tuim::is_pressed(tuim::keyboard::ENTER)) {
            //row clicked
        }

        if(tuim::is_pressed(tuim::keyboard::BACKSPACE)) tuim::set_active_id(NULL);
    }

    if(tuim::is_item_hovered()) {
        if(tuim::is_pressed(tuim::keyboard::key::ENTER)) {
            tuim::set_active_id(item_id);
        }
        if(tuim::is_item_active()) tuim::print("[*]\n");
        else tuim::print("[x]\n");
    }
    else tuim::print("[ ]\n");

    /* Calculate the max width for each columns */
    std::vector<int> columns_width = tuim::calc_columns_width(columns, rows, padding);

    auto print_table_border = [&](const char *c1, const char *c2, const char *c3) {
        tuim::print(c1);
        for(int i = 0; i < columns.size(); i++) {
            if(i != 0) tuim::print(c2);
            tuim::print("%s", tuim::string::fill(u8"─", columns_width[i]).c_str());
        }
        tuim::print(c3);
        tuim::print("\n");
    };

    print_table_border(u8"┌", u8"┬", u8"┐");

    for(int i = 0; i < columns.size(); i++) {
        if(i == 0) tuim::print(u8"│");

        std::string padding_text = tuim::string::fill(" ", padding);
        std::string column_name = columns[i];

        tuim::print("%s%s%s│", padding_text.c_str(), columns[i].c_str(), padding_text.c_str());
    }
    tuim::print("\n");
    
    for(int i = 0; i < rows.size(); i++) {
        print_table_border(u8"├", u8"┼", u8"┤");

        for(int j = 0; j < columns.size(); j++) {
            if(j == 0) tuim::print(u8"│");

            bool selected = tuim::is_item_active() && i == *cursor && j == key;

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

std::string tuim::string::fill(const std::string &str, int length) {
    std::string res = "";
    for(int i = 0; i < length; i++) res += str;
    return res;
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