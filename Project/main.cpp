#include "field.h"
#include "utils.h"

int main() {
	// 隱藏游標
	HideCursor();
	
	while (1) {
		
		Field field;
		
		// 設定場地的初始狀態 (四周為牆壁，中間為空地)		
		InitializeField(&field);			
		
		// 設定蛇頭的初始位置
		Position head = {5, 5};

		// 設定蛇尾的初始位置，設成與蛇頭相同
		Position tail = head;
		
		// 在場地狀態中，蛇頭的位置放置一個蛇身
		PlaceSnake(&field, head); 
			
		// 設定食物的初始位置，設成場地中隨機找的一個空地
		Position food = RandomlySelectEmptyPosition(&field);
		
		// 在場地狀態中，食物的位置放置一個食物
		PlaceFood(&field, food);

		// 把場地依照現在的狀態畫出來 ("牆"、"蛇"、"食" 或 " ")
		DrawField(&field);
		
		// 設定蛇的初始前進方向
		Direction snakeDirection = RIGHT;

		while (1) {

			// 如果有敲擊方向鍵
			if (IsArrowKeyHit()) {
				// 設定蛇前進的方向為敲擊的方向鍵方向
				snakeDirection = GetHitArrowKey();
			}

			// 在場地上紀錄蛇移動的方向
			RecordDirection(&field, head, snakeDirection);
			
			// 依照蛇前進的方向，移動蛇頭的位置一格
			Move(&head, &field, snakeDirection);
			
			State state = GetState(&field, head);

			// 如果撞到牆壁或蛇身
			if (state == WALL || state == SNAKE)
				break;
	
			// 如果撞到食物
			if (state == FOOD) {
				// 設定新的食物位置，設成場地中隨機找的一個空地
				PlaceFood(&field, RandomlySelectEmptyPosition(&field));

        		// 在食物的位置繪製一個 "食" 
				Print(food, "食");
			} else {
        		// 將場地狀態中，蛇尾的位置修改為空地
        		PlaceEmpty(&field, tail);

 				// 在蛇尾位置繪製一個 " "
				Print(tail, "　");

        		// 依照場地上在蛇尾位置紀錄的前進的方向移動蛇尾一格
        		Move(&tail, &field, GetDirection(&field, tail));
			}

			// 將場地狀態中，蛇頭的位置修改為蛇身  
			PlaceSnake(&field, head);

			// 在蛇頭位置繪製一個 "蛇"
			Print(head, "蛇");

			// 等待一小段時間
			Wait(200);
		}

		// 印出 GameOver
		PrintXY(0, HEIGHT, "Game Over!\n");
		Pause();
	}
	return 0;
}
