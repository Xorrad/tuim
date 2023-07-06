#include "tuim.hpp"

#include <math.h>

int main(int argc, char* argv[])
{
    tuim::init(argc, argv);
    tuim::set_title("test");

    tuim::keyboard::key key;

    double width = 100;
    int cursor = 0;

    do {
        tuim::clear();

        tuim::move(10, 200);
        tuim::clear_line();
        tuim::print("\r&8&_ccursor:&r &7%d\n", cursor);

        for(double i = cursor; i < cursor + width; i += 0.1) {
            
            int r = round(((sin(i/width + i) + 1)/2) * 255.0);
            int g = round(((cos(i/width + i) + 1)/2) * 255.0);
            int b = round(((cos(i/width + i) + 1)/2) * 255.0);
            unsigned int color = (r << 16) + (g << 8) + b;
            
            if(floor(i) == cursor) color = 0;

            //tuim::move(i, 10);
            tuim::move(i, 20 + (sin(i/10)*10));
            tuim::print("#_%06x ", color);
        }
        key = tuim::keyboard::get_pressed();

        if(key == tuim::keyboard::LEFT) cursor = std::max(0, cursor - 1);
        if(key == tuim::keyboard::RIGHT) cursor = std::min(int(width), cursor + 1);

    } while(key != tuim::keyboard::ESCAPE);

    //std::cin.get();

    return 0;
}