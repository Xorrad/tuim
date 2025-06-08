#include "../tuim.hpp"
#include "backward/signal_handler.hpp"

SignalHandler signalHandler;

int main(int argc, char* argv[]) {    
    // Setup tuim settings.
    tuim::CreateContext(argc, argv);
    tuim::SetTitle("tuim - dev");
    tuim::SetFramerate(1.f);

    tuim::DefineStyle('b', tuim::Style::BOLD);        // &b for bold
    tuim::DefineForeground('g', tuim::Color(0, 255, 0));     // &g for green foreground
    tuim::DefineBackground('u', tuim::Color(0, 0, 128));     // &u for dark blue background (underline was 'u', changed to 'U' for example)
    tuim::DefineStyle('U', tuim::Style::UNDERLINE);  // &U for underline
    tuim::DefineStyle('i', tuim::Style::ITALIC);     // &i for italic
    tuim::DefineStyle('l', tuim::Style::BLINKING);   // &l for blinking

    std::string str = "";
    int containerWidth = 40;
    int containerHeight = 36;

    char32_t keyCode = 0;
    while (keyCode != tuim::Key::F1) {
        keyCode = tuim::PollKeyCode();
        tuim::Update(keyCode);
        tuim::Clear();

        if (tuim::IsKeyPressed(tuim::Key::LEFT))
            containerWidth = std::max(0, containerWidth - 1);
        else if (tuim::IsKeyPressed(tuim::Key::RIGHT))
            containerWidth++;
        else if (tuim::IsKeyPressed(tuim::Key::UP))
            containerHeight = std::max(0, containerHeight - 1);
        else if (tuim::IsKeyPressed(tuim::Key::DOWN))
            containerHeight++;
        else if (keyCode != 0)
            str += tuim::Utf8Char32ToString(keyCode);

        // std::cout << std::hex << (uint32_t) keyCode << "\t" << tuim::Utf8Char32ToString(keyCode) << std::endl;
        // std::cout << std::hex << str.data() << std::endl;

        tuim::vec2 size = tuim::Terminal::GetTerminalSize();

        tuim::Print("terminal size: {} {}\n", size.x, size.y);
        tuim::Print("container width: {}\n", containerWidth);
        tuim::Print("container height: {}\n", containerHeight);

        tuim::Button("#b1", "Click Here!");

        tuim::BeginContainer("#container1", "", tuim::vec2(containerWidth, containerHeight));

        // tuim::Print("{}\n", (uint32_t) keyCode);
        tuim::Print("{}\n", str);
        // for (int i = 0; i < size.y-20; i++) {
        //     size_t n = rand() % 140;
        //     tuim::Print("{}\t", std::string(n, '-'));
        //     tuim::Print("{} {}\n", i, n);
        // }

        tuim::Print("#ff0000This is red text.&r\n");
        tuim::Print("Hello, &bWorld!&r\n");
        tuim::Print("&bMy text is bold.&r\n");
        tuim::Print("#_0000ffThis has a blue background.&r\n");
        tuim::Print("&gThis is green text from a defined tag.&r\n");
        tuim::Print("&uThis is dark blue background from a defined tag.&r\n");
        tuim::Print("&b&gThis is bold green text.&r\n");
        tuim::Print("&UThis text is underlined.&r\n");
        tuim::Print("&iThis text is italic!&r\n");
        tuim::Print("&lThis text is blinking!&r\n");
        tuim::Print("#FFFF00&b&U&l#_FF00FFAll &rstyles combined!&r\n"); // Yellow fg, bold, uline, magenta bg
        tuim::Print("Normal text again.\n");

        tuim::Print("line 1\t\tline1\tline1\nline 2\nline 3\n");
        tuim::Print("你\t好\n");
        tuim::Print("😃\t😃\n");

        std::string_view colorSv = "#_ff0000";
        tuim::Color color = tuim::StringToColor(colorSv);
        tuim::Print("color: {}-> ({},{},{}) {}&r\n", colorSv, color.r, color.g, color.b, color.bg);

        tuim::vec2 container2Pos = tuim::GetCurrentCursor();
        container2Pos.x += 10;
        tuim::BeginContainer("#container2", "", tuim::vec2(4, 5));
        tuim::Print("1\n");
        tuim::Print("2\n");
        tuim::Print("3\n");
        tuim::Print("4\n");
        tuim::Print("5\n");
        tuim::EndContainer();

        tuim::SetCurrentCursor(container2Pos);
        tuim::BeginContainer("#container3", "", tuim::vec2(20, 7));
        tuim::Print("text inside the container 3\n");
        tuim::EndContainer();

        tuim::EndContainer();

        tuim::SetCurrentCursor(tuim::vec2(0, 35));
        tuim::Print("35\n");
        tuim::Print("36\n");
        tuim::Print("37\n");
        tuim::Print("38\n");

        tuim::Display();

        // usleep(200 * 1000);
    }

    // Delete the context to avoid memory leaks.
    tuim::DeleteContext();

    return 0;
}