#ifndef MINDBW_TYPES_H
#define MINDBW_TYPES_H

#include <zephyr/CExport.h>

Z_NS_START(mindbw)

Z_ENUM_CLASS ZD(mindbw)Operator
{
    ZD(mindbw)LT,
    ZD(mindbw)LTE,
    ZD(mindbw)GT,
    ZD(mindbw)GTE,
    ZD(mindbw)EQ,
    ZD(mindbw)NEQ,
};


Z_NS_END

#endif
