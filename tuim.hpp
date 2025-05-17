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
#include <stack> // std::stack
#include <cstdint> // uint32_t...
#include <functional> // std::function
#include <format> // std::format
#include <memory> // std::shared_ptr, std::unique_ptr...
#include <locale> // setlocale
#include <unordered_map> // std::unordered_map

#ifdef __linux__
#include <unistd.h> // STDOUT_FILENO
#include <termios.h> // termios, tcgetattr, tcsetattr
#include <sys/ioctl.h> // winsize, ioctl
#include <sys/select.h> // select
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

    using ItemId = unsigned long;

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

    #define TUIM_MAKE_KEY2(a, b) (b<<8) + c
    #define TUIM_MAKE_KEY3(a, b, c) (a<<16) + (b<<8) + c
    #define TUIM_MAKE_KEY4(a, b, c, d) (a<<24) + (b<<16) + (c<<8) + d

    enum Key : uint32_t {
        NONE = 0,
        
        // Controls
        TAB        = 9,
        ENTER      = 10,
        ESCAPE     = 27,
        BACKSPACE  = 127,
        SPACE      = 32,
    
        // Arrows & Home/End
        UP    = TUIM_MAKE_KEY3(27, 91, 'A'),
        DOWN  = TUIM_MAKE_KEY3(27, 91, 'B'),
        RIGHT = TUIM_MAKE_KEY3(27, 91, 'C'),
        LEFT  = TUIM_MAKE_KEY3(27, 91, 'D'),
        HOME  = TUIM_MAKE_KEY3(27, 79, 'H'),
        END   = TUIM_MAKE_KEY3(27, 79, 'F'),
    
        // Function Keys
        F1  = TUIM_MAKE_KEY3(27, 79, 80),
        F2  = TUIM_MAKE_KEY3(27, 79, 81),
        F3  = TUIM_MAKE_KEY3(27, 79, 82),
        F4  = TUIM_MAKE_KEY3(27, 79, 83),
        F5  = TUIM_MAKE_KEY3(27, 79, 84),
        F6  = TUIM_MAKE_KEY3(27, 79, 85),
        F7  = TUIM_MAKE_KEY3(27, 79, 86),
        F8  = TUIM_MAKE_KEY3(27, 79, 87),
        F9  = TUIM_MAKE_KEY3(27, 79, 89),
        F10 = TUIM_MAKE_KEY3(27, 79, 90),
        F11 = TUIM_MAKE_KEY3(27, 79, 91),
        F12 = TUIM_MAKE_KEY3(27, 79, 92),
    
        // Edit
        INSERT    = TUIM_MAKE_KEY4(27, 91, 50, 126), // ESC [ 2 ~
        DELETE    = TUIM_MAKE_KEY4(27, 91, 51, 126), // ESC [ 3 ~
        PAGE_UP   = TUIM_MAKE_KEY4(27, 91, 53, 126), // ESC [ 5 ~
        PAGE_DOWN = TUIM_MAKE_KEY4(27, 91, 54, 126), // ESC [ 6 ~
    
        // Digits
        DIGIT_0 = '0',
        DIGIT_1 = '1',
        DIGIT_2 = '2',
        DIGIT_3 = '3',
        DIGIT_4 = '4',
        DIGIT_5 = '5',
        DIGIT_6 = '6',
        DIGIT_7 = '7',
        DIGIT_8 = '8',
        DIGIT_9 = '9',
    
        // Lowercase Letters
        A = 'a',
        B = 'b',
        C = 'c',
        D = 'd',
        E = 'e',
        F = 'f',
        G = 'g',
        H = 'h',
        I = 'i',
        J = 'j',
        K = 'k',
        L = 'l',
        M = 'm',
        N = 'n',
        O = 'o',
        P = 'p',
        Q = 'q',
        R = 'r',
        S = 's',
        T = 't',
        U = 'u',
        V = 'v',
        W = 'w',
        X = 'x',
        Y = 'y',
        Z = 'z',
    };
    
    char32_t PollKeyCode(); // Wait until timeout for a key to be pressed
    Key GetPressedKey(); // Get the current frame pressed key as an enum key
    bool IsKeyPressed(); // Check if a key has been pressed
    bool IsKeyPressed(Key key); // Check if a specific key has been pressed

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
    void SetFramerate(float framerate); // Change the delay between two frame are calculated and drawn

    void Update(char32_t keyCode); // Update the frame depending on the key pressed
    void Clear(); // Clear the current frame buffer
    void Display(); // Draw the current frame buffer to the terminal

    /***********************************************************
    *                TERMINAL/NATIVE FUNCTIONS                 *
    ***********************************************************/

    namespace Terminal {
        bool IsUserInputsVisible(); // Determine if user inputs are visible
        vec2 GetTerminalSize(); // Determine the size in cells of the terminal window

        void SetTitle(std::string_view title); // Change the terminal title
        void SetCursorVisibility(bool visible); // Change the terminal visibility
        void SetUserInputsVisibility(bool visible); // Change the user inputs visibility
        void SetAlternateBuffer(bool enabled); // Toggle the terminal alternate buffer
        void SetCursorPos(const vec2& pos); // Change the cursor position

        void Clear(); // Clear the whole terminal
        void ClearLine(); // Clear the terminal current line
        void ClearLineEnd(); // Clear the terminal current line from the cursor
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
        std::shared_ptr<Cell> Get(const vec2& pos);
        std::shared_ptr<Cell> Get(size_t x, size_t y);
        bool Has(size_t x, size_t y) const;
        void Set(const vec2& pos, std::shared_ptr<Cell> cell);
        void Clear();

        vec2 m_Cursor;
        std::vector<std::vector<std::shared_ptr<Cell>>> m_Cells;
    };

    /***********************************************************
    *                    COMPONENTS/ITEMS                      *
    ***********************************************************/

    class Item {
        public:
            Item() : m_Id(0), m_Size(vec2(0, 0)), m_Pos(vec2(0, 0)) {}
            virtual ~Item() = default;
    
            ItemId m_Id;
            vec2 m_Size;
            vec2 m_Pos;
        };

    /***********************************************************
    *                       CONTAINERS                         *
    ***********************************************************/

    class Container : public Item {
    public:
        Container() : Item(), m_Flags(CONTAINER_FLAGS_NONE), m_Frame(nullptr) {}
        ~Container() = default;

        ContainerFlags m_Flags;
        std::shared_ptr<Frame> m_Frame;
    };

    void BeginContainer(std::string_view id, std::string_view label, const vec2& size);
    void EndContainer();

    /***********************************************************
    *                         ITEMS                            *
    ***********************************************************/

    template <typename... Args> void Print(std::format_string<Args...> fmt, Args&&... args); // Print a formatted string to the current frame.

    /***********************************************************
    *                    STRING FUNCTIONS                      *
    ***********************************************************/

    int Utf8CharWidth(char32_t ch); // Returns the width (in columns) of a character.
    uint8_t Utf8CharLength(char c); // Returns the expected UTF-8 length of a specific character
    std::string Utf8Char32ToString(char32_t ch); // Returns a string from a UTF-8 character
    char32_t Utf8Decode(const char* bytes, size_t length); // Returns a 32 bytes UTF-8 character from an array of bytes
    void Utf8IterateString(std::string_view sv, std::function<void(char32_t, size_t)> func); // Iterate over UTF-8 characters in a regular string
    ItemId StringToId(std::string_view sv); // Hash a string to get an integer.

    /**********************************************************
    *                        CONTEXT                          *
    **********************************************************/

    class Context {
    public:
        Context() {
            m_Framerate = 60.f;
            m_PressedKeyCode = 0;
            m_Frame = std::make_shared<Frame>();
            m_PrevFrame = nullptr;
        }
        ~Context() = default;

        float m_Framerate;
        char32_t m_PressedKeyCode;
        std::shared_ptr<Frame> m_Frame; // Final screen frame that is going to be displayed to the screen
        std::shared_ptr<Frame> m_PrevFrame; // Last displayed frame to compare with when building the new one
        std::vector<std::shared_ptr<Item>> m_ItemsOrdered; // Insertion order of items.
        std::unordered_map<ItemId, std::shared_ptr<Item>> m_Items; // Mapped addresses of the frame items.
        std::stack<ItemId> m_ContainersStack;
    };

    void AddItem(std::shared_ptr<Item> item);
    void MergeFrame(std::shared_ptr<Frame> frame); // Merge a given frame into the current context frame (can be global or a container).
    
    vec2 GetCurrentCursor(); // Returns the currently active frame cursor (global or container...)
    std::shared_ptr<Frame> GetCurrentFrame(); // Returns the frame that is currently active (global or container...)

    /***********************************************************
    *                    GLOBAL VARIABLES                      *
    ***********************************************************/

    inline Context* ctx; // Global context variable for the gui
}

