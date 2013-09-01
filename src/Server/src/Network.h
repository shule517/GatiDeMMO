
#include <stdio.h>
#include <process.h>
#include <winsock2.h>
#include <list>
#include <math.h>

#include "../../Common/GlobalDefine.h"

using namespace std;

void SynchroThread( void *lpvoid );
void AcceptThread( void *lpvoid );
void ClientThread( void *lpvoid );

/************************/
#define GetNowTime		GetTickCount
/************************/
enum SENDMODE
{
	SEND_ME,
	SEND_ALL,
};

struct ClientData
{
	SOCKET	socket;
	char	*ip;
	int		id;

	float	x;
	float	y;
	float	dx;
	float	dy;

	float			px, py;				// 前の目標地点
//	STATE_TYPE		state;				// キャラの状態
	unsigned int	move_start_time;	// 移動し始めた時間
	unsigned int	move_end_time;		// 移動に残りかかる時間

	ClientData()
	{
		socket = -1;
		ip = NULL;
		id = -1;
		x = 1.0f;
		y = 1.0f;
		dx = 1.0f;
		dy = 1.0f;
		px = 0;
		py = 0;
		move_end_time = 0;
		move_start_time = 0;
	}
};

class CNetwork
{
private:
	list<ClientData> ClientList;				// クライアントリスト

public:
	SOCKET		listen_socket;					// listenソケット

public:	// frame
	void		init( u_short PortNo );
	void		add( SOCKET socket, char *ip );
	ClientData	*get( int no );
	ClientData	*searchUnused( int *PlayerNo );
	void		erase( int PlayerNo );
	void		sendPacket( SENDMODE mode, int PlayerNo, char *Buf );
	void		synchro();
	void		moveChara();

public: // send
	void		CreatePacket( char *dat, char *fmt, ... );
	void		send_PK_OBJ_ADD( int PlayerNo );
	void		send_PK_OBJ_REMOVE( int PlayerNo );
	void		send_PK_OBJ_UPDATE_ACT( int PlayerNo, int x, int y );
	void		send_PK_USER_AUTH( int PlayerNo );
	void		send_PK_DAMAGE( int PlayerNo, int DamagePlayerNo, int damage );

	void		sendNowPlayer( int PlayerNo );

public: // proc
	void		proc( int PlayerNo, int res, char *message );
	void		procPacket( char *message, char *fmt, ... );
	void		proc_PK_CMD_MV( int PlayerNo, int res, char *message );
	void		proc_PK_USER_AUTH( int PlayerNo, int res, char *message );
	void		proc_PK_ATTACK( int PlayerNo, int res, char *message );
};
