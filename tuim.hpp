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
#include <optional> // std::optional
#include <charconv> // std::from_chars

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
    using ItemFlags = uint32_t;
    using ContainerFlags = uint32_t;
    using InputTextFlags = uint32_t;

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

    struct Color {
        uint8_t r, g, b;
        bool bg;

        Color() : r(0), g(0), b(0), bg(false) {}
        Color(uint8_t r, uint8_t g, uint8_t b, bool bg = false) : r(r), g(g), b(b), bg(bg) {}

        bool operator==(const Color &other) const {
            return r == other.r && g == other.g && b == other.b && bg == other.bg;
        }

        bool operator!=(const Color &other) const {
            return r != other.r || g != other.g || b != other.b || bg != other.bg;
        }
    };

    Color StringToColor(std::string_view sv); // Returns the color of a formatted string.
    std::string ColorToAnsi(const Color& color); // Returns the ANSI escape sequence for a given color.

    /***********************************************************
    *                         STYLE                            *
    ***********************************************************/
   
    enum class Style : uint32_t {
        NONE          = 0,
        BOLD          = 1 << 0,
        FAINT         = 1 << 1,
        ITALIC        = 1 << 2,
        UNDERLINE     = 1 << 3,
        BLINKING      = 1 << 4,
        REVERSE       = 1 << 5,
        HIDDEN        = 1 << 6,
        STRIKETHROUGH = 1 << 7
    };

    Style operator|(Style a, Style b) {
        return static_cast<Style>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }
    
    Style operator&(Style a, Style b) {
        return static_cast<Style>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
    }
    
    Style operator^(Style a, Style b) {
        return static_cast<Style>(static_cast<uint32_t>(a) ^ static_cast<uint32_t>(b));
    }
    
    Style& operator|=(Style& a, Style b) {
        a = static_cast<Style>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
        return a;
    }
    
    Style& operator&=(Style& a, Style b) {
        a = static_cast<Style>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
        return a;
    }

    Style operator~(Style a) {
        return static_cast<Style>(~static_cast<uint32_t>(a));
    }

    std::string StyleToAnsi(Style style); // Returns the ANSI escape sequence for a given set of styles.
   
    /***********************************************************
    *                         FLAGS                            *
    ***********************************************************/

    enum ItemFlags_ : uint32_t {
        ITEM_FLAGS_NONE        = 0,
        ITEM_FLAGS_DISABLED    = 1 << 0,
        ITEM_FLAGS_STAY_ACTIVE = 1 << 1,
    };
    
    enum ContainerFlags_ : uint32_t {
        CONTAINER_FLAGS_NONE = 0,
        CONTAINER_FLAGS_BORDERLESS = 1 << 0,
    };
    
    enum InputTextFlags_ : uint32_t {
        INPUT_TEXT_FLAGS_NONE = 0,
        INPUT_TEXT_FLAGS_CONFIRM_ON_ENTER = 1 << 0,
        INPUT_TEXT_FLAGS_NUMERIC_ONLY = 1 << 1,
        INPUT_TEXT_FLAGS_ALPHANUMERIC_ONLY = 1 << 2,
    };

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

    void DefineStyle(char tag, Style style);
    void DefineForeground(char tag, Color color);
    void DefineBackground(char tag, Color color);

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
        Cell(char32_t ch, Style style = Style()) : m_Character(ch), m_Style(style) {}
        ~Cell() = default;

        bool operator==(const Cell& other) const {
            return m_Character == other.m_Character &&
                   m_Style == other.m_Style &&
                   m_Foreground == other.m_Foreground &&
                   m_Background == other.m_Background;
        }

        bool operator!=(const Cell& other) const {
            return !(*this == other);
        }

        char32_t m_Character;
        Style m_Style;
        std::optional<Color> m_Foreground;
        std::optional<Color> m_Background;
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
        Item() : m_Id(0), m_Size(vec2(0, 0)), m_Pos(vec2(0, 0)), m_Flags(ITEM_FLAGS_NONE) {}
        virtual ~Item() = default;

        ItemId m_Id;
        vec2 m_Size;
        vec2 m_Pos;
        ItemFlags m_Flags;
    };

    ItemId GetCurrentItemId();
    uint32_t GetItemIndex(ItemId id);
    uint32_t getHoveredItemIndex();
    bool WasItemActive();
    bool IsItemActive();
    bool IsItemHovered();
    void SetActiveItemId(ItemId id);
    void SetHoveredItemId(ItemId id);
    bool HasHoverable();

    /***********************************************************
    *                       CONTAINERS                         *
    ***********************************************************/

    class Container : public Item {
    public:
        Container() : Item(), m_ContainerFlags(CONTAINER_FLAGS_NONE), m_Frame(nullptr) {}
        ~Container() = default;

        ContainerFlags m_ContainerFlags;
        std::shared_ptr<Frame> m_Frame;
    };

    void BeginContainer(std::string_view id, std::string_view label, vec2 size, ContainerFlags flags = CONTAINER_FLAGS_NONE);
    void EndContainer();

    void MergeContainer(std::shared_ptr<Container> container); // Merge a given container into the current context frame (can be global or another container).

    /***********************************************************
    *                         ITEMS                            *
    ***********************************************************/

    template <typename... Args> void Print(const std::string& fmt, Args&&... args); // Print a formatted string to the current frame.
    bool Button(const std::string& id, const std::string& text, ItemFlags flags = ITEM_FLAGS_NONE); // Print a button that can be pressed.
    bool TextInput(const std::string& id, std::string_view fmt, std::string* value, InputTextFlags flags = INPUT_TEXT_FLAGS_CONFIRM_ON_ENTER); // Print an string input.
    bool IntSlider(const std::string& id, std::string_view fmt, int* value, int min, int max, int step = 1, int width = 100); // Print an integer slider.
    bool FloatSlider(const std::string& id, std::string_view fmt, float* value, float min, float max, float step = 0.01, int width = 100); // Print a float number slider.

    /***********************************************************
    *                    STRING FUNCTIONS                      *
    ***********************************************************/
    
    constexpr unsigned char UTF8_CONT_MASK = 0xC0;  // 11000000
    constexpr unsigned char UTF8_CONT_TAG  = 0x80;  // 10000000

    int Utf8CharWidth(char32_t ch); // Returns the width (in columns) of a character.
    uint8_t Utf8CharLength(char c); // Returns the expected UTF-8 length of a specific character
    std::string Utf8Char32ToString(char32_t ch); // Returns a string from a UTF-8 character
    char32_t Utf8Decode(const char* bytes, size_t length); // Returns a 32 bytes UTF-8 character from an array of bytes
    void Utf8IterateString(std::string_view sv, std::function<void(char32_t, size_t)> func); // Iterate over UTF-8 characters in a regular string
    ItemId StringToId(std::string_view sv); // Hash a string to get an integer.
    bool IsPrintable(char32_t ch); // Determines if a character is "printable".

    size_t Utf8CharLastIndex(std::string_view sv, size_t index); // Returns the index of the last UTF8 character.
    size_t Utf8CharNextIndex(std::string_view sv, size_t index); // Returns the index of the next UTF8 character.

    bool IsDigit(const std::string& str);
    bool IsAlphaNumeric(const std::string& str);

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

            m_HoveredItemId = 0;
            m_ActiveItemId = 0;
            m_LastActiveItemId = 0;

            m_CurrentForeground = std::nullopt;
            m_CurrentBackground = std::nullopt;
            m_CurrentStyle = Style::NONE;
        }
        ~Context() = default;

        float m_Framerate;
        char32_t m_PressedKeyCode;
        std::shared_ptr<Frame> m_Frame; // Final screen frame that is going to be displayed to the screen
        std::shared_ptr<Frame> m_PrevFrame; // Last displayed frame to compare with when building the new one
        std::vector<std::shared_ptr<Item>> m_ItemsOrdered; // Insertion order of items.
        std::unordered_map<ItemId, std::shared_ptr<Item>> m_Items; // Mapped addresses of the frame items.
        std::stack<ItemId> m_ContainersStack;
        ItemId m_HoveredItemId; // Id of the item hovered during current frame.
        ItemId m_ActiveItemId; // Id of the active item during current frame.
        ItemId m_LastActiveItemId; // Id of the active item during last frame

        // Current active styles applied during Print operations
        std::optional<Color> m_CurrentForeground;
        std::optional<Color> m_CurrentBackground;
        Style m_CurrentStyle;

        // User-defined style maps.
        std::unordered_map<char, Style> m_UserStyles;
        std::unordered_map<char, Color> m_UserForegrounds;
        std::unordered_map<char, Color> m_UserBackgrounds;
    };

    void AddItem(std::shared_ptr<Item> item);
    void MergeFrame(std::shared_ptr<Frame> frame); // Merge a given frame into the current context frame (can be global or a container).
    
    void SetCurrentCursor(const vec2& cursor); // Changes the position of the current frame's cursor.
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
    if (static_cast<unsigned char>(buffer[0]) == Key::ESCAPE) {
        expectedBytes = 5;

        fd_set seqSet;
        FD_ZERO(&seqSet);
        FD_SET(STDIN_FILENO, &seqSet);

        timeval seqTimeout;
        seqTimeout.tv_sec = 0;
        seqTimeout.tv_usec = 1000 * 5;

        // if there are no more bytes to come, then it is a single Escape key.
        if (select(STDIN_FILENO + 1, &seqSet, NULL, NULL, &seqTimeout) != 1) {
            tcsetattr(STDIN_FILENO, TCSANOW, &oldState);
            return Key::ESCAPE;
        }

        // Reset the timeout.
        select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);
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
*                         COLORS                           *
***********************************************************/

