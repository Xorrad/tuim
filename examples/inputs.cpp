#include "../tuim.hpp"

enum weapon {
    NONE,
    SWORD,
    WARHAMMER,
    SPEAR,
    BOW,
    NUM_WEAPONS
};
const std::map<weapon, std::string> weapon_labels = {
    {NONE, "None"}, {SWORD, "Sword"}, {WARHAMMER, "Warhammer"}, {SPEAR, "Spear"}, {BOW, "Bow"}
};

int main(int argc, char* argv[])
{    
    setlocale(LC_ALL, "");

    tuim::init(argc, argv);
    tuim::set_title("tuim demo");

    tuim::set_cursor_visible(false);

    tuim::keyboard::keycode key;
    tuim::keyboard::keycode last_key;
    int a = 0;

    int value1 = 50;
    float value2 = 0.5f;
    std::string value3 = "tuim";
    weapon value4 = NONE;
    bool value5 = false;

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
        tuim::print("value3: %s\n", value3.c_str());
        tuim::print("value4: %d %s\n", value4, weapon_labels.at(value4).c_str());
        tuim::print("value5: %d\n\n", value5);
        if(key != 0) last_key = key;
        tuim::print("key: %lu %lu %lu %lu\n\n", (last_key >> 24), (last_key >> 16) & 0xFF, (last_key >> 8) & 0xFF, (last_key & 0xFF));
        tuim::print("utf-8 'ë' [0xC3AB]: %s\n", tuim::string::int_to_utf8(0xC3AB).c_str());

        tuim::slider<int>("#s1", &value1, 0, 100, 1);
        tuim::new_line();
        tuim::slider<float>("#s2", &value2, 0.f, 0.5f, 0.01f);
        tuim::new_line();
        tuim::input_number<int>("#i1", "< %d >", &value1, 0, 100, 1);
        tuim::new_line();
        tuim::input_string("#i2", &value3, "");
        tuim::new_line();
        tuim::input_enum<weapon>("#i3", "< %s >", &value4, NUM_WEAPONS, weapon_labels);
        tuim::new_line();
        tuim::input_bool("#i4", "< %s >", &value5);
        tuim::new_line();
        tuim::checkbox("#c1", "value5: %s", &value5);

        tuim::print("\n\nPress F1 to exit.");
        
    } while(key != tuim::keyboard::F1);

    tuim::delete_context();

    return 0;
}