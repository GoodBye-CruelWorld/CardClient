#pragma once
#include"HelloWorldScene.h"

class MainLayer :public Layer
{
public:
	virtual void onEnter() override;
	//ģʽѡ��
public:
	//start game
	void startTrainingEvent(Ref*pSender, TouchEventType type); //��ϰģʽ
	void adventureEvent(Ref*pSender,TouchEventType type); //ð��ģʽ
	void startVersusEvent(Ref*pSender, TouchEventType type); //��սģʽ
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