#include "headers.h"
#include "flappy.h"

FlappyGame::FlappyGame()
{
	Application_name = "Flappy Bird";
	reset_game = false;
	attempts_count = 0;
	score = 0;
	max_score = 0;
	fl = make_shared<Bird>();
	ob = make_shared<Obstacles>();
}

FlappyGame::~FlappyGame()
{
	// pusty destruktor
}

// Wywołanie poprzez klasę Console
bool FlappyGame::Create()
{
	reset_game = true;
	ob->set_zero();
	ob->changeSectionWidth((float)ScreenWidth() / (float)(ob->getListSize() - 1));

	return true;
}

// Wywołanie poprzez klasę Console
bool FlappyGame::Update(float elapsed_time)
{
	thread score_thread;

	if (reset_game)
	{
		fl->resetBird();
		reset_game = false;
		ob->list_reset();
		score = 0;
		attempts_count++;
	}

	if (fl->ifColided() == true)
	{
		GameOver();
		if (keys[VK_SPACE].key_released)
		{
			reset_game = true;
		}
	}
	else
	{
		if (keys[VK_SPACE].key_pressed && fl->getVelocity() >= (fl->getGravity() / 10.0f))
		{
			fl->changeAcceleration(0.0f);
			fl->changeVelocity(-fl->getGravity() / 4.0f);
		}
		else
		{
			fl->changeAcceleration(fl->getAcceleration() + (fl->getGravity() * elapsed_time));
		}

		if (fl->getAcceleration() >= fl->getGravity())
		{
			fl->changeAcceleration(fl->getGravity());
		}

		fl->changeVelocity(fl->getVelocity() + (fl->getAcceleration() * elapsed_time));
		fl->changeY(fl->getY() + (fl->getVelocity() * elapsed_time));

		ob->changeLevelPosition(ob->getLevelPosition() + 14.0f * elapsed_time);
		 
		if (ob->getLevelPosition() > ob->getSectionWidth())
		{
			ob->changeLevelPosition(ob->getLevelPosition() - ob->getSectionWidth());
			ob->removeSelectionList();
			int i = rand() % (ScreenHeight() - 20);
			if (i <= 10)
			{
				i = 0;
			}
			ob->addSectionList(i);
		}

		// Obraz
		Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');

		// Rysowanie obiektów
		int nSection = 0;
		list<int>::iterator it;
		for (auto s : ob->getList())
		{
			if (s != 0)
			{
				Fill(nSection * ob->getSectionWidth() + 10 - ob->getLevelPosition(), ScreenHeight() - s, nSection * ob->getSectionWidth() + 15 - ob->getLevelPosition(), ScreenHeight(), PIXEL_SOLID, obs_color);
				Fill(nSection * ob->getSectionWidth() + 10 - ob->getLevelPosition(), 0, nSection * ob->getSectionWidth() + 15 - ob->getLevelPosition(), ScreenHeight() - s - 18 /*gate*/, PIXEL_SOLID, obs_color);
			}
			
			// Wynik
			it = next(ob->getList().begin());
			if ((int)fl->getX() == (int)(nSection * ob->getSectionWidth() + 15 - ob->getLevelPosition()) && *it != 0)
			{
				score_thread = thread(scoreUP, ref(score));
				score_thread.join();
				if (score > max_score)
				{
					max_score = score;
				}
			}
			nSection++;
		}

		// Detekcja kolizji
		int xx = (int)fl->getX();
		int yy = (int)fl->getY();
		
		if (yy < 2 || yy > ScreenHeight() - 2 ||
			screen_buffer[(yy + 0) * ScreenWidth() + xx].Char.UnicodeChar != L' ' ||
			screen_buffer[(yy + 1) * ScreenWidth() + xx].Char.UnicodeChar != L' ' ||
			screen_buffer[(yy + 0) * ScreenWidth() + xx + 6].Char.UnicodeChar != L' ' ||
			screen_buffer[(yy + 1) * ScreenWidth() + xx + 6].Char.UnicodeChar != L' ')
		{
			fl->changeColided(true);
		}
		
		// Rysowanie obiektu gracza
		if (fl->getVelocity() > 0)
		{
			DrawString(fl->getX(), fl->getY() + 0, L"vvv-Q");
			DrawString(fl->getX(), fl->getY() + 1, L"vvv");
		}
		else
		{
			DrawString(fl->getX(), fl->getY() + 0, L"^^^_Q");
			DrawString(fl->getX(), fl->getY() + 1, L"^^^");
		}

		DrawScore(1, 45, score, max_score, attempts_count);
	}

	return true;
}

void FlappyGame::DrawScore(int posX, int posY, int current, int max, int attempt) 
{
	DrawString(posX, posY, L"Próba: " + to_wstring(attempt) + L" Wynik: " + to_wstring(current) + L" Rekord: " + to_wstring(max));
}

void FlappyGame::GameOver()
{
	DrawString(screen_width/2 - 5, screen_hight/2, L"KONIEC GRY", FG_RED);
	DrawScore(screen_width / 2 - 13, screen_hight / 2 + 1, score, max_score, attempts_count);
	DrawString(screen_width / 2 - 4, screen_hight / 2 + 2, L"Medal:", FG_WHITE);

	if (score >= 10 && score < 20)
	{
		DrawString(screen_width / 2 - 4, screen_hight / 2 + 3, L"BRAZ", FG_DARK_YELLOW);
	}
	else if (score >= 20 && score < 30)
	{
		DrawString(screen_width / 2 - 4, screen_hight / 2 + 3, L"SREBRO", FG_WHITE);
	}
	else if (score >= 30 && score < 40)
	{
		DrawString(screen_width / 2 - 4, screen_hight / 2 + 3, L"ZLOTO", FG_YELLOW);
	}
	else if (score >= 40)
	{
		DrawString(screen_width / 2 - 4, screen_hight / 2 + 3, L"PLATYNA", FG_CYAN);
	}
	else
	{
		DrawString(screen_width / 2 - 4, screen_hight / 2 + 3, L"BRAK", FG_BLUE);
	}
}