/***********************************************************
*                   INLINE DEFINITIONS                     *
***********************************************************/

/***********************************************************
*                         INPUTS                           *
***********************************************************/

char32_t tuim::PollKeyCode() {
    Context* ctx = tuim::GetCtx();

    // Save original state.
    struct termios oldState, newState;
    tcgetattr(STDIN_FILENO, &oldState);
    newState = oldState;

    // Disable canonical mode, echo and set a timeout depending on the definied framerate.
    newState.c_lflag &= ~(ICANON | ECHO);
    newState.c_cc[VMIN] = 1;
    newState.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newState);

    // Wait for an input using a timeout.
    fd_set set;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = ((int) (1000 / ctx->m_Framerate)) * 1000;
    
    if (select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout) != 1) {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldState);
        return 0;
    }
    
    char buffer[4] = {0};
    ssize_t totalBytes = 0;
    ssize_t expectedBytes = 0;

    // Read the first byte.
    if (read(STDIN_FILENO, &buffer[0], 1) != 1) {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldState);
        return 0;
    }
    totalBytes = 1;

    // Handle Escape sequences by initializing the number of expected bytes
    // to 5 in order to avoid the next step of calculating expected bytes
    // for UTF8 characters.
    // TODO: handle special keys better, including F6, F7... which are 5 bytes escape sequences.
    if (static_cast<unsigned char>(buffer[0]) == 0x1B) {
        expectedBytes = 5;
    }

    // Determine the expected number of bytes left to read (if it isn't an escape sequence).
    if (expectedBytes == 0) {
        char8_t first = buffer[0];
        if ((first & 0x80) == 0x00) expectedBytes = 1;  // ASCII
        else if ((first & 0xE0) == 0xC0) expectedBytes = 2; // 2-byte UTF-8
        else if ((first & 0xF0) == 0xE0) expectedBytes = 3; // 3-byte UTF-8
        else if ((first & 0xF8) == 0xF0) expectedBytes = 4; // 4-byte UTF-8
        else {  // Invalid UTF-8 start byte
            tcsetattr(STDIN_FILENO, TCSANOW, &oldState);
            return 0;
        }
    }

    // Read remaining bytes.
    ssize_t readBytes = read(STDIN_FILENO, &buffer[totalBytes], std::min(expectedBytes, 4L)-totalBytes);
    totalBytes += readBytes;

    if (totalBytes != expectedBytes && expectedBytes != 5) {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldState);
        return 0;
    }

    // Pack bytes into a 32 bits char.
    char32_t keyCode = tuim::Utf8Decode(buffer, totalBytes);

    // Restore original terminal flags.
    tcsetattr(STDIN_FILENO, TCSANOW, &oldState);

    return keyCode;
}

