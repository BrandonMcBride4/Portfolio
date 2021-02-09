#ifndef LAB8DEFS_H
#define LAB8DEFS_H

#define STRAIGHT 1
#define CORNER 0
#define SIMPTRIS_SEED 32773

#define HORI 0
#define VERT 1

#define BOT_LEFT 0
#define BOT_RIGHT 1
#define TOP_RIGHT 2
#define TOP_LEFT 3

#define MAX_IND_PIECE_INFO_STRUCT 20
#define MAX_IND_CMD_INFO_STRUCT 20

typedef enum
{
    SLIDE_R,
    SLIDE_L,
    ROTATE_CW,
    ROTATE_CCW,
} Cmd;

//0     1   2    3
//*     *   **   **
//**   **    *   *

struct PieceInfo
{
    int ident; //unique
    int type;
    int orient;
    int col;
};

struct CmdInfo
{
    int pieceId; //unique
    int cmd;
};

#endif // LAB8DEFS_H