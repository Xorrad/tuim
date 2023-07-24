#include "tuim.hpp"

#include <math.h>
#include <time.h>

int main(int argc, char* argv[])
{    
    tuim::init(argc, argv);
    tuim::set_title("tuim demo");

    tuim::keyboard::key key;
    int a = 0;

    int table_cursor = 0;
    std::vector<std::string> columns = { "id", "name", "money" };
    std::vector<std::vector<std::string>> rows = {
        { "0", "a", "10€" },
        { "1", "b", "0€" },
        { "2", "c", "20€" },
        { "3", "d", "50€" },
    };

    do {
        key = tuim::keyboard::NONE;
        if(a < 2) {
            a++;
        } else {
            key = tuim::keyboard::get_pressed();
            a = 1;
        }
        /*if(tuim::keyboard::is_pressed()) {
            key = tuim::keyboard::get_pressed();
        }*/

        tuim::clear();
        tuim::update(key);

        tuim::text("#t2", "cctim\n");
        tuim::button("#b0", "&_c&atest2&r\n", NULL);
        tuim::text("#t4", "hello world\n");

        if(tuim::button("#b1", "&_c&6test1&r\n", NULL)) {
            tuim::text("#t1", "\tpressed\n");
        }

        tuim::text("#t3", "\nhovered :" + std::to_string(tuim::ctx->hovered_id) + "\n\n");

        tuim::button("#b2", "&_c&atest2&r\n", NULL);
        tuim::button("#b3", "&_c&5test3&r\n", NULL);

        tuim::scroll_table("#table", &table_cursor, 0, columns, rows, 10, 2);

        usleep(1000 * 100);
        //std::cin.get();

    } while(key != tuim::keyboard::ESCAPE);

    tuim::delete_context();

    return 0;
}