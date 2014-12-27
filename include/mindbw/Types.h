#ifndef MINDBW_TYPES_H
#define MINDBW_TYPES_H

#include <zephyr/CExport.h>

Z_NS_START(mindbw)

Z_ENUM_CLASS(mindbw, Operator)
{
    LT,
    LTE,
    GT,
    GTE,
    EQ,
    NEQ,
};


Z_NS_END

#endif