tuim::Key tuim::GetPressedKey() {
    Context* ctx = tuim::GetCtx();
    return static_cast<tuim::Key>(ctx->m_PressedKeyCode);
}

bool tuim::IsKeyPressed() {
    Context* ctx = tuim::GetCtx();
    return ctx->m_PressedKeyCode == 0;
}

bool tuim::IsKeyPressed(tuim::Key key) {
    Context* ctx = tuim::GetCtx();
    return static_cast<tuim::Key>(ctx->m_PressedKeyCode) == key;
}

/***********************************************************
*                    CONTEXT FUNCTIONS                     *
***********************************************************/

void tuim::CreateContext() {
    tuim::Terminal::SetAlternateBuffer(true);
    tuim::Terminal::SetUserInputsVisibility(false);
    tuim::Terminal::SetCursorVisibility(false);
    tuim::Terminal::Clear();
    setlocale(LC_CTYPE, "");  // Enable Unicode processing
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

void tuim::SetFramerate(float framerate) {
    Context* ctx = tuim::GetCtx();
    ctx->m_Framerate = framerate;
}

void tuim::Update(char32_t keyCode) {
    Context* ctx = tuim::GetCtx();
    ctx->m_PressedKeyCode = keyCode;
    
    // TODO: implement update function.
}

void tuim::Clear() {
    Context* ctx = tuim::GetCtx();
    ctx->m_PrevFrame = ctx->m_Frame;
    ctx->m_Frame = std::make_shared<Frame>();
    ctx->m_ItemsOrdered.clear();
    ctx->m_Items.clear();
    // std::stack<ItemId>().swap(ctx->m_ContainersStack);
}

void tuim::Display() {
    if (!ctx->m_ContainersStack.empty())
        throw std::runtime_error("error: container stack is not empty.");

    // tuim::Terminal::Clear();
    tuim::Terminal::SetCursorPos(vec2(0, 0));

    Context* ctx = tuim::GetCtx();
    vec2 terminalSize = tuim::Terminal::GetTerminalSize();
    vec2 frameSize = ctx->m_Frame->GetSize();
    vec2 prevPos = vec2(-1, 0);

    for (size_t y = 0; y < std::min(frameSize.y, terminalSize.y-1); y++) {
        size_t lineWidth = std::min(
            std::max(
                ctx->m_Frame->m_Cells[y].size(),
                (ctx->m_PrevFrame != nullptr ? ctx->m_PrevFrame->m_Cells[y].size() : 0)
            ),
            (size_t) terminalSize.x
        );
        for (size_t x = 0; x < lineWidth; x++) {
            std::shared_ptr<Cell> cell = (ctx->m_Frame->Has(x, y) ? ctx->m_Frame->Get(x, y) : nullptr);
            std::shared_ptr<Cell> prevCell = (ctx->m_PrevFrame != nullptr && ctx->m_PrevFrame->Has(x, y) ? ctx->m_PrevFrame->Get(x, y) : nullptr);

            // Print a character only if it is different that during the previous frame.
            if (cell != prevCell || (cell != nullptr && prevCell != nullptr && cell->m_Character != prevCell->m_Character)) {
                // Set cursor to pixel if consecutives characters have not been changed since the previous frame.
                if (prevPos.x != x-1) {
                    tuim::Terminal::SetCursorPos(vec2(x, y));
                }
                // Replace the previous character by the new one.
                if (cell == nullptr) {
                    std::cout << ' ';
                    prevPos = vec2(x, y);
                }
                else {
                    uint8_t width = tuim::Utf8CharWidth(cell->m_Character);
                    if (width == 2)
                        x++;
                    std::cout << Utf8Char32ToString(cell->m_Character);
                    prevPos = vec2(x, y);
                }
            }
        }
        // Clear the remaining of the line.
        if (lineWidth < terminalSize.x-1) {
            std::cout << ' ';
            tuim::Terminal::ClearLineEnd();
        }
        // Print a new line to the terminal for every line except the last one.
        if (y != frameSize.y-1) {
            std::cout << '\n';
            prevPos = vec2(-1, y+1);
        }
    }
    tuim::Terminal::ClearEnd();
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
    std::cout << "\033]0;" << title << "\007";
}

void tuim::Terminal::SetCursorVisibility(bool visible) {
    std::cout << "\033[?25" << (visible ? 'h' : 'l');
}

void tuim::Terminal::SetUserInputsVisibility(bool visible) {
    termios term;
    tcgetattr(0, &term);
    if (visible) term.c_lflag |= ECHO;
    else term.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &term);
}

