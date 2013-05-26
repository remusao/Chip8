#ifndef OPCODES_HH_
# define OPCODES_HH_

# include "utility.hh"

namespace chip8
{
    enum Opcode
    {
        ADD_CARRY_XY,
        ADD_IX,
        ADD_XNN,
        CALL,
        CLEAR,
        DRAW,
        FILLS_0X,
        JUMP,
        JUMP_0NNN,
        KEY_AWAIT,
        RAND,
        RETURNS,
        SET_AND_XY,
        SET_INN,
        SET_I_SPRITE,
        SET_OR_XY,
        SET_SOUNDX,
        SET_TIMERX,
        SET_XNN,
        SET_XOR_XY,
        SET_XTIMER,
        SET_XY,
        SHIFT_LEFT_X,
        SHIFT_RIGHT_X,
        SKIPS_EQ_XNN,
        SKIPS_EQ_XY,
        SKIPS_NEQ_XNN,
        SKIPS_NEQ_XY,
        SKIPS_NPRESS,
        SKIPS_PRESS,
        STORE_0X,
        STORE_BINARY,
        SUB_BORROW_XY,
        SUB_BORROW_YX,
        UNKNOWN
    };

# ifdef DEBUG
    template <typename Word>
    void prettyPrint(Opcode opcode, Word value)
    {
        std::cerr << "\033[32m" << std::hex << value << "\033[37m ";
        switch (opcode)
        {
            case CLEAR:
                debug("Clear");
                break;
            case RETURNS:
                debug("Returns");
                break;
            case JUMP:
                debug("Jumps");
                break;
            case CALL:
                debug("Calls");
                break;
            case SKIPS_EQ_XNN:
                debug("Skip_eq_xnn");
                break;
            case SKIPS_NEQ_XNN:
                debug("Skip_neq_xnn");
                break;
            case SKIPS_EQ_XY:
                debug("Skip_eq_xy");
                break;
            case SKIPS_NEQ_XY:
                debug("Skip_neq_xy");
                break;
            case SET_XNN:
                debug("Set_xnn");
                break;
            case ADD_XNN:
                debug("Add_xnn");
                break;
            case SET_XY:
                debug("Set_xy");
                break;
            case SET_OR_XY:
                debug("Set_or_xy");
                break;
            case SET_AND_XY:
                debug("Set_and_xy");
                break;
            case SET_XOR_XY:
                debug("Set_xor_xy");
                break;
            case ADD_CARRY_XY:
                debug("Add_carry_xy");
                break;
            case SUB_BORROW_XY:
                debug("Sub_borrow_xy");
                break;
            case SHIFT_RIGHT_X:
                debug("Shift_right_x");
                break;
            case SHIFT_LEFT_X:
                debug("Shift_left_x");
                break;
            case SUB_BORROW_YX:
                debug("Sub_borrow_yx");
                break;
            case SET_INN:
                debug("Set_inn");
                break;
            case JUMP_0NNN:
                debug("Jump_0NNN");
                break;
            case RAND:
                debug("Rand");
                break;
            case DRAW:
                debug("Draw");
                break;
            case SKIPS_PRESS:
                debug("Skips_press");
                break;
            case SKIPS_NPRESS:
                debug("Skips_npress");
                break;
            case SET_XTIMER:
                debug("Set_xTimer");
                break;
            case KEY_AWAIT:
                debug("Key_wait");
                break;
            case SET_TIMERX:
                debug("Set_Timerx");
                break;
            case SET_SOUNDX:
                debug("Set_Soundx");
                break;
            case ADD_IX:
                debug("Add_Ix");
                break;
            case SET_I_SPRITE:
                debug("Set_I_Sprite");
                break;
            case STORE_BINARY:
                debug("Store_Binary");
                break;
            case STORE_0X:
                debug("Store_0x");
                break;
            case FILLS_0X:
                debug("Fills_0X");
                break;
            case UNKNOWN:
                debug("Unknown");
                break;
        };
    }
# else
    template <typename Word>
    void prettyPrint(Opcode, Word)
    {
    }
# endif