tuim::Color tuim::StringToColor(std::string_view sv) {
    Color color(255, 255, 255);

    if (sv.size() < 2)
        return color;

    // Check if there is the background character.
    color.bg = (sv[1] == '_');
    
    // Remove the format and background characters.
    sv.remove_prefix(1 + color.bg);

    // Determine how the color is formatted (hex or code).
    // Parse the color as a style code defined by the user.
    if (sv.front() == '&') {
        return color;
    }

    unsigned long hex;
    auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), hex, 16);
    if (ec != std::errc())
        return color;

    color.r = static_cast<uint8_t>((hex >> 16) & 0xFF);
    color.g = static_cast<uint8_t>((hex >> 8) & 0xFF);
    color.b = static_cast<uint8_t>(hex & 0xFF);

    return color;
}

std::string tuim::ColorToAnsi(const tuim::Color& color) {
    return std::format(
        "\33[{};2;{};{};{}m",
        (color.bg ? "48" : "38"),
        color.r,
        color.g,
        color.b
    );
}

/***********************************************************
*                         STYLE                            *
***********************************************************/

std::string tuim::StyleToAnsi(Style style) {
    std::string str;
    if ((style & Style::BOLD) == Style::BOLD) str += "\033[1m";
    if ((style & Style::FAINT) == Style::FAINT) str += "\033[2m";
    if ((style & Style::ITALIC) == Style::ITALIC) str += "\033[3m";
    if ((style & Style::UNDERLINE) == Style::UNDERLINE) str += "\033[4m";
    if ((style & Style::BLINKING) == Style::BLINKING) str += "\033[5m";
    if ((style & Style::REVERSE) == Style::REVERSE) str += "\033[7m";
    if ((style & Style::HIDDEN) == Style::HIDDEN) str += "\033[8m";
    if ((style & Style::STRIKETHROUGH) == Style::STRIKETHROUGH) str += "\033[9m";
    return str;
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

void tuim::DefineStyle(char tag, Style style) {
    Context* ctx = tuim::GetCtx();
    ctx->m_UserStyles[tag] = style;
}

void tuim::DefineForeground(char tag, Color color) {
    Context* ctx = tuim::GetCtx();
    ctx->m_UserForegrounds[tag] = color;
}

void tuim::DefineBackground(char tag, Color color) {
    Context* ctx = tuim::GetCtx();
    ctx->m_UserBackgrounds[tag] = color;
}

void tuim::Update(char32_t keyCode) {
    Context* ctx = tuim::GetCtx();
    ctx->m_PressedKeyCode = keyCode;

    if(ctx->m_ActiveItemId == 0 || !(ctx->m_ItemsOrdered.at(tuim::GetItemIndex(ctx->m_ActiveItemId))->m_Flags & ITEM_FLAGS_STAY_ACTIVE)) {
        ctx->m_LastActiveItemId = ctx->m_ActiveItemId;
        ctx->m_ActiveItemId = 0;

        int hoveredIndex = tuim::getHoveredItemIndex();
        bool hasHoverable = tuim::HasHoverable();

        // Set the first hoverable item hovered if no item is
        if(hoveredIndex == -1 && hasHoverable) {
            for(size_t i = 0; i < ctx->m_ItemsOrdered.size(); i++) {
                if(!(ctx->m_ItemsOrdered.at(i)->m_Flags & ITEM_FLAGS_DISABLED)) {
                    ctx->m_HoveredItemId = ctx->m_ItemsOrdered.at(i)->m_Id;
                    hoveredIndex = i;
                    break;
                }
            }
        }

        // Move cursor to previous hoverable item
        if(keyCode == Key::UP) {
            if(hasHoverable) {
                ItemId id = 0;
                if(hoveredIndex != -1) {
                    size_t index = std::max(0, hoveredIndex - 1);
                    while(index > 0 && (ctx->m_ItemsOrdered.at(index)->m_Flags & ITEM_FLAGS_DISABLED)) index--;
                    if((ctx->m_ItemsOrdered.at(index)->m_Flags & ITEM_FLAGS_DISABLED)) index = hoveredIndex;
                    id = ctx->m_ItemsOrdered.at(index)->m_Id;
                }
                ctx->m_HoveredItemId = id;
            }
        }

        // Move cursor to next hoverable item
        if(keyCode == Key::DOWN) {
            if(hasHoverable) {
                ItemId id = 0;
                if(hoveredIndex != -1) {
                    size_t index = std::min(hoveredIndex + 1, (int) ctx->m_ItemsOrdered.size() - 1);
                    while(index < (ctx->m_ItemsOrdered.size() - 1) && (ctx->m_ItemsOrdered.at(index)->m_Flags) & ITEM_FLAGS_DISABLED) index++;
                    if((ctx->m_ItemsOrdered.at(index)->m_Flags & ITEM_FLAGS_DISABLED)) index = hoveredIndex;
                    id = ctx->m_ItemsOrdered.at(index)->m_Id;
                }
                ctx->m_HoveredItemId = id;
            }
        }
    }

    // ctx->m_ItemsOrdered.clear();
}

void tuim::Clear() {
    vec2 terminalSize = tuim::Terminal::GetTerminalSize();
    Context* ctx = tuim::GetCtx();

    ctx->m_PrevFrame = ctx->m_Frame;
    ctx->m_Frame = std::make_shared<Frame>(terminalSize);
    ctx->m_ItemsOrdered.clear();
    ctx->m_Items.clear();

    ctx->m_CurrentForeground = std::nullopt;
    ctx->m_CurrentBackground = std::nullopt;
    ctx->m_CurrentStyle = Style::NONE;
}

void tuim::Display() {
    if (!ctx->m_ContainersStack.empty())
        throw std::runtime_error("error: container stack is not empty.");

    // tuim::Terminal::Clear();
    tuim::Terminal::ClearStyles();
    tuim::Terminal::SetCursorPos(vec2(0, 0));

    Context* ctx = tuim::GetCtx();
    vec2 terminalSize = tuim::Terminal::GetTerminalSize();
    vec2 prevPos = vec2(-1, 0);
    
    size_t frameHeight = ctx->m_Frame->m_Cells.size();
    size_t displayHeight = std::min(frameHeight, (size_t) terminalSize.y);

    // Current ANSI styles actually applied to the terminal to avoid redundant codes.
    std::optional<Color> currentForeground = std::nullopt;
    std::optional<Color> currentBackground = std::nullopt;
    Style currentStyle = Style::NONE;

    for (size_t y = 0; y < displayHeight; y++) {
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

            // Compare cells, including character and all style attributes.
            // bool cellChanged = (cell == nullptr && prevCell != nullptr) ||
            //                     (cell != nullptr && prevCell == nullptr) ||
            //                     (cell != nullptr && prevCell != nullptr && *cell != *prevCell);
            bool cellChanged = true;
            
            if (cellChanged) {
                // Set cursor to pixel if consecutive characters have not been changed since the previous frame,
                // or if there's a jump in position.
                if (prevPos.x != x-1) {
                    tuim::Terminal::SetCursorPos(vec2(x, y));
                }

                // Apply ANSI styles if current cell's effective style differs from applied style
                // This logic is crucial for efficiency and correct terminal state.
                std::string ansi;

                // If the new cell is null (empty), reset all styles to default for the space.
                if (cell == nullptr) {
                    if (currentForeground.has_value() || currentBackground.has_value() || currentStyle != Style::NONE) {
                        ansi += "\033[0m"; // Reset all attributes
                        currentForeground = std::nullopt;
                        currentBackground = std::nullopt;
                        currentStyle = Style::NONE;
                    }
                }
                else {
                    if (cell->m_Foreground != currentForeground || cell->m_Background != currentBackground || cell->m_Style != currentStyle) {
                        ansi += "\033[0m"; // Reset all attributes.
                        currentForeground = std::nullopt;
                        currentBackground = std::nullopt;
                        currentStyle = Style::NONE;

                        // Re-apply styles and colors for the new cell.
                        if (cell->m_Foreground.has_value()) {
                            ansi += tuim::ColorToAnsi(cell->m_Foreground.value());
                            currentForeground = cell->m_Foreground;
                        }
                        if (cell->m_Background.has_value()) {
                            ansi += tuim::ColorToAnsi(cell->m_Background.value());
                            currentBackground = cell->m_Background;
                        }
                        ansi += tuim::StyleToAnsi(cell->m_Style);
                        currentStyle = cell->m_Style;
                    }
                }

                // Print accumulated ANSI color and style codes.
                std::cout << ansi;

                // Replace the previous character by the new one.
                if (cell == nullptr) {
                    std::cout << ' ';
                    prevPos = vec2(x, y);
                }
                else {
                    uint8_t width = tuim::Utf8CharWidth(cell->m_Character);
                    // Increment x for wide characters to account for the second column.
                    if (width == 2)
                        x++;
                    std::cout << tuim::Utf8Char32ToString(cell->m_Character);
                    prevPos = vec2(x, y);
                }
            }
        }
        
        // Clear the remaining of the line and reset styles at the end of the line.
        // Also reset styles to default so that newlines or next lines start clean.
        // If the line width is less than terminal width, clear the rest of the line.
        if (lineWidth < terminalSize.x-1) {
            tuim::Terminal::ClearStyles();
            std::cout << ' ';
            tuim::Terminal::ClearLineEnd();

            currentForeground = std::nullopt;
            currentBackground = std::nullopt;
            currentStyle = Style::NONE;
        }

        // Print a new line to the terminal for every line except the last one.
        if (y != displayHeight-1) {
            std::cout << '\n';
            prevPos = vec2(-1, y+1);
        }
    }

    tuim::Terminal::ClearStyles();
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
    if (pos.y >= m_Cells.size())
        m_Cells.resize(pos.y+1, std::vector<std::shared_ptr<Cell>>(pos.x, nullptr));
    if (pos.x >= m_Cells[pos.y].size())
        m_Cells[pos.y].resize(pos.x+1, nullptr);
    m_Cells[pos.y][pos.x] = cell;
}

