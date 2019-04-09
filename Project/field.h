#ifndef _H_FIELD
#define _H_FIELD

#define FIELD_WIDTH  20
#define FIELD_HEIGHT 15

typedef enum {
	DIRECTION_NONE  = -1,
	DIRECTION_UP,
	DIRECTION_RIGHT,
	DIRECTION_DOWN,
	DIRECTION_LEFT
} Direction;

typedef enum {
	STATE_WALL = -1,
	STATE_EMPTY,
	STATE_SNAKE,
	STATE_FOOD
} State;

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

#endif
