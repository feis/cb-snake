#include <stdlib.h> 
#include "field.h"
#include "utils.h"

void InitializeField(Field* field) {
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			if (x == 0 || x == WIDTH - 1 ||
				y == 0 || y == HEIGHT - 1) {
				field->cells[x][y].state = WALL;
			} else {
				field->cells[x][y].state = EMPTY;
			}
		}
	}
}

void DrawField(const Field* field) {
	ClearScreen();
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			const char *sprite;
			switch (field->cells[x][y].state) {
				case WALL:  sprite = "Àð"; break;
				case EMPTY: sprite = "¡@"; break;
				case SNAKE: sprite = "³D"; break;
				case FOOD:  sprite = "­¹"; break;
			}
			PrintXY(x, y, sprite);
		}
	}
}

Position RandomlySelectEmptyPosition(const Field* field) {		
	Position food; 
	do {
		food.x = rand() % WIDTH;
		food.y = rand() % HEIGHT;
	} while (field->cells[food.x][food.y].state != EMPTY);
	return food;
} 

void PlaceSnake(Field* field, Position pos) {
	field->cells[pos.x][pos.y].state = SNAKE;
}

void PlaceFood(Field* field, Position pos) {
	field->cells[pos.x][pos.y].state = FOOD;
}

void PlaceEmpty(Field* field, Position pos) {
	field->cells[pos.x][pos.y].state = EMPTY;
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
		case UP:
			pos->y--;
			break;
		case RIGHT:
			pos->x++;
			break;
		case DOWN:
			pos->y++;
			break;
		case LEFT:
			pos->x--;
			break;
	}
} 
	
void Print(Position pos, const char* s) {
	PrintXY(pos.x, pos.y, s);
}
