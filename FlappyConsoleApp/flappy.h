#pragma once
#include "headers.h"
#include "console.h"
#include "obstacles.h"
#include "bird.h"

class FlappyGame : public Console
{
private:
	shared_ptr<Bird> fl;
	shared_ptr<Obstacles> ob;
	
	list<int>::iterator it;
	bool reset_game;
	int attempts_count;
	int score;
	int max_score;

protected:
	// Wywołanie poprzez klasę Console
	virtual bool Create();
	// Wywołanie poprzez klasę Console
	virtual bool Update(float elapsed_time);

public:
	COLOR obs_color;
	FlappyGame();
	~FlappyGame();
	void DrawScore(int posX, int posY, int current, int max, int attempt);
	void GameOver();
};