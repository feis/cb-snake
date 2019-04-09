#include <stdlib.h> 
#include "field.h"
#include "utils.h"

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
				case STATE_WALL:  sprite = "Àð"; break;
				case STATE_EMPTY: sprite = "¡@"; break;
				case STATE_SNAKE: sprite = "³D"; break;
				case STATE_FOOD:  sprite = "­¹"; break;
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
