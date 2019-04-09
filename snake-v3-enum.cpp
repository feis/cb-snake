#include "Windows.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH  20
#define HEIGHT 15

typedef enum {
	WALL = -1,
	STATE_EMPTY,
	SNAKE,
	FOOD
} State;

typedef enum {
	NONE  = -1,
	UP,
	RIGHT,
	DOWN,
	LEFT
} Direction;

void HideCursor();
int IsArrowKeyHit();
Direction GetHitArrowKey();
void ClearScreen();
void PrintXY(int, int, const char *);
void Pause();
void Wait(int ms);

int main() {
	// 隱藏游標
	HideCursor();
	
	while (1) {
		// 設定場地的初始狀態 (四周為牆壁，中間為空地)
		State fieldCellStates[WIDTH][HEIGHT];
		for (int x = 0; x < WIDTH; x++) {
			for (int y = 0; y < HEIGHT; y++) {
				if (x == 0 || x == WIDTH - 1 ||
					y == 0 || y == HEIGHT - 1) {
					fieldCellStates[x][y] = WALL;
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
		fieldCellStates[headPositionX][headPositionY] = SNAKE;
				
		// 設定食物的初始位置，設成場地中隨機找的一個空地
		int foodPositionX, foodPositionY;
		
		do {
			foodPositionX = rand() % WIDTH;
			foodPositionY = rand() % HEIGHT;
		} while (fieldCellStates[foodPositionX][foodPositionY] != STATE_EMPTY);
		
		// 在場地狀態中，食物的位置放置一個食物
		fieldCellStates[foodPositionX][foodPositionY] = FOOD;

		// 把場地依照現在的狀態畫出來 ("牆"、"蛇"、"食" 或 " ")
		ClearScreen();
		for (int x = 0; x < WIDTH; x++) {
			for (int y = 0; y < HEIGHT; y++) {
				const char *sprite;
				switch (fieldCellStates[x][y]) {
					case WALL:  sprite = "牆"; break;
					case STATE_EMPTY: sprite = "　"; break;
					case SNAKE: sprite = "蛇"; break;
					case FOOD:  sprite = "食"; break;
				}
				PrintXY(x, y, sprite);
			}
		}

		
		// 設定蛇的初始前進方向
		Direction snakeDirection = RIGHT;
		
		// 紀錄場地上每個位置蛇所移動過的方向 
		Direction fieldCellDirections[WIDTH][HEIGHT];

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
				case UP:
					headPositionY--;
					break;
				case RIGHT:
					headPositionX++;
					break;
				case DOWN:
					headPositionY++;
					break;
				case LEFT:
					headPositionX--;
					break;
			}
			
			State state = fieldCellStates[headPositionX][headPositionY];

			// 如果撞到牆壁或蛇身
			if (state == WALL || state == SNAKE)
				break;
	
			// 如果撞到食物
			if (state == FOOD) {
				// 設定新的食物位置，設成場地中隨機找的一個空地
				do {
					foodPositionX = rand() % WIDTH;
					foodPositionY = rand() % HEIGHT;
				} while (fieldCellStates[foodPositionX][foodPositionY] != STATE_EMPTY);

				fieldCellStates[foodPositionX][foodPositionY] = FOOD;

        		// 在食物的位置繪製一個 "食" 
				PrintXY(foodPositionX, foodPositionY, "食");
			} else {
        		// 將場地狀態中，蛇尾的位置修改為空地
				fieldCellStates[tailPositionX][tailPositionY] = STATE_EMPTY;

 				// 在蛇尾位置繪製一個 " "
				PrintXY(tailPositionX, tailPositionY, "　");

        		// 依照場地上在蛇尾位置紀錄的前進的方向移動蛇尾一格
				switch (fieldCellDirections[tailPositionX][tailPositionY]) {
					case UP:
						tailPositionY--;
						break;
					case RIGHT:
						tailPositionX++;
						break;
					case DOWN:
						tailPositionY++;
						break;
					case LEFT:
						tailPositionX--;
						break;
				}			
			}

			// 將場地狀態中，蛇頭的位置修改為蛇身  
			fieldCellStates[headPositionX][headPositionY] = SNAKE;

			// 在蛇頭位置繪製一個 "蛇"
			PrintXY(headPositionX, headPositionY, "蛇");

			// 等待一小段時間
			Wait(200);
		}

		// 印出 GameOver
		PrintXY(0, HEIGHT, "Game Over!\n");
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

Direction GetHitArrowKey() {
	switch (_getch()) {
		case 72: return UP;
		case 77: return RIGHT;
		case 80: return DOWN;
		case 75: return LEFT;
	}
	return NONE;
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
