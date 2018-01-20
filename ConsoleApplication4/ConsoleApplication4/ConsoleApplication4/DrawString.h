#pragma once

#include <string>
#include <list>
#include "Util.h"

using namespace std;

class CustomString {
  public:
    int x, y;			//위치
    string str;			//문자열
};

class Draw {
  private:
    int x, y;
    eColor forgColor = eColor::None;
    eColor backColor = eColor::None;
    int line = 0;
    list<CustomString> StringList;
  public:
    void SetColor(eColor ForgC, eColor BackC);
    void SetPos(int x, int y);
    void SetString(string str);
	void SetString(string str[], int size);
    void DrawingStr();
  private:
    void Parsing(string str);
};