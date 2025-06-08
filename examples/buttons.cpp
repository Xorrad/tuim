#include "../tuim.hpp"
#include "backward/signal_handler.hpp"

SignalHandler signalHandler;

int main(int argc, char* argv[]) {    
    // Setup tuim settings.
    tuim::CreateContext(argc, argv);
    tuim::SetTitle("tuim - buttons");
    tuim::SetFramerate(1.f);

    tuim::DefineStyle('b', tuim::Style::BOLD);        // &b for bold
    tuim::DefineStyle('U', tuim::Style::UNDERLINE);  // &U for underline
    tuim::DefineStyle('i', tuim::Style::ITALIC);     // &i for italic

    char32_t keyCode = 0;
    while (keyCode != tuim::Key::F1) {
        keyCode = tuim::PollKeyCode();
        tuim::Update(keyCode);
        tuim::Clear();

        if (tuim::Button("#b1", "Click Here!")) {
            tuim::Print(" Clicked");
        }
        tuim::Print("\n");
        
        tuim::BeginContainer("#container1", "", tuim::vec2(50, 10));
        if (tuim::Button("#b2", "Click Here!")) {
            tuim::Print(" Clicked too");
        }
        tuim::EndContainer();

        tuim::Display();
    }

    // Delete the context to avoid memory leaks.
    tuim::DeleteContext();

    return 0;
}