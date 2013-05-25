#include "chip8.hh"

#define SCALE 4
#define WIDTH 64
#define HEIGHT 32

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        chip8::Chip8<unsigned char, unsigned short, SCALE> chip8;

        // Graphics
        sf::RenderWindow window(sf::VideoMode(WIDTH * SCALE, HEIGHT * SCALE), "Chip8 Emulator");

        // init
        chip8.initialize();
        chip8.loadGame(argv[1]);

        std::cerr << "Running game..." << std::endl;

        // Emulation loop

        while (window.isOpen())
        {
            chip8.cycle(window);

            if (chip8.getDrawFlag())
            {
                window.clear();
                chip8.draw(window);
                window.display();
                chip8.setDrawFlag(false);
            }

            sf::Event event;
            while (window.pollEvent(event))
            {
                switch (event.type)
                {
                    case sf::Event::Closed:
                        window.close();
                        break;
                    case sf::Event::KeyPressed:
                        chip8.setKey();
                        break;
                    default:
                        break;
                };
            }
        }
    }
    return 0;
}
