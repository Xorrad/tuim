#include "../tuim.hpp"
#include "backward/signal_handler.hpp"

SignalHandler signalHandler;

int main(int argc, char* argv[]) {    
    // Setup tuim settings.
    tuim::CreateContext(argc, argv);
    tuim::SetTitle("tuim - text");
    tuim::SetFramerate(30.f);

    std::string str = "";

    char32_t keyCode = 0;
    while (keyCode != tuim::Key::F1) {
        keyCode = tuim::PollKeyCode();
        tuim::Update(keyCode);
        tuim::Clear();

        if (keyCode != 0)
            str += tuim::Utf8Char32ToString(keyCode);

        // std::cout << std::hex << (uint32_t) keyCode << "\t" << tuim::Utf8Char32ToString(keyCode) << std::endl;

        tuim::vec2 size = tuim::Terminal::GetTerminalSize();
        tuim::Print("{}\n", (uint32_t) keyCode);
        tuim::Print("{}\n", str);
        tuim::Print("line 1\t\tline1\tline1\nline 2\nline 3");
        tuim::Print("\n你好");
        tuim::Print("terminal size: {} {}", size.x, size.y);

        tuim::Display();

        // usleep(200 * 1000);
    }

    // Delete the context to avoid memory leaks.
    tuim::DeleteContext();

    return 0;
}