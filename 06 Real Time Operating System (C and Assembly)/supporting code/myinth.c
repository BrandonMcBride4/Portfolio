#include "clib.h"
#include "yakk.h"
#include "lab8defs.h"


//structs
extern YKQ *newPieces;
extern YKQ *cmds;
extern YKSEM *sReceived;

//heap
extern struct PieceInfo hPieces[];
int indPiecesHeap = 0;

//from clib.s
extern unsigned NewPieceID;
extern unsigned NewPieceType;
extern unsigned NewPieceOrientation;
extern unsigned NewPieceColumn;
//extern int numActivePieces;

int linesCleared = 0;

void updIndPiecesHeap(void)
{
    YKEnterISR(); //avoid out of bounds

    indPiecesHeap++;
    //rollover
    if (indPiecesHeap >= MAX_IND_PIECE_INFO_STRUCT)
    {
        indPiecesHeap = 0;
    }

    YKExitMutex();
}

void updPiecesHeap(int ind)
{
    YKEnterMutex(); //avoid inconsistency

    hPieces[ind].ident = NewPieceID;
    hPieces[ind].type = NewPieceType;
    hPieces[ind].orient = NewPieceOrientation;
    hPieces[ind].col = NewPieceColumn;

    YKExitMutex();
}

void isrResetHandler(void)
{
    exit(0);
}

void isrKeyboardHandler(void)
{
//    printString("\n\nKEYPRESS (");
//    printChar(KeyBuffer);
//    printString(") IGNORED\n");
}

void isrGameOverHandler(void)
{
    printString("\nGame Over\n");
    exit(0);
}

void isrNewPieceHandler(void)
{
    int status;
    YKEnterMutex(); //avoid overwrite

//    updPiecesHeap(indPiecesHeap); //now incase another piece comes
    hPieces[indPiecesHeap].ident = NewPieceID;
    hPieces[indPiecesHeap].type = NewPieceType;
    hPieces[indPiecesHeap].orient = NewPieceOrientation;
    hPieces[indPiecesHeap].col = NewPieceColumn;

    status = YKQPost(newPieces, (void *) &hPieces[indPiecesHeap]); //give it a ptr
//    printString("added piece\n");
    if (!status)
    {
        printString("ERROR: overflow on newPieces queue\n");
    }
//    updIndPiecesHeap();
    indPiecesHeap++;
    //rollover
    if (indPiecesHeap >= MAX_IND_PIECE_INFO_STRUCT)
    {
        indPiecesHeap = 0;
    }

//    numActivePieces++;

    YKExitMutex();
}

void isrReceivedCmdHandler(void)
{
    YKSemPost(sReceived);
}

void isrTouchdownHandler(void)
{
//    numActivePieces--;
}

void isrLineClearHandler(void)
{
    //nothing
}
