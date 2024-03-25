#include "headers.h"
#include "console.h"
#include "flappy.h"
#include "myerror.h"


int main(int argc, char* argv[])
{
	srand(time(NULL));
	bool active = true;
	FlappyGame game;
	thread color_randomizer(randomCOLOR, ref(game.obs_color), ref(active));
	color_randomizer.detach();

	try
	{
		game.ConstructConsole(SCREEN_WIDTH, SCREEN_HIGHT, PIXEL_SIZE, PIXEL_SIZE);
	}
	catch (MyError &err)
	{
		cout << err.what() << endl;
	}
	catch (...)
	{
		cout << "Error." << endl;
	}
	game.Start();

	active = false;
	color_randomizer.join();
    return 0;
}