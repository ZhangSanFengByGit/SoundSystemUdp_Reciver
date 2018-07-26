#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include<Ws2tcpip.h>    //ip_mreq头文件
#include <fstream>
#include <string>
#include "winsock2.h"
#include <string>
#include "MSTcpIP.h"
#pragma comment(lib,"ws2_32.lib") //引用库文件

using namespace std;

char buffer1[400];
char buffer2[400];
char buffer3[400];
char buffer4[400];
char buffer5[400];
char buffer6[400];

bool flag[6] = {0,0,0,0,0,0};



typedef struct Param2 {
	u_short port_num;
}Param2;

DWORD WINAPI WriteProFunc(LPVOID lpParam) {


	Param2 *param = (Param2*)lpParam;
	int portNum = param->port_num;
	//portNum -= 1000;
	char id1, id2;
	char tmp_buffer[320];

	while (true) {
		while (flag[portNum-1000] != 1) {
			Sleep(5);
		}

		switch (portNum)
		{
		case 1000:
			id1 = buffer1[28];
			id2 = buffer1[29];
			break;
		case 1001:
			id1 = buffer2[28];
			id2 = buffer2[29];
			break;
		case 1002:
			id1 = buffer3[28];
			id2 = buffer3[29];
			break;
		case 1003:
			id1 = buffer4[28];
			id2 = buffer4[29];
			break;
		case 1004:
			id1 = buffer5[28];
			id2 = buffer5[29];
			break;
		case 1005:
			id1 = buffer6[28];
			id2 = buffer6[29];
			break;
		default:
			break;
		}

		int ID = int(id1) * 10 + int(id2);
		string id = to_string(ID);
		id.append(".pcm");
		const char *idd = id.c_str();

		FILE *fp;
		fp = fopen(idd, "a");

		
		

		switch (portNum)
		{
		case 1000:
			for (int i = 0; i < 320; i++) {
				if (i % 2 == 0) {
					tmp_buffer[i] = buffer1[32 + i +1];
				}
				else
				{
					tmp_buffer[i] = buffer1[32 + i -1];
				}
			}
			fprintf(fp, "%s", tmp_buffer);
			fclose(fp);
			break;
		case 1001:
			for (int i = 0; i < 320; i++) {
				tmp_buffer[i] = buffer1[32 + i];
			}
			fprintf(fp, "%s", tmp_buffer);
			fclose(fp);
			break;
		case 1002:
			for (int i = 0; i < 320; i++) {
				tmp_buffer[i] = buffer1[32 + i];
			}
			fprintf(fp, "%s", tmp_buffer);
			fclose(fp);
			break;
		case 1003:
			for (int i = 0; i < 320; i++) {
				tmp_buffer[i] = buffer1[32 + i];
			}
			fprintf(fp, "%s", tmp_buffer);
			fclose(fp);
			break;
		case 1004:
			for (int i = 0; i < 320; i++) {
				tmp_buffer[i] = buffer1[32 + i];
			}
			fprintf(fp, "%s", tmp_buffer);
			fclose(fp);
			break;
		case 1005:
			for (int i = 0; i < 320; i++) {
				tmp_buffer[i] = buffer1[32 + i];
			}
			fprintf(fp, "%s", tmp_buffer);
			fclose(fp);
			break;
		default:
			break;
		}

		flag[portNum - 1000] = 0;

		
	}
	return 0;
}

