#include "headers.h"
#include "console.h"
#include "myerror.h"

Console::Console()
{
	screen_width = SCREEN_WIDTH;
	screen_hight = SCREEN_HIGHT;

	Console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	Console_input_handle = GetStdHandle(STD_INPUT_HANDLE);

	memset(key_NewState, 0, 256 * sizeof(short));
	memset(key_OldState, 0, 256 * sizeof(short));
	memset(keys, 0, 256 * sizeof(KeyState));

	Application_name = "Default";
}

int Console::ScreenWidth()	// Get dla szerokości okna
{
	return screen_width;
}

int Console::ScreenHeight()	// Get dla wysokości okna
{
	return screen_hight;
}

void Console::Clip(int& x, int& y) // Ograniczenie obszaru w konsoli
{
	if (x < 0) 
	{ 
		x = 0; 
	}
	if (x >= screen_width) 
	{ 
		x = screen_width; 
	}
	if (y < 0) 
	{ 
		y = 0; 
	}
	if (y >= screen_hight) 
	{ 
		y = screen_hight; 
	}
}

void Console::Draw(int x, int y, short c, short col)
{
	if (x >= 0 && x < screen_width && y >= 0 && y < screen_hight)
	{
		screen_buffer[y * screen_width + x].Char.UnicodeChar = c;
		screen_buffer[y * screen_width + x].Attributes = col;
	}
}

void Console::Fill(int x1, int y1, int x2, int y2, short c, short col)
{
	Clip(x1, y1);
	Clip(x2, y2);
	for (int x = x1; x < x2; x++)
	{
		for (int y = y1; y < y2; y++)
		{
			Draw(x, y, c, col);
		}
	}
}

void Console::DrawString(int x, int y, wstring c, short col)
{
	for (size_t i = 0; i < c.size(); i++)
	{
		screen_buffer[y * (int)screen_width + x + i].Char.UnicodeChar = c[i];
		screen_buffer[y * (int)screen_width + x + i].Attributes = col;
	}
}

int Console::ConstructConsole(int width, int height, int fontw, int fonth)
{
	if (Console_handle == INVALID_HANDLE_VALUE)
	{
		throw MyError("Bad Handle");
	}

	screen_width = width;
	screen_hight = height;

	// Ustawienie rozmiaru czcionki 
	CONSOLE_FONT_INFOEX info;
	info.cbSize = sizeof(info);
	info.nFont = 0;
	info.dwFontSize.X = fontw;
	info.dwFontSize.Y = fonth;
	info.FontFamily = FF_DONTCARE;
	info.FontWeight = FW_NORMAL;

	window_rectangle = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(Console_handle, TRUE, &window_rectangle);

	// Sprawdzenie rozmiaru bufora
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(Console_handle, &csbi))
	{
		throw MyError("Get Console Screen Buffer Info");
	}
	if (screen_hight > csbi.dwMaximumWindowSize.Y)
	{
		throw MyError("Screen Height / Font Height Too Big");
	}
	if (screen_width > csbi.dwMaximumWindowSize.X)
	{
		throw MyError("Screen Width / Font Width Too Big");
	}

	// Bufor ekranu
	COORD buffer = { (short)screen_width, (short)screen_hight };
	if (!SetConsoleScreenBufferSize(Console_handle, buffer))  // Txt bufor
	{
		throw MyError("SetConsoleScreenBufferSize");
	}

	// Przypisanie bufora ekranu do konsoli
	if (!SetConsoleActiveScreenBuffer(Console_handle))
	{
		throw MyError("SetConsoleActiveScreenBuffer");
	}

	wcscpy_s(info.FaceName, L"Console");
	if (!SetCurrentConsoleFontEx(Console_handle, false, &info))
	{
		throw MyError("Set Current Console Font");
	}

	// Ustawienie rozmiaru konsoli
	window_rectangle = { 0, 0, (short)(screen_width - 1), (short)(screen_hight - 1) };
	if (!SetConsoleWindowInfo(Console_handle, TRUE, &window_rectangle))  // Wielkość okna
	{
		throw MyError("Set Console Window Info");
	}

	// Alokowanie pamięci dla bufora ekranu
	screen_buffer = new CHAR_INFO[screen_width * screen_hight];
	memset(screen_buffer, 0, sizeof(CHAR_INFO) * screen_width * screen_hight);

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CloseHandler, TRUE);
	return 1;
}

BOOL Console::CloseHandler(DWORD evt)
{
	if (evt == CTRL_CLOSE_EVENT)
	{
		AtomActive = false;
		// Oczekiwanie na koniec wątku
		unique_lock<mutex> ul(game_mux);
		GameFinished.wait(ul);
	}
	return true;
}

void Console::Start()
{
	// Zacznij wątek
	AtomActive = true;
	thread game_thread(&Console::MainGameThread, this);
	// Oczekiwanie na koniec wątku
	game_thread.join();
}

void Console::MainGameThread()
{
	if (!Create())
	{
		AtomActive = false;
	}

	// Pomiar czasu
	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	while (AtomActive)
	{
		while (AtomActive)
		{
			// Zarządanie czasem
			tp2 = chrono::system_clock::now();
			chrono::duration<float> elapsedTime = tp2 - tp1;
			tp1 = tp2;
			float elapsed_time = elapsedTime.count();

			// Zarządzanie wejściem (klawiatura)
			for (int i = 0; i < 256; i++)
			{
				key_NewState[i] = GetAsyncKeyState(i);

				keys[i].key_pressed = false;
				keys[i].key_released = false;

				if (key_NewState[i] != key_OldState[i])
				{
					if (key_NewState[i] & 0x8000)
					{
						keys[i].key_pressed = !keys[i].key_hold;
						keys[i].key_hold = true;
					}
					else
					{
						keys[i].key_released = true;
						keys[i].key_hold = false;
					}
				}

				key_OldState[i] = key_NewState[i];
			}

			INPUT_RECORD inBuf[32];
			DWORD events = 0;
			GetNumberOfConsoleInputEvents(Console_input_handle, &events);
			if (events > 0)
			{
				ReadConsoleInput(Console_input_handle, inBuf, events, &events);
			}

			// Zarządzanie eventami
			for (DWORD i = 0; i < events; i++)
			{
				if (inBuf[i].EventType == FOCUS_EVENT)
				{
					console_focus = inBuf[i].Event.FocusEvent.bSetFocus;
				}
			}
			// Aktualizacja klatek
			if (!Update(elapsed_time))
			{
				AtomActive = false;
			}

			// Pasek aplikacji
			wchar_t s[256];
			swprintf_s(s, 256, L"FLAPPY_BIRD");
			SetConsoleTitle(s);
			WriteConsoleOutput(Console_handle, screen_buffer, { (short)screen_width, (short)screen_hight }, { 0,0 }, &window_rectangle);
		}

		if (Destroy())
		{
			delete[] screen_buffer;
			SetConsoleActiveScreenBuffer(Console_handle_original);
			GameFinished.notify_one();
		}
		else
		{
			AtomActive = true;
		}
	}
}

Console::~Console()
{
	SetConsoleActiveScreenBuffer(Console_handle_original);
	delete[] screen_buffer;
}

// Definicje zmiennych statycznych
atomic<bool> Console::AtomActive(false);
condition_variable Console::GameFinished;
mutex Console::game_mux;