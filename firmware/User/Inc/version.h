#ifndef __VERSION_H
#define __VERSION_H

#define __IS_BUILD_MONTH_JAN \
    (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define __IS_BUILD_MONTH_FEB (__DATE__[0] == 'F')
#define __IS_BUILD_MONTH_MAR \
    (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define __IS_BUILD_MONTH_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define __IS_BUILD_MONTH_MAY \
    (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define __IS_BUILD_MONTH_JUN \
    (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define __IS_BUILD_MONTH_JUL \
    (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define __IS_BUILD_MONTH_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define __IS_BUILD_MONTH_SEP (__DATE__[0] == 'S')
#define __IS_BUILD_MONTH_OCT (__DATE__[0] == 'O')
#define __IS_BUILD_MONTH_NOV (__DATE__[0] == 'N')
#define __IS_BUILD_MONTH_DEC (__DATE__[0] == 'D')

#define BUILD_YEAR_CH0 (__DATE__[7])
#define BUILD_YEAR_CH1 (__DATE__[8])
#define BUILD_YEAR_CH2 (__DATE__[9])
#define BUILD_YEAR_CH3 (__DATE__[10])
#define BUILD_MONTH_CH0                                                     \
    ((__IS_BUILD_MONTH_OCT || __IS_BUILD_MONTH_NOV || __IS_BUILD_MONTH_DEC) \
         ? '1'                                                              \
         : '0')
#define BUILD_MONTH_CH1             \
    ((__IS_BUILD_MONTH_JAN)   ? '1' \
     : (__IS_BUILD_MONTH_FEB) ? '2' \
     : (__IS_BUILD_MONTH_MAR) ? '3' \
     : (__IS_BUILD_MONTH_APR) ? '4' \
     : (__IS_BUILD_MONTH_MAY) ? '5' \
     : (__IS_BUILD_MONTH_JUN) ? '6' \
     : (__IS_BUILD_MONTH_JUL) ? '7' \
     : (__IS_BUILD_MONTH_AUG) ? '8' \
     : (__IS_BUILD_MONTH_SEP) ? '9' \
     : (__IS_BUILD_MONTH_OCT) ? '0' \
     : (__IS_BUILD_MONTH_NOV) ? '1' \
     : (__IS_BUILD_MONTH_DEC) ? '2' \
                              : '?')
#define BUILD_DAY_CH0 ((__DATE__[4] >= '0') ? (__DATE__[4]) : '0')
#define BUILD_DAY_CH1 (__DATE__[5])
#define BUILD_HOUR_CH0 (__TIME__[0])
#define BUILD_HOUR_CH1 (__TIME__[1])
#define BUILD_MIN_CH0 (__TIME__[3])
#define BUILD_MIN_CH1 (__TIME__[4])
#define BUILD_SEC_CH0 (__TIME__[6])
#define BUILD_SEC_CH1 (__TIME__[7])

#define FW_VERSION    \
    {BUILD_YEAR_CH2,  \
     BUILD_YEAR_CH3,  \
     BUILD_MONTH_CH0, \
     BUILD_MONTH_CH1, \
     BUILD_DAY_CH0,   \
     BUILD_DAY_CH1,   \
     '.',             \
     BUILD_HOUR_CH0,  \
     BUILD_HOUR_CH1,  \
     BUILD_MIN_CH0,   \
     BUILD_MIN_CH1,   \
     '\0'}

#endif
