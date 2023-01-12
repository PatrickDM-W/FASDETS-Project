/****************************************
Nov 07, 2022
Patrick Wain

Menuing.h
Header file containing menu support functions
printing menus and cursor control

Version 1.1
- Header file created
****************************************/

/*
-Options for opening a file
- Mode
- "w+" - creates an empty file for R& W
- "a+" - opens a file for appending, fseek() is ignored
- "w" - creates new file(unlinks existing) for writing**
- "r+" - opens for reading and writing, file MUST exist**
*/ 

#pragma once
#ifndef MENUING_H
#define MENUING_H

#include "FASDETS.h"

/* current menu options */
enum options {QUIT, RUN, EDIT, INIT, ADD, VIEW, SERVICE};

/* menu controls */
extern void display_given_files(int argc, char* argv[]);
extern void main_menu(void);


/* cursor controls */
extern void move_cursor(int y, int x);
HANDLE Console;

#endif // !MENUING_H