#include "../tuim.hpp"
#include "backward/signal_handler.hpp"

SignalHandler signalHandler;

int main(int argc, char* argv[]) {    
    
    // Setup tuim settings.
    tuim::CreateContext(argc, argv);
    tuim::SetTitle("tuim - inputs");
    tuim::SetFramerate(1.f);

    // Define global variables for each input.
    std::string str1 = "abcdefg1234$*ù€_-{{}}[]";
    std::string str2 = "12345";
    std::string str3 = "abcdefg1234";
    int number1 = 10;
    float number2 = 5.f;

    // Start the main loop.
    char32_t keyCode = 0;
    while (keyCode != tuim::Key::F1) {
        keyCode = tuim::PollKeyCode();
        tuim::Update(keyCode);
        tuim::Clear();

        // Display a text input that accepts any characters and return true when enter is pressed.
        if (tuim::TextInput("#input1", "Text: {}", &str1, tuim::INPUT_TEXT_FLAGS_CONFIRM_ON_ENTER)) {
            tuim::Print(" Changed");
        }
        tuim::Print("\n");
        
        // Display a text input that only accepts digits and return true when enter is pressed.
        if (tuim::TextInput("#input2", "Only digits: {}", &str2, tuim::INPUT_TEXT_FLAGS_CONFIRM_ON_ENTER | tuim::INPUT_TEXT_FLAGS_NUMERIC_ONLY)) {
            tuim::Print(" Changed");
        }
        tuim::Print("\n");
        
        // Display a text input that only accepts digits and letters and return true when enter is pressed.
        if (tuim::TextInput("#input3", "Only digits and letters: {}", &str3, tuim::INPUT_TEXT_FLAGS_CONFIRM_ON_ENTER | tuim::INPUT_TEXT_FLAGS_ALPHANUMERIC_ONLY)) {
            tuim::Print(" Changed");
        }
        tuim::Print("\n");
        
        // Display a integer slider with min, max, step and width (of the progress bar) parameters.
        if (tuim::IntSlider("#input4", "Int: {} {}", &number1, 0, 50, 1, 50)) {
            tuim::Print(" Changed");
        }
        tuim::Print("\n");
        
        // Display a decimal slider with min, max, step and width (of the progress bar) parameters.
        if (tuim::FloatSlider("#input5", "Float: {} {:.2f}", &number2, 0.f, 5.f, 0.01f, 50)) {
            tuim::Print(" Changed");
        }
        tuim::Print("\n");

        tuim::Display();
    }

    // Delete the context to avoid memory leaks.
    tuim::DeleteContext();

    return 0;
}