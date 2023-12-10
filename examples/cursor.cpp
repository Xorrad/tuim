#include "../tuim.hpp"

int main(int argc, char* argv[])
{
    tuim::init(argc, argv);
    tuim::create_context();
    tuim::set_title("tuim - cursor");

    tuim::keyboard::keycode key;
    tuim::vec2 cursor = tuim::get_cursor();

    do {
        tuim::clear();
        tuim::gotoxy(tuim::vec2{0, 20});
        tuim::print("&bcursor&r: %d;%d\n", cursor.x, cursor.y);
        tuim::gotoxy(cursor);
        tuim::print("█");
        tuim::display();

        key = tuim::keyboard::get_pressed();
        if(key == tuim::keyboard::LEFT) cursor.x = std::max(1, cursor.x - 1);
        else if(key == tuim::keyboard::RIGHT) cursor.x++;
        else if(key == tuim::keyboard::UP) cursor.y = std::max(1, cursor.y - 1);
        else if(key == tuim::keyboard::DOWN) cursor.y++;
    } while(key != tuim::keyboard::F1);

    tuim::delete_context();

    return 0;
}