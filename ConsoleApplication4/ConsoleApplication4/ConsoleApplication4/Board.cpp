#include "stdafx.h"
#include "Board.h"

#include <iostream>

#include "Util.h"
#include "Block.h"

Board::Board() {
    y = 1;
    for (int y = 0; y < BoardSizeY + 1; ++y) {
        for (int x = 0; x < BoardSizeX + 2; ++x) {
            BoardBlock[y][x].backColor = eColor::Black;
            BoardBlock[y][x].forgColor = eColor::Black;
            if(y == BoardSizeY || x == 0 || x == BoardSizeX + 1)
                BoardBlock[y][x] = 5;
            else
                BoardBlock[y][x] = 0;
        }
    }
}

void Board::DrawBoard() {
    for (int y = 0; y < BoardSizeY + 1; ++y) {
        for (int x = 0; x < BoardSizeX + 1; ++x) {

            //���̵� �� �׸���
            if (BoardBlock[y][x] == 11) {
                DrawBlock(x, y, "��");
                BoardBlock[y][x]--;
                continue;
            }
            //���̵� �� ����
            else if(BoardBlock[y][x] == 10) {
                DrawBlock(x, y, "  ", true);
                BoardBlock[y][x] = 0;
                continue;
            }

            //�Ϲ� �� Ư�� �� Ȯ��
            if (BoardBlock[y][x] > 0 && BoardBlock[y][x] < 4)
                BoardBlock[y][x]--;
            else {
                //Ư�� �� �ƹ��͵� ����.
                continue;
            }

            //���� �׷����ִ� �� �ȱ׸��� 1�θ���.
            if (BoardBlock[y][x] == 2) {
                BoardBlock[y][x]--;
            } else {
                //�Ϲ� �� �׸���
                if (BoardBlock[y][x] == 1) {
                    DrawBlock(x, y, "��");
                } else {
                    //���ڸ� �����
                    DrawBlock(x, y, "  ", true);
                }

            }
        }
    }
}

void Board::DrawBlock(int x, int y, char* str, bool isEmpty) {

    if(isEmpty)
        ClearConsoleToColors((int)eColor::Black, (int)eColor::Black);
    else
        ClearConsoleToColors((int)BoardBlock[y][x].forgColor, (int)BoardBlock[y][x].backColor);
    gotoxy(this->x + (x * 2), this->y + y);

    std::cout << str;
}

void Board::CheckLine() {
    for (int y = BoardSizeY; y > 0; --y) {
        int Count = 0;
        for (int x = 1; x < BoardSizeX + 1; ++x) {
            if (BoardBlock[y][x] == 4)
                Count++;
            else
                break;
        }

        if (Count == BoardSizeX) {
            DeletionLine(y);
            y--;
        }
    }
}

void Board::DeletionLine(int line) {
    for (int y = line; y > 0; --y) {
        int Count = 0;
        for (int x = 1; x < BoardSizeX + 1; ++x) {
            BoardBlock[y][x].Copy(BoardBlock[y - 1][x]);
            if (BoardBlock[y][x] > 0) {
                DrawBlock(x, y, "��");
                Count++;
            } else {
                DrawBlock(x, y, "  ", true);
            }
        }
        if (Count == 0)
            break;
    }
}

int Board::CheckMino(Tetrimino & mino) {
    for (int y = 0; y < mino.nimoSize; y++) {
        for (int x = 0; x < mino.nimoSize; x++) {
            int tempPos = mino.GetPos(x, y);
            if (mino.blockBorad[tempPos] > 0) {
                if(BoardBlock[mino.y + y][mino.x + x] > 3 && BoardBlock[mino.y + y][mino.x + x] < 10)
                    return BoardBlock[mino.y + y][mino.x + x].BlockState;
            }
        }
    }
    return 0;
}

void Board::DrawMino(Tetrimino& mino) {
    for (int y = 0; y < mino.nimoSize; y++) {
        for (int x = 0; x < mino.nimoSize; x++) {
            int tempPos = mino.GetPos(x, y);
            if (mino.blockBorad[tempPos] != 0) {
                //���̵� ���� ������ ����
                if (BoardBlock[mino.y + y][mino.x + x] > 9)
                    BoardBlock[mino.y + y][mino.x + x] = 0;

                BoardBlock[mino.y + y][mino.x + x].forgColor = mino.forgColor;
                BoardBlock[mino.y + y][mino.x + x].backColor = mino.backColor;
                BoardBlock[mino.y + y][mino.x + x] += mino.blockBorad[tempPos];
            }
        }
    }
}

void Board::DrawLock(Tetrimino & mino) {
    for (int y = 0; y < mino.nimoSize; y++) {
        for (int x = 0; x < mino.nimoSize; x++) {
            int tempPos = mino.GetPos(x, y);
            if (mino.blockBorad[tempPos] != 0) {
                BoardBlock[mino.y + y][mino.x + x] = 4;
            }
        }
    }
}

void Board::DrawGuide(Tetrimino & mino) {
    for (int y = 0; y < mino.nimoSize; y++) {
        for (int x = 0; x < mino.nimoSize; x++) {
            int tempPos = mino.GetPos(x, y);
            if (mino.blockBorad[tempPos] != 0) {
                BoardBlock[mino.y + y][mino.x + x].forgColor = mino.forgColor;
                BoardBlock[mino.y + y][mino.x + x].backColor = mino.backColor;
                BoardBlock[mino.y + y][mino.x + x] = 11;
            }
        }
    }
}