void tuim::Frame::Clear() {
    for (auto& line : m_Cells)
        std::fill(line.begin(), line.end(), nullptr);
    // TODO: maybe it would be better to reset the size instead?
}

/***********************************************************
*                    COMPONENTS/ITEMS                      *
***********************************************************/

tuim::ItemId tuim::GetCurrentItemId() {
    Context* ctx = tuim::GetCtx();
    return ctx->m_ItemsOrdered.back()->m_Id;
}

uint32_t tuim::GetItemIndex(tuim::ItemId id) {
    Context* ctx = tuim::GetCtx();
    for(size_t i = 0; i < ctx->m_ItemsOrdered.size(); i++) {
        if(ctx->m_ItemsOrdered.at(i)->m_Id == id) {
            return i;
        }
    }
    return -1;
}

uint32_t tuim::getHoveredItemIndex() {
    Context* ctx = tuim::GetCtx();
    return tuim::GetItemIndex(ctx->m_HoveredItemId);
}

bool tuim::WasItemActive() {
    Context* ctx = tuim::GetCtx();
    return ctx->m_LastActiveItemId == (ctx->m_ItemsOrdered.back()->m_Id);
}

bool tuim::IsItemActive() {
    Context* ctx = tuim::GetCtx();
    return ctx->m_ActiveItemId == (ctx->m_ItemsOrdered.back()->m_Id);
}

