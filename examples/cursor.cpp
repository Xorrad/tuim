#include "../tuim.hpp"

int main(int argc, char* argv[])
{
    tuim::init(argc, argv);
    tuim::create_context();
    tuim::set_title("cursor");

    tuim::keyboard::keycode key;

    do {
        tuim::vec2 cursor = tuim::get_cursor();

        tuim::clear();
        tuim::gotoxy(tuim::vec2{0, 20});
        tuim::print("&bcursor&r: %d;%d\n", cursor.x, cursor.y);
        tuim::gotoxy(cursor);

        key = tuim::keyboard::get_pressed();
        if(key == tuim::keyboard::LEFT) cursor.x = std::max(1, cursor.x - 1);
        else if(key == tuim::keyboard::RIGHT) cursor.x++;
        else if(key == tuim::keyboard::UP) cursor.y = std::max(1, cursor.y - 1);
        else if(key == tuim::keyboard::DOWN) cursor.y++;

        tuim::gotoxy(cursor);

    } while(key != tuim::keyboard::ESCAPE);

    tuim::delete_context();

    return 0;
}