#include "../tuim.hpp"

#include <math.h>
#include <time.h>

int main(int argc, char* argv[])
{    
    tuim::init(argc, argv);
    tuim::set_title("tuim demo");

    tuim::keyboard::key key;
    int a = 0;

    do {
        key = tuim::keyboard::NONE;
        if(a < 2) {
            a++;
        } else {
            key = tuim::keyboard::get_pressed();
            a = 1;
        }

        tuim::clear();
        tuim::update(key);

        tuim::button("#b0", "&_c&atest2&r");
        tuim::button("#b0.1", "&_c&atest2.1&r\n");
        tuim::text("#t4", "hello world\n");

        if(tuim::button("#b1", "&_c&6test1&r\n")) {
            tuim::text("#t1", "\tpressed\n");
        }

        tuim::text("#t3", "\nhovered :" + std::to_string(tuim::ctx->hovered_id) + "\n\n");

        tuim::button("#b2", "&_c&atest2&r\n");
        if(tuim::button("#b3", "&_c&5exit&r\n"))
            break;
        
        refresh();

        usleep(1000 * 100);

    } while(key != tuim::keyboard::ESCAPE);

    tuim::delete_context();

    return 0;
}