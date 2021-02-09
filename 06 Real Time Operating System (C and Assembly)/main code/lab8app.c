#include "clib.h"
#include "simptris.h"
#include "lab8defs.h"

#include "yakk.h"

#define TASK_STACK_SIZE 512

#define NUM_COLS 6

#define RIGHT 1
#define LEFT 0
#define CW 1
#define CCW 0

#define SIZE_NEW_PIECES 20
#define SIZE_CMDS 20

#define FLATS_MASK 0x00FF

//tasks stacks
int stkNewPieces[TASK_STACK_SIZE];
int stkCommands[TASK_STACK_SIZE];
int stkStat[TASK_STACK_SIZE];

//structs
void *qNewPieces[SIZE_NEW_PIECES];
void *qCmds[SIZE_CMDS];
YKQ *newPieces;
YKQ *cmds;

YKSEM *sReceived; //signaling

struct PieceInfo hPieces[MAX_IND_PIECE_INFO_STRUCT]; //stored in heap, since ptrs
struct CmdInfo hCmds[MAX_IND_PIECE_INFO_STRUCT]; //stored in heap, since ptrs

//other
int indHCmds = 0;

int numActivePieces = 0;
int prevIsCorner = 0;
int isFlat = 1;


extern unsigned ScreenBitMap0;
extern unsigned ScreenBitMap1;
extern unsigned ScreenBitMap2;
extern unsigned ScreenBitMap3;
extern unsigned ScreenBitMap4;
extern unsigned ScreenBitMap5;


void printPiece(struct PieceInfo *p)
{
    printString("ID:: ");
    printInt(p->ident);
    printString("\nType:: ");
    printInt(p->type);
    printString("\nOrientation:: ");
    printInt(p->orient);
    printString("\nColumn:  ");
    printInt(p->col);
    printString("\n\n");
}

void incIndHCmds(void)
{
    indHCmds++;
    //rollover
    if (indHCmds >= MAX_IND_CMD_INFO_STRUCT)
    {
        indHCmds = 0;
    }
}

//fine if interrupted because new piece needs to wait turn on queue
void moveLeft(int cols, struct PieceInfo *p)
{
    int i;
    int status;
    for (i = 0; i < cols; i++)
    {
        hCmds[indHCmds].cmd = SLIDE_L;
        hCmds[indHCmds].pieceId = p->ident;
        if (p->col > 0)
            p->col--;
        status = YKQPost(cmds, (void *) &hCmds[indHCmds]);
        if (!status)
        {
            printString("ERROR: overflow on cmds queue\n");
        }
        incIndHCmds();
    }
}

//fine if interrupted because new piece needs to wait turn on queue
void moveRight(int cols, struct PieceInfo *p)
{
    int i;
    int status;
    for (i = 0; i < cols; i++)
    {
        hCmds[indHCmds].cmd = SLIDE_R;
        hCmds[indHCmds].pieceId = p->ident;
        if (p->col < 5)
            p->col++;
        status = YKQPost(cmds, (void *) &hCmds[indHCmds]);
        if (!status)
        {
            printString("ERROR: overflow on cmds queue\n");
        }
        incIndHCmds();
    }
}

//fine if interrupted because new piece needs to wait turn on queue
void rotate_cw(int rots, struct PieceInfo *p)
{
    int i;
    int status;
    for (i = 0; i < rots; i++)
    {
        hCmds[indHCmds].cmd = ROTATE_CW;
        hCmds[indHCmds].pieceId = p->ident;
        //!!! Orientation is not change
        status = YKQPost(cmds, (void *) &hCmds[indHCmds]);
        if (!status)
        {
            printString("ERROR: overflow on cmds queue\n");
        }
        incIndHCmds();
    }
}

//fine if interrupted because new piece needs to wait turn on queue
void rotate_ccw(int rots, struct PieceInfo *p)
{
    int i;
    int status;
    for (i = 0; i < rots; i++)
    {
        hCmds[indHCmds].cmd = ROTATE_CCW;
        hCmds[indHCmds].pieceId = p->ident;
        //!!! Orientation is not change
        status = YKQPost(cmds, (void *) &hCmds[indHCmds]);
        if (!status)
        {
            printString("ERROR: overflow on cmds queue\n");
        }
        incIndHCmds();
    }
}

//put right
//fine if interrupted because new piece needs to wait turn on queue
void placeStraight(struct PieceInfo *piece)
{
    if (ScreenBitMap5 & FLATS_MASK)
    {
        switch (piece->orient)
        {
            case HORI:
                moveLeft(piece->col - 1, piece);
                break;
            case VERT:
                if (piece->col == 0)
                {
                    moveRight(1, piece);
                }
                else
                {
                    moveLeft(piece->col - 1, piece);
                }
                rotate_ccw(1, piece);
                break;
        }
    }
    else
    {
        switch (piece->orient)
        {
            case HORI:
                moveRight((NUM_COLS - 1) - (piece->col + 1), piece);
                break;
            case VERT:
                if (piece->col == 5)
                {
                    moveLeft(1, piece);
                }
                else
                {
                    moveRight((NUM_COLS - 1) - (piece->col + 1), piece);
                }
                rotate_ccw(1, piece);
                break;
        }
    }
}

