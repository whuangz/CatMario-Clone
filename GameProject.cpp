

// GameProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
using namespace std;
#include <iostream>

#include "menus.h"
#include "entity.h"

float offsetX=0, offsetY=0;
const int H = 14;
const int W = 100;


String TileMap[H] = {

	"0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
	"0                                B                                                                 0",
	"0               H                B                                                                 0",
	"0              H H               B                                                                 0",
	"0             H   H              B                                                         0       0",
	"0            H  H  H             B                                                         0       W",
	"0           H       H            B                                                         0       W",
	"000        HHHHHHHHHHH           B           BBBTTTTBBBB        BBBB                       0       W",
	"0                                BB         BBBBTTTTTBBBB                                  0       W",
	"0                                          BBBBBTTTTTBBBBB                                         W",
	"0    B         BB         BB              TTBBBBTTTTTBBBBBT                                        W",
	"BBBBBBBBBBBBBBBBBBBBTTBBBBBBTTBBBBBBBBBBBBTTBBBBTTTTTBBBBBTTBBBBBTTBBBBBBBBBBBTTTBBBBBBBBBBBBBBBBBBBB",
	"BBBBBBBBBBBBBBBBBBBBTTBBBBBBTTBBBBBBBBBBBBTTBBBBTTTTTBBBBBTTBBBBBTTBBBBBBBBBBBTTTBBBBBBBBBBBBBBBBBBBB",
	"BBBBBBBBBBBBBBBBBBBBDDBBBBBBDDBBBBBBBBBBBBDDBBBBDDDDDBBBBBDDBBBBBDDBBBBBBBBBBBDDDBBBBBBBBBBBBBBBBBBBB",
};

RenderWindow window(VideoMode(600, 400), "Me works!");

int generateRandom(int max)
{
	int randomNumber = rand();
	float random = (randomNumber % max) + 1;
	int myRandom = random;
	return myRandom;
}

class MakeString
{
	public:
		template <typename T>
		MakeString& operator<< (const T& value)
		{
			myStream << value;
			return *this;
		}
		
		operator string() const
		{
			return myStream.str();
		}
		
	private:
		stringstream myStream;
};

class Player : public entity{
public:
	float dx,dy;
	bool life;
	FloatRect rect;
	bool onGround;
	bool win;
	Sprite sprite;
	float currentFrame;

	Player(Texture &image){
		sprite.setTexture(image);
		rect = FloatRect(7 * 32, 9 * 32, 40, 50);
		dx=dy=0.1;
		life = true;
		win = false;
		currentFrame = 0;
	}

	void update(float time){

		rect.left += dx*time;
		Collision(0);

		if(!onGround) dy = dy+ 0.0005*time;
		rect.top += dy*time;
		onGround = false;
		Collision(1);

		if (!life) sprite.setTextureRect(IntRect(0, 0, 0,0));
		
		currentFrame +=0.005*time;
		
		if (currentFrame > 5) currentFrame -= 5;;
		if (dx < 0)sprite.setTextureRect(IntRect(64 * int(currentFrame) + 64, 10, -60, 50));//x,y,width,height
		if (dx > 0)sprite.setTextureRect(IntRect(64 * int(currentFrame), 10, 60, 50));

		sprite.setPosition(rect.left - offsetX, rect.top );
		dx = 0;
	}

	void updateMovement(){
		if(Keyboard::isKeyPressed(Keyboard::Left)){
			dx = -0.1;
		}
		if(Keyboard::isKeyPressed(Keyboard::Right)){
			dx = 0.1;
		}
		if(Keyboard::isKeyPressed(Keyboard::Up)){
			if(onGround){dy=-0.35;onGround=false;}
		}
		//if(Keyboard::isKeyPressed(Keyboard::Space)){
		//	if (currentFrame > 2) currentFrame -=2 ;
		//	//sprite.setTextureRect(IntRect(40*int(currentFrame),1310,60,50));
		//	if(dx > 0) sprite.setTextureRect(IntRect(52*int(currentFrame),1310,53,50));
		//	else if(dx < 0){
		//		sprite.setTextureRect(IntRect(52*int(currentFrame)+52,1310,-53,50));
		//	}else if(dx == 0){
		//		sprite.setTextureRect(IntRect(52*int(currentFrame),1310,53,50));
		//	}
		//		
		//}
	}

	void Collision(int dir) {
		
		for (int i = rect.top / 32; i<(rect.top + rect.height) / 32; i++) {
			for (int j = rect.left / 32; j<(rect.left + rect.width) / 32; j++) {
				if (TileMap[i][j] == 'B') {
					if ((dx > 0) && (dir == 0)) rect.left = j * 32 - rect.width;
					if ((dx < 0) && (dir == 0)) rect.left = j * 32 + 32;
					if ((dy > 0) && (dir == 1)) { rect.top = i * 32 - rect.height; dy = 0; onGround = true; }
					if ((dy < 0) && (dir == 1)) { rect.top = i * 32 + 32; dy = 0; }
				}

				if (TileMap[i][j] == '0') {
					TileMap[i][j] = ' ';
					dx=0;
					dy=0.05;
				}
				if (TileMap[i][j] == 'T') {
					TileMap[i][j] = ' ';
				}
				if (TileMap[i][j] == 'D') {
					life = false;
				}
				if (TileMap[i][j] == 'W') {
					TileMap[i][j] = ' ';
					win = true;
				}
				
			}
		}
	}

};

