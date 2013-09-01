
#include "StdAfx.h"
#include "MyChara.h"

/* ========================================================================= */
/* �֐����FCMyChara::CMyChara												 */
/* ���e�F�R���X�g���N�^														 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
CMyChara::CMyChara( void )
{
}

/* ========================================================================= */
/* �֐����FCMyChara::~CMyChara												 */
/* ���e�F�f�X�g���N�^														 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
CMyChara::~CMyChara( void )
{
}

/* ========================================================================= */
/* �֐����FCMyChara::init													 */
/* ���e�F������																 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
void CMyChara::init( CNetwork *_Network )
{
	// �l�b�g���[�N�̃A�h���X
	pNetwork = _Network;

	charalist.clear();	//���L�����̃��X�g����ɂ���

	// ��������Ȃ��ă��[�U���F�ŃL�������쐬����i���j
	CharaData jiki;		//���[�J���ϐ�jiki�̐錾
	jiki.id	= 0;
	jiki.dx = 0;
	jiki.dy = 0;
	jiki.x	= 0;
	jiki.y	= 0;
	jiki.hp_max = 1000;
	jiki.hp = 1000;
	sprintf_s( jiki.name, "NULL" );

	charalist.push_back( jiki );	//���L�����f�[�^�Z�b�g

	/*********************/
	int input_no = CreateEditBox();
	/*********************/
}

/* ========================================================================= */
/* �֐����FCMyChara::loadFiles												 */
/* ���e�F�t�@�C���̓ǂݍ���													 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
void CMyChara::loadFiles() throw( int )
{
	int ghandle[16];
	if( LoadDivGraph( "data/swordman001.bmp", 16, 4, 4, 32, 32, ghandle ) == -1 ) throw( -1 );

	CharaAnim.setImage( 0, ghandle[0] );
	CharaAnim.setImage( 0, ghandle[1] );
	CharaAnim.setImage( 0, ghandle[2] );
	CharaAnim.setImage( 0, ghandle[3] );
	CharaAnim.setGap( 0, 6 );

	CharaAnim.setImage( 1, ghandle[4] );
	CharaAnim.setImage( 1, ghandle[5] );
	CharaAnim.setImage( 1, ghandle[6] );
	CharaAnim.setImage( 1, ghandle[7] );
	CharaAnim.setGap( 1, 6 );

	CharaAnim.setImage( 2, ghandle[8] );
	CharaAnim.setImage( 2, ghandle[9] );
	CharaAnim.setImage( 2, ghandle[10] );
	CharaAnim.setImage( 2, ghandle[11] );
	CharaAnim.setGap( 2, 6 );

	CharaAnim.setImage( 3, ghandle[12] );
	CharaAnim.setImage( 3, ghandle[13] );
	CharaAnim.setImage( 3, ghandle[14] );
	CharaAnim.setImage( 3, ghandle[15] );
	CharaAnim.setGap( 3, 6 );

	// HP�o�[
	int ghandle2[3];
	if( LoadDivGraph( "data/hp_bar.bmp", 3, 3, 1, 1, 5, ghandle2 ) == -1 ) throw( -1 );

	CharaAnim.setImage( 4, ghandle2[0] );	// hp
	CharaAnim.setImage( 5, ghandle2[1] );	// ��
	CharaAnim.setImage( 6, ghandle2[2] );	// �g

	/*
	int ghandle3[8];
	if( LoadDivGraph( "data/gun/gun_m_walk_01_01.bmp", 1, 1, 1, 43, 98, &ghandle3[0] ) == -1 ) throw( -1 );
	if( LoadDivGraph( "data/gun/gun_m_walk_01_02.bmp", 1, 1, 1, 43, 98, &ghandle3[1] ) == -1 ) throw( -1 );
	if( LoadDivGraph( "data/gun/gun_m_walk_01_03.bmp", 1, 1, 1, 43, 98, &ghandle3[2] ) == -1 ) throw( -1 );
	if( LoadDivGraph( "data/gun/gun_m_walk_01_04.bmp", 1, 1, 1, 43, 98, &ghandle3[3] ) == -1 ) throw( -1 );
	if( LoadDivGraph( "data/gun/gun_m_walk_01_05.bmp", 1, 1, 1, 43, 98, &ghandle3[4] ) == -1 ) throw( -1 );
	if( LoadDivGraph( "data/gun/gun_m_walk_01_06.bmp", 1, 1, 1, 43, 98, &ghandle3[5] ) == -1 ) throw( -1 );
	if( LoadDivGraph( "data/gun/gun_m_walk_01_07.bmp", 1, 1, 1, 43, 98, &ghandle3[6] ) == -1 ) throw( -1 );
	if( LoadDivGraph( "data/gun/gun_m_walk_01_08.bmp", 1, 1, 1, 43, 98, &ghandle3[7] ) == -1 ) throw( -1 );

	CharaAnim.setImage( 0, ghandle3[0] );
	CharaAnim.setImage( 0, ghandle3[1] );
	CharaAnim.setImage( 0, ghandle3[2] );
	CharaAnim.setImage( 0, ghandle3[3] );
	CharaAnim.setImage( 0, ghandle3[4] );
	CharaAnim.setImage( 0, ghandle3[5] );
	CharaAnim.setImage( 0, ghandle3[6] );
	CharaAnim.setImage( 0, ghandle3[7] );
	CharaAnim.setGap( 0, 3 );
	*/
}

