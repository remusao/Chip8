#include "chip8.hh"

#define WIDTH 64
#define HEIGHT 32

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        chip8::Chip8<unsigned char, unsigned short> chip8;

        // Graphics
        sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Chip8 Emulator");

        // init
        chip8.initialize();
        chip8.loadGame(argv[1]);

        std::cerr << "Running game..." << std::endl;

        // Emulation loop
        sf::Clock   clock;
        unsigned    nbCyclesPerSec = 10;
        unsigned    nbCycles = 0;

        // Running emulator
        while (window.isOpen())
        {
            if (nbCycles < nbCyclesPerSec)
            {
                chip8.cycle();
                ++nbCycles;
            }

            if (clock.getElapsedTime().asMilliseconds() >= 1000 / 60)
            {
                // Update timers
                chip8.updateTimers();
                nbCycles = 0;

                // Update screen
                if (chip8.getDrawFlag())
                {
                    window.clear();
                    chip8.draw(window);
                    window.display();
                    chip8.setDrawFlag(false);
                }

                // Deal with events
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

                // Restart clock
                clock.restart();
            }
        }
    }
    return 0;
}