class Enemy : public entity {
public:
	float dx, dy;
	FloatRect rect;
	bool onGround;
	Sprite sprite;
	float currentFrame;
	bool life;
	int move;
	int counter;

	Enemy(Texture &image) {
		sprite.setTexture(image);
		sprite.setColor(Color(255, 255, 0));
		rect = FloatRect(7 * 32, 9 * 32, 40, 50);
		dx = dy = 0.05;
		currentFrame = 0;
		life = true;
	}

	void update(float time) {

		rect.left += dx*time;
		Collision(0);

		if (!onGround) dy = dy + 0.0005*time;
		rect.top += dy*time;
		onGround = false;
		Collision(1);

		if (!life) sprite.setTextureRect(IntRect(64, 65, 60,50));

		currentFrame += 0.005*time;
		if (currentFrame > 5) currentFrame -= 5;;

		if (dx > 0)sprite.setTextureRect(IntRect(64 * int(currentFrame) + 64, 10, -60, 50));//x,y,width,height
		if (dx < 0)sprite.setTextureRect(IntRect(64 * int(currentFrame), 10, 60, 50));


		sprite.setPosition(rect.left - offsetX, rect.top);
	
		move = generateRandom(1500);
		if (move == 1) dx = -0.1;
		if (move == 2) dx = 0.1;
		if (move == 3) dx = 0;

		

	}


	void Collision(int dir) {
		for (int i = rect.top / 32; i<(rect.top + rect.height) / 32; i++) {
			for (int j = rect.left / 32; j<(rect.left + rect.width) / 32; j++) {
				if (TileMap[i][j] == 'B' || TileMap[i][j] == 'T' ||TileMap[i][j] == 'D') {
					if ((dx > 0) && (dir == 0)) {
						rect.left = j * 32 - rect.width;
					}
					if ((dx < 0) && (dir == 0)) {
						rect.left = j * 32 + 32;
					}
					if ((dy > 0) && (dir == 1)) { rect.top = i * 32 - rect.height; dy = 0; onGround = true; }
					if ((dy < 0) && (dir == 1)) { rect.top = i * 32 + 32; dy = 0; }
				}

				if (TileMap[i][j] == '0') {
					
				}
				if(TileMap[i][j] == 'D'){
					life = false;
				}
			}
		}
	}

};

void mapSet(){
	
	Texture tileSet;
	tileSet.loadFromFile("images/Mario_Tileset.png");
	Sprite tile(tileSet);

	for (int i = 0; i<H; i++)
				for (int j = 0; j<W; j++)
			{
				if (TileMap[i][j] == 'B')tile.setTextureRect( IntRect(143-16*3,112,32,32) );
				//if (TileMap[i][j] == 'F') 
				if (TileMap[i][j] == 'T') tile.setTextureRect( IntRect(143-16*3,112,32,32) );
				if (TileMap[i][j] == 'D') tile.setTextureRect( IntRect(143-16*3,112,32,32) );
				if (TileMap[i][j] == 'H') tile.setTextureRect( IntRect(99,224,140-99,255-224) );
				if ((TileMap[i][j]==' ') || (TileMap[i][j]=='0')) continue;


				tile.setPosition(j * 32 - offsetX, i * 32);
				window.draw(tile);
			}
}
int main();
void gameOver();
void winning();

