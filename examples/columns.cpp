#include "../tuim.hpp"

#include <math.h>
#include <time.h>

int main(int argc, char* argv[])
{    
    tuim::init(argc, argv);
    tuim::set_title("tuim demo");

    std::string s = "\33[2J\33[3J\33[HH \033[1;31mW!\033[0m";
    tuim::print("%s\t%d", s.c_str(), tuim::calc_text_width(s, 0));
    std::cin.get();

    tuim::keyboard::key key;
    int a = 0;

    int table_cursor = 0;
    int table_key = 0;

    std::vector<std::string> columns = { "&_bid&r", "&_aname&r", "&_emoney&r" };
    std::vector<std::vector<std::string>> rows = {
        { "0", "&ca", "10€" },
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

        //tuim::print("123456\n");
        //tuim::set_cursor({5, 5});
        //tuim::vec2 cursor_pos = tuim::get_cursor();

        //tuim::print("1234567\n");
        //tuim::print("%d", time(0));
        //printf("(%d,%d)", cursor_pos.x, cursor_pos.y);
        //tuim::set_title(std::to_string(cursor_pos.x) + "," + std::to_string(cursor_pos.y));
        //tuim::set_cursor({5, 5});

        tuim::text("#t", "choix ");
        tuim::print("\n");
        tuim::print("\n");

        //cursor_pos = tuim::get_cursor();
        //printf("(%d,%d)", cursor_pos.x, cursor_pos.y);

        tuim::start_container(); {
            tuim::start_column();
            tuim::text("#t4", "test1");
            tuim::text("#t4", "test2");
            tuim::end_column();

            tuim::start_column();
            tuim::text("#t4", "test3");
            tuim::text("#t4", "test4");
            tuim::end_column();
        }
        tuim::end_container();

        // for(int i = 0; i < 10; i++)
        //     tuim::text("#t" + std::to_string(i), "text " + std::to_string(i) + "\n");

        tuim::button("#b0", "&_c&atest2&r", NULL);
        tuim::button("#b0.1", "&_c&atest2.1&r\n", NULL);
        tuim::text("#t4", "hello world\n");

        if(tuim::button("#b1", "&_c&6test1&r\n", NULL)) {
            tuim::text("#t1", "\tpressed\n");
        }

        tuim::text("#t3", "\nhovered :" + std::to_string(tuim::ctx->hovered_id) + "\n\n");

        tuim::button("#b2", "&_c&atest2&r\n", NULL);
        tuim::button("#b3", "&_c&5test3&r\n", NULL);

        tuim::scroll_table("#table", &table_cursor, &table_key, columns, rows, 10, 2);
        
        usleep(1000 * 100);
        //std::cin.get();

    } while(key != tuim::keyboard::ESCAPE);

    tuim::delete_context();

    return 0;
}