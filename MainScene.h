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
	void startAdventureEvent(Ref*pSender,TouchEventType type); //冒险模式
	void startVersusEvent(Ref*pSender, TouchEventType type); //对战模式
	void startShopEvent(Ref*pSender, TouchEventType type); //商店模式
	void startBookEvent(Ref*pSender, TouchEventType type); //图鉴模式
	void startOptionEvent(Ref*pSender, TouchEventType type); //选项模式

	void startGame(bool mode, bool firstHand); //开始游戏
	void update(float dt);

	void setGameSocket(GameSocket* socket);
	void setMode(int mode);
	void setAccount(std::string);
private:
	GameSocket *_socket;
	int _gameState;
	int _mode;
	Sprite *_bg;
	Sprite *_frame;
	Sprite *_modeImg;
	Button *_buttons[6];
	Sprite *_arrow;
	std::string _account;
};