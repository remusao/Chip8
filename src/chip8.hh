#ifndef CHIP8_HH_
# define CHIP8_HH_

# include <fstream>
# include <iostream>
# include <limits>
# include <stdlib.h>
# include <stdio.h>
# include <array>
# include <SFML/Graphics.hpp>

namespace chip8
{
    namespace
    {
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
    }

    template <typename Byte, typename TwoBytes>
    class Chip8
    {
        public:
            Chip8() = default;
            ~Chip8() = default;

            // Methods
            void initialize();
            void loadGame(const char* rom);
            void cycle();

        private:

            void decode(TwoBytes opcode);
            template <unsigned offset>
            Byte get(TwoBytes opcode);

            // Chip8 internal
            std::array<Byte, 4096>      memory_; // Memory
            std::array<Byte, 16>        registers_; // registers
            TwoBytes                    I_; // Index register
            TwoBytes                    pc_; // program counter
            std::array<Byte, 64 * 32>   gfx_; // Screen

            // Timers
            Byte        delay_timer_;
            Byte        sound_timer_;

            // Stack
            std::array<TwoBytes, 16>    stack_;
            TwoBytes                    sp_;

            // Gamepad
            std::array<Byte, 16>        key_;
    };


    template <typename Byte, typename TwoBytes>
    void Chip8<Byte, TwoBytes>::initialize()
    {
        // Static checks
        static_assert(sizeof(Byte) == 1, "sizeof (Byte) != 1");
        static_assert(sizeof(TwoBytes) == 2, "sizeof (TwoBytes) != 2");

        std::cerr << "Initializing chip8 emulator" << std::endl;

        // Init random generator
        std::cerr << "Init random generator" << std::endl;
        srand(time(NULL));

        // Init internals
        std::cerr << "Init internals" << std::endl;
        memory_.fill(0);
        registers_.fill(0);
        I_ = 0;
        pc_ = 0x200;
        gfx_.fill(0);

        delay_timer_ = 0;
        sound_timer_ = 0;

        stack_.fill(0);
        sp_ = 0;

        key_.fill(0);

        // Load fontset
        std::cerr << "Init fontset" << std::endl;
        for(int i = 0; i < 80; ++i)
            memory_[i] = chip8_fontset[i];
    }


    template <typename Byte, typename TwoBytes>
    void Chip8<Byte, TwoBytes>::loadGame(const char* rom)
    {
        std::ifstream ifs;
        ifs.open(rom, std::ifstream::in);
        std::cerr << "Loading game: " << rom << std::endl;
        for (unsigned i = 0; ifs.good(); ++i)
        {
            char c = ifs.get();       // get character from file
            if (ifs.good())
                memory_[0x200 + i] = c;
        }
        ifs.close();
    }


    template <typename Byte, typename TwoBytes>
    void Chip8<Byte, TwoBytes>::cycle()
    {
        // Used to synchronize emulate at 60 Hz
        static sf::Clock    clock;
        static unsigned     nbCycles = 0;

        if (clock.getElapsedTime().asMilliseconds() >= 1000)
        {
            clock.restart();
            nbCycles = 0;
        }

        if (nbCycles < 60)
        {
            // Fetch opcode
            TwoBytes opcode =
                (memory_[pc_] << sizeof (TwoBytes) * 8)
                | memory_[pc_ + 1];

            // Decode and execute opcode
            decode(opcode);

            // Update timers
            if (delay_timer_ > 0)
                --delay_timer_;

            if (sound_timer_ > 0)
            {
                if (sound_timer_ == 1)
                    std::cout << '\a';
                --sound_timer_;
            }

            ++nbCycles;
        }
    }


    template <typename Byte, typename TwoBytes>
    template <unsigned offset>
    Byte Chip8<Byte, TwoBytes>::get(TwoBytes opcode)
    {
        return (opcode >> (12 - offset * 4) & 0x000F);
    }

