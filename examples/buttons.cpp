#include "../tuim.hpp"

#include <math.h>
#include <time.h>

int main(int argc, char* argv[])
{    
    tuim::init(argc, argv);
    tuim::set_title("tuim demo");

    keypad(stdscr, true);
    noecho();

    tuim::keyboard::key key;
    int a = 0;

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

        if(tuim::button("#b1", "test1\n"))
            tuim::text("#t1", "\tpressed\n");

        if(tuim::button("#b3", "exit\n"))
            break;
        
        refresh();

        // usleep(1000 * 100);

    } while(key != 27);

    tuim::delete_context();

    return 0;
}