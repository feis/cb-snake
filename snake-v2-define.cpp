#include "Windows.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define FIELD_WIDTH  20
#define FIELD_HEIGHT 15

#define STATE_WALL -1
#define STATE_EMPTY 0
#define STATE_SNAKE 1
#define STATE_FOOD  2

#define DIRECTION_NONE  -1
#define DIRECTION_UP    0
#define DIRECTION_RIGHT 1
#define DIRECTION_DOWN  2
#define DIRECTION_LEFT  3

void HideCursor();
int IsArrowKeyHit();
int GetHitArrowKey();
void ClearScreen();
void PrintXY(int, int, const char *);
void Pause();
void Wait(int ms);

int main() {
	// 隱藏游標
	HideCursor();
	
	while (1) {
		// 設定場地的初始狀態 (四周為牆壁，中間為空地)
		int fieldCellStates[FIELD_WIDTH][FIELD_HEIGHT];
		for (int x = 0; x < FIELD_WIDTH; x++) {
			for (int y = 0; y < FIELD_HEIGHT; y++) {
				if (x == 0 || x == FIELD_WIDTH - 1 ||
					y == 0 || y == FIELD_HEIGHT - 1) {
					fieldCellStates[x][y] = STATE_WALL;
				} else {
					fieldCellStates[x][y] = STATE_EMPTY;
				}
			}
		}
		
		// 設定蛇頭的初始位置
		int headPositionX = 5;
		int headPositionY = 5;

		// 設定蛇尾的初始位置，設成與蛇頭相同
		int tailPositionX = headPositionX;
		int tailPositionY = headPositionY;
		
		// 在場地狀態中，蛇頭的位置放置一個蛇身
		fieldCellStates[headPositionX][headPositionY] = STATE_SNAKE;
				
		// 設定食物的初始位置，設成場地中隨機找的一個空地
		int foodPositionX, foodPositionY;
		
		do {
			foodPositionX = rand() % FIELD_WIDTH;
			foodPositionY = rand() % FIELD_HEIGHT;
		} while (fieldCellStates[foodPositionX][foodPositionY] != STATE_EMPTY);
		
		// 在場地狀態中，食物的位置放置一個食物
		fieldCellStates[foodPositionX][foodPositionY] = STATE_FOOD;

		// 把場地依照現在的狀態畫出來 ("牆"、"蛇"、"食" 或 " ")
		ClearScreen();
		for (int x = 0; x < FIELD_WIDTH; x++) {
			for (int y = 0; y < FIELD_HEIGHT; y++) {
				const char *sprite;
				switch (fieldCellStates[x][y]) {
					case STATE_WALL:  sprite = "牆"; break;
					case STATE_EMPTY: sprite = "　"; break;
					case STATE_SNAKE: sprite = "蛇"; break;
					case STATE_FOOD:  sprite = "食"; break;
				}
				PrintXY(x, y, sprite);
			}
		}

		
		// 設定蛇的初始前進方向
		int snakeDirection = DIRECTION_RIGHT;
		
		int fieldCellDirections[FIELD_WIDTH][FIELD_HEIGHT];

		while (1) {

			// 如果有敲擊方向鍵
			if (IsArrowKeyHit()) {
				// 設定蛇前進的方向為敲擊的方向鍵方向
				snakeDirection = GetHitArrowKey();
			}

			// 在場地上紀錄蛇移動的方向
			fieldCellDirections[headPositionX][headPositionY] =	snakeDirection;
			
			// 依照蛇前進的方向，移動蛇頭的位置一格
			switch (snakeDirection) {
				case DIRECTION_UP:
					headPositionY--;
					break;
				case DIRECTION_RIGHT:
					headPositionX++;
					break;
				case DIRECTION_DOWN:
					headPositionY++;
					break;
				case DIRECTION_LEFT:
					headPositionX--;
					break;
			}
			
			int state = fieldCellStates[headPositionX][headPositionY];

			// 如果撞到牆壁或蛇身
			if (state == STATE_WALL || state == STATE_SNAKE)
				break;
	
			// 如果撞到食物
			if (state == STATE_FOOD) {
				// 設定新的食物位置，設成場地中隨機找的一個空地
				do {
					foodPositionX = rand() % FIELD_WIDTH;
					foodPositionY = rand() % FIELD_HEIGHT;
				} while (fieldCellStates[foodPositionX][foodPositionY] != STATE_EMPTY);

				fieldCellStates[foodPositionX][foodPositionY] = STATE_FOOD;

        		// 在食物的位置繪製一個 "食" 
				PrintXY(foodPositionX, foodPositionY, "食");
			} else {
        		// 將場地狀態中，蛇尾的位置修改為空地
				fieldCellStates[tailPositionX][tailPositionY] = STATE_EMPTY;

 				// 在蛇尾位置繪製一個 " "
				PrintXY(tailPositionX, tailPositionY, "　");

        		// 依照場地上在蛇尾位置紀錄的前進的方向移動蛇尾一格
				switch (fieldCellDirections[tailPositionX][tailPositionY]) {
				case DIRECTION_UP:
					tailPositionY--;
					break;
				case DIRECTION_RIGHT:
					tailPositionX++;
					break;
				case DIRECTION_DOWN:
					tailPositionY++;
					break;
				case DIRECTION_LEFT:
					tailPositionX--;
					break;
				}			
			}

			// 將場地狀態中，蛇頭的位置修改為蛇身  
			fieldCellStates[headPositionX][headPositionY] = STATE_SNAKE;

			// 在蛇頭位置繪製一個 "蛇"
			PrintXY(headPositionX, headPositionY, "蛇");

			// 等待一小段時間
			Wait(200);
		}

		// 印出 GameOver
		PrintXY(0, FIELD_HEIGHT, "Game Over!\n");
		Pause();
	}
	return 0;
}

// 以下的函式我們直接拿來用，不解釋其實做原理
void HideCursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

int IsArrowKeyHit() {
	if (_kbhit() && _getch() == 224)
		return 1;
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	return 0;
}

int GetHitArrowKey() {
	switch (_getch()) {
		case 72: return DIRECTION_UP;
		case 77: return DIRECTION_RIGHT;
		case 80: return DIRECTION_DOWN;
		case 75: return DIRECTION_LEFT;
	}
	return DIRECTION_NONE;
}

void ClearScreen() {
	system("cls");
}

void PrintXY(int x, int y, const char *sprite) {
	COORD coord;
	coord.X = x * 2;
	coord.Y = y;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord);
	printf(sprite);
}

void Pause() {
	system("pause");
}

void Wait(int ms) {
	Sleep(ms);
}
