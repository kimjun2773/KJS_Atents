#pragma once

#include "Block.h"


class Tetrimino;

class Board {
  public:
    Board();
    int x, y;
    static const int BoardSizeX = 10, BoardSizeY = 20;
    Block BoardBlock[BoardSizeY + 1][BoardSizeX + 2];

    void DrawBoard();
    void DrawBlock(int x, int y, char* str, bool isEmpty = false);

    void CheckLine();
    void DeletionLine(int line);

    int CheckMino(Tetrimino& mino);
    void DrawMino(Tetrimino& mino);
    void DrawLock(Tetrimino& mino);
    void DrawGuide(Tetrimino& mino);
};