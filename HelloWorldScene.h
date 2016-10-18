#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d::ui;

#include "ui/CocosGUI.h"

#include"cocostudio/CocoStudio.h"
using namespace cocostudio;

#include"audio\include\SimpleAudioEngine.h" //ÒôÆµ
using namespace CocosDenshion;

#include "../extensions/cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;

//#include"Outline.h";
#include"GameResource.h"
//#include"TextCartoon.h"
#include"GameSocket.h"
class HelloWorld : public cocos2d::Layer, public cocos2d::extension::EditBoxDelegate
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
   
//ÓÃ»§µÇÂ¼
public:
	cocos2d::extension::EditBox *_userName;
	cocos2d::extension::EditBox *_userPwd;


	//µÇÂ¼
	void loginEvent(Ref*pSender, TouchEventType type);
	//×¢²á
	void registerEvent(Ref*pSender, TouchEventType type);

	//ÍË³ö
	void exitEvent(Ref*pSender, TouchEventType type);


	virtual void editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox);
	virtual void editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox);
	virtual void editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string& text);
	virtual void editBoxReturn(cocos2d::extension::EditBox* editBox);


	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);
private:
	GameSocket *_socket;
	int _gameState;
	Sprite*_bg;
	Sprite*_frame;
	Sprite*_submitText;
	Button*_submit;
	Button*_register;
	Button*_exit;
	Sprite*_title;
	//Sprite *_;

};

#endif // __HELLOWORLD_SCENE_H__