//int isFlat(struct PieceInfo * p)
//{
//    if (p->prevCorner && numActivePieces > 1)
//    {
//        if (ScreenBitMap0 == ScreenBitMap1 && ScreenBitMap1 == ScreenBitMap2)
//            return 0;
//        else
//            return 1;
//    }
//    else
//    {
//        return ScreenBitMap0 == ScreenBitMap1 && ScreenBitMap1 == ScreenBitMap2;
//    }
//}

// Piece should be in top right position
void placeCornerInColTwo(struct PieceInfo *piece)
{
    if (piece->col > 2)
    {
        moveLeft(piece->col - 2, piece);
    }
    if (piece->col < 2)
    {
        moveRight(2 - piece->col, piece);
    }
}

//put left
//fine if interrupted because new piece needs to wait turn on queue
void placeCorner(struct PieceInfo *piece)
{
    if (piece->col == 0)
    {
        moveRight(1, piece);
    }
    else if (piece->col == 5)
    {
        moveLeft(1, piece);
    }
    if (isFlat)
    {
        switch (piece->orient)
        {

            case BOT_LEFT:
                moveLeft(piece->col, piece);
                break;
            case BOT_RIGHT:
                rotate_cw(1, piece);
                moveLeft(piece->col, piece);
                break;
            case TOP_RIGHT:
                rotate_cw(2, piece);
                moveLeft(piece->col, piece);
                break;
            case TOP_LEFT:
                rotate_ccw(1, piece);
                moveLeft(piece->col, piece);
                break;
        }
    }
    else
    {
        switch (piece->orient)
        {
            case BOT_LEFT:
                rotate_cw(2, piece);
                placeCornerInColTwo(piece);
                break;
            case BOT_RIGHT:
                rotate_ccw(1, piece);
                placeCornerInColTwo(piece);
                break;
            case TOP_RIGHT:
                placeCornerInColTwo(piece);
                break;
            case TOP_LEFT:
                rotate_cw(1, piece);
                placeCornerInColTwo(piece);
                break;
        }
    }
}

void newPiecesTask(void)
{
    struct PieceInfo *piece;
    int prevIsCorner = 0;
    int numConseqCorner = 0;
    printString("Welcome to the newPiecesTask\r\n");
    //simptris
//    printString("Starting Simptris\r\n");
    SeedSimptris((long) SIMPTRIS_SEED);
    StartSimptris();

    while (1)
    {
        piece = (struct PieceInfo *) YKQPend(newPieces); //block
//        printString("In newPieceTask\r\n");

        //find best orientation
        switch (piece->type)
        {
            case STRAIGHT:
                placeStraight(piece);
                break;
            case CORNER:
                placeCorner(piece);
                break;
            default:
                printString("ERROR: unrecognizable piece\r\n");
                break;
        }
//        prevIsCorner = piece->type == CORNER;
        if(piece->type == CORNER)
        {
            isFlat = !isFlat;
        }
    }
}

//!!! stop when piece is placed, in case it's early
void commandsTask(void)
{
    struct CmdInfo *cmdInfo;
    printString("Welcome to the commandsTask\r\n");

    while (1)
    {
        cmdInfo = (struct CmdInfo *) YKQPend(cmds); //block
//        printString("In commands\r\n");

        //find best orientation
        switch (cmdInfo->cmd)
        {
            case SLIDE_L:
                SlidePiece(cmdInfo->pieceId, LEFT);
                break;
            case SLIDE_R:
                SlidePiece(cmdInfo->pieceId, RIGHT);
                break;
            case ROTATE_CW:
                RotatePiece(cmdInfo->pieceId, CW);
                break;
            case ROTATE_CCW:
                RotatePiece(cmdInfo->pieceId, CCW);
                break;
            default:
                printString("ERROR: unrecognizable cmd\r\n");
                break;
        }

        YKSemPend(sReceived); //can only handle one cmd at a time
    }
}

void statTask(void)           /* tracks statistics */
{
    unsigned max, switchCount, idleCount;
    int tmp;

    YKDelayTask(1);
    printString("Welcome to Simptris\r\n");
    printString("Determining CPU capacity\r\n");
    YKDelayTask(1);
    YKIdleCount = 0;
    YKDelayTask(5);
    max = YKIdleCount / 25;
    YKIdleCount = 0;

    // Create tasks
    YKNewTask(newPiecesTask, (void *) &stkNewPieces[TASK_STACK_SIZE], 1);
    YKNewTask(commandsTask, (void *) &stkCommands[TASK_STACK_SIZE], 2);

    while (1)
    {
        YKDelayTask(20);

        YKEnterMutex();
        switchCount = YKCtxSwCount;
        idleCount = YKIdleCount;
        YKExitMutex();

        printString("<CS: ");
        printInt((int) switchCount);
        printString(", CPU: ");
        tmp = (int) (idleCount / max);
        printInt(100 - tmp);
        printString(">\r\n");

        YKEnterMutex();
        YKCtxSwCount = 0;
        YKIdleCount = 0;
        YKExitMutex();
    }
}

void main(void)
{
    YKInitialize();

    // Create queues
    newPieces = YKQCreate(qNewPieces, SIZE_NEW_PIECES);
    cmds = YKQCreate(qCmds, SIZE_CMDS);

    // Create emaphores
    sReceived = YKSemCreate(0);

    printString("Initialized\n");

    // tasks
    YKNewTask(statTask, (void *) &stkStat[TASK_STACK_SIZE], 0); //!!! priority
    YKRun();
}


