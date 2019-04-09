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
	// ���ô��
	HideCursor();
	
	while (1) {
		// �]�w���a����l���A (�|�P������A�������Ŧa)
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
		
		// �]�w�D�Y����l��m
		int headPositionX = 5;
		int headPositionY = 5;

		// �]�w�D������l��m�A�]���P�D�Y�ۦP
		int tailPositionX = headPositionX;
		int tailPositionY = headPositionY;
		
		// �b���a���A���A�D�Y����m��m�@�ӳD��
		fieldCellStates[headPositionX][headPositionY] = STATE_SNAKE;
				
		// �]�w��������l��m�A�]�����a���H���䪺�@�ӪŦa
		int foodPositionX, foodPositionY;
		
		do {
			foodPositionX = rand() % FIELD_WIDTH;
			foodPositionY = rand() % FIELD_HEIGHT;
		} while (fieldCellStates[foodPositionX][foodPositionY] != STATE_EMPTY);
		
		// �b���a���A���A��������m��m�@�ӭ���
		fieldCellStates[foodPositionX][foodPositionY] = STATE_FOOD;

		// ����a�̷Ӳ{�b�����A�e�X�� ("��"�B"�D"�B"��" �� " ")
		ClearScreen();
		for (int x = 0; x < FIELD_WIDTH; x++) {
			for (int y = 0; y < FIELD_HEIGHT; y++) {
				const char *sprite;
				switch (fieldCellStates[x][y]) {
					case STATE_WALL:  sprite = "��"; break;
					case STATE_EMPTY: sprite = "�@"; break;
					case STATE_SNAKE: sprite = "�D"; break;
					case STATE_FOOD:  sprite = "��"; break;
				}
				PrintXY(x, y, sprite);
			}
		}

		
		// �]�w�D����l�e�i��V
		int snakeDirection = DIRECTION_RIGHT;
		
		int fieldCellDirections[FIELD_WIDTH][FIELD_HEIGHT];

		while (1) {

			// �p�G���V����V��
			if (IsArrowKeyHit()) {
				// �]�w�D�e�i����V���V������V���V
				snakeDirection = GetHitArrowKey();
			}

			// �b���a�W�����D���ʪ���V
			fieldCellDirections[headPositionX][headPositionY] =	snakeDirection;
			
			// �̷ӳD�e�i����V�A���ʳD�Y����m�@��
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

			// �p�G��������γD��
			if (state == STATE_WALL || state == STATE_SNAKE)
				break;
	
			// �p�G���쭹��
			if (state == STATE_FOOD) {
				// �]�w�s��������m�A�]�����a���H���䪺�@�ӪŦa
				do {
					foodPositionX = rand() % FIELD_WIDTH;
					foodPositionY = rand() % FIELD_HEIGHT;
				} while (fieldCellStates[foodPositionX][foodPositionY] != STATE_EMPTY);

				fieldCellStates[foodPositionX][foodPositionY] = STATE_FOOD;

        		// �b��������mø�s�@�� "��" 
				PrintXY(foodPositionX, foodPositionY, "��");
			} else {
        		// �N���a���A���A�D������m�קאּ�Ŧa
				fieldCellStates[tailPositionX][tailPositionY] = STATE_EMPTY;

 				// �b�D����mø�s�@�� " "
				PrintXY(tailPositionX, tailPositionY, "�@");

        		// �̷ӳ��a�W�b�D����m�������e�i����V���ʳD���@��
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

			// �N���a���A���A�D�Y����m�קאּ�D��  
			fieldCellStates[headPositionX][headPositionY] = STATE_SNAKE;

			// �b�D�Y��mø�s�@�� "�D"
			PrintXY(headPositionX, headPositionY, "�D");

			// ���ݤ@�p�q�ɶ�
			Wait(200);
		}

		// �L�X GameOver
		PrintXY(0, FIELD_HEIGHT, "Game Over!\n");
		Pause();
	}
	return 0;
}

// �H�U���禡�ڭ̪������ӥΡA��������갵��z
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
