#include "headers.h"
#include "obstacles.h"


Obstacles::Obstacles()
{
	level_position = 0.0f;
	section_width = 0;
	section_list = { 0, 0, 0, 0 };
}

Obstacles::~Obstacles()
{
	// pusty destruktor
}

float& Obstacles::getLevelPosition()
{
	return level_position;
}

void Obstacles::changeLevelPosition(float arg)
{
	level_position = arg;
}

float& Obstacles::getSectionWidth()
{
	return section_width;
}

void Obstacles::changeSectionWidth(float arg)
{
	section_width = arg;
}

int Obstacles::getListSize()
{
	return section_list.size();
}

void Obstacles::addSectionList(int arg)
{
	section_list.push_back(arg);
}

void Obstacles::removeSelectionList()
{
	section_list.pop_front();
}

list<int>& Obstacles::getList()
{
	return section_list;
}

void Obstacles::set_zero()
{
	level_position = 0.0f;
	section_width = 0;
	section_list = { 0, 0, 0, 0 };
}

void Obstacles::list_reset()
{
	section_list = { 0, 0, 0, 0 };
}