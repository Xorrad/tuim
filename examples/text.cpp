#include "../tuim.hpp"

int main(int argc, char* argv[]) {    
    // Setup tuim settings.
    tuim::CreateContext(argc, argv);
    tuim::SetTitle("tuim - text");
    tuim::SetFramerate(60.f);

    bool isRunning = true;

    while (isRunning) {
        // Wait for a key or the framerate timeout to update the frame.
        tuim::Key key = tuim::GetPressedKey();
        tuim::Update(key);

        // Exit if key F1 is pressed.
        if (key == tuim::Key::F1) {
            isRunning = false;
            break;
        }

        // Clear the buffer to begin a new frame.
        tuim::Clear();

        tuim::Text("normal\n");
        tuim::Text("&bbold&r\n");
        tuim::Text("&ddim&r\n");
        tuim::Text("&iitalic&r\n");
        tuim::Text("&uunderline&r\n");
        tuim::Text("&gblinking&r\n");
        tuim::Text("&ninverse&r\n");
        tuim::Text("&hhidden&r\n");
        tuim::Text("&sstrikethrough&r\n");
        tuim::Text("#ff0000red&r\n");
        tuim::Text("#_ff0000background red&r\n\n");

        // Draw the current frame to the terminal.
        tuim::Display();
    }

    // Delete the context to avoid memory leaks.
    tuim::DeleteContext();

    return 0;
}