bool tuim::IsItemHovered() {
    Context* ctx = tuim::GetCtx();
    return ctx->m_HoveredItemId == (ctx->m_ItemsOrdered.back()->m_Id);
}

void tuim::SetActiveItemId(ItemId id) {
    Context* ctx = tuim::GetCtx();
    ctx->m_ActiveItemId = id;
}

void tuim::SetHoveredItemId(ItemId id) {
    Context* ctx = tuim::GetCtx();
    ctx->m_HoveredItemId = id;
}

bool tuim::HasHoverable() {
    Context* ctx = tuim::GetCtx();
    for(size_t i = 0; i < ctx->m_ItemsOrdered.size(); i++) {
        if(!(ctx->m_ItemsOrdered.at(i)->m_Flags & ITEM_FLAGS_DISABLED))
            return true;
    }
    return false;
}

/***********************************************************
*                       CONTAINERS                         *
***********************************************************/

void tuim::BeginContainer(std::string_view id, std::string_view label, tuim::vec2 size, ContainerFlags flags) {
    Context* ctx = tuim::GetCtx();
    std::shared_ptr<Frame> frame = tuim::GetCurrentFrame();

    // Make sure that the size is positive.
    if (size.x < 0) size.x = 0;
    if (size.y < 0) size.y = 0;

    // Create the container object.
    ItemId itemId = tuim::StringToId(id);
    std::shared_ptr<Container> container = std::make_shared<Container>();
    container->m_Id = itemId;
    container->m_Flags = ITEM_FLAGS_DISABLED;
    container->m_ContainerFlags = flags;
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

    // Merge the container with the new active container or global frame.
    tuim::MergeContainer(container);
}

