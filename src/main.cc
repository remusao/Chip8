#include "chip8.hh"


int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        chip8::Chip8<unsigned char, unsigned short> chip8;

        // Graphics
        sf::RenderWindow window(sf::VideoMode(64, 32), "Chip8 Emulator");
        sf::CircleShape shape(10.f);
        shape.setFillColor(sf::Color::Green);

        // init
        chip8.initialize();
        chip8.loadGame(argv[1]);

        std::cerr << "Running game..." << std::endl;

        // Emulation loop

        while (window.isOpen())
        {
            chip8.cycle();
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
            window.draw(shape);
            window.display();
        }
    }
    return 0;
}
