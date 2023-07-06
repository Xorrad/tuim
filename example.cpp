#include <iostream>

namespace Menu
{
    tuim::Menu* createMainMenu();
};

int main()
{
    tuim::Console console("My Title");

    console.add(Menu::createMainMenu());

    while(console.isOpen())
    {
        tuim::Keyboard::Key key;
        if(console.pullEvent(&key))
        {
            
        }

        console.update();
        console.clear();
        console.display();
    }
}

Menu* Menu::createMainMenu()
{
    tuim::Menu* m = new tuim::Menu("My Menu");

    m->addListener([this, &](tuim::Keyboard::Key key){

    });

    m->add(new tuim::Component::Text("hello world"));

    return m;
}
