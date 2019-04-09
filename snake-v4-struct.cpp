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
	// ���ô��
	HideCursor();
	
	while (1) {

		Field field;
		
		// �]�w���a����l���A (�|�P������A�������Ŧa)		
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
		
		// �]�w�D�Y����l��m
		Position head = {5, 5};

		// �]�w�D������l��m�A�]���P�D�Y�ۦP
		Position tail = head;
		
		// �b���a���A���A�D�Y����m��m�@�ӳD��
		field.cells[head.x][head.y].state = STATE_SNAKE;
				
		// �]�w��������l��m�A�]�����a���H���䪺�@�ӪŦa
		Position food;
		
		do {
			food.x = rand() % FIELD_WIDTH;
			food.y = rand() % FIELD_HEIGHT;
		} while (field.cells[food.x][food.y].state != STATE_EMPTY);
		
		// �b���a���A���A��������m��m�@�ӭ���
		field.cells[food.x][food.y].state = STATE_FOOD;

		// ����a�̷Ӳ{�b�����A�e�X�� ("��"�B"�D"�B"��" �� " ")
		ClearScreen();
		for (int x = 0; x < FIELD_WIDTH; x++) {
			for (int y = 0; y < FIELD_HEIGHT; y++) {
				const char *sprite;
				switch (field.cells[x][y].state) {
					case STATE_WALL:  sprite = "��"; break;
					case STATE_EMPTY: sprite = "�@"; break;
					case STATE_SNAKE: sprite = "�D"; break;
					case STATE_FOOD:  sprite = "��"; break;
				}
				PrintXY(x, y, sprite);
			}
		}
		
		// �]�w�D����l�e�i��V
		Direction snakeDirection = DIRECTION_RIGHT;

		while (1) {
			// �p�G���V����V��
			if (IsArrowKeyHit()) {
				// �]�w�D�e�i����V���V������V���V
				snakeDirection = GetHitArrowKey();
			}

			// �b���a�W�����D���ʪ���V
			field.cells[head.x][head.y].direction = snakeDirection;
			
			// �̷ӳD�e�i����V�A���ʳD�Y����m�@��
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

			// �p�G��������γD��
			if (state == STATE_WALL || state == STATE_SNAKE)
				break;
	
			// �p�G���쭹��
			if (state == STATE_FOOD) {
				// �]�w�s��������m�A�]�����a���H���䪺�@�ӪŦa
				do {
					food.x = rand() % FIELD_WIDTH;
					food.y = rand() % FIELD_HEIGHT;
				} while (field.cells[food.x][food.y].state != STATE_EMPTY);

				field.cells[food.x][food.y].state = STATE_FOOD;

        		// �b��������mø�s�@�� "��" 
				PrintXY(food.x, food.y, "��");
			} else {
        		// �N���a���A���A�D������m�קאּ�Ŧa
				field.cells[tail.x][tail.y].state = STATE_EMPTY;

 				// �b�D����mø�s�@�� " "
				PrintXY(tail.x, tail.y, "�@");

        		// �̷ӳ��a�W�b�D����m�������e�i����V���ʳD���@��
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

			// �N���a���A���A�D�Y����m�קאּ�D��  
			field.cells[head.x][head.y].state = STATE_SNAKE;

			// �b�D�Y��mø�s�@�� "�D"
			PrintXY(head.x, head.y, "�D");

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
