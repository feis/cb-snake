#ifndef _H_FIELD
#define _H_FIELD

#define WIDTH  20
#define HEIGHT 15

typedef enum {
	NONE  = -1,
	UP,
	RIGHT,
	DOWN,
	LEFT
} Direction;

typedef enum {
	WALL = -1,
	EMPTY,
	SNAKE,
	FOOD
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
	Cell cells[WIDTH][HEIGHT];
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
