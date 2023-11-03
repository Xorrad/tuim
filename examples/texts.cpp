#include "../tuim.hpp"

int main(int argc, char* argv[])
{    
    tuim::init(argc, argv);
    tuim::set_title("tuim demo");

    tuim::set_cursor_visible(false);

    tuim::clear();

    tuim::text("#t0", "normal\n");
    tuim::text("#t1", "&bbold&r\n");
    tuim::text("#t2", "&ddim&r\n");
    tuim::text("#t3", "&iitalic&r\n");
    tuim::text("#t4", "&uunderline&r\n");
    tuim::text("#t5", "&gblinking&r\n");
    tuim::text("#t6", "&ninverse&r\n");
    tuim::text("#t7", "&hhidden&r\n");
    tuim::text("#t8", "&sstrikethrough&r\n");
    tuim::text("#t9", "#ff0000red&r\n");
    tuim::text("#t10", "#_ff0000background red&r\n");

    std::cin.get();

    tuim::delete_context();

    return 0;
}