
#include "StdAfx.h"
#include "Game.h"

/* ========================================================================= */
/* �֐����FCGame::CGame														 */
/* ���e�F�R���X�g���N�^														 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
CGame::CGame( void )
{
}

/* ========================================================================= */
/* �֐����FCGame::~CGame													 */
/* ���e�F�f�X�g���N�^														 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
CGame::~CGame( void )
{
}

/* ========================================================================= */
/* �֐����FCGame::loadFiles													 */
/* ���e�F�t�@�C���̓ǂݍ���													 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
void CGame::loadFiles( void ) throw(int)
{
	EffectCnt.loadFiles();
	MapCnt.loadFiles();
	PlayerChara.loadFiles();
	MyChara.loadFiles();
}

/* ========================================================================= */
/* �֐����FCGame::initStage													 */
/* ���e�F�X�e�[�W�̓ǂݍ���													 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
void CGame::initStage( void )
{
	EffectCnt.init();
	MapCnt.init();
	PlayerChara.init();
	MyChara.init( &Network );

	Network.start( MyChara.getCharaList(), PlayerChara.getPlayerList(), EffectCnt.getEffectList() ); // �l�b�g�ڑ�
}

/* ========================================================================= */
/* �֐����FCGame::drawGameMain												 */
/* ���e�F�Q�[�����C��														 */
/* ���t�F2007/09/11															 */
/* ========================================================================= */
void CGame::drawGameMain( void )
{
	MapCnt.move( MyChara.getX(), MyChara.getY() );
	PlayerChara.move( MyChara.getX(), MyChara.getY() );
	MyChara.move();
	EffectCnt.move( MyChara.getX(), MyChara.getY() );
}