void tuim::Terminal::SetAlternateBuffer(bool enabled) {
    std::cout << "\033[?1049" << (enabled ? 'h' : 'l');
}

void tuim::Terminal::SetCursorPos(const tuim::vec2& pos) {
    std::cout << "\033[" << pos.y+1 << ";" << pos.x+1 << "H";
}

void tuim::Terminal::Clear() {
    std::cout << "\033[2J\033[0m\033[H";
}

void tuim::Terminal::ClearLine() {
    std::cout << "\033[2K";
}

void tuim::Terminal::ClearLineEnd() {
    std::cout << "\033[0K";
}

void tuim::Terminal::ClearEnd() {
    std::cout << "\033[0J";
}

void tuim::Terminal::ClearStyles() {
    std::cout << "\033[0m";
}

/***********************************************************
*                          FRAME                           *
***********************************************************/

tuim::Frame::Frame() {
    vec2 terminalSize = tuim::Terminal::GetTerminalSize();
    m_Cursor = vec2(0, 0);
    m_Cells = std::vector<std::vector<std::shared_ptr<Cell>>>(terminalSize.y, std::vector<std::shared_ptr<Cell>>(terminalSize.x, nullptr));
}

tuim::Frame::Frame(const vec2& size) {
    m_Cursor = vec2(0, 0);
    m_Cells = std::vector<std::vector<std::shared_ptr<Cell>>>(size.y, std::vector<std::shared_ptr<Cell>>(size.x, nullptr));
}

