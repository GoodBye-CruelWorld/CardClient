#pragma once

#include"cocos2d.h"
#include"ODSocket.h"
USING_NS_CC;
/*@GameSocket
*brief ����ͻ��������������������Ϣ����
*ÿ�ν�����Ϣ��Ҫ����
*������Ϣ�����ͻ��˵�ai�ͷ������ж���ʲô��Ӧ
*���硰msg="RE";sendMsg��msg��;����round end���غϽ���������������֪ͨ���� ���ĻغϿ�ʼ
*���硰recvMsg(msg);if(msg=="AT1")����attack card num 1����������ս���ϵ�һ�ſ��ƣ���ս����������֪ͨAIִ�д�������
*����msg���ݵĹ淶���´λ�����ȷ��
*/
class GameSocket
{
public:
	GameSocket();
	bool connect();  //���ӵ�������,ʧ���򷵻�false
	bool sendMsg(std::string message);  //�������������Ϣ
	void recvMsg(); //������Ϣ
	std::string getMsg();
public:
	bool _recv;
	bool _send;
	std::string _recvMsg;
private:
	//bool GameSocket::GetLocalIP(char* ip);
	//char *_ip;
	std::string _sendMsg;

	void Thread();
	//void sendThread(std::string &message);
	//void recvThread(std::string &message);
	//bool _isConnect;
	ODSocket *_socket;
};