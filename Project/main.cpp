#include "field.h"
#include "utils.h"

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
		Direction snakeDirection = RIGHT;

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
			if (state == WALL || state == SNAKE)
				break;
	
			// �p�G���쭹��
			if (state == FOOD) {
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
		PrintXY(0, HEIGHT, "Game Over!\n");
		Pause();
	}
	return 0;
}