void mainmenu(){
	//Rectangle + texture = sprite
	int score=0;
	int counter;
	

	Font font;
	font.loadFromFile("sansation.ttf");
	Text text;
	text.setFont(font);

	Texture t;
	t.loadFromFile("images/hero.png");

	Player p(t);
	    
	Music music;
    music.openFromFile("Mario_Theme.ogg");
    music.play();

	Music endMusic;
	endMusic.openFromFile("Mario_end.ogg");
	

	SoundBuffer buffer;
	buffer.loadFromFile("Jump.ogg");
	Sound sound(buffer);

	Texture e;
	e.loadFromFile("images/sprite.png");

	
	
	Enemy e1(e);
	Enemy e2(e);
	Enemy e3(e);
	Enemy e4(e);
	Clock clock;

	e1.rect.left = 600;
	e2.rect.left = 300;
	e3.rect.left = 1000;
	e4.rect.left = 1380;
	e4.rect.top = 150;

	
	while (window.isOpen())
	{
		cout << "x=" << p.rect.left << "     y=" << p.rect.top << endl;
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time / 700;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			p.dx = -0.1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			p.dx = 0.1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			if (p.onGround) { p.dy = -0.3; p.onGround = false;sound.play(); }
		}

		p.update(time);
		p.updateMovement();

		e1.update(time);
		e2.update(time);
		e3.update(time);
		e4.update(time);

		string str = MakeString() << "Score: " << score;
		text.setString(str);
	if  (p.rect.intersects(e1.rect) ){
			 if (e1.life) {
				 if (p.dy>0) {
					 e1.dx=0; e1.dy=-0.2; e1.life=false; p.dy = -0.25; score++;
				 }
				 else {
					 p.life = false;
				}
			}
		 } 
		if(p.rect.intersects(e2.rect) ){
			 if (e2.life) {
				if (p.dy>0) {
					 e2.dx=0; e2.dy=-0.2; e2.life=false; p.dy = -0.25;score++;
				 }
				else {
					 p.life = false;
				}
			}
		}
		if(p.rect.intersects(e3.rect) ){
			 if (e3.life) {
				if (p.dy>0) {
					 e3.dx=0; e3.dy=-0.2; e3.life=false; p.dy = -0.25;score++;
				 }
				else {
					 p.life = false;
				}
			}
		}
		if(p.rect.intersects(e4.rect) ){
			if (e4.life) {
				if (p.dy>0) {
					 e4.dx=0; e4.dy=-0.2; e4.life=false; p.dy = -0.25;score++;
				 }
				else {
					 p.life = false;
				}
			}
		}


		if (p.rect.left > 300)offsetX = p.rect.left - 300;
		offsetY = p.rect.top - 200;

		window.clear(Color(107,140,255));

		if(p.life == true)mapSet();
		
			
		window.draw(p.sprite);
		window.draw(e1.sprite);window.draw(e2.sprite);window.draw(e3.sprite);window.draw(e4.sprite);
		if(e1.life== false){
			e1.dx=0;
			Sleep(10);
			if(p.dy >0){
				e1.sprite.setColor(Color(255,255,255,0));
			}
		}
		if(e2.life== false){
			e2.dx=0;
			Sleep(10);
			if(p.dy >0){
				e2.sprite.setColor(Color(255,255,255,0));
			}
		}
		if(e3.life== false){
			e3.dx=0;
			Sleep(10);
			if(p.dy >0){
				e3.sprite.setColor(Color(255,255,255,0));
			}
		}
		if(e4.life== false){
			e4.dx=0;
			Sleep(10);
			if(p.dy >0){
				e4.sprite.setColor(Color(255,255,255,0));
			}
		}

		if(score==4){
			music.pause();
		}

		window.draw(text);
		if(p.life==false){
			Sleep(600);
			music.stop();
			endMusic.stop();
			offsetY = offsetX = 0;
			gameOver();
		}

		if(p.win == true){
			Sleep(100);
			music.stop();
			winning();
		}
		window.display();
    }
}

void how(){
	Texture gameTitle;
	gameTitle.loadFromFile("how_title.png");
	Sprite gameset(gameTitle);
	gameset.setTexture(gameTitle);
	gameset.setPosition(1,1);

	while (window.isOpen())
	{
		
		Event event;
        while (window.pollEvent(event))
        {
			switch(event.type){
			case Event::KeyReleased:
				switch(event.key.code){
					case Keyboard::Escape:
						main();
						break;
				}
			}
		}
		window.clear();
		window.draw(gameset);
        window.display();
    }
}

void gameOver(){
	Texture gameTitle;
	gameTitle.loadFromFile("game over.png");
	Sprite gameset(gameTitle);
	gameset.setTexture(gameTitle);
	gameset.setPosition(1,1);

	while (window.isOpen())
	{
		
		Event event;
        while (window.pollEvent(event))
        {
			switch(event.type){
			case Event::KeyReleased:
				switch(event.key.code){
					case Keyboard::Escape:
						main();
						break;
				}
			}
		}
		window.clear();
		window.draw(gameset);
        window.display();
    }

}

void winning(){
	Texture gameTitle;
	gameTitle.loadFromFile("Winning Screen.png");
	Sprite gameset(gameTitle);
	gameset.setTexture(gameTitle);
	gameset.setPosition(1,1);

	while (window.isOpen())
	{
		
		Event event;
        while (window.pollEvent(event))
        {
			switch(event.type){
			case Event::KeyReleased:
				switch(event.key.code){
					case Keyboard::Return:
						main();
						break;
				}
			}
		}
		window.clear();
		window.draw(gameset);
        window.display();
    }

}

int main()
{
	Texture gameTitle;
	gameTitle.loadFromFile("game_title.png");
	Sprite gameset(gameTitle);
	gameset.setTexture(gameTitle);
	gameset.setPosition(1,1);

	menus menu(window.getSize().x, window.getSize().y);

    while (window.isOpen())
	{
		
		Event event;
        while (window.pollEvent(event))
        {
			switch(event.type){
			case Event::KeyReleased:
				switch(event.key.code){
					case Keyboard::Up:
						menu.moveUp();break;

					case Keyboard::Down:
						menu.moveDown();break;

					case Keyboard::Return:
						switch(menu.getPressedItem()){
							case 0:
								cout << "Played button has been pressed" <<endl;
								mainmenu();
								break;
							case 1:
								how();
								break;
							case 2:
								window.close();
								break;
							}
							break;
						}
					break;

			case Event::Closed:
				window.close();
				break;
			}
        }
	
	
    
		window.clear();
		window.draw(gameset);
		menu.draw(window);
        window.display();
    }

    return 0;
}