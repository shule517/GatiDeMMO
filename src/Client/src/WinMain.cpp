
#include "StdAfx.h"
#include "Game.h"

/*--------------------------------------------
	���C��
---------------------------------------------*/
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow )
{
	CGame *game = new CGame();
	game->doMainLoop( hInst, nCmdShow );
	delete game;

	return 0;
}