tuim::vec2 tuim::Frame::GetSize() const {
    size_t maxWidth = m_Cells.front().size();
    for (size_t i = 0; i < m_Cells.size(); i++)
        maxWidth = std::max(maxWidth, m_Cells[i].size());
    return vec2(m_Cells.size(), m_Cells.size());
}

std::shared_ptr<tuim::Cell> tuim::Frame::Get(const tuim::vec2& pos) {
    return m_Cells[pos.y][pos.x];
}

std::shared_ptr<tuim::Cell> tuim::Frame::Get(size_t x, size_t y) {
    return m_Cells[y][x];
}

bool tuim::Frame::Has(size_t x, size_t y) const {
    return m_Cells.size() > y && m_Cells[y].size() > x;
}

void tuim::Frame::Set(const tuim::vec2& pos, std::shared_ptr<tuim::Cell> cell) {
    // Make sure to resize the vectors in case the position is
    // beyond the column or line size.
    if (m_Cells.size() <= pos.y)
        m_Cells.resize(pos.y+1, std::vector<std::shared_ptr<Cell>>(pos.x, nullptr));
    if (m_Cells[pos.y].size() <= pos.x)
        m_Cells[pos.y].resize(pos.x+1, nullptr);
    m_Cells[pos.y][pos.x] = cell;
}

void tuim::Frame::Clear() {
    for (auto& line : m_Cells)
        std::fill(line.begin(), line.end(), nullptr);
    // TODO: maybe it would be better to reset the size instead?
}

/***********************************************************
*                       CONTAINERS                         *
***********************************************************/

void tuim::BeginContainer(std::string_view id, std::string_view label, const tuim::vec2& size) {
    Context* ctx = tuim::GetCtx();
    std::shared_ptr<Frame> frame = tuim::GetCurrentFrame();

    // Create the container object.
    ItemId itemId = tuim::StringToId(id);
    std::shared_ptr<Container> container = std::make_shared<Container>();
    container->m_Id = itemId;
    container->m_Size = size;
    container->m_Pos = frame->m_Cursor;
    container->m_Frame = std::make_shared<Frame>(size);

    // Save the container to the context.
    tuim::AddItem(container);
    ctx->m_ContainersStack.push(itemId);
}

void tuim::EndContainer() {
    Context* ctx = tuim::GetCtx();
    
    if (ctx->m_ContainersStack.empty())
        throw std::out_of_range("error: cannot pop empty containers stack.");
    
    // Remove the current container from the stack.
    ItemId itemId = ctx->m_ContainersStack.top();
    ctx->m_ContainersStack.pop();
        
    // Retrieve the container object using its item id.
    auto it = ctx->m_Items.find(itemId);
    if (it == ctx->m_Items.end())
        throw std::out_of_range("error: undefined container from stack.");
    std::shared_ptr<Container> container = std::dynamic_pointer_cast<Container>(it->second);

    // Merge the container frame with the new active frame.
    tuim::MergeFrame(container->m_Frame);
}

/***********************************************************
*                         ITEMS                            *
***********************************************************/

template <typename... Args> void tuim::Print(std::format_string<Args...> fmt, Args&&... args) {
    std::string str = std::format(fmt, std::forward<Args>(args)...);
    // TODO: handle colors and styles.
    Context* ctx = tuim::GetCtx();
    std::shared_ptr<Frame> frame = tuim::GetCurrentFrame();
    tuim::Utf8IterateString(str, [ctx, frame](char32_t ch, size_t index) {
        if (ch == '\t') {
            frame->m_Cursor.x += 4;
        }
        else if (ch == '\n') {
            frame->m_Cursor = vec2(0, frame->m_Cursor.y+1);
        }
        else {
            uint8_t width = tuim::Utf8CharWidth(ch);
            if (width < 1)
                return;
            if (width >= 1) {
                frame->Set(frame->m_Cursor, std::make_shared<Cell>(ch));
                frame->m_Cursor.x++;
            }
            if (width == 2) {
                frame->Set(frame->m_Cursor, nullptr);
                frame->m_Cursor.x++;
            }
        }
    });
}

