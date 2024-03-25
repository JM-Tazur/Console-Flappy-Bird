#include "headers.h"

void randomCOLOR(COLOR &col, bool &active)
{
	while (true)
	{
		col = (COLOR)(rand() % 14 + 1);
		this_thread::sleep_for(chrono::seconds(3));
		if (active == false)
		{
			break;
		}
	}
}

void scoreUP(int& score)
{
	score++;
	this_thread::sleep_for(chrono::milliseconds(70));
}