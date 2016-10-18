#pragma once
#include"HelloWorldScene.h"

class MainLayer :public Layer
{
public:
	virtual void onEnter() override;
	//模式选择
public:
	//start game
	void startTrainingEvent(Ref*pSender, TouchEventType type); //练习模式
	void adventureEvent(Ref*pSender,TouchEventType type); //冒险模式
	void startVersusEvent(Ref*pSender, TouchEventType type); //对战模式
	void startGame(bool mode, bool firstHand);
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
	Sprite *_buttons[6];
	Sprite *_arrow;

};