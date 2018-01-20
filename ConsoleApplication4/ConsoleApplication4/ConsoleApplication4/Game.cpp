#include "stdafx.h"

#include "Board.h"
#include "Block.h"
#include "DrawString.h"
#include "Util.h"

#include "Game.h"

void Game::Setup() {
    SetCursor(false);
    CreateNextMino();
    CurrentBlockCheck();
}

void Game::InitbackGround() {
    string out[] = {
        "бсбсбсбсбсбсбсбсбсбсбсбс",
        "бс                    бс",
        "бс                    бс",
        "бс                    бс",
        "бс                    бс",
        "бс                    бс",
        "бс                    бс",
        "бс                    бс",
        "бс                    бс",
        "бс                    бс",
        "бс                    бс",
        "бс                    бс",
        "бс                    бс",
        "бс                    бс",
        "бс                    бс",
        "бс                    бс",
        "бс                    бс",
        "бс                    бс",
        "бс                    бс",
        "бс                    бс",
        "бс                    бс",
        "бсбсбсбсбсбсбсбсбсбсбсбс",
    };
    backGround.SetString(out, sizeof(out) / sizeof(out[0]));
    backGround.SetColor(eColor::Red, eColor::Black);
}

Game::Game() {
    Setup();
    InitbackGround();
}

Game::~Game() {
}

void Game::CreateNextMino() {
    switch (GetRandom()) {
    case 0:
        NextMino = std::move(new Imino());
        break;
    case 1:
        NextMino = std::move(new Omino());
        break;
    case 2:
        NextMino = std::move(new Zmino());
        break;
    case 3:
        NextMino = std::move(new Smino());
        break;
    case 4:
        NextMino = std::move(new Jmino());
        break;
    case 5:
        NextMino = std::move(new Lmino());
        break;
    }
}

int Game::CreateGuideMino() {
    GuideMino.Copy(*CurrentMino);
    int count = 1;
    while (0 == board.CheckMino(GuideMino)) {
        GuideMino.y++;
        count++;
    }

    GuideMino.y -= 1;
    count -= 1;
    return count;
}

int Game::GetRandom() {
    random_device rn;
    ranNum.seed(rn());
    uniform_int_distribution<int> range(0, 5);
    return range(ranNum);
}

void Game::CurrentBlockCheck() {
    if (CurrentMino == nullptr) {
        CurrentMino = NextMino;
        CreateNextMino();
    }
}

void Game::Update() {
    CurrentBlockCheck();

    FutureMino.Copy(*CurrentMino);
    FutureMino.y++;

    if (board.CheckMino(FutureMino)) {
        CurrentMino->BlockLock();
    } else {
        CurrentMino->Copy(FutureMino);
    }
}

void Game::DrawBackground() {
    backGround.DrawingStr();
}

void Game::DrawBoard() {
    board.DrawBoard();
    board.CheckLine();
}

void Game::DrawBlock() {
    if (CurrentMino == nullptr)
        return;

    if (CreateGuideMino() > 0) {
        board.DrawGuide(GuideMino);
    }

    if (CurrentMino->State == 1)
        board.DrawLock(*CurrentMino);
    else
        board.DrawMino(*CurrentMino);

    if (CurrentMino->State) {
        delete CurrentMino;
        CurrentMino = nullptr;
    }
}

void Game::PreesKey(int key) {

    if (CurrentMino == nullptr)
        return;

    if (key == (int)eKey::LEFT) {
        FutureMino.Copy(*CurrentMino);
        FutureMino.x -= 1;
    } else if (key == (int)eKey::RIGHT) {
        FutureMino.Copy(*CurrentMino);
        FutureMino.x += 1;
    } else if (key == (int)eKey::UP) {
        FutureMino.Copy(*CurrentMino);
        FutureMino.NextDirect();
    } else if (key == (int)eKey::DOWN) {
        FutureMino.Copy(*CurrentMino);
        FutureMino.y += 1;
    } else if (key == (int)eKey::SpaceBar) {
        CurrentMino->Copy(GuideMino);
        return;
    } else {
        return;
    }

    if ( 0 == board.CheckMino(FutureMino)) {
        CurrentMino->Copy(FutureMino);
    }
}
