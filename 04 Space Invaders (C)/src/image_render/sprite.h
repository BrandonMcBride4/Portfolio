#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>

#define packWord24(b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packWord18(b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b17 << 17) | (b16 << 16) |						  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packWord17(b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b16 << 16) |						  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packWord16(b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packWord15(b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packWord14(b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |	 \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packWord12(b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packWord6(b5,b4,b3,b2,b1,b0) \
((b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ))

#define packWord5(b4,b3,b2,b1,b0) \
((b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ))

#define packWord3(b2,b1,b0) \
((b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ))

#define packWord1(b0) (b0  << 0 )


static const uint32_t saucer_18x7[] =
        {
                packWord18(0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0),
                packWord18(0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0),
                packWord18(0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0),
                packWord18(0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0),
                packWord18(0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0),
                packWord18(0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0),
                packWord18(0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0)
        };

static const uint32_t saucer_gone_18x7[] =
        {
                packWord18(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord18(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord18(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord18(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord18(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord18(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord18(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        };

static const uint32_t alien_explosion_14x10[] =
        {
                packWord14(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0),
                packWord14(0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0),
                packWord14(0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0),
                packWord14(0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0),
                packWord14(0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0),
                packWord14(0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0)
        };

static const uint32_t alien_gone_14x10[] =
        {
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        };

static const uint32_t alien_top_in_14x10[] =
        {
                packWord14(0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0),
                packWord14(0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0),
                packWord14(0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0),
                packWord14(0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0),
                packWord14(0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0),
                packWord14(0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        };

static const uint32_t alien_top_out_14x10[] =
        {
                packWord14(0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0),
                packWord14(0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0),
                packWord14(0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0),
                packWord14(0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        };

static const uint32_t alien_middle_in_14x10[] =
        {
                packWord14(0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0),
                packWord14(0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0),
                packWord14(0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0),
                packWord14(0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0),
                packWord14(0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0),
                packWord14(0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        };

static const uint32_t alien_middle_out_14x10[] =
        {
                packWord14(0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0),
                packWord14(0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0),
                packWord14(0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0),
                packWord14(0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0),
                packWord14(0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0),
                packWord14(0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0),
                packWord14(0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0),
                packWord14(0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        };

static const uint32_t alien_bottom_in_14x10[] =
        {
                packWord14(0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0),
                packWord14(0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0),
                packWord14(0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0),
                packWord14(0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0),
                packWord14(0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0),
                packWord14(0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0),
                packWord14(0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0),
                packWord14(0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        };

static const uint32_t alien_bottom_out_14x10[] =
        {
                packWord14(0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0),
                packWord14(0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0),
                packWord14(0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0),
                packWord14(0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0),
                packWord14(0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0),
                packWord14(0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0),
                packWord14(0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0),
                packWord14(0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord14(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        };

static const uint32_t tank_17x8[] =
        {
                packWord17(0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0),
                packWord17(0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0),
                packWord17(0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0),
                packWord17(0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0),
                packWord17(0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0),
                packWord17(0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0),
                packWord17(0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0),
                packWord17(0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
        };

// Shape of the entire bunker.
static const uint32_t bunker_24x18[] =
        {
                packWord24(0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0),
                packWord24(0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0),
                packWord24(0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0),
                packWord24(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1),
                packWord24(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1),
                packWord24(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1),
                packWord24(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1),
                packWord24(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1),
                packWord24(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1),
                packWord24(1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1),
                packWord24(1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1),
                packWord24(1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1),
                packWord24(1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1),
                packWord24(1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1),
                packWord24(1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1),
                packWord24(1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1),
                packWord24(1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1),
                packWord24(1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1)
        };

// These are the blocks that comprise the bunker and each time a bullet
// strikes one of these blocks, you erode the block as you sequence through
// these patterns.
static uint32_t bunkerDamage0_6x6[] = {
        packWord6(0, 1, 1, 0, 0, 0),
        packWord6(0, 0, 0, 0, 0, 1),
        packWord6(1, 1, 0, 1, 0, 0),
        packWord6(1, 0, 0, 0, 0, 0),
        packWord6(0, 0, 1, 1, 0, 0),
        packWord6(0, 0, 0, 0, 1, 0)
};

static uint32_t bunkerDamage1_6x6[] = {
        packWord6(1, 1, 1, 0, 1, 0),
        packWord6(1, 0, 1, 0, 0, 1),
        packWord6(1, 1, 0, 1, 1, 1),
        packWord6(1, 0, 0, 0, 0, 0),
        packWord6(0, 1, 1, 1, 0, 1),
        packWord6(0, 1, 1, 0, 1, 0)
};

static uint32_t bunkerDamage2_6x6[] = {
        packWord6(1, 1, 1, 1, 1, 1),
        packWord6(1, 0, 1, 1, 0, 1),
        packWord6(1, 1, 0, 1, 1, 1),
        packWord6(1, 1, 0, 1, 1, 0),
        packWord6(0, 1, 1, 1, 0, 1),
        packWord6(1, 1, 1, 1, 1, 1)
};

static uint32_t bunkerDamage3_6x6[] = {
        packWord6(1, 1, 1, 1, 1, 1),
        packWord6(1, 1, 1, 1, 1, 1),
        packWord6(1, 1, 1, 1, 1, 1),
        packWord6(1, 1, 1, 1, 1, 1),
        packWord6(1, 1, 1, 1, 1, 1),
        packWord6(1, 1, 1, 1, 1, 1)
};

static uint32_t bunker_upper_left_gone_6x6[] = {
        packWord6(0, 0, 0, 1, 1, 1),
        packWord6(0, 0, 1, 1, 1, 1),
        packWord6(0, 1, 1, 1, 1, 1),
        packWord6(1, 1, 1, 1, 1, 1),
        packWord6(1, 1, 1, 1, 1, 1),
        packWord6(1, 1, 1, 1, 1, 1)
};

static uint32_t bunker_upper_right_gone_6x6[] = {
        packWord6(1, 1, 1, 0, 0, 0),
        packWord6(1, 1, 1, 1, 0, 0),
        packWord6(1, 1, 1, 1, 1, 0),
        packWord6(1, 1, 1, 1, 1, 1),
        packWord6(1, 1, 1, 1, 1, 1),
        packWord6(1, 1, 1, 1, 1, 1)
};

static uint32_t bunker_lower_left_gone_6x6[] = {
        packWord6(1, 1, 1, 1, 1, 1),
        packWord6(1, 1, 1, 1, 1, 1),
        packWord6(0, 0, 1, 1, 1, 1),
        packWord6(0, 0, 0, 1, 1, 1),
        packWord6(0, 0, 0, 0, 1, 1),
        packWord6(0, 0, 0, 0, 0, 1)
};

static uint32_t bunker_lower_right_gone_6x6[] = {
        packWord6(1, 1, 1, 1, 1, 1),
        packWord6(1, 1, 1, 1, 1, 1),
        packWord6(1, 1, 1, 1, 0, 0),
        packWord6(1, 1, 1, 0, 0, 0),
        packWord6(1, 1, 0, 0, 0, 0),
        packWord6(1, 0, 0, 0, 0, 0)
};

static uint32_t bunkerDamageGone[] = {
        packWord6(0, 0, 0, 0, 0, 0),
        packWord6(0, 0, 0, 0, 0, 0),
        packWord6(0, 0, 0, 0, 0, 0),
        packWord6(0, 0, 0, 0, 0, 0),
        packWord6(0, 0, 0, 0, 0, 0),
        packWord6(0, 0, 0, 0, 0, 0)
};

static const uint32_t tank_explosion1_17x8[] = {
        packWord17(0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
        packWord17(0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0),
        packWord17(0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0),
        packWord17(0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0),
        packWord17(0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0),
        packWord17(0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0),
        packWord17(0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0),
        packWord17(0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0)
};

static const uint32_t tank_explosion2_17x8[] = {
        packWord17(0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0),
        packWord17(0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
        packWord17(0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0),
        packWord17(0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0),
        packWord17(0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0),
        packWord17(0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0),
        packWord17(0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0),
        packWord17(0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0)
};

static const uint32_t tank_gone_17x8[] = {
        packWord17(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
        packWord17(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
        packWord17(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
        packWord17(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
        packWord17(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
        packWord17(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
        packWord17(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
        packWord17(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
};

static const uint32_t tankbullet_1x7[] = {
        packWord1(1),
        packWord1(1),
        packWord1(1),
        packWord1(1),
        packWord1(1),
        packWord1(0),
        packWord1(0)
};

static const uint32_t tankbullet_gone_1x7[] = {
        packWord1(0),
        packWord1(0),
        packWord1(0),
        packWord1(0),
        packWord1(0),
        packWord1(0),
        packWord1(0)
};

static const uint32_t alienbullet1_down_3x7[] = {
        packWord3(0, 0, 0),
        packWord3(0, 0, 0),
        packWord3(0, 1, 0),
        packWord3(0, 1, 0),
        packWord3(0, 1, 0),
        packWord3(1, 1, 1),
        packWord3(0, 1, 0)
};

static const uint32_t alienbullet1_up_3x7[] = {
        packWord3(0, 0, 0),
        packWord3(0, 0, 0),
        packWord3(0, 1, 0),
        packWord3(1, 1, 1),
        packWord3(0, 1, 0),
        packWord3(0, 1, 0),
        packWord3(0, 1, 0)
};

static const uint32_t alienbullet2_up_3x7[] = {
        packWord3(0, 0, 0),
        packWord3(0, 0, 0),
        packWord3(0, 1, 0),
        packWord3(1, 0, 0),
        packWord3(0, 1, 0),
        packWord3(0, 0, 1),
        packWord3(0, 1, 0)
};

static const uint32_t alienbullet2_down_3x7[] = {
        packWord3(0, 0, 0),
        packWord3(0, 0, 0),
        packWord3(0, 1, 0),
        packWord3(0, 0, 1),
        packWord3(0, 1, 0),
        packWord3(1, 0, 0),
        packWord3(0, 1, 0)
};


static const uint32_t alienbullet_gone_3x7[] = {
        packWord3(0, 0, 0),
        packWord3(0, 0, 0),
        packWord3(0, 0, 0),
        packWord3(0, 0, 0),
        packWord3(0, 0, 0),
        packWord3(0, 0, 0),
        packWord3(0, 0, 0)
};

static const uint32_t letterA_5x5[] = {
        packWord5(0, 1, 1, 1, 0),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 1, 1, 1, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 0, 1)
};

static const uint32_t letterB_5x5[] = {
        packWord5(1, 1, 1, 1, 0),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 1, 1, 1, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 1, 1, 1, 0)
};

static const uint32_t letterC_5x5[] = {
        packWord5(0, 1, 1, 1, 1),
        packWord5(1, 0, 0, 0, 0),
        packWord5(1, 0, 0, 0, 0),
        packWord5(1, 0, 0, 0, 0),
        packWord5(0, 1, 1, 1, 1)
};

static const uint32_t letterD_5x5[] = {
        packWord5(1, 1, 1, 1, 0),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 1, 1, 1, 0)
};

static const uint32_t letterE_5x5[] = {
        packWord5(1, 1, 1, 1, 1),
        packWord5(1, 0, 0, 0, 0),
        packWord5(1, 1, 1, 1, 0),
        packWord5(1, 0, 0, 0, 0),
        packWord5(1, 1, 1, 1, 1)
};

static const uint32_t letterF_5x5[] = {
        packWord5(1, 1, 1, 1, 1),
        packWord5(1, 0, 0, 0, 0),
        packWord5(1, 1, 1, 1, 0),
        packWord5(1, 0, 0, 0, 0),
        packWord5(1, 0, 0, 0, 0)
};

static const uint32_t letterG_5x5[] = {
        packWord5(0, 1, 1, 1, 1),
        packWord5(1, 0, 0, 0, 0),
        packWord5(1, 0, 0, 1, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(0, 1, 1, 1, 0)
};

static const uint32_t letterH_5x5[] = {
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 1, 1, 1, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 0, 1)
};

static const uint32_t letterI_5x5[] = {
        packWord5(0, 0, 1, 0, 0),
        packWord5(0, 0, 1, 0, 0),
        packWord5(0, 0, 1, 0, 0),
        packWord5(0, 0, 1, 0, 0),
        packWord5(0, 0, 1, 0, 0)
};

static const uint32_t letterJ_5x5[] = {
        packWord5(1, 1, 1, 1, 1),
        packWord5(0, 0, 1, 0, 0),
        packWord5(0, 0, 1, 0, 0),
        packWord5(0, 0, 1, 0, 0),
        packWord5(1, 1, 1, 0, 0)
};

static const uint32_t letterK_5x5[] = {
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 1, 0),
        packWord5(1, 1, 1, 0, 0),
        packWord5(1, 0, 0, 1, 0),
        packWord5(1, 0, 0, 0, 1)
};

static const uint32_t letterL_5x5[] = {
        packWord5(1, 0, 0, 0, 0),
        packWord5(1, 0, 0, 0, 0),
        packWord5(1, 0, 0, 0, 0),
        packWord5(1, 0, 0, 0, 0),
        packWord5(1, 1, 1, 1, 1)
};

static const uint32_t letterM_5x5[] = {
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 1, 0, 1, 1),
        packWord5(1, 0, 1, 0, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 0, 1)
};

static const uint32_t letterN_5x5[] = {
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 1, 0, 0, 1),
        packWord5(1, 0, 1, 0, 1),
        packWord5(1, 0, 0, 1, 1),
        packWord5(1, 0, 0, 0, 1)
};

static const uint32_t letterO_5x5[] = {
        packWord5(0, 1, 1, 1, 0),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(0, 1, 1, 1, 0)
};

static const uint32_t letterP_5x5[] = {
        packWord5(0, 1, 1, 1, 0),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 1, 1, 1, 1),
        packWord5(1, 0, 0, 0, 0),
        packWord5(1, 0, 0, 0, 0)
};

static const uint32_t letterQ_5x5[] = {
        packWord5(0, 1, 1, 1, 0),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 1, 0),
        packWord5(0, 1, 1, 0, 1)
};

static const uint32_t letterR_5x5[] = {
        packWord5(1, 1, 1, 1, 0),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 1, 1, 1, 0),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 0, 1)
};

static const uint32_t letterS_5x5[] = {
        packWord5(0, 1, 1, 1, 1),
        packWord5(1, 0, 0, 0, 0),
        packWord5(0, 1, 1, 1, 0),
        packWord5(0, 0, 0, 0, 1),
        packWord5(1, 1, 1, 1, 0)
};

static const uint32_t letterT_5x5[] = {
        packWord5(1, 1, 1, 1, 1),
        packWord5(0, 0, 1, 0, 0),
        packWord5(0, 0, 1, 0, 0),
        packWord5(0, 0, 1, 0, 0),
        packWord5(0, 0, 1, 0, 0)
};

static const uint32_t letterU_5x5[] = {
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(0, 1, 1, 1, 0)
};

static const uint32_t letterV_5x5[] = {
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(0, 1, 0, 1, 0),
        packWord5(0, 0, 1, 0, 0)
};

static const uint32_t letterW_5x5[] = {
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 1, 0, 1),
        packWord5(1, 0, 1, 0, 1),
        packWord5(0, 1, 0, 1, 0)
};

static const uint32_t letterX_5x5[] = {
        packWord5(1, 0, 0, 0, 1),
        packWord5(0, 1, 0, 1, 0),
        packWord5(0, 0, 1, 0, 0),
        packWord5(0, 1, 0, 1, 0),
        packWord5(1, 0, 0, 0, 1)
};

static const uint32_t letterY_5x5[] = {
        packWord5(1, 0, 0, 0, 1),
        packWord5(0, 1, 0, 1, 0),
        packWord5(0, 0, 1, 0, 0),
        packWord5(0, 0, 1, 0, 0),
        packWord5(0, 0, 1, 0, 0)
};

static const uint32_t letterZ_5x5[] = {
        packWord5(1, 1, 1, 1, 1),
        packWord5(0, 0, 0, 1, 0),
        packWord5(0, 0, 1, 0, 0),
        packWord5(0, 1, 0, 0, 0),
        packWord5(1, 1, 1, 1, 1)
};

static const uint32_t number0_5x5[] = {
        packWord5(0, 1, 1, 1, 0),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(0, 1, 1, 1, 0)
};

static const uint32_t number1_5x5[] = {
        packWord5(1, 1, 1, 0, 0),
        packWord5(0, 0, 1, 0, 0),
        packWord5(0, 0, 1, 0, 0),
        packWord5(0, 0, 1, 0, 0),
        packWord5(1, 1, 1, 1, 1)
};

static const uint32_t number2_5x5[] = {
        packWord5(1, 1, 1, 1, 1),
        packWord5(0, 0, 0, 0, 1),
        packWord5(1, 1, 1, 1, 1),
        packWord5(1, 0, 0, 0, 0),
        packWord5(1, 1, 1, 1, 1)
};

static const uint32_t number3_5x5[] = {
        packWord5(1, 1, 1, 1, 1),
        packWord5(0, 0, 0, 0, 1),
        packWord5(1, 1, 1, 1, 1),
        packWord5(0, 0, 0, 0, 1),
        packWord5(1, 1, 1, 1, 1)
};

static const uint32_t number4_5x5[] = {
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 1, 1, 1, 1),
        packWord5(0, 0, 0, 0, 1),
        packWord5(0, 0, 0, 0, 1)
};

static const uint32_t number5_5x5[] = {
        packWord5(1, 1, 1, 1, 1),
        packWord5(1, 0, 0, 0, 0),
        packWord5(1, 1, 1, 1, 1),
        packWord5(0, 0, 0, 0, 1),
        packWord5(1, 1, 1, 1, 1)
};

static const uint32_t number6_5x5[] = {
        packWord5(0, 1, 1, 1, 1),
        packWord5(1, 0, 0, 0, 0),
        packWord5(1, 1, 1, 1, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 1, 1, 1, 1)
};

static const uint32_t number7_5x5[] = {
        packWord5(1, 1, 1, 1, 1),
        packWord5(0, 0, 0, 0, 1),
        packWord5(0, 0, 0, 0, 1),
        packWord5(0, 0, 0, 0, 1),
        packWord5(0, 0, 0, 0, 1)
};

static const uint32_t number8_5x5[] = {
        packWord5(1, 1, 1, 1, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 1, 1, 1, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 1, 1, 1, 1)
};

static const uint32_t number9_5x5[] = {
        packWord5(1, 1, 1, 1, 1),
        packWord5(1, 0, 0, 0, 1),
        packWord5(1, 1, 1, 1, 1),
        packWord5(0, 0, 0, 0, 1),
        packWord5(1, 1, 1, 1, 1)
};

static const uint32_t letterBLANK_5x5[] = {
        packWord5(0, 0, 0, 0, 0),
        packWord5(0, 0, 0, 0, 0),
        packWord5(0, 0, 0, 0, 0),
        packWord5(0, 0, 0, 0, 0),
        packWord5(0, 0, 0, 0, 0)
};

#endif // SPRITE_H
