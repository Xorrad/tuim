#include "../tuim.hpp"
#include "backward/signal_handler.hpp"

SignalHandler signalHandler;

int main(int argc, char* argv[]) {    
    // Setup tuim settings.
    tuim::CreateContext(argc, argv);
    tuim::SetTitle("tuim - text");

    while (true) {
        tuim::Clear();

        tuim::vec2 size = tuim::Terminal::GetTerminalSize();
        tuim::Print("line 1\t\tline1\tline1\nline 2\nline 3");
        // tuim::Print("terminal size: {}{}", size.x, size.y);

        tuim::Display();

        // usleep(200 * 1000);
        std::cin.get();
    }

    // Delete the context to avoid memory leaks.
    tuim::DeleteContext();

    return 0;
}