/* ========================================================================= */
/* �֐����FCMyChara::loadFiles												 */
/* ���e�F���L�����̏���														 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
void CMyChara::move()
{
	list<CharaData>::iterator it;
	it = charalist.begin();

	/*
	// �L�����N�^�[�ړ�
	if( (*it).x < (*it).dx )
	{	// �E�ֈړ�
		(*it).x += CGameFrame::IdouHosei( 4 );
		if( (*it).x > (*it).dx )
			(*it).x = (*it).dx;

		(*it).dir = 2;
	}
	if( (*it).x > (*it).dx )
	{	// ���ֈړ�
		(*it).x -= CGameFrame::IdouHosei( 4 );
		if( (*it).x < (*it).dx )
			(*it).x = (*it).dx;

		(*it).dir = 1;
	}
	if( (*it).y < (*it).dy )
	{	// ���ֈړ�
		(*it).y += CGameFrame::IdouHosei( 4 );
		if( (*it).y > (*it).dy )
			(*it).y = (*it).dy;

		(*it).dir = 0;
	}
	if( (*it).y > (*it).dy )
	{	// ��ֈړ�
		(*it).y -= CGameFrame::IdouHosei( 4 );
		if( (*it).y < (*it).dy )
			(*it).y = (*it).dy;

		(*it).dir = 3;
	}
	*/

	/*
	// �L�����N�^�[�ړ�
	if( (*it).x < (*it).dx )
	{	// �E�ֈړ�
		(*it).x = (*it).dx - (*it).px 
		/*
		float direction = atan2( (*it).dy - (*it).y, (*it).dx - (*it).x );
		(*it).x += cos( direction ) * move_speed;
		*
		if( (*it).x > (*it).dx )
		{
			(*it).x = (*it).dx;
			(*it).state = STATE_STOP;
		}

		(*it).dir = 2;
	}
	if( (*it).x > (*it).dx )
	{	// ���ֈړ�
		float direction = atan2( (*it).dy - (*it).y, (*it).dx - (*it).x );
		(*it).x += cos( direction ) * move_speed;
		if( (*it).x < (*it).dx )
			(*it).x = (*it).dx;

		(*it).dir = 1;
	}
	if( (*it).y < (*it).dy )
	{	// ���ֈړ�
		float direction = atan2( (*it).dy - (*it).y, (*it).dx - (*it).x );
		(*it).y += sin( direction ) * move_speed;
		if( (*it).y > (*it).dy )
			(*it).y = (*it).dy;

		(*it).dir = 0;
	}
	if( (*it).y > (*it).dy )
	{	// ��ֈړ�
		float direction = atan2( (*it).dy - (*it).y, (*it).dx - (*it).x );
		(*it).y += sin( direction ) * move_speed;
		if( (*it).y < (*it).dy )
			(*it).y = (*it).dy;

		(*it).dir = 3;
	}
	*/

	// �ړ�����
	move2();

	// �}�E�X����
	int key = GetInput();

	// ���N���b�N�ňړ�
	if( key & MOUSE_INPUT_LEFT )
	{
		int x, y;
		GetMousePoint( &x, &y );
		float dx = x - WIDTH/2 + (*it).x;
		float dy = y - HEIGHT/2 + (*it).y;

		if( dx > 1 && dy > 1/* && (*it).state == STATE_STOP*/ )
		{
			(*it).px = (*it).x;
			(*it).py = (*it).y;

			(*it).dx = dx;
			(*it).dy = dy;

			(*it).move_start_time = GetNowTime();

			float x = (*it).dx - (*it).x;
			float y = (*it).dy - (*it).y;
			float time = ( sqrtf( x*x + y*y ) / move_speed * (1.0f/60.0f) )*1000;
			(*it).move_end_time = GetNowTime() + (int)time;

			(*it).state = STATE_MOVE;

			pNetwork->send_PK_CMD_MV( (int)(*it).x, (int)(*it).y, (int)dx, (int)dy );	// �T�[�o�[�ɖړI�n���M
		}
	}

	char data[256];
	sprintf_s( data, "x = %f", (*it).x );
	DrawRichString( 10, 10, data, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ) );
	sprintf_s( data, "y = %f", (*it).y );
	DrawRichString( 10, 30, data, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ) );
	sprintf_s( data, "dx = %f", (*it).dx );
	DrawRichString( 10, 50, data, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ) );
	sprintf_s( data, "dy = %f", (*it).dy );
	DrawRichString( 10, 70, data, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ) );
	sprintf_s( data, "px = %f", (*it).px );
	DrawRichString( 10, 90, data, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ) );
	sprintf_s( data, "py = %f", (*it).py );
	DrawRichString( 10, 110, data, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ) );

	// �E�N���b�N�ōU��
	if( key & MOUSE_INPUT_RIGHT )
	{
		int x, y;
		GetMousePoint( &x, &y );
		float attack_x = x - WIDTH/2 + (*it).x;
		float attack_y = y - HEIGHT/2 + (*it).y;

		if( attack_x > 1 && attack_y > 1 )
		{
			char data[256];
			sprintf_s( data, "(%d,%d)", (int)attack_x, (int)attack_y );
			DrawString( 10, 10, data, RGB( 255, 0, 0 ) );

			// �T�[�o�ɍU���p�P�𑗐M
			pNetwork->send_PK_ATTACK( (int)attack_x, (int)attack_y );
		}
	}

	// ��ʂ̒����Ɏ����̃L������`��
	CharaAnim.draw( (*it).dir, WIDTH/2 - 32/2, HEIGHT/2 - 32/2 );

	char Name[256];
	sprintf_s( Name, "%d�F%s", (*it).id, (*it).name );

	// ���O�\��
	DrawRichString( WIDTH/2 - strlen( Name )*5/2, HEIGHT/2 - 32/2 + 45, Name, RGB( 0, 255, 0 ), RGB( 0, 0, 0 ) );

	// HP�\�� : �摜���g��Ȃ��Ă��C�C�悤�ɕς���
	if( (*it).hp_max != 0 ) // 0�Ŋ���Ȃ��悤��
	{
		int x = WIDTH/2;
		int y = HEIGHT/2;

		CharaAnim.draw( 6, (float)(x-1-30), (float)(y+20) );
		float hp = (float)(*it).hp;
		float hp_max = (float)(*it).hp_max;

		for( int i = 0; i < 60; i++ )
		{
			if( i >= (int)( 60*( hp / hp_max ) ) )
			{
				CharaAnim.draw( 5, (float)(x+i-30), (float)(y+20) );
			}
			else
			{
				CharaAnim.draw( 4, (float)(x+i-30), (float)(y+20) );
			}
		}
		CharaAnim.draw( 6, (float)(x+60-30), (float)(y+20) );
	}

	// �`���b�g�{�b�N�X�`��
	DrawEditBox( 0, 10, 300 );
}

void CMyChara::move2( void )
{
	list<CharaData>::iterator it;
	it = charalist.begin();

	if( (*it).state == STATE_MOVE )
	{
		float DiffTime = ( GetNowTime() - (*it).move_start_time ) / 1000.0f;

		if( GetNowTime() >= (*it).move_end_time )
		{
			(*it).x = (*it).dx;
			(*it).y = (*it).dy;
			(*it).state = STATE_STOP;
			return;
		}

		float time = (float)( (float)( (*it).move_end_time - (*it).move_start_time ) / 1000.0f );
		DiffTime /= time;

		(*it).x = (float)( ( (float)(*it).dx - (float)(*it).px ) * (float)DiffTime + (float)(*it).px );
		(*it).y = (float)( ( (float)(*it).dy - (float)(*it).py ) * (float)DiffTime + (float)(*it).py );
	}
}
