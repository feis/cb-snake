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

void InitializeField(Field*);
void DrawField(const Field*);
Position RandomlySelectEmptyPosition(const Field*);
void PlaceSnake(Field*, Position);
void PlaceFood(Field*, Position);
void PlaceEmpty(Field*, Position);
void RecordDirection(Field*, Position, Direction);  
State GetState(const Field*, Position);
Direction GetDirection(const Field*, Position);
void Move(Position*, const Field*, Direction);	
void Print(Position, const char*);

int main() {
	// ���ô��
	HideCursor();
	
	while (1) {		
		Field field;
		
		// �]�w���a����l���A (�|�P������A�������Ŧa)		
		InitializeField(&field);			
		
		// �]�w�D�Y����l��m
		Position head = {5, 5};

		// �]�w�D������l��m�A�]���P�D�Y�ۦP
		Position tail = head;
		
		// �b���a���A���A�D�Y����m��m�@�ӳD��
		PlaceSnake(&field, head); 
			
		// �]�w��������l��m�A�]�����a���H���䪺�@�ӪŦa
		Position food = RandomlySelectEmptyPosition(&field);
		
		// �b���a���A���A��������m��m�@�ӭ���
		PlaceFood(&field, food);

		// ����a�̷Ӳ{�b�����A�e�X�� ("��"�B"�D"�B"��" �� " ")
		DrawField(&field);
		
		// �]�w�D����l�e�i��V
		Direction snakeDirection = DIRECTION_RIGHT;

		while (1) {
			// �p�G���V����V��
			if (IsArrowKeyHit()) {
				// �]�w�D�e�i����V���V������V���V
				snakeDirection = GetHitArrowKey();
			}

			// �b���a�W�����D���ʪ���V
			RecordDirection(&field, head, snakeDirection);
			
			// �̷ӳD�e�i����V�A���ʳD�Y����m�@��
			Move(&head, &field, snakeDirection);
			
			State state = GetState(&field, head);

			// �p�G��������γD��
			if (state == STATE_WALL || state == STATE_SNAKE)
				break;
	
			// �p�G���쭹��
			if (state == STATE_FOOD) {
				// �]�w�s��������m�A�]�����a���H���䪺�@�ӪŦa
				PlaceFood(&field, RandomlySelectEmptyPosition(&field));

        		// �b��������mø�s�@�� "��" 
				Print(food, "��");
			} else {
        		// �N���a���A���A�D������m�קאּ�Ŧa
        		PlaceEmpty(&field, tail);

 				// �b�D����mø�s�@�� " "
				Print(tail, "�@");

        		// �̷ӳ��a�W�b�D����m�������e�i����V���ʳD���@��
        		Move(&tail, &field, GetDirection(&field, tail));
			}

			// �N���a���A���A�D�Y����m�קאּ�D��  
			PlaceSnake(&field, head);

			// �b�D�Y��mø�s�@�� "�D"
			Print(head, "�D");

			// ���ݤ@�p�q�ɶ�
			Wait(200);
		}

		// �L�X GameOver
		PrintXY(0, FIELD_HEIGHT, "Game Over!\n");
		Pause();
	}
	return 0;
}

void InitializeField(Field* field) {
	for (int x = 0; x < FIELD_WIDTH; x++) {
		for (int y = 0; y < FIELD_HEIGHT; y++) {
			if (x == 0 || x == FIELD_WIDTH - 1 ||
				y == 0 || y == FIELD_HEIGHT - 1) {
				field->cells[x][y].state = STATE_WALL;
			} else {
				field->cells[x][y].state = STATE_EMPTY;
			}
		}
	}
}

void DrawField(const Field* field) {
	ClearScreen();
	for (int x = 0; x < FIELD_WIDTH; x++) {
		for (int y = 0; y < FIELD_HEIGHT; y++) {
			const char *sprite;
			switch (field->cells[x][y].state) {
				case STATE_WALL:  sprite = "��"; break;
				case STATE_EMPTY: sprite = "�@"; break;
				case STATE_SNAKE: sprite = "�D"; break;
				case STATE_FOOD:  sprite = "��"; break;
			}
			PrintXY(x, y, sprite);
		}
	}
}

Position RandomlySelectEmptyPosition(const Field* field) {		
	Position food; 
	do {
		food.x = rand() % FIELD_WIDTH;
		food.y = rand() % FIELD_HEIGHT;
	} while (field->cells[food.x][food.y].state != STATE_EMPTY);
	return food;
} 

void PlaceSnake(Field* field, Position pos) {
	field->cells[pos.x][pos.y].state = STATE_SNAKE;
}

void PlaceFood(Field* field, Position pos) {
	field->cells[pos.x][pos.y].state = STATE_FOOD;
}

void PlaceEmpty(Field* field, Position pos) {
	field->cells[pos.x][pos.y].state = STATE_EMPTY;
}
 
void RecordDirection(Field* field, Position pos, Direction direction) {
	field->cells[pos.x][pos.y].direction = direction;
}
  
State GetState(const Field* field, Position pos) {
	return field->cells[pos.x][pos.y].state;
}

Direction GetDirection(const Field* field, Position pos) {
	return field->cells[pos.x][pos.y].direction;
}

void Move(Position* pos, const Field* field, Direction direction) { 
	switch (direction) {
		case DIRECTION_UP:
			pos->y--;
			break;
		case DIRECTION_RIGHT:
			pos->x++;
			break;
		case DIRECTION_DOWN:
			pos->y++;
			break;
		case DIRECTION_LEFT:
			pos->x--;
			break;
	}
} 
	
void Print(Position pos, const char* s) {
	PrintXY(pos.x, pos.y, s);
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