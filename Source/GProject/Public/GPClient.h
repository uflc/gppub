#pragma once

#include "GProject.h"
#include "HAL/Runnable.h"
#include "../GPServer/common/GPPacket.h"

#include "Windows/AllowWindowsPlatformAtomics.h"
#include <WinSock2.h>
#include <iosfwd>
#include "Windows/HideWindowsPlatformAtomics.h"
#pragma comment(lib,"ws2_32")


class GPROJECT_API FGPClient : public FRunnable
{

public:
	FGPClient();
	virtual ~FGPClient() override;

private:
	///** Singleton instance, can access the thread any time via static accessor, if it is active! */
	//static  FGPClient* Runnable; //singleton has issue on 'Run under single process' option.

	/** Thread to run the worker FRunnable on */
	FRunnableThread* Thread;

	class FAsyncTask<class FGPRecvTask>* AuthRecvTask;

public:
	/*
		Start the thread and the worker from static (easy access)!
		This code ensures only thread will be able to run at a time.
		This function returns a handle to the newly started instance.
	*/
	//static FGPClient* GetGPClient();

public:
	// Begin FRunnable interface. 

	// �� �Լ����� FRunnableThreadWin::Run�� ���� ȣ�� ��.

	virtual bool Init() override; //true�� �����ϸ� Run.
	virtual uint32 Run() override; //recv loop
	virtual void Exit() override; //Run�� ���� �ڿ� ���ҽ� Ŭ������ ���� ȣ��.

	////

	// FRunnableThreadWin�� �Ҹ��� �� ������(Run)�� �����ϱ� ���� ȣ��.
	virtual void Stop() override; //Run�� ����(�� �ٶ�)�� �޼ҵ�.

	// End FRunnable interface

	// Runnable �ν��Ͻ� �Ҹ�.
	void Shutdown();

protected:
	void CreateAsyncSendTask(SOCKET sock, std::stringstream& ss, GPPacketType pt);
	void CreateAsyncAuthRecvTask(SOCKET sock, FGPClient* cl);

public:
	void CreateAsyncMainSendTask(std::stringstream& ss, GPPacketType pt);
	void CreateAsyncAuthSendTask(std::stringstream& ss, GPPacketType pt);


private:
	class AGProjectPlayerController* PlayerCon;
	class AGProjectGameMode* GameMode;
	class UGPGameInstanceBase* Game;

public:
	//convenient setters

	void SetPlayerController(AGProjectPlayerController* PC) { PlayerCon = PC; }
	void SetGameMode(AGProjectGameMode* GM) { GameMode = GM; }
	void SetGameInstance(UGPGameInstanceBase* GI) { Game = GI; }

	////

protected:
	SOCKET MainSocket;
	SOCKET AuthSocket; //

	char ID[MAX_ID_LEN];
	//char* AuthSecret; //credential
	//HANDLE ConnEvent; //���� ��� �̺�Ʈ.

	char RecvBuf[MAX_PKT_SIZ]; //
	
public:
	// Winsock api mostly boilerplate stuffs

	bool Connect(SOCKET sock, u_short port, char* ip = "127.0.0.1"); //todo async //���� �� ������ ������ �и��ϱ� ���� GPEntry������ Connect���ְ� ����.
	bool Send(SOCKET sock, char* buf, int len);
	////

	// GI features.
	bool ConnectAll(); //
	void SignUp(FString id, FString pwd);
	void Login(FString id, FString pwd); //test
	void Join();
	void PostLogin();
	void PostLogout();
	void PostSignup(bool bResult);
	bool SendStream(SOCKET sock, std::stringstream& ss, GPPacketType pt);
	bool SendChat(FString Chat);
	bool SendHeader(GPPacketType pt);
	////

	// Player features.

	bool SendPlayerData();
};
