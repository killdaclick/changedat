// ZmienDat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <direct.h>
#include <Windows.h>
#include "dirent.h"

char currDir[FILENAME_MAX];

bool isDatFile( char* fname )
{
	char* f = fname;
	char* datExt = ".dat";
	char datExtPtr = 0;
	while( *f != '\0' )
	{
		if( *f == datExt[datExtPtr] )
		{
			datExtPtr++;
			if( datExtPtr == 4 )
			{
				char* ff = f;
				ff++;
				if( *ff == '\0' )
					return true;
				datExtPtr = 0;
			}
		}
		else
			datExtPtr = 0;
		f++;
	}
	return false;
}

bool getFileNameWithoutDat( char* fname, char* newName )
{
	char* f = fname;
	char fCnt = 0;
	char* datExt = ".dat\0";
	char datExtPtr = 0;
	while( f[fCnt] != '\0' )
	{
		if( f[fCnt] == datExt[datExtPtr] )
		{
			datExtPtr++;
			if( datExtPtr == 4 )
			{
				if( f[fCnt+1] == '\0' )
				{
					for( int i=0; i< fCnt-3; i++ )
						newName[i] = f[i];
					return true;
				}
				datExtPtr = 0;
			}
		}
		else
			datExtPtr = 0;
		fCnt++;
	}
	return false;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if( !_getcwd(currDir, sizeof(currDir)) )
		return errno;
	printf("Katalog roboczy: %s\n\n", currDir);

	DIR* dir;
	struct dirent* ent;
	if( (dir = opendir(currDir)) != NULL )
	{
		while( (ent = readdir(dir)) != NULL )
		{
			if( isDatFile(ent->d_name) )
			{
				char newName[FILENAME_MAX] = {0};
				if( getFileNameWithoutDat(ent->d_name, newName) )
				{
					if( rename(ent->d_name, newName) != 0 )
						printf("Blad zmiany nazwy pliku '%s' na nowa nazwe '%s' - pomijam\n", ent->d_name, newName);
					else
						printf("Zmieniam nazwe pliku '%s' na nowa nazwe '%s'\n", ent->d_name, newName);
				}
			}
		}
	}
	else
	{
		printf("Blad odczytu katalogu roboczego\n");
		return EXIT_FAILURE;
	}

	return 0;
}

