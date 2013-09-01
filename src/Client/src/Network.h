#pragma once

void ConnectThread( void *lpvoid );
void ClientThread( void *lpvoid );

class CNetwork
{
public:
	CNetwork( void );
	virtual ~CNetwork( void );

public:	// frame
	void sendServer( char *Buf );
	void start( list<CharaData> *_MyList, list<CharaData> *_PlayerList, list<CharaData> *_EffectList );
	void connect( void );
	void doClientMain( void );

public: // send
	void sendPacket( char *fmt, ... );
	void send_PK_CMD_MV( int x, int y, int dx, int dy );
	void send_PK_USER_AUTH( char *id, char *pw );
	void send_PK_ATTACK( int x, int y );

public:	// proc
	void procMain( short res, char *dat );
	void procPacket( char *dat, char *fmt, ... );
	void proc_PK_OBJ_ADD( int res, char *dat );
	void proc_PK_OBJ_REMOVE( int res, char *dat );
	void proc_PK_OBJ_UPDATE_ACT( int res, char *dat );
	void proc_PK_USER_AUTH( int res, char *dat );
	void proc_PK_DAMAGE( int res, char *dat );

public: // list
	void addMyList( int PlayerNo, char *Name, int x, int y );
	void addPlayerList( int PlayerNo, char *Name, int x, int y );
	void addEffectList( int x, int y, int damage );
	void movePlayer( int PlayerNo, int dx, int dy );
	void erasePlayer( int PlayerNo );
	void damagePlayer( int PlayerNo, int damage );

public:	// ïœêî
	SOCKET listen_socket;

	list<CharaData>	*MyList;
	list<CharaData>	*PlayerList;
	list<CharaData> *EffectList;
};
