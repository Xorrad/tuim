#include "../tuim.hpp"
#include "backward/signal_handler.hpp"

SignalHandler signalHandler;

int main(int argc, char* argv[]) {    
    // Setup tuim settings.
    tuim::CreateContext(argc, argv);
    tuim::SetTitle("tuim - texts");
    tuim::SetFramerate(1.f);

    tuim::DefineColor('c', tuim::Color(136, 136, 136));

    std::string text = "Tyrion was born in 273 AC as the third-born child to\n Lord Tywin and Lady Joanna Lannister. Joanna died giving birth to him, and as a result his father blames him for her death.&c[29]&r Tales about Tyrion were spread immediately after his birth; At Oldtown, it was said that Tyrion was a monster, with a tail, monstrous huge head, thick black hair, a beard, an evil eye, lion's claws, with teeth so long he was not able to close his mouth, and both male and female genitals.&c[7]&r It was said that Tyrion's birth was an omen to famine, plague and war,&c[7]&r and the smallfolk named him \"Lord Tywin\'s Doom\" and \"Lord Tywin\'s Bane\".&c[8]&r King Aerys II Targaryen stated that Tyrion was a punishment for Tywin's arrogance,&c[8]&r and the same was said by begging brothers in Oldtown, stating that Tywin had made himself greater than the king, which was only allowed by a god.&c[7]&r\n";
    int paragraphWidth = 40;

    char32_t keyCode = 0;
    while (keyCode != tuim::Key::F1) {
        keyCode = tuim::PollKeyCode();
        tuim::Update(keyCode);
        tuim::Clear();

        if (tuim::IsKeyPressed(tuim::Key::LEFT))
            paragraphWidth = std::max(0, paragraphWidth - 1);
        else if (tuim::IsKeyPressed(tuim::Key::RIGHT))
            paragraphWidth++;

        tuim::vec2 size = tuim::Terminal::GetTerminalSize();
        tuim::Print("terminal size: {} {}\n", size.x, size.y);

        tuim::BeginContainer("#container", "paragraph", tuim::vec2(paragraphWidth+2, 35));
        tuim::Paragraph("#paragraph", text, paragraphWidth);
        tuim::EndContainer();
        tuim::Print("NB: Press arrows to change paragraph width.\n");

        tuim::Display();
    }

    // Delete the context to avoid memory leaks.
    tuim::DeleteContext();

    return 0;
}