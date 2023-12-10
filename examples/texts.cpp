#include "../tuim.hpp"

int main(int argc, char* argv[])
{    
    tuim::init(argc, argv);
    tuim::set_title("tuim - texts");

    tuim::font::register_style("c", tuim::font::make_style(tuim::color::from_hex(0x888888)));

    tuim::keyboard::keycode key;
    int paragraph_width = 50;

    do {
        tuim::clear();
        tuim::update(key);

        tuim::text("normal\n");
        tuim::text("&bbold&r\n");
        tuim::text("&ddim&r\n");
        tuim::text("&iitalic&r\n");
        tuim::text("&uunderline&r\n");
        tuim::text("&gblinking&r\n");
        tuim::text("&ninverse&r\n");
        tuim::text("&hhidden&r\n");
        tuim::text("&sstrikethrough&r\n");
        tuim::text("#ff0000red&r\n");
        tuim::text("#_ff0000background red&r\n\n");

        std::string text1 = "Tyrion was born in 273 AC as the third-born child to\n Lord Tywin and Lady Joanna Lannister. Joanna died giving birth to him, and as a result his father blames him for her death.&c[29]&r Tales about Tyrion were spread immediately after his birth; At Oldtown, it was said that Tyrion was a monster, with a tail, monstrous huge head, thick black hair, a beard, an evil eye, lion's claws, with teeth so long he was not able to close his mouth, and both male and female genitals.&c[7]&r It was said that Tyrion's birth was an omen to famine, plague and war,&c[7]&r and the smallfolk named him \"Lord Tywin\'s Doom\" and \"Lord Tywin\'s Bane\".&c[8]&r King Aerys II Targaryen stated that Tyrion was a punishment for Tywin's arrogance,&c[8]&r and the same was said by begging brothers in Oldtown, stating that Tywin had made himself greater than the king, which was only allowed by a god.&c[7]&r\n";
        std::string text2 = "Hello world, how are you today my friend?\n";
        tuim::gotoxy(tuim::vec2{10, tuim::get_cursor().y});
        tuim::paragraph("#p1", text1, paragraph_width);
        tuim::gotoxy(tuim::vec2{10, tuim::get_cursor().y});
        tuim::print("NB: Press arrows to change paragraph width.\n");

        tuim::print("\n\nPress F1 to exit.");

        tuim::display();

        key = tuim::keyboard::get_pressed();
        if(key == tuim::keyboard::LEFT) paragraph_width = std::max(1, paragraph_width-1);
        else if(key == tuim::keyboard::RIGHT) paragraph_width++;
    } while(key != tuim::keyboard::F1);

    tuim::delete_context();

    return 0;
}