void tuim::MergeContainer(std::shared_ptr<tuim::Container> container) {
    std::shared_ptr<Frame> src = container->m_Frame;
    std::shared_ptr<Frame> dst = tuim::GetCurrentFrame();
    
    bool hasBorder = !(container->m_ContainerFlags & CONTAINER_FLAGS_BORDERLESS);

    vec2 origin = dst->m_Cursor;

    // Relative heights of the frames.
    size_t dstHeight = dst->m_Cells.size() - origin.y;

    // Size of the source container, including the borders.
    size_t containerWidth = container->m_Size.x;
    size_t containerHeight = container->m_Size.y;

    auto GetRowSize = [&](int y) { return dst->m_Cells[y].size(); };
    auto SetCell = [&](int x, int y, char32_t ch) { dst->m_Cells[y][x] = std::make_shared<Cell>(ch); };

    // Don't display no-width or no-height containers but move the cursors regardless.
    if (container->m_Size.x > 0 && container->m_Size.y > 0) {
        // Surround the container frame with a border if it does not have the borderless flags.
        if (hasBorder) {
            // TODO: check for pre-existing borders.

            // Coordinates of the end of the src container (origin + size).
            size_t maxX = origin.x + containerWidth - 1;
            size_t maxY = origin.y + containerHeight - 1;

            bool canWriteFirstRow = dstHeight >= 1;
            bool canWriteLastRow = dstHeight >= containerHeight;

            // Draw top-left corner.
            if (canWriteFirstRow && GetRowSize(origin.y) > origin.x)
                SetCell(origin.x, origin.y, U'+');

            // Draw top-right corner.
            if (canWriteFirstRow && GetRowSize(origin.y) > maxX)
                SetCell(maxX, origin.y, U'+');

            // Draw bottom-left corner.
            if (canWriteLastRow && GetRowSize(maxY) > origin.x)
                SetCell(origin.x, maxY, U'+');

            // Draw bottom-right corner.
            if (canWriteLastRow && GetRowSize(maxY) > maxX)
                SetCell(maxX, maxY, U'+');

            // Draw top and bottom borders.
            for (size_t x = 1; x < containerWidth-1; x++) {
                if (canWriteFirstRow && GetRowSize(origin.y) > origin.x + x)
                    SetCell(origin.x + x, origin.y, U'-');
                if (canWriteLastRow && GetRowSize(maxY) > origin.x + x)
                    SetCell(origin.x + x, maxY, U'-');
            }
            
            // Draw left and right borders.
            for (size_t y = 1; y < containerHeight-1; y++) {
                if (dstHeight > y && GetRowSize(origin.y + y) > origin.x)
                    SetCell(origin.x, origin.y + y, U'|');
                if (dstHeight > y && GetRowSize(origin.y + y) > maxX)
                    SetCell(maxX, origin.y + y, U'|');
            }
        }

        // Move the origin to not overwrite the border.
        vec2 contentOrigin = vec2(origin.x + hasBorder, origin.y + hasBorder);

        // Parse to int and size_t in order to have negatives and return 0.
        size_t minY = std::min({
            (size_t) std::max(0, (int) containerHeight - 2*hasBorder), // Height of the container (without borders).
            src->m_Cells.size(), // Height of source frame.
            (size_t) std::max(0, (int) dst->m_Cells.size() - contentOrigin.y) // Relative height of dest frame.
        });

        // Merge source frame characters into the dest frame.
        for (size_t y = 0; y < minY; y++) {
            
            // Parse to int and size_t in order to have negatives and return 0.
            size_t minX = std::min({
                (size_t) std::max(0, (int) containerWidth - 2*hasBorder), // Width of the container (without borders).
                src->m_Cells[y].size(), // Width of source frame.
                (size_t) std::max(0, (int) dst->m_Cells[contentOrigin.y + y].size() - contentOrigin.x) // Relative width of dest frame.
            });

            for (size_t x = 0; x < minX; x++) {
                dst->m_Cells[contentOrigin.y + y][contentOrigin.x + x] = src->m_Cells[y][x];
            }
        }
    }

    // Move the dest frame cursor to not overwrite when printing characters after merging.
    dst->m_Cursor = vec2(origin.x, origin.y + containerHeight);
}

/***********************************************************
*                         ITEMS                            *
***********************************************************/
    
