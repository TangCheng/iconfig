#ifndef __COMMON_H__
#define __COMMON_H__

#include <glib.h>

typedef union _Color
{
    struct
    {
        guint32 blue:8;
        guint32 green:8;
        guint32 red:8;
        guint32 alpha:8;
    };
    guint32 value;
} Color;

#endif /* __COMMON_H__ */
