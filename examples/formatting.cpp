#include "../tuim.hpp"

int main(int argc, char* argv[]) {    
    // Setup tuim settings.
    tuim::CreateContext(argc, argv);
    tuim::SetTitle("tuim - text");
    tuim::SetFramerate(60.f);

    // Define custom style tags.
    tuim::DefineStyle('b', tuim::Style::BOLD);
    tuim::DefineStyle('d', tuim::Style::FAINT);
    tuim::DefineStyle('i', tuim::Style::ITALIC);
    tuim::DefineStyle('u', tuim::Style::UNDERLINE);
    tuim::DefineStyle('g', tuim::Style::BLINKING);
    tuim::DefineStyle('n', tuim::Style::REVERSE);
    tuim::DefineStyle('h', tuim::Style::HIDDEN);
    tuim::DefineStyle('s', tuim::Style::STRIKETHROUGH);
    tuim::DefineColor('a', tuim::Color(0, 255, 0));

    // Start the main loop.
    char32_t keyCode = 0;
    while (keyCode != tuim::Key::F1) {
        keyCode = tuim::PollKeyCode();
        tuim::Update(keyCode);
        tuim::Clear();

        tuim::Print("normal\n");
        tuim::Print("&bbold&r\n");
        tuim::Print("&ddim&r\n");
        tuim::Print("&iitalic&r\n");
        tuim::Print("&uunderline&r\n");
        tuim::Print("&gblinking&r\n");
        tuim::Print("&ninverse&r\n");
        tuim::Print("&hhidden&r\n");
        tuim::Print("&sstrikethrough&r\n");
        tuim::Print("#ff0000red foreground&r\n");
        tuim::Print("#_ff0000red background&r\n");
        tuim::Print("#_ff0000#ff0000 red background and foreground&r\n");
        tuim::Print("&a&_agreen background and foreground&r\n");
        tuim::Print("&a&_agreen background and &r#ff0000red foreground&r\n");
        tuim::Print("&&a&&_aescaped tags and ##ff0000colors\n");

        tuim::Display();
    }

    // Delete the context to avoid memory leaks.
    tuim::DeleteContext();

    return 0;
}