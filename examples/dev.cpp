#include "../tuim.hpp"
#include "backward/signal_handler.hpp"

SignalHandler signalHandler;

int main(int argc, char* argv[]) {    
    // Setup tuim settings.
    tuim::CreateContext(argc, argv);
    tuim::SetTitle("tuim - dev");
    tuim::SetFramerate(1.f);

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

        tuim::BeginContainer("#container1", "", tuim::vec2(containerWidth, containerHeight));

        // tuim::Print("{}\n", (uint32_t) keyCode);
        tuim::Print("{}\n", str);
        for (int i = 0; i < size.y-20; i++) {
            size_t n = rand() % 140;
            tuim::Print("{}\t", std::string(n, '-'));
            tuim::Print("{} {}\n", i, n);
        }
        tuim::Print("line 1\t\tline1\tline1\nline 2\nline 3\n");
        tuim::Print("你\t好\n");
        tuim::Print("😃\t😃\n");
        tuim::Print("terminal size: {} {}\n", size.x, size.y);
        tuim::Print("container width: {}\n", containerWidth);
        tuim::Print("container height: {}\n", containerHeight);

        std::string_view colorSv = "#_ff00ff";
        tuim::Color color = tuim::StringToColor(colorSv);
        tuim::Print("color:  {} -> ({},{},{}) {}\n", colorSv, color.r, color.g, color.b, color.bg);

        tuim::vec2 container2Pos = tuim::GetCurrentCursor();
        container2Pos.x += 10;
        tuim::BeginContainer("#container2", "", tuim::vec2(4, 5));
        tuim::Print("text inside the container 2\n");
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