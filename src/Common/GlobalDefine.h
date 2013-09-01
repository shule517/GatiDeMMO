
#pragma once

/*************************************/
const float move_speed = 2.0f;
/*************************************/

/****************************************/

/*frame base*/
#define GAME_FRAME 	0x0F
#define HALF_SEC	0x07
#define D_HALF_SEC	0x03
#define REGEN		0x0F*30

/* frame setting*/
#define REGEN_TIME GAME_FRAME*2
#define AFRAME	900000/GAME_FRAME
#define _ONE_SEC	GAME_FRAME
#define _TWO_SEC	GAME_FRAME*2
#define _THREE_SEC	GAME_FRAME*3
#define _FOUR_SEC	GAME_FRAME*4
#define _FIVE_SEC	GAME_FRAME*5
#define _TWO_MIN	GAME_FRAME*120
/****************************************/

//	"192.168.3.2"
//	"127.0.0.1"
//	"219.49.174.163"
//	"219.49.172.2"
//	"shota6848.ddo.jp"
//#define SERVER_IP		"shota6848.ddo.jp"
#define SERVER_IP		"118.105.131.69"
#define SERVER_PORT		1000

#define PEND			0xC8D1	// パケットエンドマーカー
#define MAX_PACKET_SIZE 4096
#define MIN_PACKET_SIZE 5

#define	MAXLINE			1024

/*packet header definition*/
enum PACKET_HEADER
{
	PK_USER_AUTH	= 1,

	PK_USER_CHAT	= 10,		PK_INVEN_INFO,

	PK_REQ_LOC		= 20,		PK_PT_REQ,				PK_PT_PERMIT,
	PK_PT_LEAVE,				PK_PT_BREAK,			PK_PT_BAN, PK_PT_CHAT,
	PK_PT_INFO,

	PK_CMD_MV		= 30,		PK_ATTACK,				
	PK_DAMAGE, // test
	PK_LEVELUP,
	PK_SKILL_USE,				PK_SKILL_GET,			PK_SKILL_LVUP,
	PK_SKILL_INFO,

	PK_OBJ_ADD		= 40,		PK_OBJ_UPDATE_ACT,		PK_OBJ_UPDATE_EQUI,
	PK_OBJ_REMOVE,

	PK_ITEM_BUY		= 50,		PK_ITEM_SELL,			PK_ITEM_PICK,
	PK_ITEM_DROP,				PK_ITEM_POS,			PK_ITEM_WEAR,

	PK_TRADE_REQ	= 60,		PK_TRADE_REQ_OK,		PK_TRADE_LIST,
	PK_TRADE_OK,				PK_TRADE_CANCEL,		PK_TRADE_SUCC,
	PK_WARE_INFO,				PK_WARE_POS,			PK_WARE_DEPOS,

	PK_MAP_MOVE_REQ	= 70,		PK_MAP_MOVE_AUTH
};
