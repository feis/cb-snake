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
	// ���ô��
	HideCursor();
	
	while (1) {
		// �]�w���a����l���A (�|�P������A�������Ŧa)
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
		
		// �]�w�D�Y����l��m
		int headPositionX = 5;
		int headPositionY = 5;

		// �]�w�D������l��m�A�]���P�D�Y�ۦP
		int tailPositionX = headPositionX;
		int tailPositionY = headPositionY;
		
		// �b���a���A���A�D�Y����m��m�@�ӳD��
		fieldCellStates[headPositionX][headPositionY] = SNAKE;
				
		// �]�w��������l��m�A�]�����a���H���䪺�@�ӪŦa
		int foodPositionX, foodPositionY;
		
		do {
			foodPositionX = rand() % WIDTH;
			foodPositionY = rand() % HEIGHT;
		} while (fieldCellStates[foodPositionX][foodPositionY] != STATE_EMPTY);
		
		// �b���a���A���A��������m��m�@�ӭ���
		fieldCellStates[foodPositionX][foodPositionY] = FOOD;

		// ����a�̷Ӳ{�b�����A�e�X�� ("��"�B"�D"�B"��" �� " ")
		ClearScreen();
		for (int x = 0; x < WIDTH; x++) {
			for (int y = 0; y < HEIGHT; y++) {
				const char *sprite;
				switch (fieldCellStates[x][y]) {
					case WALL:  sprite = "��"; break;
					case STATE_EMPTY: sprite = "�@"; break;
					case SNAKE: sprite = "�D"; break;
					case FOOD:  sprite = "��"; break;
				}
				PrintXY(x, y, sprite);
			}
		}

		
		// �]�w�D����l�e�i��V
		Direction snakeDirection = RIGHT;
		
		// �������a�W�C�Ӧ�m�D�Ҳ��ʹL����V 
		Direction fieldCellDirections[WIDTH][HEIGHT];

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

			// �p�G��������γD��
			if (state == WALL || state == SNAKE)
				break;
	
			// �p�G���쭹��
			if (state == FOOD) {
				// �]�w�s��������m�A�]�����a���H���䪺�@�ӪŦa
				do {
					foodPositionX = rand() % WIDTH;
					foodPositionY = rand() % HEIGHT;
				} while (fieldCellStates[foodPositionX][foodPositionY] != STATE_EMPTY);

				fieldCellStates[foodPositionX][foodPositionY] = FOOD;

        		// �b��������mø�s�@�� "��" 
				PrintXY(foodPositionX, foodPositionY, "��");
			} else {
        		// �N���a���A���A�D������m�קאּ�Ŧa
				fieldCellStates[tailPositionX][tailPositionY] = STATE_EMPTY;

 				// �b�D����mø�s�@�� " "
				PrintXY(tailPositionX, tailPositionY, "�@");

        		// �̷ӳ��a�W�b�D����m�������e�i����V���ʳD���@��
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

			// �N���a���A���A�D�Y����m�קאּ�D��  
			fieldCellStates[headPositionX][headPositionY] = SNAKE;

			// �b�D�Y��mø�s�@�� "�D"
			PrintXY(headPositionX, headPositionY, "�D");

			// ���ݤ@�p�q�ɶ�
			Wait(200);
		}

		// �L�X GameOver
		PrintXY(0, HEIGHT, "Game Over!\n");
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
