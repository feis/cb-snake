#include "Windows.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define FIELD_WIDTH  20
#define FIELD_HEIGHT 15

typedef enum {
	STATE_WALL = -1,
	STATE_EMPTY,
	STATE_SNAKE,
	STATE_FOOD
} State;

typedef enum {
	DIRECTION_NONE  = -1,
	DIRECTION_UP,
	DIRECTION_RIGHT,
	DIRECTION_DOWN,
	DIRECTION_LEFT
} Direction;

typedef struct {
	int x;
	int y;
} Position;

typedef struct {
	State state;
	Direction direction;
} Cell;

typedef struct {
	Cell cells[FIELD_WIDTH][FIELD_HEIGHT];
} Field;

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

		Field field;
		
		// 設定場地的初始狀態 (四周為牆壁，中間為空地)		
		for (int x = 0; x < FIELD_WIDTH; x++) {
			for (int y = 0; y < FIELD_HEIGHT; y++) {
				if (x == 0 || x == FIELD_WIDTH - 1 ||
					y == 0 || y == FIELD_HEIGHT - 1) {
					field.cells[x][y].state = STATE_WALL;
				} else {
					field.cells[x][y].state = STATE_EMPTY;
				}
			}
		}
		
		// 設定蛇頭的初始位置
		Position head = {5, 5};

		// 設定蛇尾的初始位置，設成與蛇頭相同
		Position tail = head;
		
		// 在場地狀態中，蛇頭的位置放置一個蛇身
		field.cells[head.x][head.y].state = STATE_SNAKE;
				
		// 設定食物的初始位置，設成場地中隨機找的一個空地
		Position food;
		
		do {
			food.x = rand() % FIELD_WIDTH;
			food.y = rand() % FIELD_HEIGHT;
		} while (field.cells[food.x][food.y].state != STATE_EMPTY);
		
		// 在場地狀態中，食物的位置放置一個食物
		field.cells[food.x][food.y].state = STATE_FOOD;

		// 把場地依照現在的狀態畫出來 ("牆"、"蛇"、"食" 或 " ")
		ClearScreen();
		for (int x = 0; x < FIELD_WIDTH; x++) {
			for (int y = 0; y < FIELD_HEIGHT; y++) {
				const char *sprite;
				switch (field.cells[x][y].state) {
					case STATE_WALL:  sprite = "牆"; break;
					case STATE_EMPTY: sprite = "　"; break;
					case STATE_SNAKE: sprite = "蛇"; break;
					case STATE_FOOD:  sprite = "食"; break;
				}
				PrintXY(x, y, sprite);
			}
		}
		
		// 設定蛇的初始前進方向
		Direction snakeDirection = DIRECTION_RIGHT;

		while (1) {
			// 如果有敲擊方向鍵
			if (IsArrowKeyHit()) {
				// 設定蛇前進的方向為敲擊的方向鍵方向
				snakeDirection = GetHitArrowKey();
			}

			// 在場地上紀錄蛇移動的方向
			field.cells[head.x][head.y].direction = snakeDirection;
			
			// 依照蛇前進的方向，移動蛇頭的位置一格
			switch (snakeDirection) {
				case DIRECTION_UP:
					head.y--;
					break;
				case DIRECTION_RIGHT:
					head.x++;
					break;
				case DIRECTION_DOWN:
					head.y++;
					break;
				case DIRECTION_LEFT:
					head.x--;
					break;
			}
			
			State state = field.cells[head.x][head.y].state;

			// 如果撞到牆壁或蛇身
			if (state == STATE_WALL || state == STATE_SNAKE)
				break;
	
			// 如果撞到食物
			if (state == STATE_FOOD) {
				// 設定新的食物位置，設成場地中隨機找的一個空地
				do {
					food.x = rand() % FIELD_WIDTH;
					food.y = rand() % FIELD_HEIGHT;
				} while (field.cells[food.x][food.y].state != STATE_EMPTY);

				field.cells[food.x][food.y].state = STATE_FOOD;

        		// 在食物的位置繪製一個 "食" 
				PrintXY(food.x, food.y, "食");
			} else {
        		// 將場地狀態中，蛇尾的位置修改為空地
				field.cells[tail.x][tail.y].state = STATE_EMPTY;

 				// 在蛇尾位置繪製一個 " "
				PrintXY(tail.x, tail.y, "　");

        		// 依照場地上在蛇尾位置紀錄的前進的方向移動蛇尾一格
				switch (field.cells[tail.x][tail.y].direction) {
					case DIRECTION_UP:
						tail.y--;
						break;
					case DIRECTION_RIGHT:
						tail.x++;
						break;
					case DIRECTION_DOWN:
						tail.y++;
						break;
					case DIRECTION_LEFT:
						tail.x--;
						break;
				}			
			}

			// 將場地狀態中，蛇頭的位置修改為蛇身  
			field.cells[head.x][head.y].state = STATE_SNAKE;

			// 在蛇頭位置繪製一個 "蛇"
			PrintXY(head.x, head.y, "蛇");

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

Direction GetHitArrowKey() {
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
