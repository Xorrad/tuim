#include "../tuim.hpp"

int main(int argc, char* argv[])
{    
    tuim::init(argc, argv);
    tuim::set_title("tuim - animations");

    tuim::font::register_style("c", tuim::font::make_style(tuim::color::from_hex(0x888888)));

    tuim::keyboard::keycode key;

    int animation_frame = 0;
    float animation_speed = 1.f; // in seconds
    std::vector<std::string> animation_frames = { "―", "\\", "|", "/" };

    do {
        tuim::clear();
        
        tuim::print("loading(%d,%f): ", animation_frame, animation_speed);
        tuim::animation("#a1", &animation_frame, animation_speed, animation_frames);
        tuim::new_line();

        tuim::print("\n\nPress F1 to exit.\n");

        tuim::display();

        key = tuim::keyboard::NONE;
        if(tuim::keyboard::is_pressed())
            key = tuim::keyboard::get_pressed();
        // else
            // usleep(50 * 1000);
        if(key == tuim::keyboard::LEFT) animation_speed = std::max(0.1f, animation_speed-0.1f);
        else if(key == tuim::keyboard::RIGHT) animation_speed += 0.1f;

    } while(key != tuim::keyboard::F1);

    tuim::delete_context();

    return 0;
}