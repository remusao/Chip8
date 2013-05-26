#ifndef UTILITY_HH_
# define UTILITY_HH_

namespace chip8
{
    // Used to retreive some part of a 16-bits word
    template <unsigned offset, typename Word>
    Word get(Word opcode)
    {
        return (opcode >> (12 - offset * 4)) & 0x000F;
    }

# ifdef DEBUG
    // Used to debug information on cerr
    template <typename T>
    void debug(T message)
    {
        std::cerr << message << std::endl;
    }

    template <typename T, typename... Args>
    void debug(T e, Args... args)
    {
        std::cerr << e;
        debug(args...);
    }
# else
    template <typename... Args>
    void debug(Args...)
    {
    }
# endif

    unsigned char chip8_fontset[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };


    unsigned getKey()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            return 0;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            return 1;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
            return 2;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
            return 3;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            return 4;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            return 5;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            return 6;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            return 7;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            return 8;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            return 9;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            return 10;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
            return 11;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            return 12;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
            return 13;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
            return 14;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
            return 15;
        else
            return 16;

    }
}

#endif /* !UTILITY_HH_ */
