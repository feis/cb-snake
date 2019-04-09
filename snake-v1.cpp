#include "Windows.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

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
		// 設定場地的初始狀態 (四周為牆壁，中間為空地) (0 表示空, -1 表示牆) 
		int fieldCellStates[20][15];
		for (int x = 0; x < 20; x++) {
			for (int y = 0; y < 15; y++) {
				if (x == 0 || x == 19 || y == 0 || y == 14)	{
					fieldCellStates[x][y] = -1;
				} else {
					fieldCellStates[x][y] = 0;
				}
			}
		}
		
		// 設定蛇頭的初始位置
		int headPositionX = 5;
		int headPositionY = 5;

		// 設定蛇尾的初始位置，設成與蛇頭相同
		int tailPositionX = headPositionX;
		int tailPositionY = headPositionY;
		
		// 在場地狀態中，蛇頭的位置放置一個蛇身 (1: 表示蛇身)
		fieldCellStates[headPositionX][headPositionY] = 1;
				
		// 設定食物的初始位置，設成場地中隨機找的一個空地
		int foodPositionX, foodPositionY;
		
		do {
			foodPositionX = rand() % 20;
			foodPositionY = rand() % 15;
		} while (fieldCellStates[foodPositionX][foodPositionY] != 0);
		
		// 在場地狀態中，食物的位置放置一個食物 (2: 表示食物)
		fieldCellStates[foodPositionX][foodPositionY] = 2;

		// 把場地依照現在的狀態畫出來 ("牆"、"蛇"、"食" 或 " ")
		ClearScreen();
		for (int x = 0; x < 20; x++) {
			for (int y = 0; y < 15; y++) {
				const char *sprite;
				switch (fieldCellStates[x][y]) {
					case -1: sprite = "牆"; break;
					case 0: sprite = "　"; break;
					case 1: sprite = "蛇"; break;
					case 2: sprite = "食"; break;
				}
				PrintXY(x, y, sprite);
			}
		}

		
		// 設定蛇的初始前進方向 (0: 上, 1: 右, 2: 下, 3: 左)
		int snakeDirection = 1;
		
		int fieldCellDirections[20][15];

		while (1) {

			// 如果有敲擊方向鍵
			if (IsArrowKeyHit()) {
				// 設定蛇前進的方向為敲擊的方向鍵方向 (0: 上, 1: 右, 2: 下, 3: 左)
				snakeDirection = GetHitArrowKey();
			}

			// 在場地上紀錄蛇移動的方向
			fieldCellDirections[headPositionX][headPositionY] =	snakeDirection;
			
			// 依照蛇前進的方向，移動蛇頭的位置一格
			switch (snakeDirection) {
				case 0:
					headPositionY--;
					break;
				case 1:
					headPositionX++;
					break;
				case 2:
					headPositionY++;
					break;
				case 3:
					headPositionX--;
					break;
			}
			
			int state = fieldCellStates[headPositionX][headPositionY];

			// 如果撞到牆壁或蛇身 (-1: 牆壁, 1 蛇身)
			if (state == -1 || state == 1)
				break;
	
			// 如果撞到食物 (2: 食物)
			if (state == 2) {
				// 設定新的食物位置，設成場地中隨機找的一個空地
				do {
					foodPositionX = rand() % 20;
					foodPositionY = rand() % 15;
				} while (fieldCellStates[foodPositionX][foodPositionY] != 0);

				fieldCellStates[foodPositionX][foodPositionY] = 2;

        		// 在食物的位置繪製一個 "食" 
				PrintXY(foodPositionX, foodPositionY, "食");
			} else {
        		// 將場地狀態中，蛇尾的位置修改為空地
				fieldCellStates[tailPositionX][tailPositionY] = 0;

 				// 在蛇尾位置繪製一個 " "
				PrintXY(tailPositionX, tailPositionY, "　");

        		// 依照場地上在蛇尾位置紀錄的前進的方向移動蛇尾一格
				switch (fieldCellDirections[tailPositionX][tailPositionY]) {
					case 0:
						tailPositionY--;
						break;
					case 1:
						tailPositionX++;
						break;
					case 2:
						tailPositionY++;
						break;
					case 3:
						tailPositionX--;
						break;
				}			
			}

			// 將場地狀態中，蛇頭的位置修改為蛇身  
			fieldCellStates[headPositionX][headPositionY] = 1;

			// 在蛇頭位置繪製一個 "蛇"
			PrintXY(headPositionX, headPositionY, "蛇");

			// 等待一小段時間
			Wait(200);
		}

		// 印出 GameOver
		PrintXY(0, 15, "Game Over!\n");
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
	// (0: 上, 1 : 右, 2 : 下, 3 : 左)
	switch (_getch()) {
		case 72: return 0;
		case 77: return 1;
		case 80: return 2;
		case 75: return 3;
	}
	return -1;
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
