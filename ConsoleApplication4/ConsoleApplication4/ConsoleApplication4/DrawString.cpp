// ConsoleApplication4.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "DrawString.h"

#include <iostream>

void Draw::SetColor(eColor ForgC, eColor BackC) {
    forgColor = ForgC;
    backColor = BackC;
}

void Draw::SetPos(int x, int y) {
    this->x = x;
    this->y = y;
}

void Draw::SetString(string str) {
    Parsing(str);
    line++;
}

void Draw::SetString(string str[], int size) {

    for (int i = 0; i < size; i++) {
        Parsing(str[i]);
        line++;
    }
}

void Draw::DrawingStr() {

    if (forgColor != eColor::None || backColor != eColor::None) {
        if (forgColor == eColor::None) {
            forgColor = eColor::White;
        } else if (backColor == eColor::None) {
            backColor = eColor::Black;
        }

        ClearConsoleToColors((int)forgColor, (int)backColor);
    }

    for (auto i : StringList) {
        gotoxy(i.x + x, i.y + y);
        cout << i.str;
    }
    ClearConsoleToColors((int)eColor::White, (int)eColor::Black);
}

void Draw::Parsing(string str) {
    string *temp = new string();
    int startPos = 0;
    for (size_t i = 0; i <= str.length(); i++) {
        if (str[i] != '\n' && str[i] != '\r' && str[i] != ' '&& str[i] != '\0') {
            if(temp->length() == 0)
                startPos = i;
            temp->append(&str[i], 1);
        } else {
            if (temp->length() > 0) {
                StringList.push_back(CustomString { startPos, line, *temp });
                temp = new string();
            }
        }
    }
}

