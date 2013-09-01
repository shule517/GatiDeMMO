
#include <stdio.h>
#include <winsock2.h>
#include "Network.h"

#pragma comment ( lib, "ws2_32.lib" )
#pragma comment (lib, "winmm.lib")

int main( int argc, char* argv[] )
{
	CNetwork nw;

	printf( "Charctor data loading...\n" );
	Sleep( 100 );
	printf( "-> Total 582 charctors loaded.\n\n");

	printf( "Item data loading...\n" );
	Sleep( 20 );
	printf( "-> Total 102 items loaded.\n\n" );

	printf( "Monster data loading...\n" );
	Sleep( 50 );
	printf( "-> Total 216 monsters loaded.\n\n" );

	printf( "Map data loading...\n" );
	Sleep( 380 );
	printf( "-> Total 38 maps loaded.\n\n" );

	printf( "*** Startup Success ***\n" );

	nw.init( 1000 );

	while( TRUE )
	{
		Sleep( 1000000 );
	}

	return 0;
}
