#ifndef _H_UTILS
#define _H_UTILS 

#include "field.h"
 
void HideCursor();
int IsArrowKeyHit();
Direction GetHitArrowKey();
void ClearScreen();
void PrintXY(int, int, const char *);
void Pause();
void Wait(int ms);

#endif