DWORD WINAPI ThreadProFunc(LPVOID lpParam)
{
	Param2 *myPort = (Param2*)lpParam;
	u_short port = myPort->port_num;
	int portNum = myPort->port_num;
	//portNum -= 1000;

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		printf("winsock initial failed");
		return -1;
	}

	/*if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != -1) {
		printf("Version error");
		WSACleanup();
		return -2;
	}*/

	SOCKET socketRecv = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == socketRecv) {     //INVALID_SOCKET就是“无效套接字”的意思
		cout << "socket failed!" << endl;
		WSACleanup();//释放套接字资源;
		return  -1;
	}

	SOCKADDR_IN addrSrv;
	/*
	addrSrv.sin_addr.S_un.S_addr = inet_addr("192.168.2.2");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);*/
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);             //发送端使用的发送端口，可以根据需要更改
	addrSrv.sin_addr.s_addr = INADDR_ANY;

	/*err=connect(socketRecv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (err != 0) {
		cout << "connect failed" << endl;
		return -1;
	}*/

	/*if (bind(socketRecv, (SOCKADDR FAR *)&addrSrv, sizeof(addrSrv)) != 0)
	{
		printf("Can't bind socket to local port!Program stop.\n");//初始化失败返回-1
		return -1;
	}*/

	//加入组播组
	ip_mreq multiCast;
	multiCast.imr_interface.S_un.S_addr = INADDR_ANY;     //本地某一网络设备接口的IP地址。
	multiCast.imr_multiaddr.S_un.S_addr = inet_addr("224.1.1.11"); //组播组的IP地址。

	setsockopt(socketRecv, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&multiCast, sizeof(multiCast));

	if (SOCKET_ERROR == bind(socketRecv, (struct sockaddr *)&addrSrv, sizeof(addrSrv)))
	{
		closesocket(socketRecv);
		cout << WSAGetLastError << endl;
		printf("..............................Error……");
		getchar();
		exit(-1);
	}

	

	//DWORD writeThreadid;
	//HANDLE writeThread = CreateThread(NULL, NULL, WriteProFunc, myPort, 0, &writeThreadid);

	//listen(socketRecv, 5);
	//设置该SOCKET为接收所有流经绑定的IP的网卡的所有数据，包括接收和发送的数据包
	u_long sioarg = 1;
	DWORD dwValue = 0;
	/*if (SOCKET_ERROR == WSAIoctl(socketRecv, SIO_RCVALL, &sioarg, sizeof(sioarg), NULL, 0, &dwValue, NULL, NULL))
	{
		closesocket(socketRecv);
		cout << WSAGetLastError();
		exit(-1);
	}*/

	int len = 0;

	//listen(socketRecv, 5);
	sockaddr_in clientAddr;
	int iAddrlen = sizeof(clientAddr);

	//
	/*const char *idd = "13.pcm";
	FILE *fp;
	fp = fopen(idd, "a");*/
	char tmp_buffer1[400];
	char tmp_buffer2[400];
	char tmp_buffer3[400];
	char tmp_buffer4[400];
	char tmp_buffer5[400];
	char tmp_buffer6[400];


	ofstream output[6];
	//const char *fname = "13.pcm";
	//output.open(fname, std::ios::binary|std::ios::app);
	
	/*
	stringstream ss;
	ss << "channel_" << i << ".pcm";
	output[i].open(ss.str().c_str(), std::ios::binary);
	ss.str() = "";
	*/

	while (true) {
		while (flag[portNum - 1000] != 0) {
			Sleep(10);
		}
		//fp = fopen(idd, "a");

		//char * buff = &(buffer1[3]);

		switch (portNum)
		{


		case 1000:
			len = recv(socketRecv, buffer1, sizeof(buffer1), 0);
			output[0].open((((to_string(int(buffer1[0]) * 10 + int(buffer1[1]))).append(".pcm")).c_str()), std::ios::binary | std::ios::app);
			 
			if (len > 0) {
				for (int i = 0; i < len - 4; i += 2)
				{
					tmp_buffer1[i] = buffer1[i + 1 + 4];
					tmp_buffer1[i + 1] = buffer1[i + 4];
				}
				if (output[0].good()) {
					output[0].write(tmp_buffer1, len - 4);
					output[0].close();
				}
			}
			break;



		case 1001:
			len = recv(socketRecv, buffer2, sizeof(buffer2), 0);
			output[1].open((((to_string(int(buffer2[0]) * 10 + int(buffer2[1]))).append(".pcm")).c_str()), std::ios::binary | std::ios::app);

			if (len > 0) {
				for (int i = 0; i < len - 4; i += 2)
				{
					tmp_buffer2[i] = buffer2[i + 1 + 4];
					tmp_buffer2[i + 1] = buffer2[i + 4];
				}
				if (output[1].good()) {
					output[1].write(tmp_buffer2, len - 4);
					output[1].close();
				}
			}
			break;



		case 1002:
			len = recv(socketRecv, buffer3, sizeof(buffer3), 0);
			output[2].open((((to_string(int(buffer3[0]) * 10 + int(buffer3[1]))).append(".pcm")).c_str()), std::ios::binary | std::ios::app);

			if (len > 0) {
				for (int i = 0; i < len - 4; i += 2)
				{
					tmp_buffer3[i] = buffer3[i + 1 + 4];
					tmp_buffer3[i + 1] = buffer3[i + 4];
				}
				if (output[2].good()) {
					output[2].write(tmp_buffer3, len - 4);
					output[2].close();
				}
			}
			break;



		case 1003:
			len = recv(socketRecv, buffer4, sizeof(buffer4), 0);
			output[3].open((((to_string(int(buffer4[0]) * 10 + int(buffer4[1]))).append(".pcm")).c_str()), std::ios::binary | std::ios::app);

			if (len > 0) {
				for (int i = 0; i < len - 4; i += 2)
				{
					tmp_buffer4[i] = buffer4[i + 1 + 4];
					tmp_buffer4[i + 1] = buffer4[i + 4];
				}
				if (output[3].good()) {
					output[3].write(tmp_buffer4, len - 4);
					output[3].close();
				}
			}
			break;



		case 1004:
			len = recv(socketRecv, buffer5, sizeof(buffer5), 0);
			output[4].open((((to_string(int(buffer5[0]) * 10 + int(buffer5[1]))).append(".pcm")).c_str()), std::ios::binary | std::ios::app);

			if (len > 0) {
				for (int i = 0; i < len - 4; i += 2)
				{
					tmp_buffer5[i] = buffer5[i + 1 + 4];
					tmp_buffer5[i + 1] = buffer5[i + 4];
				}
				if (output[4].good()) {
					output[4].write(tmp_buffer5, len - 4);
					output[4].close();
				}
			}
			break;



		case 1005:
			len = recv(socketRecv, buffer6, sizeof(buffer6), 0);
			output[5].open((((to_string(int(buffer6[0]) * 10 + int(buffer6[1]))).append(".pcm")).c_str()), std::ios::binary | std::ios::app);

			if (len > 0) {
				for (int i = 0; i < len - 4; i += 2)
				{
					tmp_buffer6[i] = buffer6[i + 1 + 4];
					tmp_buffer6[i + 1] = buffer6[i + 4];
				}
				if (output[5].good()) {
					output[5].write(tmp_buffer6, len - 4);
					output[5].close();
				}
			}
			break;



		default:
			break;
		}

		//flag[portNum-1000] = 1;
		
	}


	/*
	do
	{
		Param recvParam;
		len = recv(socketRecv, recvParam.recvSound, sizeof(recvParam.recvSound), 0);
		if (len > 0)
		{  
			//报文处理
			DWORD writeThreadid;
			HANDLE writeThread = CreateThread(NULL, NULL, WriteProFunc, &recvParam, 0, &writeThreadid);
		}
	} while (len > 0);


	
	char recvBuff[3300];
	recv(socketRecv, recvBuff, 3300, 0);

	Param recvParam;
	for (int i = 0; i<3300; i++) {
		recvParam.recvSound[i] = recvBuff[i];
	}

	DWORD writeThreadid;
	HANDLE writeThread = CreateThread(NULL, NULL, WriteProFunc, &recvParam, 0, &writeThreadid);*/

	return 0;
}



