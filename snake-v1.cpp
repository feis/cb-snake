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
	// ���ô�� 
	HideCursor();
	
	while (1) {
		// �]�w���a����l���A (�|�P������A�������Ŧa) (0 ��ܪ�, -1 �����) 
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
		
		// �]�w�D�Y����l��m
		int headPositionX = 5;
		int headPositionY = 5;

		// �]�w�D������l��m�A�]���P�D�Y�ۦP
		int tailPositionX = headPositionX;
		int tailPositionY = headPositionY;
		
		// �b���a���A���A�D�Y����m��m�@�ӳD�� (1: ��ܳD��)
		fieldCellStates[headPositionX][headPositionY] = 1;
				
		// �]�w��������l��m�A�]�����a���H���䪺�@�ӪŦa
		int foodPositionX, foodPositionY;
		
		do {
			foodPositionX = rand() % 20;
			foodPositionY = rand() % 15;
		} while (fieldCellStates[foodPositionX][foodPositionY] != 0);
		
		// �b���a���A���A��������m��m�@�ӭ��� (2: ��ܭ���)
		fieldCellStates[foodPositionX][foodPositionY] = 2;

		// ����a�̷Ӳ{�b�����A�e�X�� ("��"�B"�D"�B"��" �� " ")
		ClearScreen();
		for (int x = 0; x < 20; x++) {
			for (int y = 0; y < 15; y++) {
				const char *sprite;
				switch (fieldCellStates[x][y]) {
					case -1: sprite = "��"; break;
					case 0: sprite = "�@"; break;
					case 1: sprite = "�D"; break;
					case 2: sprite = "��"; break;
				}
				PrintXY(x, y, sprite);
			}
		}

		
		// �]�w�D����l�e�i��V (0: �W, 1: �k, 2: �U, 3: ��)
		int snakeDirection = 1;
		
		int fieldCellDirections[20][15];

		while (1) {

			// �p�G���V����V��
			if (IsArrowKeyHit()) {
				// �]�w�D�e�i����V���V������V���V (0: �W, 1: �k, 2: �U, 3: ��)
				snakeDirection = GetHitArrowKey();
			}

			// �b���a�W�����D���ʪ���V
			fieldCellDirections[headPositionX][headPositionY] =	snakeDirection;
			
			// �̷ӳD�e�i����V�A���ʳD�Y����m�@��
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

			// �p�G��������γD�� (-1: ���, 1 �D��)
			if (state == -1 || state == 1)
				break;
	
			// �p�G���쭹�� (2: ����)
			if (state == 2) {
				// �]�w�s��������m�A�]�����a���H���䪺�@�ӪŦa
				do {
					foodPositionX = rand() % 20;
					foodPositionY = rand() % 15;
				} while (fieldCellStates[foodPositionX][foodPositionY] != 0);

				fieldCellStates[foodPositionX][foodPositionY] = 2;

        		// �b��������mø�s�@�� "��" 
				PrintXY(foodPositionX, foodPositionY, "��");
			} else {
        		// �N���a���A���A�D������m�קאּ�Ŧa
				fieldCellStates[tailPositionX][tailPositionY] = 0;

 				// �b�D����mø�s�@�� " "
				PrintXY(tailPositionX, tailPositionY, "�@");

        		// �̷ӳ��a�W�b�D����m�������e�i����V���ʳD���@��
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

			// �N���a���A���A�D�Y����m�קאּ�D��  
			fieldCellStates[headPositionX][headPositionY] = 1;

			// �b�D�Y��mø�s�@�� "�D"
			PrintXY(headPositionX, headPositionY, "�D");

			// ���ݤ@�p�q�ɶ�
			Wait(200);
		}

		// �L�X GameOver
		PrintXY(0, 15, "Game Over!\n");
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
	// (0: �W, 1 : �k, 2 : �U, 3 : ��)
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
