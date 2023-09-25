#include "../tuim.hpp"

#include <vector>

namespace menu {

    enum menu {
        MAIN,
        OPTION,
        NEW_CHARACTER
    };

    std::vector<menu> stack;
    void push(menu menu);
    void pop();

    void main_menu();
    void option_menu();
};

int main(int argc, char* argv[]) {
    tuim::init(argc, argv); //tuim::ui::create_context();
    tuim::set_title("tuim demo");

    while(true) {
        tuim::keyboard::key key;
        if(tuim::keyboard::is_pressed()) {
            key = tuim::keyboard::get_pressed();
            tuim::update(key);
        }

        tuim::clear();

        menu::main_menu();

        tuim::display();
    }
}

void scroll_table(const char* id, int& cursor, std::vector<std::string> columns, std::vector<std::vector<std::string>> rows, int height, int padding) {
    
    if(tuim::is_item_active()) {
        if(tuim::is_pressed(tuim::keyboard::UP)) {
            cursor = std::max(0, cursor - 1);
        }
        if(tuim::is_pressed(tuim::keyboard::DOWN)) {
            cursor = std::min(cursor + 1, (int) rows.size());
        }
    }

    /* Calculate the max width for each columns */
    std::vector<int> columns_width = tuim::calc_columns_width(columns, rows, padding);

    auto print_table_border = [&](const char *c1, const char *c2, const char *c3) {
        tuim::print(c1);
        for(int i = 0; i < columns.size(); i++) {
            if(i != 0) tuim::print(c2);
            tuim::print("%s", std::string(u8"─", columns_width[i]).c_str());
        }
        tuim::print(c3);
    };

    print_table_border(u8"┌", u8"┬", u8"┐");

    tuim::print(u8"├");
    for(int i = 0; i < columns.size(); i++) {
        if(i != 0) tuim::print(u8"┼");

        std::string padding_text = std::string(" ", padding);
        std::string column_name = padding_text + columns[i] + padding_text;

        tuim::print("%s", padding_text, column_name.c_str());
    }
    tuim::print(u8"┤");
    
    print_table_border(u8"├", u8"┼", u8"┤");
    
    tuim::print(u8"├");
    for(int i = 0; i < rows.size(); i++) {
        if(int j = 0; j < columns.size(); j++) {
            if(j != 0) tuim::print(u8"┼");

            std::string padding_text = std::string(" ", padding/2);
            std::string column_name = columns[i];

            if(tuim::is_item_active() && i == cursor) column_name = "[ " + column_name + " ]";
            column_name = padding_text + column_name + padding_text;

            tuim::print("%s", column_name.c_str());
        }
    }
    tuim::print(u8"┤");

    print_table_border(u8"└", u8"┴", u8"┘");

    //print box top
}

void menu::main_menu() {
    static bool d = true;

    //tuim::start_centered(20);
    tuim::text("#title_text", "Main Menu", tuim::ITEM_FLAGS_DISABLED);
    tuim::hr("#title_separator", 20);
    //tuim::end_centered();

    if(d) {

        if(tuim::button("#supprimer", "&_cSupprimer")) {
            menu::push(menu::NEW_CHARACTER);
            //initialize character, world...

            d = true;
        }

        if(tuim::button("#new_game", "&_cNew Game")) {
            menu::push(menu::NEW_CHARACTER);
            //initialize character, world...
        }
    }

    tuim::menu* m = new tuim::menu("My Menu");

    m->add_event([this, &](tuim::keyboard::key key){
        
    });

    m->add(new tuim::component::text("martial: 10"));

    return m;
}