template <typename... Args> void tuim::Print(const std::string& fmt, Args&&... args) {
    std::string str = std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));

    Context* ctx = tuim::GetCtx();
    std::shared_ptr<Frame> frame = tuim::GetCurrentFrame();

    // Retrieve the current active styles from context.
    std::optional<Color> currentForeground = ctx->m_CurrentForeground;
    std::optional<Color> currentBackground = ctx->m_CurrentBackground;
    Style currentStyle = ctx->m_CurrentStyle;

    // Function to apply the current styles and colors to a Cell object.
    auto CopyStylesToCell = [&](std::shared_ptr<Cell>& cell) {
        cell->m_Foreground = currentForeground;
        cell->m_Background = currentBackground;
        cell->m_Style = currentStyle;
    };

    bool escaped = false;

    size_t i = 0;
    while (i < str.length()) {
        char c = str[i];
        char cc = (i+1 < str.length()) ? str[i+1] : '\0';

        if (!escaped) {
            if (c == '#') {
                // Escape the color code by repeating the same character.
                if (cc == '#') {
                    escaped = true;
                    i++;
                    continue;
                }

                // Possible hex color: #rrggbb or #_rrggbb.
                bool isBackground = (i + 1 < str.length() && str[i+1] == '_');
                size_t codeSize = 1 + isBackground + 6;

                // Check if there are enough characters for a hex code (6 characters).
                if (i + codeSize <= str.length()) {
                    std::string_view code(str.data() + i, codeSize);
                    Color newColor = tuim::StringToColor(code);
                    if (isBackground) currentBackground = newColor;
                    else currentForeground = newColor;

                    // Move past the style code.
                    i += codeSize;
                    continue;
                }
            }
            else if (c == '&') {
                // Escape the color code by repeating the same character.
                if (cc == '&') {
                    escaped = true;
                    i++;
                    continue;
                }
                if (i + 1 < str.length()) {
                    char tag = str[i+1];

                    // Check for '&r' first, as it's a special reset tag.
                    if (tag == 'r') {
                        currentForeground = std::nullopt;
                        currentBackground = std::nullopt;
                        currentStyle = Style::NONE;
                        
                        // Consume '&r'.
                        i += 2;
                        continue;
                    }

                    // Check user-defined styles
                    auto styleIt = ctx->m_UserStyles.find(tag);
                    if (styleIt != ctx->m_UserStyles.end()) {
                        currentStyle |= styleIt->second;

                        // Consume '&' + tag character.
                        // Continue to the next part of the string after a successful tag match.
                        i += 2;
                        continue;
                    }
                    
                    // Check user-defined foreground colors
                    auto fgIt = ctx->m_UserForegrounds.find(tag);
                    if (fgIt != ctx->m_UserForegrounds.end()) {
                        currentForeground = fgIt->second;
                        
                        // Consume '&' + tag character.
                        // Continue to the next part of the string after a successful tag match.
                        i += 2;
                        continue;
                    }

                    // Check user-defined background colors
                    auto bgIt = ctx->m_UserBackgrounds.find(tag);
                    if (bgIt != ctx->m_UserBackgrounds.end()) {
                        currentBackground = bgIt->second;
                        
                        // Consume '&' + tag character.
                        // Continue to the next part of the string after a successful tag match.
                        i += 2;
                        continue;
                    }
                }
            }
        }

        escaped = false;

        // Handle regular characters (not style tags)
        uint8_t charLength = tuim::Utf8CharLength(c);
        if (charLength == 0 || i + charLength > str.length()) {
            // Invalid UTF-8 sequence, treat as single byte and advance.
            charLength = 1;
        }
        char32_t ch = tuim::Utf8Decode(str.data() + i, charLength);

        // tab: advance cursor by 4, filling with space cells that have current styles.
        if (ch == '\t') {
            for (int k = 0; k < 4; k++) {
                // Ensure we don't write beyond the terminal width.
                if (frame->m_Cursor.x >= tuim::Terminal::GetTerminalSize().x)
                    break;
                std::shared_ptr<Cell> cell = std::make_shared<Cell>(U' ');
                CopyStylesToCell(cell);
                frame->Set(frame->m_Cursor, cell);
                frame->m_Cursor.x++;
            }
        }
        // new line: move cursor to beginning of next line.
        else if (ch == '\n') {
            frame->m_Cursor.x = 0;
            frame->m_Cursor.y++;
        }
        else {
            // Regular printable character
            // Ensure we don't write beyond the terminal boundaries
            if (frame->m_Cursor.x < tuim::Terminal::GetTerminalSize().x && frame->m_Cursor.y < tuim::Terminal::GetTerminalSize().y) {
                std::shared_ptr<Cell> cell = std::make_shared<Cell>(ch);
                CopyStylesToCell(cell);
                frame->Set(frame->m_Cursor, cell);

                uint8_t width = tuim::Utf8CharWidth(ch);
                frame->m_Cursor.x += width;
            }
        }
        i += charLength; // Move to the next UTF-8 character
    }

    // After processing the entire string, update the context's current styles.
    // This ensures that subsequent Print calls inherit the styles active at the end of this Print.
    ctx->m_CurrentForeground = currentForeground;
    ctx->m_CurrentBackground = currentBackground;
    ctx->m_CurrentStyle = currentStyle;
}

bool tuim::Button(const std::string& id, const std::string& text, tuim::ItemFlags flags) {
    std::shared_ptr<Frame> frame = tuim::GetCurrentFrame();

    // Create a new item and push it to the stack.
    ItemId itemId = tuim::StringToId(id);
    std::shared_ptr<Item> item = std::make_shared<Item>();
    item->m_Id = itemId;
    item->m_Size = vec2(text.size(), 1); // TODO: replace with CalcTextWidth().
    item->m_Pos = frame->m_Cursor;
    item->m_Flags = flags;
    tuim::AddItem(item);

    if(tuim::IsItemHovered()) {
        if(tuim::IsKeyPressed(Key::ENTER)) {
            tuim::SetActiveItemId(item->m_Id);
        }
        tuim::Print("[x] ");
    }
    else tuim::Print("[ ] ");

    tuim::Print(text);

    return tuim::IsItemActive();
}

