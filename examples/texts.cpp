#include "../tuim.hpp"

int main(int argc, char* argv[])
{    
    tuim::init(argc, argv);
    tuim::set_title("tuim demo");
    tuim::set_cursor_visible(false);

    tuim::clear();

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

    std::string text1 = "Tyrion was born in 273 AC as the third-born child to Lord Tywin and Lady Joanna Lannister. Joanna died giving birth to him, and as a result his father blames him for her death.[29] Tales about Tyrion were spread immediately after his birth; At Oldtown, it was said that Tyrion was a monster, with a tail, monstrous huge head, thick black hair, a beard, an evil eye, lion's claws, with teeth so long he was not able to close his mouth, and both male and female genitals.[7] It was said that Tyrion's birth was an omen to famine, plague and war,[7] and the smallfolk named him \"Lord Tywin\'s Doom\" and \"Lord Tywin\'s Bane\".[8] King Aerys II Targaryen stated that Tyrion was a punishment for Tywin's arrogance,[8] and the same was said by begging brothers in Oldtown, stating that Tywin had made himself greater than the king, which was only allowed by a god.[7]\n\n";
    std::string text2 = "Hello world, how are you today my friend?";
    tuim::gotoxy(tuim::vec2{50, tuim::get_cursor().y});
    tuim::paragraph("#p1", text1, 50);

    std::cin.get();

    tuim::delete_context();

    return 0;
}