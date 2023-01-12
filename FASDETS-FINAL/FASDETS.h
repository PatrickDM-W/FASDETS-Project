/****************************************
Nov 07, 2022
Patrick Wain

FASDETS.h
Header file containing common files and definition
building structures

Version 1.0
- removed building height/width
-	width/height constant used directly
Version 1.1
- reorganization
- file access
- display_given_files() moved to Menuing.h
- move_cursor() moved to Menuing.h
Dec 4
- added DEPRECATED definition for uneccessary functions
- anything under a DEPRECATED tag is from an old version
****************************************/

/*
- Streets run north to south
- Avenues run east to west
*/

#pragma once
#ifndef FASDETS_HEADER
#define FASDETS_HEADER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>		// for _O_TEXT and _O_BINARY

/* constants */
#define NUL 0x00
#define ESC 0x1b
#define TRUE 1
#define FALSE 0
#define FILENAMELEN 64
#define MAX_SMALL_ARRAY 16
#define COMMENT_LEN 256

/* character offsets */
#define NUM_OFFSET 48

/* max/mins */
#define MAX_CHAR_IN 8

/* macros */
#define DISP(x) putc((x),stdout)
#define TRUNCATE(string) string[strlen(string)-1] = NUL
#define MY_CLOSE(file) if(file) fclose(file)

/* external files */
FILE* cityStore;
FILE* taxiStore;
FILE* passengerReq;
FILE* activeTaxis;
FILE* maintainHist;

/* misc functions, may need to be moved */
/* these files all in Emulator.c currently */
extern int runEmulator(int delay_ms);
extern int read_activeTaxis(int);
extern void emptyActiveList();
extern void emptyInactiveList();
#ifdef DEPRECATED
extern void emptyBuildings();
#endif
extern int lowest_int(int arr[], int size);		//could be modified for any basic data type

#endif // !FASDETS_HEADER