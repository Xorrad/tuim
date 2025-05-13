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

#include <iostream> // std::cout, std::cin...
#include <string> // std::string
#include <string_view> // std::string_view
#include <vector> // std::vector
#include <queue> // std::queue
#include <cstdint> // uint32_t...
#include <functional> // std::function
#include <format> // std::format

#ifdef __linux__
#include <unistd.h> // STDOUT_FILENO
#include <termios.h> // termios, tcgetattr, tcsetattr
#include <sys/ioctl.h> // winsize, ioctl
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
    class Container;
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

    enum ContainerFlags : uint32_t {
        CONTAINER_FLAGS_NONE = 0,
    };
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
    *                          FRAME                           *
    ***********************************************************/
    
    class Cell {
    public:
        Cell();
        Cell(char32_t ch) : m_Character(ch) {}
        ~Cell() = default;

        char32_t m_Character;
        // TODO: add attributes such as fg color, bg color, bold, italic...
    };

    class Frame {
    public:
        Frame();
        Frame(const vec2& size);
        ~Frame() = default;
    
        vec2 GetSize() const;
        Cell* Get(const vec2& pos);
        Cell* Get(size_t x, size_t y);
        void Set(const vec2& pos, Cell* cell);
        void Clear();

        std::vector<std::vector<Cell*>> m_Cells;
    };

    /***********************************************************
    *                       CONTAINERS                         *
    ***********************************************************/

    class Container {
    public:
        Container();
        ~Container() = default;

        vec2 m_Size;
        vec2 m_Pos;
        ContainerFlags m_Flags;
        Frame m_Frame;
    };

    /***********************************************************
    *                         ITEMS                            *
    ***********************************************************/

    template <typename... Args> void Print(std::format_string<Args...> fmt, Args&&... args); // Print a formatted string to the current frame.

    /***********************************************************
    *                    COMPONENTS/ITEMS                      *
    ***********************************************************/

    //

    /***********************************************************
    *                    STRING FUNCTIONS                      *
    ***********************************************************/

    uint8_t Utf8CharLength(char c);
    std::string Utf8Char32ToString(char32_t ch);
    void Utf8IterateString(std::string_view sv, std::function<void(char32_t, size_t)> func);
    
    /**********************************************************
    *                        CONTEXT                          *
    **********************************************************/

    class Context {
    public:
        Context() {
            m_Cursor = vec2(0, 0);
        }
        ~Context() = default;

        vec2 m_Cursor;
        Frame m_Frame; // Final screen frame that is going to be displayed to the screen.
        std::queue<Container> m_ContainersStack;
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
    tuim::Terminal::SetAlternateBuffer(true);
    tuim::Terminal::SetUserInputsVisibility(false);
    tuim::Terminal::SetCursorVisibility(false);
    tuim::Terminal::Clear();
    tuim::ctx = new tuim::Context();
}

void tuim::CreateContext(int argc, char* argv[]) {
    tuim::CreateContext();
}

void tuim::DeleteContext() {
    tuim::Terminal::SetAlternateBuffer(false);
    tuim::Terminal::SetUserInputsVisibility(true);
    tuim::Terminal::SetCursorVisibility(true);
    delete tuim::ctx;
}

tuim::Context* tuim::GetCtx() {
    if(tuim::ctx == nullptr)
        throw std::runtime_error("error: context is undefined.");
    return tuim::ctx;
}

void tuim::SetTitle(std::string_view title) {
    tuim::Terminal::SetTitle(title);
}

void tuim::SetCursorVisibility(bool visible) {
    tuim::Terminal::SetCursorVisibility(visible);
}

void tuim::SetFullscreen(bool fullscreen) {
    throw std::runtime_error("error: tuim::Fullscreen() is not implemented yet.");
    // TODO: restart the terminal in fullscreen (compatible with at least gnome-terminal).
}

void tuim::Update(tuim::Key key) {
    throw std::runtime_error("error: tuim::Update() is not implemented yet.");
    // TODO: implement update function.
}

void tuim::Clear() {
    Context* ctx = tuim::GetCtx();
    ctx->m_Frame.Clear();
    ctx->m_Cursor = vec2(0, 0);
}

