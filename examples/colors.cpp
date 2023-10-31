#include "../tuim.hpp"

#include <math.h>

int main(int argc, char* argv[])
{
    tuim::init(argc, argv);
    tuim::create_context();
    tuim::set_title("test");

    tuim::set_cursor_visible(false);
    tuim::font::register_style("a", tuim::font::make_style(tuim::color::from_hex(0x55FF55)));

    tuim::keyboard::key key;
    double width = 100;
    int cursor = 0;

    do {
        tuim::clear();
        tuim::print("&b&_acursor:&r &a%d&r\n", cursor);

        for(double i = cursor; i < width; i += 0.1) {

            unsigned int color = ((int) (255.0 * (floor(i)/width)) << 8);

            if(floor(i) == cursor) color = 0;

            tuim::set_cursor({i, 20 + (sin(i/10)*10)});
            tuim::print("#_%06x ", color);
        }
        key = tuim::keyboard::get_pressed();

        if(key == tuim::keyboard::LEFT) cursor = std::max(0, cursor - 1);
        if(key == tuim::keyboard::RIGHT) cursor = std::min(int(width), cursor + 1);

    } while(key != tuim::keyboard::ESCAPE);


    tuim::delete_context();
    //std::cin.get();

    return 0;
}