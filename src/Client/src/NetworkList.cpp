
#include "StdAfx.h"
#include "Network.h"

/* ========================================================================= */
/* �֐����FCNetwork::addPlayerList											 */
/* ���e�F�}�C���X�g�ɃL�����N�^�[��ǉ�										 */
/* ���t�F2007/09/12															 */
/* ========================================================================= */
void CNetwork::addMyList( int PlayerNo, char *Name, int x, int y )
{
	// �Ƃ肠�����f�[�^�̒�������
	list<CharaData>::iterator it;
	it = MyList->begin();

	(*it).id = PlayerNo;
	sprintf_s( (*it).name, "%s", Name );
	(*it).x = (float)x;
	(*it).y = (float)y;
	(*it).dx = (*it).x;
	(*it).dy = (*it).y;
	(*it).dir = 0;
	(*it).hp = 600;
	(*it).hp_max = 1000;

	/*
	// ����ID�̃L���������Ȃ����m�F
	list<CharaData>::iterator it;
	it = MyList->begin();

	while( it != MyList->end() )
	{
		if( (*it).id == PlayerNo )
		{
			// �֐��E�o
			return;
		}
		++it;
	}

	// �L�����ǉ�
	CharaData temp;
	temp.id = PlayerNo;
	sprintf_s( temp.name, "%s", Name );
	temp.x = (float)x;
	temp.y = (float)y;
	temp.dx = temp.x;
	temp.dy = temp.y;
	temp.dir = 0;
	// temp.anim = &CharaAnim;

	MyList->push_back( temp );	// �L�����f�[�^�Z�b�g
	*/
}

/* ========================================================================= */
/* �֐����FCNetwork::addPlayerList											 */
/* ���e�F�v���C���[���X�g�ɃL�����N�^�[��ǉ�								 */
/* ���t�F2007/09/12															 */
/* ========================================================================= */
void CNetwork::addPlayerList( int PlayerNo, char *Name, int x, int y )
{
	// �����̃L�����ɓ���ID�����Ȃ����m�F
	list<CharaData>::iterator it;
	it = MyList->begin();

	while( it != MyList->end() )
	{
		if( (*it).id == PlayerNo )
		{
			// �֐��E�o
			return;
		}
		++it;
	}

	// ����ID�̃L���������Ȃ����m�F
//	list<CharaData>::iterator it;
	it = PlayerList->begin();

	while( it != PlayerList->end() )
	{
		if( (*it).id == PlayerNo )
		{
			// �֐��E�o
			return;
		}
		++it;
	}

	// �L�����ǉ�
	CharaData temp;
	temp.id = PlayerNo;
	sprintf_s( temp.name, "%s", Name );
	temp.x = (float)x;
	temp.y = (float)y;
	temp.dx = temp.x;
	temp.dy = temp.y;
	temp.dir = 0;
	temp.hp = 1000;
	temp.hp_max = 1000;
	// temp.anim = &CharaAnim;

	PlayerList->push_back( temp );	// �L�����f�[�^�Z�b�g
}

/* ========================================================================= */
/* �֐����FCNetwork::addEffectList											 */
/* ���e�F�G�t�F�N�g�����X�g�ɒǉ�											 */
/* ���t�F2007/09/14															 */
/* ========================================================================= */
void CNetwork::addEffectList( int x, int y, int damage )
{
	list<CharaData>::iterator it;
	it = EffectList->begin();

	CharaData effect;
	effect.x = (float)x;
	effect.y = (float)y;
	effect.damage = damage;
	effect.alpha = 255;

	EffectList->push_back( effect );
}

/* ========================================================================= */
/* �֐����FCNetwork::movePlayer												 */
/* ���e�F�L�����N�^�[�̈ړ�													 */
/* ���t�F2007/09/12															 */
/* ========================================================================= */
void CNetwork::movePlayer( int PlayerNo, int dx, int dy )
{
	list<CharaData>::iterator it;
	it = PlayerList->begin();

	while( it != PlayerList->end() )
	{
		if( (*it).id == PlayerNo )
		{
			(*it).px = (*it).x;
			(*it).py = (*it).y;

			(*it).dx = (float)dx;
			(*it).dy = (float)dy;

			(*it).move_start_time = GetNowTime();

			float x = (*it).dx - (*it).x;
			float y = (*it).dy - (*it).y;
			float time = ( sqrtf( x*x + y*y ) / move_speed * (1.0f/60.0f) )*1000;
			(*it).move_end_time = GetNowTime() + (int)time;

			(*it).state = STATE_MOVE;
			break;
		}
		++it;
	}
}

/* ========================================================================= */
/* �֐����FCNetwork::erasePlayer											 */
/* ���e�F�L�����N�^�[�̍폜													 */
/* ���t�F2007/09/12															 */
/* ========================================================================= */
void CNetwork::erasePlayer( int PlayerNo )
{
	list<CharaData>::iterator it;
	it = PlayerList->begin();

	while( it != PlayerList->end() )
	{
		if( (*it).id == PlayerNo )
		{
			(*it).id = -1;
			break;
		}
		else
		{
			++it;
		}
	}
/*
	list<CharaData>::iterator it;
	it = PlayerList->begin();

	int size = PlayerList->size();

	// �G���[�΍�L�����Ə��������L������2�L�����ȏア��Ȃ�폜�\
	if( size >= 2 )
	{
		while( it != PlayerList->end() )
		{
			if( (*it).id == PlayerNo )
			{
				PlayerList->erase( it );
				return;
			}
			++it;
		}
	}
*/
}

/* ========================================================================= */
/* �֐����FCNetwork::damagePlayer											 */
/* ���e�F�_���[�W����														 */
/* ���t�F2007/09/14															 */
/* ========================================================================= */
void CNetwork::damagePlayer( int PlayerNo, int damage )
{
	// �_���[�W���󂯂��̂�����
	list<CharaData>::iterator it;
	it = MyList->begin();

	while( it != MyList->end() )
	{
		if( (*it).id == PlayerNo )
		{
			(*it).hp -= damage;
			if( (*it).hp < 0)
				(*it).hp = 0;
			// �G�t�F�N�g�ǉ�
			addEffectList( (int)(*it).x, (int)(*it).y, damage );
			// �֐��E�o
			return;
		}
		++it;
	}

	// �_���[�W���󂯂��̂��v���C���[
	it = PlayerList->begin();

	while( it != PlayerList->end() )
	{
		if( (*it).id == PlayerNo )
		{
			(*it).hp -= damage;
			if( (*it).hp < 0)
				(*it).hp = 0;
			// �G�t�F�N�g�ǉ�
			addEffectList( (int)(*it).x, (int)(*it).y, damage );
			// �֐��E�o
			return;
		}
		++it;
	}
}
