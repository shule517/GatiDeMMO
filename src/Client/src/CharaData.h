#pragma once

enum STATE_TYPE
{
	STATE_STOP, STATE_MOVE
};

struct CharaData
{
	int				id;					// id
	float			x, y;				// ���W
	float			dx, dy;				// �ړ��ړI���W	
	float			px, py;				// �O�̖ڕW�n�_
	int				hp;					// HP
	int				hp_max;				// �ő�HP
	int				dir;				// �����Ă������
	char			name[256];			// ���O
	int				damage;				// �G�t�F�N�g�p�_���[�W
	int				alpha;				// �����x
	STATE_TYPE		state;				// �L�����̏��
	unsigned int	move_start_time;	// �ړ����n�߂�����
	unsigned int	move_end_time;		// �ړ��Ɏc�肩���鎞��

	//CAnimDraw *anim;	// AnimDraw�̃|�C���^
/*
	int life;			// ���C�t
	int image_w;		// �C���[�W�̑傫��
	int image_h;
	int bounds_w;		// ����̈�̑傫��
	int bounds_h;
	int flash;
	ATARIMODE amode;	// �����蔻�葮��
	int yobi[6];		// �\��

	BaseMover *mover;	// move�֐��p�|�C���^
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
