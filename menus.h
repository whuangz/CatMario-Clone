#pragma once
#include "entity.h"
#define MAX_NUMBER_OF_ITEMS 3

class menus
{
public:
	menus(float width, float height);
	~menus();

	void draw(RenderWindow &windows);
	void moveUp();

	void moveDown();
	int getPressedItem(){	
		return selectedItemIndex;
	}

private:
	int selectedItemIndex;
	Font font;
	Text text[MAX_NUMBER_OF_ITEMS];
};

