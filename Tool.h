#pragma once
#include"cocos2d.h"
#include"string.h"
#include "..\cocos2d\external\win32-specific\icon\include\iconv.h"
USING_NS_CC;
std::string;

class Tool
{
public:

	/*��ȡϵͳ��ʱ��*/
	static int getGameTime();

	/**�������ı�ǩ
	  *����texts/texts.xml��������Ӧ���ĵļ�ֵ*/
	static Label* createLabelByKey(const std::string key, const std::string fontType="Arial",float fontSize=24);

	/**�������ı�ǩ����
	*����texts/texts.xml��������Ӧ���ĵļ�ֵ*/
	static void setLabelString(Label* label, const std::string key);


	/*����Ϸ��ʹ�õı�����������ʽ*/
	static Label*createTitleLabel(int fontSize=20);
	/*����Ϸ��ʹ�õ�������������ʽ*/
	static Label*createTextLabel();

	static Label*createEnglishLabel();

	static bool IConvConvert(const char *from_charset, const char *to_charset, const char *inbuf, int inlen, char *outbuf, int outlen);

	static std::string IConvConvert_GBKToUTF8(const std::string& str);


};