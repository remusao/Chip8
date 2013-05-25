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

    // Used to debug information on cerr
    template <typename T>
    void debug(T message)
    {
# ifdef DEBUG
        std::cerr << message << std::endl;
# endif
    }

    template <typename T, typename... Args>
    void debug(T e, Args... args)
    {
# ifdef DEBUG
        std::cerr << e;
        debug(args...);
# endif
    }
}

#endif /* !UTILITY_HH_ */
