#ifndef H_UTILS
#define H_UTILS 

#include "direction.h"
 
void HideCursor();
int IsArrowKeyHit();
Direction GetHitArrowKey();
void ClearScreen();
void PrintXY(int, int, const char *);
void Pause();
void Wait(int ms);

#endif
