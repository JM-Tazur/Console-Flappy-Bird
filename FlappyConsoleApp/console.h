#pragma once
#include "headers.h"

class Console
{
	friend class Bird;
protected:
	HANDLE Console_handle_original;
	HANDLE Console_handle;																		// Uchwyt konsoli
	HANDLE Console_input_handle;																	// Uchwyt parametrów wejściowych dla konsoli
	SMALL_RECT window_rectangle;																// Przechowuje koordynaty rogów prostokątu
	string Application_name;																		// Nazwa aplikacji/okna
	int screen_width;																		// Szerokość okna
	int screen_hight;																	// Wysokość okna

	CHAR_INFO* screen_buffer;																	// Bufor dla ekranu

	struct KeyState																		// Inforamcje o stanie klawisza spacji
	{
		bool key_pressed;
		bool key_released;
		bool key_hold;
	}
	keys[256];

	short key_OldState[256] = { 0 };
	short key_NewState[256] = { 0 };

	bool console_focus = true;
	static atomic<bool> AtomActive;	// Typ atomowy, przechowuje informacje dotyczace stanu gry (działa/nie działa)
	static condition_variable GameFinished;	// Zmienna warunkowa, koniec gry
	static mutex game_mux;

	bool global_score_lock = false;

public:
	virtual bool Create() = 0;
	virtual bool Update(float elapsed_time) = 0;
	virtual bool Destroy()
	{
		return true;
	}

	Console();
	int ScreenWidth();
	int ScreenHeight();
	void Clip(int& x, int& y);
	virtual void Draw(int x, int y, short c = 0x2588, short col = 0x000F);
	void Fill(int x1, int y1, int x2, int y2, short c = 0x2588, short col = 0x000F);
	void DrawString(int x, int y, wstring c, short col = 0x000F);
	int ConstructConsole(int width, int height, int fontw, int fonth);
	static BOOL CloseHandler(DWORD evt);
	void Start();
	void MainGameThread();
	~Console();
};