/***********************************************************
*                    STRING FUNCTIONS                      *
***********************************************************/

int tuim::Utf8CharWidth(char32_t ch) {
    wchar_t wc = static_cast<wchar_t>(ch);
    return wcwidth(wc); // Returns -1, 0, 1, or 2.
}

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

char32_t tuim::Utf8Decode(const char* bytes, size_t length) {
    unsigned char b0 = bytes[0];

    // Do not try to decode escape sequence as UTF-8.
    if (b0 != 0x1B) {
        if ((b0 & 0x80) == 0x00) return b0;  // 1-byte ASCII
        if ((b0 & 0xE0) == 0xC0) return ((b0 & 0x1F) << 6) | (bytes[1] & 0x3F); // 2-bytes UTF-8
        if ((b0 & 0xF0) == 0xE0) return ((b0 & 0x0F) << 12) | ((bytes[1] & 0x3F) << 6) | (bytes[2] & 0x3F); // 3-bytes UTF-8
        if ((b0 & 0xF8) == 0xF0) return ((b0 & 0x07) << 18) | ((bytes[1] & 0x3F) << 12) | ((bytes[2] & 0x3F) << 6) | (bytes[3] & 0x3F); // 4-bytes UTF-8
    }

    // Just fill the 32-bytes with the buffer bytes if it isn't a UTF-8 character.
    char32_t ch = 0;
    for (int i = 0; i < length; i++)
        ch = (ch << 8) | static_cast<unsigned char>(bytes[i]);
    return ch; 
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

// http://www.cse.yorku.ca/~oz/hash.html
tuim::ItemId tuim::StringToId(std::string_view sv) {
    unsigned long hash = 5381;
    for (size_t i = 0; i < sv.size(); i++)
        hash = 33 * hash + (unsigned char) sv[i];
    return hash;
}

/**********************************************************
*                        CONTEXT                          *
**********************************************************/

void tuim::AddItem(std::shared_ptr<tuim::Item> item) {
    Context* ctx = tuim::GetCtx();
    ctx->m_ItemsOrdered.push_back(item);
    ctx->m_Items.emplace(item->m_Id, item);
}

void tuim::MergeFrame(std::shared_ptr<tuim::Frame> src) {
    std::shared_ptr<Frame> dst = tuim::GetCurrentFrame();
    // TODO: should avoid calling Frame::GetSize() for performances.
    // vec2 srcSize = src->GetSize();
    // vec2 dstSize = src->GetSize();

    // TODO: should auto resize dest frame only if it has the appropriate flag
    //       or if it is the global frame.
    // Resize vertically the dst frame if it is too small.
    // if (dstSize.y < srcSize.y)
    //     dst->m_Cells.resize(srcSize.y, std::vector<std::shared_ptr<Cell>>(srcSize.x, nullptr));

    vec2 origin = dst->m_Cursor;

    for (size_t y = 0; y < std::min(src->m_Cells.size(), dst->m_Cells.size() - origin.y); y++) {
        
        // Resize horizontally the dst line if it is too small.
        // size_t srcLineSize = src->m_Cells[y].size();
        // if (dst->m_Cells[y].size() <= srcLineSize)
        //     dst->m_Cells[y].resize(srcLineSize, nullptr);

        for (size_t x = 0; x < std::min(src->m_Cells[y].size(), dst->m_Cells[y].size() - origin.x); x++) {
            dst->m_Cells[origin.y + y][origin.x + x] = src->m_Cells[y][x];
        }
    }

    // Move the dest frame cursor to not overwrite when printing characters after merging.
    dst->m_Cursor = vec2(0, origin.y + src->m_Cells.size());
}

tuim::vec2 tuim::GetCurrentCursor() {
    return tuim::GetCurrentFrame()->m_Cursor;
}

std::shared_ptr<tuim::Frame> tuim::GetCurrentFrame() {
    Context* ctx = tuim::GetCtx();
    if (ctx->m_ContainersStack.empty())
        return ctx->m_Frame;
    ItemId itemId = ctx->m_ContainersStack.top();
    auto it = ctx->m_Items.find(itemId);
    if (it == ctx->m_Items.end())
        throw std::out_of_range("error: undefined container from stack.");
    return std::dynamic_pointer_cast<Container>(it->second)->m_Frame;
}

#endif // TUIM_HPP