#include "StdAfx.h"
#include "menus.h"

menus::menus(float width, float height)
{
	if(!font.loadFromFile("sansation.ttf")){
		//handle error

	}

	text[0].setFont(font);
	text[0].setColor(Color::Red);
	text[0].setString("Play");
	text[0].setPosition(Vector2f((width/2)-30, height/(MAX_NUMBER_OF_ITEMS + 0.3)* 1));

	text[1].setFont(font);
	text[1].setColor(Color::White);
	text[1].setString("How to play");
	text[1].setPosition(Vector2f(width/2-75, height/(MAX_NUMBER_OF_ITEMS + 1.5)* 2));

	text[2].setFont(font);
	text[2].setColor(Color::White);
	text[2].setString("Exit");
	text[2].setPosition(Vector2f(width/2-30, height/(MAX_NUMBER_OF_ITEMS + 2)* 3));

	
	selectedItemIndex = 0;
}


menus::~menus(void)
{
}

void menus::draw(RenderWindow &window){
	for(int i =0 ; i < MAX_NUMBER_OF_ITEMS; i++){
		window.draw(text[i]);
	}
}


void menus::moveUp(){
	if(selectedItemIndex - 1 >= 0){
		text[selectedItemIndex].setColor(Color::White);
		selectedItemIndex--;
		text[selectedItemIndex].setColor(Color::Red);
	}
}


void menus::moveDown(){
	if(selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS){
		text[selectedItemIndex].setColor(Color::White);
		selectedItemIndex++;
		text[selectedItemIndex].setColor(Color::Red);
	}
}