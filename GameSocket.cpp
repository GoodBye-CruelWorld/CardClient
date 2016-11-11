#include"GameSocket.h"

GameSocket::GameSocket()
{
	_socket = nullptr;
	_send = false;
	_recv = false;
}



bool GameSocket::connect()
{
	_socket = new ODSocket();
	_socket->Init();
	_socket->Create(AF_INET, SOCK_STREAM, 0);

	//auto isConnect=_socket->Connect("127.0.0.1", 9999);  //测试用
	auto isConnect = _socket->Connect("223.3.113.217", 10000);  //测试用
	if (isConnect == true)
	{
		std::thread t1(&GameSocket::Thread, this);
		t1.detach();
	}
	//_isConnect = _socket->Connect(_ip, 9999);
	return isConnect;
}

void GameSocket::Thread()
{
	while (true)
	{
		if (_send)
		{
		
			log(("is sending"+_sendMsg).c_str());
			_socket->Send(_sendMsg.c_str(), strlen(_sendMsg.c_str()) + 1, 1);
			_sendMsg = "";
			_send = false;
		
		}
		if (_recv)
		{
			char  recvBuf[20] = "\0";
			_socket->Recv(recvBuf, 20, 0);
			_recvMsg = std::string(recvBuf);
			_recv = false;
		}
	}
	log("exit thread!");
}

//bool GameSocket::sendMsg(std::string &message)
//{
//	if (!_isConnect)
//		return false;
//	
//	//发送信息
//	//std::thread t1(&GameSocket::sendThread,message, this);//将收文件的监听置于此     
//	//t1.detach();  //可用，将子线程从主线程中分离出来
//	return true;
//	
//}

//void GameSocket::sendThread(std::string &message)
//{
//	_socket->Send(message.c_str(), strlen(message.c_str()) + 1, 1);
//	_isConnect = false;	
//}
//
void GameSocket::recvMsg()
{
	//if (!_isConnect)
	//return false;
	//std::thread t2(&GameSocket::recvThread, this, message);//将收文件的监听置于此     
	//t2.detach();  //可用，将子线程从主线程中分离出来
	_recv = true;

}

bool GameSocket::sendMsg(std::string message)
{
	if (_recv)
	{
		log("is recving msg,maybe cannot sendMsg");
	}
	//if (!_isConnect)
	//return false;
	//std::thread t2(&GameSocket::recvThread, this, message);//将收文件的监听置于此     
	//t2.detach();  //可用，将子线程从主线程中分离出来
	_sendMsg = message;
	_send = true;
	return true;
}

std::string GameSocket::getMsg()
{
	auto msg = _recvMsg;
	_recvMsg = "";
	return msg;
}