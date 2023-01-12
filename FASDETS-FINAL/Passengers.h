/****************************************
Nov 07, 2022
Patrick Wain

Passengers.h
Header file containing definitions for
passengers and function decelerations

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ChangeLog

Version 1.0
- created new header for passenger functions and decelerations specifically
Version 1.1
- no changes so far
- intending to include more store management functions
- made passenger into a structure?
- changed get_pass_info to work with a struct
****************************************/

#pragma once
#ifndef PASSENGERS_H
#define PASSENGERS_H

#include "FASDETS.h"

struct passenger 
{
	int passTime;
	int passLocAve;
	int passDestAve;
	int passLocStr;
	int passDestStr;
	struct passenger* nextPass;
};
typedef struct passenger PASSENGER;

extern PASSENGER* get_pass_info(void);
extern void add_PassWait(PASSENGER* passenger);

/* global lists */
PASSENGER* waitingPassList;

#endif // !PASSENGERS_H