bool tuim::TextInput(const std::string& id, std::string_view fmt, std::string* value, InputTextFlags flags) {
    static size_t s_Cursor = value->length();
    
    std::shared_ptr<Frame> frame = tuim::GetCurrentFrame();

    // Create a new item and push it to the stack.
    ItemId itemId = tuim::StringToId(id);
    std::shared_ptr<Item> item = std::make_shared<Item>();
    item->m_Id = itemId;
    // item->m_Size = vec2(text.size(), 1); // TODO: replace with CalcTextWidth().
    item->m_Pos = frame->m_Cursor;
    item->m_Flags = ITEM_FLAGS_STAY_ACTIVE;
    tuim::AddItem(item);

    bool hasChanged = false;

    // Handle keyboard inputs to change text, move cursor...
    if(tuim::IsItemActive()) {
        if(tuim::IsKeyPressed(Key::ESCAPE)) {
            tuim::SetActiveItemId(0);
        }
        else if(tuim::IsKeyPressed(Key::LEFT)) {
            s_Cursor = tuim::Utf8CharLastIndex(*value, s_Cursor);
        }
        else if(tuim::IsKeyPressed(Key::RIGHT)) {
            s_Cursor = tuim::Utf8CharNextIndex(*value, s_Cursor);
        }
        else if(tuim::IsKeyPressed(Key::DELETE)) {
            if(s_Cursor < value->length()) {
                size_t nextIndex = tuim::Utf8CharNextIndex(*value, s_Cursor);
                *value = value->erase(s_Cursor, nextIndex - s_Cursor);
                if (!(flags & INPUT_TEXT_FLAGS_CONFIRM_ON_ENTER))
                    hasChanged = true;
            }
        }
        else if(tuim::IsKeyPressed(Key::BACKSPACE)) {
            if(s_Cursor > 0) {
                size_t lastIndex = tuim::Utf8CharLastIndex(*value, s_Cursor);
                *value = value->erase(lastIndex, s_Cursor - lastIndex);
                s_Cursor = lastIndex;
                if (!(flags & INPUT_TEXT_FLAGS_CONFIRM_ON_ENTER))
                    hasChanged = true;
            }
        }
        else {
            char32_t keyCode = ctx->m_PressedKeyCode;
            if(keyCode != 0 && tuim::IsPrintable(keyCode)) {
                std::string ch = tuim::Utf8Char32ToString(keyCode);

                if((flags & INPUT_TEXT_FLAGS_NUMERIC_ONLY && tuim::IsDigit(ch))
                || (flags & INPUT_TEXT_FLAGS_ALPHANUMERIC_ONLY && tuim::IsAlphaNumeric(ch))
                || !(flags & (INPUT_TEXT_FLAGS_NUMERIC_ONLY | INPUT_TEXT_FLAGS_ALPHANUMERIC_ONLY))) {
                    value->insert(s_Cursor, ch);
                    s_Cursor += ch.length();
                    if (!(flags & INPUT_TEXT_FLAGS_CONFIRM_ON_ENTER))
                        hasChanged = true;
                }
            }
        }
    }
        
    if(tuim::IsItemHovered()) {
        if(tuim::IsKeyPressed(Key::ENTER)) {
            if (tuim::IsItemActive()) {
                tuim::SetActiveItemId(0);
                hasChanged = true;
            }
            else {
                tuim::SetActiveItemId(item->m_Id);
            }
        }
        if (tuim::IsItemActive()) tuim::Print("[*] ");
        else tuim::Print("[x] ");
    }
    else tuim::Print("[ ] ");

    
    // Calculate how the text should be displayed with highlighting and escaped formatting.
    std::string displayedValue = "#_555555";
    for (size_t i = 0; i < value->length();) {
        size_t charLength = tuim::Utf8CharLength((*value)[i]);
        std::string_view sv = std::string_view(value->data()+i, charLength);

        // Escape tuim formatting.
        if(tuim::IsItemActive() && sv == "#")
            displayedValue += '#';
        if(tuim::IsItemActive() && sv == "&")
            displayedValue += '&';

        // Add cursor to be displayed.
        if(tuim::IsItemActive() && !tuim::IsKeyPressed() && s_Cursor == i) {
            displayedValue += "#_ffffff#555555";
            displayedValue += sv;
            displayedValue += "&r#_555555";
        }
        else {
            displayedValue += sv;
        }

        i += charLength;
    }

    // Add cursor if it is at the end the text.
    if(tuim::IsItemActive() && !tuim::IsKeyPressed() && s_Cursor >= value->length())
        displayedValue += "#_ffffff ";

    // Reset any styles after the input text.
    displayedValue += "&r";

    std::string text = std::vformat(fmt, std::make_format_args(displayedValue));
    item->m_Size = vec2(text.size(), 1); // TODO: replace with CalcTextWidth().

    // Display the actual input text.
    tuim::Print(text);

    return hasChanged;
}

