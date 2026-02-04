#include "../tuim.hpp"

int main(int argc, char* argv[]) {    
    // Setup tuim settings.
    tuim::CreateContext(argc, argv);
    tuim::SetTitle("tuim - dev");
    tuim::SetFramerate(1.f);

    tuim::DefineStyle('b', tuim::Style::BOLD);        // &b for bold
    tuim::DefineColor('g', tuim::Color(0, 255, 0));     // &g for green foreground
    tuim::DefineColor('u', tuim::Color(0, 0, 128));     // &u for dark blue background (underline was 'u', changed to 'U' for example)
    tuim::DefineStyle('U', tuim::Style::UNDERLINE);  // &U for underline
    tuim::DefineStyle('i', tuim::Style::ITALIC);     // &i for italic
    tuim::DefineStyle('l', tuim::Style::BLINKING);   // &l for blinking

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
        
        tuim::vec2 size = tuim::Terminal::GetTerminalSize();
        tuim::Print("terminal size: {} {}\n", size.x, size.y);
        tuim::Print("container width: {}\n", containerWidth);
        tuim::Print("container height: {}\n", containerHeight);

        tuim::Button("#b1", "Click Here!");

        tuim::Print("\n");

        tuim::BeginContainer("#container1", "", tuim::vec2(size.x, size.y-4), tuim::CONTAINER_FLAGS_NONE, tuim::ALIGN_CENTER | tuim::ALIGN_MIDDLE);
        tuim::BeginContainer("#container2", "", tuim::vec2(containerWidth, containerHeight));
        
        tuim::Print("{}abcd\n", std::string(containerWidth-6, ' '));
        for (int i = 0; i < containerHeight-2; i++) {
            size_t n = rand() % std::max(1, containerWidth-10);
            tuim::Print("{}\t", std::string(n, '-'));
            tuim::Print("{} {}\n", i, n);
        }

        tuim::EndContainer();
        tuim::EndContainer();

        tuim::SetCurrentCursor(tuim::vec2(0, 35));
        tuim::Print("35\n");
        tuim::Print("36\n");
        tuim::Print("37\n");
        tuim::Print("38\n");
        tuim::SetCurrentCursor(tuim::vec2(size.x-2, size.y-1));
        tuim::Print("x");

        tuim::Display();
    }

    // Delete the context to avoid memory leaks.
    tuim::DeleteContext();

    return 0;
}