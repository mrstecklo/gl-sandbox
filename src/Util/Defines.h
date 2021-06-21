#pragma once

#if defined(_MSC_VER) && (_MSC_VER < 1910)
    #define CONSTEXPR14
#else
    #define FULL_CONSTEXPR
    #define C14_VECTOR
    #define CONSTEXPR14 constexpr
#endif
