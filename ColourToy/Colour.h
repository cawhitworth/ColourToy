#pragma once

#define R(col) ( (col) & 0xff )
#define G(col) ( ( (col) >> 8 ) & 0xff )
#define B(col) ( ( (col) >> 16 ) & 0xff )
#define A(col) ( ( (col) >> 24 ) & 0xff )

#define Colour(r,g,b,a) \
    ( \
        ( (r) & 0xff ) |\
        ( ( (g) & 0xff ) << 8) |\
        ( ( (b) & 0xff ) << 16) |\
        ( ( (a) & 0xff ) << 24)\
    )

#define Colour(r,g,b) \
    ( \
        ( (r) & 0xff ) |\
        ( ( (g) & 0xff ) << 8) |\
        ( ( (b) & 0xff ) << 16) |\
        ( 0xff << 24)\
    )

