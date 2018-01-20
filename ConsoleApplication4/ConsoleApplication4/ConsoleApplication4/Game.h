#pragma once

#include <random>
#include <chrono>

enum class eKey {
    SpaceBar = 32,
    LEFT = 75,
    RIGHT = 77,
    UP = 72,
    DOWN = 80,
};

class Game {
  private:
    Draw backGround;
    Board board;

    Tetrimino  GuideMino;
    Tetrimino  FutureMino;
    Tetrimino* CurrentMino = nullptr;
    Tetrimino* NextMino = nullptr;

    std::mt19937_64 ranNum;

  private:
    void Setup();
    void InitbackGround();

    void CreateNextMino();
    int CreateGuideMino();

    int GetRandom();
  public:
    Game();
    ~Game();

    void CurrentBlockCheck();
    void Update();
    void PreesKey(int key);

    void DrawBackground();
    void DrawBoard();
    void DrawBlock();
};

