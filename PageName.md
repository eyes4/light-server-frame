# Introduction（介绍） #

一个基于C++开发的简单易用的服务器程序编写框架。通过多线程异步方式处理从客户端发来的消息。

# Details #

使用templete，可自由为server提供socket连接方式、消息处理方式:
  * 可处理字符串类消息或二进制类消息，字符串类消息可指定尾字符，自动分割出完整的消息体
  * 通过线程池处理消息，处理消息的类从基类继承。
  * 下面是一个简单的服务程序。

```
class CEchoTask
	:public CStrMsgTaskBase
{
    void DoAcceptSocket(int nSocket, string strClientIP)
    {
        printf("new client connected: %s\n", strClientIP);
    }

    int ProcessMessage(int nSocket, string strClientIP, string strMsg)
    {
        printf("recv msg:%s\n", strMsg.c_str());
        //echo
        send(nSocket, (void *)strMsg.c_str(), strMsg.length());
        return 0;
    }
   
};

int main(int argc, _TCHAR* argv[])
{
       //create server
	CServer<CEchoTask, CStrMsgProcessor> *pEchoServer = new CServer<EchoTask,     CStrMsgProcessor>("0.0.0.0", 6000);

       //set string message tail
       pEchoServer->SetMsgTail("\r\n", 2);

       //start server
	pEchoServer->Start();

        //main loop
	while(1)
	{
                //do something
		Sleep(10000);
	}
        
        //stop server
	pEchoServer->Stop();
        //delete server
	delete pEchoServer;
	return 0;
}

```