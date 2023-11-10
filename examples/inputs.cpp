#include "../tuim.hpp"

#include <math.h>
#include <time.h>

int main(int argc, char* argv[])
{    
    setlocale(LC_ALL, "");

    tuim::init(argc, argv);
    tuim::set_title("tuim demo");

    tuim::set_cursor_visible(false);

    tuim::keyboard::keycode key;
    int a = 0;

    int value1 = 50;
    float value2 = 0.5f;
    std::string value3 = "tuim";
    tuim::keyboard::keycode lkey;

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

        tuim::print("value1: %d\n", value1);
        tuim::print("value2: %f\n", value2);
        tuim::print("value3: %s\n\n", value3.c_str());
        if(key != 0) lkey = key;
        tuim::print("key: %lu %lu %lu\n\n", (lkey >> 16), (lkey >> 8) & 0xFF, (lkey & 0xFF));

        tuim::slider<int>("#s1", &value1, 0, 100, 1);
        tuim::print("\n");
        tuim::slider<float>("#s2", &value2, 0.f, 0.5f, 0.01f);
        tuim::print("\n");
        tuim::input_number<int>("#i1", "< %d >", &value1, 0, 100, 1);
        tuim::print("\n");
        tuim::input_string("#i2", "#_555555%s&r", &value3, "");

    } while(key != tuim::keyboard::F1);

    tuim::delete_context();

    return 0;
}