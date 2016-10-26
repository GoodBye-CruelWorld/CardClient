#pragma once
#include"HelloWorldScene.h"

class BookLayer :public Layer
{
public:
	virtual void onEnter() override;
	//模式选择
public:
	//start game


	void startGame(bool mode, bool firstHand); //开始游戏
	void update(float dt);

	void setGameSocket(GameSocket* socket);
	void setMode(int mode);
private:
	GameSocket *_socket;
	int _gameState;
	int _mode;
	Sprite *_bg;
	Sprite *_frame;
	Sprite *_modeImg;
	Button *_buttons[6];
	Sprite *_arrow;

};