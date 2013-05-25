#ifndef UTILITY_HH_
# define UTILITY_HH_

namespace chip8
{
    // Used to error information on cerr
    template <typename T>
    void error(T message)
    {
# ifdef DEBUG
        std::cerr << message << std::endl;
# endif
    }

    template <typename T, typename... Args>
    void error(T e, Args... args)
    {
# ifdef DEBUG
        std::cerr << e;
        error(args...);
# endif
    }
}

#endif /* !UTILITY_HH_ */
