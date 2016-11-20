#pragma once
#include"cocos2d.h"
#include"string.h"
#include "..\cocos2d\external\win32-specific\icon\include\iconv.h"
USING_NS_CC;
std::string;

class Tool
{
public:

	/*获取系统的时间*/
	static int getGameTime();

	/**创建中文标签
	  *请在texts/texts.xml中设置相应中文的键值*/
	static Label* createLabelByKey(const std::string key, const std::string fontType="Arial",float fontSize=24);

	/**更改中文标签内容
	*请在texts/texts.xml中设置相应中文的键值*/
	static void setLabelString(Label* label, const std::string key);


	/*本游戏中使用的标题型字体样式*/
	static Label*createTitleLabel(int fontSize=20);
	/*本游戏中使用的正文型字体样式*/
	static Label*createTextLabel();

	static Label*createEnglishLabel();

	static bool IConvConvert(const char *from_charset, const char *to_charset, const char *inbuf, int inlen, char *outbuf, int outlen);

	static std::string IConvConvert_GBKToUTF8(const std::string& str);


};