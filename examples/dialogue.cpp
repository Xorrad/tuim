#include "../tuim.hpp"
#include "backward/signal_handler.hpp"

SignalHandler signalHandler;

const std::vector<std::string> portrait1 = {
    "       ███▓▓██▓▓█▓██                 ",
    "     ████████████▓▓▓█▓▓              ",
    "     ██████████▒▒▓█▓▓▓█              ",
    "     █████▓▒▒░░░▒▒▒▓▓▓█              ",
    "     ██▓▓▒░░░░▒▒▒▒▒▓▓█▓▓             ",
    "     ██▓▓▓▒▒▒▒▒▒▒▒▒▓▓▓███            ",
    "      ██▒▓▓▒▓▒▓█▓██▓▓▓███            ",
    "      █████▓▓████▓▒▒▒▓███            ",
    "       ████▓▒▒▒▒▓▒▒▒▓▓██             ",
    "        ███▓▒▒▒▒▒▒▓▓███▓█            ",
    "        ████▓▓▓▒▒▓▓▓▓▓▓▓█            ",
    "         █████▓█▓▓▓▓█▓██▒▒           ",
    "          ███████▓▓▓██▒▒▓███         ",
    "            █▒▓▓▓▓██▒▒▒██████        ",
    "             █████▒▒▒▓█████████      ",
    "            ███▓▓▒▒▒██████████████   ",
    "           ████▓▓▒▓██████████████████",
    "          ███████████████████████████",
};

const std::vector<std::string> portrait2 = {
    "                 ██▓▓▒▒▓▒░   █                    ",
    "                 ▓░█▓ ▓▓▒▒▓████░                  ",
    "                 ▒ ▒█░▒▓█████ █                   ",
    "                 ██████████████░                  ",
    "                  ▒         ▓██████████           ",
    "             ░▒█▒████▓███████░█  ████             ",
    "          ▓▒▓███▓ ▓██████▒ ██▓███▒█               ",
    "             ▓█░█  ░▓█ ░    █ ▒   █▓              ",
    "              ▓█▓██▒    ████ ▒  ▒                 ",
    "               ░▓█   ░█ ██  ▒  ▓                  ",
    "               ▓▓ █ █ ████████  █                 ",
    "               ▒ █ ░█▓░ ▓▒    █  █  ░             ",
    "              ░█ █▒▒░ ▒░       ░     █░           ",
    "             ▓ █ █                    █▒          ",
    "         ░ ░▒█▓█  ██░  ██▒██▓▓░  █▒ █ ███░        ",
    "        ▓░█░▓█ ███  ███▓▓█▓█░▓▒█  ▓█ ███████░     ",
    "   ░ ▓ ██▓█████▓█████         ░▒██▓▓█░█▒█▓▓██████ ",
    " ░▓▒█████▓██ ▒█  █████████████████ ███▓██████████ ",
    "▓███▓█░▓█▓▓█████    ████▒▒████    ████▓█▓▓██▓████ ",
    "█████▓███▓▓▓████▓      ████   ▒░ █░█░██▓█▓████████",
    "██████▓▓▓███▓▓ █▓▒   ██▓░░██    ▓█▓██ ███████████ ",
};

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

    char32_t keyCode = 0;
    while (keyCode != tuim::Key::F1) {
        keyCode = tuim::PollKeyCode();
        tuim::Update(keyCode);
        tuim::Clear();

        tuim::vec2 size = tuim::Terminal::GetTerminalSize();
        tuim::Print("terminal size: {} {}\n", size.x, size.y);
        
        tuim::Image("#portrait", portrait2, tuim::IMAGE_FLAGS_CLICKABLE);

        tuim::BeginContainer("#container", "", tuim::vec2(100, 10));
        tuim::Paragraph("#p1", "You know, Vito, when the cops found the truck, they didn’t ask why it was full of frozen lobsters… and I think that’s beautiful.\n\n", 100);
        tuim::Button("#b1", "Beautiful? Sal, that’s a miracle — and in our line of work, miracles come with invoices.\n");
        tuim::Button("#b2", "Yeah, but next time, maybe don’t hide the diamonds inside the lobsters.\n");
        tuim::Button("#b3", "That’s ’cause the cops know better than to ask questions they don’t want the answers to.\n");
        tuim::EndContainer();

        tuim::Display();
    }

    // Delete the context to avoid memory leaks.
    tuim::DeleteContext();

    return 0;
}