#include "../tuim.hpp"

#include <math.h>
#include <time.h>

int main(int argc, char* argv[])
{    
    tuim::init(argc, argv);
    tuim::set_title("tuim demo");

    tuim::set_cursor_visible(false);

    tuim::keyboard::key key;
    int a = 0;
    bool t = false;

    do {
        key = tuim::keyboard::NONE;
        if(a < 2) {
            a++;
        } else {
            // key = getch();
            key = tuim::keyboard::get_pressed();
            a = 1;
        }

        tuim::clear();
        
        tuim::update(key);

        tuim::button("#b0", "test2\n");
        tuim::button("#b0.1", "test2.1\n");

        tuim::text("#t4", "\nhello world\n");
        tuim::text("#t3", "hovered :" + std::to_string(tuim::ctx->hovered_id) + "\n\n");

        if(tuim::button("#b1", "settings\n"))
            t = !t;
        if(t) {
            tuim::print(" "); tuim::button("#b1.1", "sounds\n");
            tuim::print(" "); tuim::button("#b1.2", "keybinds\n");
        }

        if(tuim::button("#b3", "exit\n"))
            break;

        // usleep(1000 * 100);

    } while(key != tuim::keyboard::ESCAPE);

    tuim::delete_context();

    return 0;
}