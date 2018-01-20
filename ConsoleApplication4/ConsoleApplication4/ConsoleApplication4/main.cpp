


#include "stdafx.h"

#include <iostream>
#include <conio.h>
#include <ctime>

#include "DrawString.h"
#include "Board.h"
#include "Block.h"
#include "Game.h"

void main() {

    clock_t tick = clock();

    Game game;

    game.DrawBackground();

    while (true) {
        clock_t now = clock();

        if (_kbhit()) {
            int nInput = _getch();
            if (nInput == 0xE0 || nInput == 0) {
                nInput = _getch();

                game.PreesKey(nInput);
            } else if (nInput == 32) {
                game.PreesKey(nInput);
                tick -= 1000;
            }
        }

        game.DrawBlock();
        game.DrawBoard();

        if (now - tick < 1000)
            continue;
        game.Update();
        tick = now;
    }
}