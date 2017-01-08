#include <stdio.h>
#include <string.h>

void PrintPass( const char * pw )
{
	int i;
	for( i = 0; i < strlen( pw ); i++ )
	{
		printf( "\\x%02x", pw[i]+64 );
	}
	printf( "\n" );
}


int main()
{
	PrintPass( "WifiNetwork" );	
	PrintPass( "thisisabadpassword" );	
	return 0;
}