    template <typename Byte, typename TwoBytes>
    void Chip8<Byte, TwoBytes>::decode(TwoBytes opcode)
    {
        Byte tmp; // used for sum and sub

        switch (opcode >> 12)
        {
            case 0:
                switch (opcode)
                {
                    case 0x00E0:
                        // 00E0 - Clear screen
                        gfx_.fill(0);
                        break;
                    case 0x00EE:
                        // 00EE - Returns from a subroutine
                        --sp_;
                        break;
                    default:
                        // 0NNN - Calls RCA 1802 program at address NNN
                        // TODO
                        break;
                };
                break;
            case 1:
                // 1NNN - Jumps to address NNN
                pc_ = opcode | 0x0FFF;
                break;
            case 2:
                // 2NNN - Calls subroutine at NNN
                stack_[sp_++] = pc_;
                break;
            case 3:
                // 3XNN - Skips the next instruction if VX equals NN
                if (registers_[get<1>(opcode)] == (opcode & 0x00FF))
                    pc_ += 2;
                break;
            case 4:
                // 3XNN - Skips the next instruction if VX doesn't equal NN
                if (registers_[get<1>(opcode)] != (opcode & 0x00FF))
                    pc_ += 2;
                break;
            case 5:
                // 5XY0 - Skips the next instruction if VX equals VY
                if (registers_[get<1>(opcode)] == registers_[get<2>(opcode)])
                    pc_ += 2;
                break;
            case 6:
                // 6XNN - Sets VX to NN
                registers_[get<1>(opcode)] = opcode & 0x00FF;
                break;
            case 7:
                // 7XNN - Adds NN to VX
                registers_[get<1>(opcode)] += opcode & 0x00FF;
                break;
            case 8:
                switch (opcode & 0x000F)
                {
                    case 0:
                        // 8XY0 - Sets VX to the value of VY
                        registers_[get<1>(opcode)] = registers_[get<2>(opcode)];
                        break;
                    case 1:
                        // 8XY1 - Sets VX to VX or VY
                        registers_[get<1>(opcode)] |= registers_[get<2>(opcode)];
                        break;
                    case 2:
                        // 8XY2 - Sets VX to VX and VY
                        registers_[get<1>(opcode)] &= registers_[get<2>(opcode)];
                        break;
                    case 3:
                        // 8XY3 - Sets VX to VX xor VY
                        registers_[get<1>(opcode)] ^= registers_[get<2>(opcode)];
                        break;
                    case 4:
                        // 8XY4 - Adds VY to VX. VF is set to 1 when there's a
                        // carry, and to 0 when there isn't
                        tmp = registers_[get<1>(opcode)] + registers_[get<2>(opcode)];
                        registers_[15] = tmp < registers_[get<1>(opcode)];
                        registers_[get<1>(opcode)] = tmp;
                        break;
                    case 5:
                        // 8XY5 - VY is subtracted from VX. VF is set to 0 when
                        // there's a borrow, and 1 when there isn't
                        tmp = registers_[get<1>(opcode)] - registers_[get<2>(opcode)];
                        registers_[15] = tmp < registers_[get<1>(opcode)];
                        registers_[get<1>(opcode)] = tmp;
                        break;
                    case 6:
                        // 8XY6 - Shifts VX right by one. VF is set to the value
                        // of the least significant bit of VX before the shift
                        registers_[15] = registers_[get<1>(opcode)] & 0x0001;
                        registers_[get<1>(opcode)] >>= 1;
                        break;
                    case 7:
                        // 8XY7 - Sets VX to VY minus VX. VF is set to 0 when
                        // there's a borrow, and 1 when there isn't
                        tmp = registers_[get<2>(opcode)] - registers_[get<1>(opcode)];
                        registers_[15] = tmp < registers_[get<2>(opcode)];
                        registers_[get<1>(opcode)] = tmp ;
                        break;
                    case 15:
                        // 8XYE - Shifts VX left by one. VF is set to the value
                        // of the most significant bit of VX before the shift
                        registers_[15] = 0x0001 & ((registers_[get<1>(opcode)] & 0x8000) >> 15);
                        registers_[get<1>(opcode)] <<= 1;
                        break;
                    default:
                        std::cerr << "Unknown instruction: " << opcode << std::endl;
                        break;
                };
                break;
            case 9:
                // 9XY0 - Skips the next instruction if VX doesn't equal VY
                if (registers_[get<1>(opcode)] != registers_[get<2>(opcode)])
                    pc_ += 2;
                break;
            case 10:
                // ANNN - Sets I to the address NNN
                I_ = opcode & 0x0FFF;
                break;
            case 11:
                // BNNN - Jumps to the address NNN plus V0
                pc_ = (opcode & 0x0FFF) + registers_[0];
                break;
            case 12:
                // CXNN - Sets VX to a random number and NN
                registers_[get<1>(opcode)] =
                    (rand() % std::numeric_limits<Byte>::max())
                    & (opcode & 0x00FF);
                break;
            case 13:
                // DXYN - Draws a sprite at coordinate (VX, VY) that has
                // a width of 8 pixels and a height of N pixels. Each row
                // of 8 pixels is read as bit-coded (with the most
                // significant bit of each byte displayed on the left)
                // starting from memory location I; I value doesn't
                // change after the execution of this instruction.
                // As described above, VF is set to 1 if any screen
                // pixels are flipped from set to unset when the sprite
                // is drawn, and to 0 if that doesn't happen
                // TODO
                break;
            case 14:
                switch (opcode & 0x00FF)
                {
                    case 0x009E:
                        // EX9E - Skips the next instruction if the key stored in VX is pressed
                        // TODO
                        break;
                    case 0x00A1:
                        // EXA1 - Skips the next instruction if the key stored in VX isn't pressed
                        // TODO
                        break;
                };
                break;
            case 15:
                switch (opcode & 0x00FF)
                {
                    case 0x0007:
                        // FX07 - Sets VX to the value of the delay timer
                        registers_[get<1>(opcode)] = delay_timer_;
                        break;
                    case 0x000A:
                        // FX0A - A key press is awaited, and then stored in VX
                        // TODO
                        break;
                    case 0x0015:
                        // FX15 - Sets the delay timer to VX
                        delay_timer_ = registers_[get<1>(opcode)];
                        break;
                    case 0x0018:
                        // FX18 - Sets the sound timer to VX
                        sound_timer_ = registers_[get<1>(opcode)];
                        break;
                    case 0x001E:
                        // FX1E - Adds VX to I
                        I_ += registers_[get<1>(opcode)];
                        registers_[15] = I_ > 0xFFF;
                        break;
                    case 0x0029:
                        // FX29 - Sets I to the location of the sprite for the character
                        // in VX. Characters 0-F (in hexadecimal) are represented by a
                        // 4x5 font
                        // TODO
                        break;
                    case 0x0033:
                        // FX33 - Stores the Binary-coded decimal representation of VX,
                        // with the most significant of three digits at the address in I,
                        // the middle digit at I plus 1, and the least significant digit
                        // at I plus 2. (In other words, take the decimal representation
                        // of VX, place the hundreds digit in memory at location in I, the
                        // tens digit at location I+1, and the ones digit at location I+2.)
                        // TODO
                        memory_[I_]     = registers_[(opcode & 0x0F00) >> 8] / 100;
                        memory_[I_ + 1] = (registers_[(opcode & 0x0F00) >> 8] / 10) % 10;
                        memory_[I_ + 2] = (registers_[(opcode & 0x0F00) >> 8] % 100) % 10;
                        pc_ += 2;
                        break;
                    case 0x0055:
                        // FX55 - Stores V0 to VX in memory starting at address I
                        for (unsigned i = 0; i < get<1>(opcode); ++i)
                            memory_[i + I_] = registers_[i];
                        break;
                    case 0x0065:
                        // FX65 - Fills V0 to VX with values from memory starting at address I
                        for (unsigned i = 0; i < get<1>(opcode); ++i)
                            registers_[i] = memory_[i + I_];
                        break;
                };
                break;
            default:
                std::cerr << "Unknown instruction: " << opcode << std::endl;
                break;
        };
    }
}

#endif /* !CHIP8_HH_ */