bool tuim::IntSlider(const std::string& id, std::string_view fmt, int* value, int min, int max, int step, int width) {
    std::shared_ptr<Frame> frame = tuim::GetCurrentFrame();

    // Create a new item and push it to the stack.
    ItemId itemId = tuim::StringToId(id);
    std::shared_ptr<Item> item = std::make_shared<Item>();
    item->m_Id = itemId;
    item->m_Pos = frame->m_Cursor;
    item->m_Flags = ITEM_FLAGS_NONE;
    tuim::AddItem(item);

    bool hasChanged = false;
        
    if(tuim::IsItemHovered()) {
        if(tuim::IsKeyPressed(Key::LEFT)) {
            *value = std::max(min, *value - step);
            hasChanged = true;
        }
        else if(tuim::IsKeyPressed(Key::RIGHT)) {
            *value = std::min(max, *value + step);
            hasChanged = true;
        }
        else if(tuim::IsKeyPressed(Key::ENTER)) {
            if (tuim::IsItemActive()) {
                tuim::SetActiveItemId(0);
                hasChanged = true;
            }
            else {
                tuim::SetActiveItemId(item->m_Id);
            }
        }
        if (tuim::IsItemActive()) tuim::Print("[*] ");
        else tuim::Print("[x] ");
    }
    else tuim::Print("[ ] ");

    std::string text = "";
    text += "#555555";
    float prct = ((float) (*value - min) / (float) (max-min))*width;
    for(int t = 0; t < width; t++) {
        if(t >= prct) text += "&r";
        text += "█";
    }
    text += "&r";

    // TODO: use a static variable to change the value manually.
    // if(tuim::IsItemHovered() && tuim::IsKeyPressed(Key::ENTER)) {}

    text = std::vformat(fmt, std::make_format_args(text, *value));
    item->m_Size = vec2(text.size(), 1); // TODO: replace with CalcTextWidth().
    
    tuim::Print(text);

    return hasChanged;
}

bool tuim::FloatSlider(const std::string& id, std::string_view fmt, float* value, float min, float max, float step, int width) {
    std::shared_ptr<Frame> frame = tuim::GetCurrentFrame();

    // Create a new item and push it to the stack.
    ItemId itemId = tuim::StringToId(id);
    std::shared_ptr<Item> item = std::make_shared<Item>();
    item->m_Id = itemId;
    item->m_Pos = frame->m_Cursor;
    item->m_Flags = ITEM_FLAGS_NONE;
    tuim::AddItem(item);

    bool hasChanged = false;
        
    if(tuim::IsItemHovered()) {
        if(tuim::IsKeyPressed(Key::LEFT)) {
            *value = std::max(min, *value - step);
            hasChanged = true;
        }
        else if(tuim::IsKeyPressed(Key::RIGHT)) {
            *value = std::min(max, *value + step);
            hasChanged = true;
        }
        else if(tuim::IsKeyPressed(Key::ENTER)) {
            if (tuim::IsItemActive()) {
                tuim::SetActiveItemId(0);
                hasChanged = true;
            }
            else {
                tuim::SetActiveItemId(item->m_Id);
            }
        }
        if (tuim::IsItemActive()) tuim::Print("[*] ");
        else tuim::Print("[x] ");
    }
    else tuim::Print("[ ] ");

    std::string text = "";
    text += "#555555";
    float prct = ((float) (*value - min) / (float) (max-min))*width;
    for(int t = 0; t < width; t++) {
        if(t >= prct) text += "&r";
        text += "█";
    }
    text += "&r";

    // TODO: use a static variable to change the value manually.
    // if(tuim::IsItemHovered() && tuim::IsKeyPressed(Key::ENTER)) {}

    text = std::vformat(fmt, std::make_format_args(text, *value));
    item->m_Size = vec2(text.size(), 1); // TODO: replace with CalcTextWidth().
    
    tuim::Print(text);

    return hasChanged;
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
    }
    else if (ch <= 0x7FF) {
        str += static_cast<char>(0xC0 | (ch >> 6));
        str += static_cast<char>(0x80 | (ch & 0x3F));
    }
    else if (ch <= 0xFFFF) {
        str += static_cast<char>(0xE0 | (ch >> 12));
        str += static_cast<char>(0x80 | ((ch >> 6) & 0x3F));
        str += static_cast<char>(0x80 | (ch & 0x3F));
    }
    else if (ch <= 0x10FFFF) {
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
        }
        else {
            ch = lead & ((1 << (7 - length)) - 1); // Mask initial bits.
            for (size_t i = 1; i < length; ++i) {
                char8_t cont = static_cast<char8_t>(sv[index + i]);
                if ((cont & UTF8_CONT_MASK) != UTF8_CONT_TAG) {
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

bool tuim::IsPrintable(char32_t ch) {
    if (std::iswcntrl(ch))
        return false;
    // Check if high-order byte is ESC (0x1B)
    if (((ch >> 16) & 0xFF) == 0x1B)
        return false;
    return true;
}

size_t tuim::Utf8CharLastIndex(std::string_view sv, size_t index) {
    if (sv.empty() || index == 0)
        return 0;
    do {
        index--;
    } while (index > 0 && (static_cast<unsigned char>(sv[index]) & UTF8_CONT_MASK) == UTF8_CONT_TAG);
    return index;
}

size_t tuim::Utf8CharNextIndex(std::string_view sv, size_t index) {
    const size_t n = sv.size();
    if (n == 0 || index >= n)
        return n;
    do {
        index++;
    } while (index < n && (static_cast<unsigned char>(sv[index]) & UTF8_CONT_MASK) == UTF8_CONT_TAG);
    return index;
}

bool tuim::IsDigit(const std::string& str) {
    auto it = str.begin();
    while (it != str.end() && std::isdigit(*it)) ++it;
    return !str.empty() && it == str.end();
}

bool tuim::IsAlphaNumeric(const std::string& str) {
    auto it = str.begin();
    while (it != str.end() && (std::isdigit(*it)
        || ('a' < (*it) && (*it) < 'z')
        || ('A' < (*it) && (*it) < 'Z')
    )) it++;
    return !str.empty() && it == str.end();
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

void tuim::SetCurrentCursor(const tuim::vec2& cursor) {
    tuim::GetCurrentFrame()->m_Cursor = cursor;
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