int main() {
	HANDLE myThread1;
	HANDLE myThread2;
	HANDLE myThread3;
	HANDLE myThread4;
	HANDLE myThread5;
	HANDLE myThread6;

	DWORD myThreadId1;
	DWORD myThreadId2;
	DWORD myThreadId3;
	DWORD myThreadId4;
	DWORD myThreadId5;
	DWORD myThreadId6;

	Param2 port1;
	Param2 port2;
	Param2 port3;
	Param2 port4;
	Param2 port5;
	Param2 port6;

	port1.port_num = 1000;
	port2.port_num = 1001;
	port3.port_num = 1002;
	port4.port_num = 1003;
	port5.port_num = 1004;
	port6.port_num = 1005;


	myThread1 = CreateThread(NULL, NULL, ThreadProFunc, &port1, 0, &myThreadId1);
	myThread2 = CreateThread(NULL, NULL, ThreadProFunc, &port2, 0, &myThreadId2);
	myThread3 = CreateThread(NULL, NULL, ThreadProFunc, &port3, 0, &myThreadId3);
	myThread4 = CreateThread(NULL, NULL, ThreadProFunc, &port4, 0, &myThreadId4);
	myThread5 = CreateThread(NULL, NULL, ThreadProFunc, &port5, 0, &myThreadId5);
	myThread6 = CreateThread(NULL, NULL, ThreadProFunc, &port6, 0, &myThreadId6);

	

	system("pause");
	return 0;
}