void tuim::Display() {
    // TODO: Keep track of the last frame to avoid flickering by clearing the whole terminal screen every frame.
    tuim::Terminal::Clear();

    Context* ctx = tuim::GetCtx();
    vec2 terminalSize = tuim::Terminal::GetTerminalSize();
    vec2 frameSize = ctx->m_Frame.GetSize();
    vec2 prevPos = vec2(0, 0);

    for (size_t y = 0; y < std::min(frameSize.y, terminalSize.y); y++) {
        for (size_t x = 0; x < std::min(ctx->m_Frame.m_Cells[y].size(), (size_t) terminalSize.x); x++) {
            Cell* cell = ctx->m_Frame.Get(x, y);
            if (cell != nullptr) {
                // Do not change the cursor position using ANSI if there are only a few spaces.
                size_t xDiff = x - prevPos.x;
                if (prevPos.y == y && xDiff >= 5) {
                    std::cout << std::string(xDiff-1, ' ');
                }
                else if (prevPos.y != y || prevPos.x != x+1) {
                    tuim::Terminal::SetCursorPos(vec2(x, y));
                }
                std::cout << Utf8Char32ToString(cell->m_Character);
                prevPos = vec2(x, y);
            }
        }
        // Print a new line to the terminal for every line except the last one.
        if (y != frameSize.y-1)
            std::cout << '\n';
    }
    std::cout << std::flush;
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

/***********************************************************
*                          FRAME                           *
***********************************************************/

tuim::Frame::Frame() {
    vec2 terminalSize = tuim::Terminal::GetTerminalSize();
    m_Cells = std::vector<std::vector<Cell*>>(terminalSize.y, std::vector<Cell*>(terminalSize.x, nullptr));
}

tuim::Frame::Frame(const vec2& size) {
    m_Cells = std::vector<std::vector<Cell*>>(size.y, std::vector<Cell*>(size.x, nullptr));
}

tuim::vec2 tuim::Frame::GetSize() const {
    size_t maxWidth = m_Cells.front().size();
    for (size_t i = 0; i < m_Cells.size(); i++)
        maxWidth = std::max(maxWidth, m_Cells[i].size());
    return vec2(m_Cells.size(), m_Cells.size());
}

tuim::Cell* tuim::Frame::Get(const tuim::vec2& pos) {
    return m_Cells[pos.y][pos.x];
}

tuim::Cell* tuim::Frame::Get(size_t x, size_t y) {
    return m_Cells[y][x];
}

void tuim::Frame::Set(const tuim::vec2& pos, tuim::Cell* cell) {
    // Make sure to resize the vectors in case the position is
    // beyond the column or line size.
    if (m_Cells.size() <= pos.y)
        m_Cells.resize(pos.y+1, std::vector<Cell*>(pos.x, nullptr));
    if (m_Cells[pos.y].size() <= pos.x)
        m_Cells[pos.y].resize(pos.x+1, nullptr);
    // Make sure to delete the previous cell pointer.
    if (m_Cells[pos.y][pos.x] != nullptr)
        delete m_Cells[pos.y][pos.x];
    m_Cells[pos.y][pos.x] = cell;
}

void tuim::Frame::Clear() {
    for (auto& line : m_Cells)
        std::fill(line.begin(), line.end(), nullptr);
    // TODO: maybe it would be better to reset the size instead?
}

/***********************************************************
*                         ITEMS                            *
***********************************************************/

template <typename... Args> void tuim::Print(std::format_string<Args...> fmt, Args&&... args) {
    std::string str = std::format(fmt, std::forward<Args>(args)...);
    // TODO: handle colors and styles.
    Context* ctx = tuim::GetCtx();
    tuim::Utf8IterateString(str, [ctx](char32_t ch, size_t index) {
        if (ch == '\t') {
            ctx->m_Cursor.x += 4;
        }
        else if (ch == '\n') {
            ctx->m_Cursor = vec2(0, ctx->m_Cursor.y+1);
        }
        else {
            ctx->m_Frame.Set(ctx->m_Cursor, new Cell(ch));
            ctx->m_Cursor.x++;
        }
    });
}

/***********************************************************
*                    STRING FUNCTIONS                      *
***********************************************************/

uint8_t tuim::Utf8CharLength(char c) {
    if ((c & 0x80) == 0x00) return 1; // 0xxxxxxx
    if ((c & 0xE0) == 0xC0) return 2; // 110xxxxx
    if ((c & 0xF0) == 0xE0) return 3; // 1110xxxx
    if ((c & 0xF8) == 0xF0) return 4; // 11110xxx
    return 0; // Invalid UTF-8 leading byte.
}

std::string tuim::Utf8Char32ToString(char32_t ch) {
    std::string str = "";
    if (ch <= 0x7F) {
        str += static_cast<char>(ch);
    } else if (ch <= 0x7FF) {
        str += static_cast<char>(0xC0 | (ch >> 6));
        str += static_cast<char>(0x80 | (ch & 0x3F));
    } else if (ch <= 0xFFFF) {
        str += static_cast<char>(0xE0 | (ch >> 12));
        str += static_cast<char>(0x80 | ((ch >> 6) & 0x3F));
        str += static_cast<char>(0x80 | (ch & 0x3F));
    } else if (ch <= 0x10FFFF) {
        str += static_cast<char>(0xF0 | (ch >> 18));
        str += static_cast<char>(0x80 | ((ch >> 12) & 0x3F));
        str += static_cast<char>(0x80 | ((ch >> 6) & 0x3F));
        str += static_cast<char>(0x80 | (ch & 0x3F));
    }
    return str;
}

void tuim::Utf8IterateString(std::string_view sv, std::function<void(char32_t, size_t)> func) {
    size_t index = 0;
    while (index < sv.size()) {
        // Determine the number of bytes for the next character
        // and check if there are enough bytes left in the string.
        uint8_t length = tuim::Utf8CharLength(sv[index]);
        if (index + length > sv.size())
            return;
        
        // Decode the full UTF8 character from the string by reading several bytes.
        char32_t ch = 0;
        char8_t lead = static_cast<char8_t>(sv[index]);
        if (length == 1) {
            ch = lead;
        } else {
            ch = lead & ((1 << (7 - length)) - 1); // Mask initial bits.
            for (size_t i = 1; i < length; ++i) {
                char8_t cont = static_cast<char8_t>(sv[index + i]);
                if ((cont & 0b11000000) != 0b10000000) {
                    // throw std::runtime_error("error: invalid UTF-8 continuation byte.");
                    return;
                }
                ch = (ch << 6) | (cont & 0b00111111);
            }
        }

        // Call the callback function and advance to the next character.
        func(ch, index);
        index += length;
    }
}

#endif // TUIM_HPP