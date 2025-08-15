#include "../tuim.hpp"
#include "backward/signal_handler.hpp"

SignalHandler signalHandler;

int main(int argc, char* argv[]) {    
    // Setup tuim settings.
    tuim::CreateContext(argc, argv);
    tuim::SetTitle("tuim - inputs");
    tuim::SetFramerate(1.f);

    tuim::DefineStyle('b', tuim::Style::BOLD);        // &b for bold
    tuim::DefineStyle('U', tuim::Style::UNDERLINE);  // &U for underline
    tuim::DefineStyle('i', tuim::Style::ITALIC);     // &i for italic

    std::string str = "";
    std::string input1 = "xorrade";
    int i = 10;
    float f = 5.f;

    char32_t keyCode = 0;
    while (keyCode != tuim::Key::F1) {
        keyCode = tuim::PollKeyCode();
        tuim::Update(keyCode);
        tuim::Clear();
        
        tuim::BeginContainer("#container1", "", tuim::vec2(100, 100));

        if (keyCode != 0 && !tuim::IsKeyPressed(tuim::Key::LEFT) && !tuim::IsKeyPressed(tuim::Key::RIGHT) && !tuim::IsKeyPressed(tuim::Key::UP) && !tuim::IsKeyPressed(tuim::Key::DOWN) && !tuim::IsKeyPressed(tuim::Key::ENTER) && !tuim::IsKeyPressed(tuim::Key::ESCAPE) && !tuim::IsKeyPressed(tuim::Key::BACKSPACE))
            str += tuim::Utf8Char32ToString(keyCode);
        tuim::Print("{}\n\n", str);

        tuim::Print("&bThis has a blue background.&r\n");
        tuim::Print("&&This has a blue background.&r\n");
        tuim::Print("#_0000ffThis has a blue background.&r\n");
        tuim::Print("##This has a blue background.&r\n");

        if (tuim::TextInput("#input1", "Username: {}", &input1, tuim::INPUT_TEXT_FLAGS_CONFIRM_ON_ENTER | tuim::INPUT_TEXT_FLAGS_NUMERIC_ONLY)) {
            tuim::Print(" Changed");
        }
        tuim::Print("\n");
        for (int i = 0; i < input1.size(); i++)
            tuim::Print("{:#04x}\t{:#04x}\n", input1[i], str[i]);
        
        if (tuim::IntSlider("#input2", "Int: {} {}", &i, 0, 50, 1, 50)) {
            tuim::Print(" Changed");
        }
        tuim::Print("\n");
        
        if (tuim::FloatSlider("#input3", "Float: {} {:.2f}", &f, 0.f, 5.f, 0.01f, 50)) {
            tuim::Print(" Changed");
        }
        tuim::Print("\n");

        tuim::EndContainer();

        tuim::Display();
    }

    // Delete the context to avoid memory leaks.
    tuim::DeleteContext();

    return 0;
}