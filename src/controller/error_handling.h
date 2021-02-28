#pragma once

#define assertIsTrueElseThrow(condition)                                 \
    if (!(condition))                                                    \
    {                                                                    \
        std::cerr << "Error in file: " << __FILE__                       \
                  << ", on line: " << __LINE__                           \
                  << ", condition \"" << #condition << "\" is false!\n"; \
        throw std::logic_error(#condition " != true!");                  \
    }