    template <typename Word>
    Opcode getOpcode(Word opcode)
    {
        Opcode res = UNKNOWN;

        switch (get<0>(opcode))
        {
            case 0:
                switch (opcode)
                {
                    case 0x00E0:
                        // 00E0 - Clear screen
                        res = CLEAR;
                        break;
                    case 0x00EE:
                        // 00EE - Returns from a subroutine
                        res = RETURNS;
                        break;
                    default:
                        // 0NNN - Calls RCA 1802 program at address NNN
                        break;
                };
                break;
            case 1:
                // 1NNN - Jumps to address NNN
                res = JUMP;
                break;
            case 2:
                // 2NNN - Calls subroutine at NNN
                res = CALL;
                break;
            case 3:
                // 3XNN - Skips the next instruction if VX equals NN
                res = SKIPS_EQ_XNN;
                break;
            case 4:
                // 4XNN - Skips the next instruction if VX doesn't equal NN
                res = SKIPS_NEQ_XNN;
                break;
            case 5:
                // 5XY0 - Skips the next instruction if VX equals VY
                res = SKIPS_EQ_XY;
                break;
            case 6:
                // 6XNN - Sets VX to NN
                res = SET_XNN;
                break;
            case 7:
                // 7XNN - Adds NN to VX
                res = ADD_XNN;
                break;
            case 8:
                switch (opcode & 0x000F)
                {
                    case 0:
                        // 8XY0 - Sets VX to the value of VY
                        res = SET_XY;
                        break;
                    case 1:
                        // 8XY1 - Sets VX to VX or VY
                        res = SET_OR_XY;
                        break;
                    case 2:
                        // 8XY2 - Sets VX to VX and VY
                        res = SET_AND_XY;
                        break;
                    case 3:
                        // 8XY3 - Sets VX to VX xor VY
                        res = SET_XOR_XY;
                        break;
                    case 4:
                        // 8XY4 - Adds VY to VX. VF is set to 1 when there's a
                        // carry, and to 0 when there isn't
                        res = ADD_CARRY_XY;
                        break;
                    case 5:
                        // 8XY5 - VY is subtracted from VX. VF is set to 0 when
                        // there's a borrow, and 1 when there isn't
                        res = SUB_BORROW_XY;
                        break;
                    case 6:
                        // 8XY6 - Shifts VX right by one. VF is set to the value
                        // of the least significant bit of VX before the shift
                        res = SHIFT_RIGHT_X;
                        break;
                    case 7:
                        // 8XY7 - Sets VX to VY minus VX. VF is set to 0 when
                        // there's a borrow, and 1 when there isn't
                        res = SUB_BORROW_YX;
                        break;
                    case 15:
                        // 8XYE - Shifts VX left by one. VF is set to the value
                        // of the most significant bit of VX before the shift
                        res = SHIFT_LEFT_X;
                        break;
                    default:
                        res = UNKNOWN;
                        break;
                };
                break;
            case 9:
                // 9XY0 - Skips the next instruction if VX doesn't equal VY
                res = SKIPS_NEQ_XY;
                break;
            case 10:
                // ANNN - Sets I to the address NNN
                res = SET_INN;
                break;
            case 11:
                // BNNN - Jumps to the address NNN plus V0
                res = JUMP_0NNN;
                break;
            case 12:
                // CXNN - Sets VX to a random number and NN
                res = RAND;
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
                res = DRAW;
                break;
            case 14:
                switch (opcode & 0x00FF)
                {
                    case 0x009E:
                        // EX9E - Skips the next instruction if the key stored in VX is pressed
                        res = SKIPS_PRESS;
                        break;
                    case 0x00A1:
                        // EXA1 - Skips the next instruction if the key stored in VX isn't pressed
                        res = SKIPS_NPRESS;
                        break;
                };
                break;
            case 15:
                switch (opcode & 0x00FF)
                {
                    case 0x0007:
                        // FX07 - Sets VX to the value of the delay timer
                        res = SET_XTIMER;
                        break;
                    case 0x000A:
                        // FX0A - A key press is awaited, and then stored in VX
                        res = KEY_AWAIT;
                        break;
                    case 0x0015:
                        // FX15 - Sets the delay timer to VX
                        res = SET_TIMERX;
                        break;
                    case 0x0018:
                        // FX18 - Sets the sound timer to VX
                        res = SET_SOUNDX;
                        break;
                    case 0x001E:
                        // FX1E - Adds VX to I
                        res = ADD_IX;
                        break;
                    case 0x0029:
                        // FX29 - Sets I to the location of the sprite for the character
                        // in VX. Characters 0-F (in hexadecimal) are represented by a
                        // 4x5 font
                        res = SET_I_SPRITE;
                        break;
                    case 0x0033:
                        // FX33 - Stores the Binary-coded decimal representation of VX,
                        // with the most significant of three digits at the address in I,
                        // the middle digit at I plus 1, and the least significant digit
                        // at I plus 2. (In other words, take the decimal representation
                        // of VX, place the hundreds digit in memory at location in I, the
                        // tens digit at location I+1, and the ones digit at location I+2.)
                        res = STORE_BINARY;
                        break;
                    case 0x0055:
                        // FX55 - Stores V0 to VX in memory starting at address I
                        res = STORE_0X;
                        break;
                    case 0x0065:
                        // FX65 - Fills V0 to VX with values from memory starting at address I
                        res = FILLS_0X;
                        break;
                };
                break;
            default:
                res = UNKNOWN;
                break;
        };

        return res;
    }
}

#endif /* !OPCODES_HH_ */
