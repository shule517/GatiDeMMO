#pragma once

enum STATE_TYPE
{
	STATE_STOP, STATE_MOVE
};

struct CharaData
{
	int				id;					// id
	float			x, y;				// 座標
	float			dx, dy;				// 移動目的座標	
	float			px, py;				// 前の目標地点
	int				hp;					// HP
	int				hp_max;				// 最大HP
	int				dir;				// 向いている方向
	char			name[256];			// 名前
	int				damage;				// エフェクト用ダメージ
	int				alpha;				// 透明度
	STATE_TYPE		state;				// キャラの状態
	unsigned int	move_start_time;	// 移動し始めた時間
	unsigned int	move_end_time;		// 移動に残りかかる時間

	//CAnimDraw *anim;	// AnimDrawのポインタ
/*
	int life;			// ライフ
	int image_w;		// イメージの大きさ
	int image_h;
	int bounds_w;		// 判定領域の大きさ
	int bounds_h;
	int flash;
	ATARIMODE amode;	// 当たり判定属性
	int yobi[6];		// 予備

	BaseMover *mover;	// move関数用ポインタ
	void move(float jx, float jy){
		if(mover!=0) mover->move(this, jx,jy);
	}
	CharaData(){ mover = 0; }
*/
	CharaData()
	{
		id				= 0;
		x				= 0;
		y				= 0;
		dx				= 0;
		dy				= 0;
		px				= 0;
		py				= 0;
		dir				= 0;
		hp				= 0;
		hp_max			= 0;
		alpha			= 0;
		damage			= 0;
		state			= STATE_STOP;
		move_start_time	= 0;
		move_end_time	= 0;
		sprintf_s( name, "null" );
	}
};
