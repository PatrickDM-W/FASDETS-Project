/****************************************
Nov 07, 2022
Patrick Wain

MenuSup.c
Functions to facilitate cursor control
other functions

ChangeLog

Version 1.0
Revamp of project
Version 1.1
- renamed to MenuSup.c
-	previously Cursor.c
-	more accurate to function(s) here
****************************************/

#include "Menuing.h"

/*
 func to move cursor to a spec location
 code provided by Dr. Hughes
*/
void move_cursor(int y, int x)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;

	/* windows function to move cursor */
	SetConsoleCursorPosition(Console, coord);
}

/*
- simple function to display given files
- I didn't want to retype it every time
*/
void display_given_files(int argc, char* argv[])
{
	printf("You have provided %d files\n", argc - 1);
	for (int i = 1; i < argc; i++)
		printf("(%d) %s\n", i, argv[i]);
}

/*
- clears screen and displays main menu
*/
void main_menu(void)
{
	system("cls");		//this function clears the window
	printf("Welcome to FASDETS\n\n");
	printf("What would you like to do?\n");
	printf("(%d) Run Emulation (WIP)\n", RUN);
	printf("(%d) View/Edit taxiStore (not supported yet)\n", EDIT);
	printf("(%d) Initialize taxiStore (will create new file)\n", INIT);
	printf("(%d) Add to taxiStore (taxiStore must be provided)\n", ADD);
	printf("(%d) View taxiStore (taxiStore must be provided)\n", VIEW);			
	printf("(%d) Add maintenance record (maintenance file must be provided)\n", SERVICE);
	printf("(%d) Quit\n", QUIT);
}

