#pragma once

#include "headers.h"

class Obstacles
{
private:
	list<int> section_list;	// Lista przeszkód
	float level_position;	// Offset przeszkody
	float section_width;	// Szerokość sekcji z przeszkodą
	
public:
	Obstacles();
	~Obstacles();
	float& getLevelPosition();
	void changeLevelPosition(float arg);
	float& getSectionWidth();
	void changeSectionWidth(float arg);
	int getListSize();
	void addSectionList(int arg);
	void removeSelectionList();
	list<int>& getList();
	void set_zero();
	void list_reset();
};