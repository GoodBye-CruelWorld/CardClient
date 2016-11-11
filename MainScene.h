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
	void startAdventureEvent(Ref*pSender,TouchEventType type); //ð��ģʽ
	void startVersusEvent(Ref*pSender, TouchEventType type); //��սģʽ
	void startShopEvent(Ref*pSender, TouchEventType type); //�̵�ģʽ
	void startBookEvent(Ref*pSender, TouchEventType type); //ͼ��ģʽ
	void startOptionEvent(Ref*pSender, TouchEventType type); //ѡ��ģʽ

	void startGame(bool mode, bool firstHand); //��ʼ��Ϸ
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