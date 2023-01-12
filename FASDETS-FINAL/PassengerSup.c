/****************************************
Nov 07, 2022
Patrick Wain

PassengerSup.c
Contains support for passenger access
file access and reading

ChangeLog

Version 1.0
Revamp of Project
Version 1.1
- slight reorganization
- passenger store should not need to change
- reorganized functions into a single function
	all hail the master function
****************************************/

#include "Passengers.h"
#include "Taxis.h"

/* 
- edits passed parameters to hold the info stored in file
	DOES NOT DO ^ NOW
- creates a passenger struct and returns the address
- info in file is stored:
	time	origin street,ave	destination street,ave
*/
PASSENGER* get_pass_info()
{
	char data;
	int time = 0;
	int LocAve = 0;
	int DestAve = 0;
	int LocStreet = 0;
	int DestStreet = 0;

	PASSENGER* pass = malloc(sizeof(PASSENGER));

	/* time */
	while ((data = fgetc(passengerReq)) <= '9' && data >= '0')
		time = time * 10 + (data - NUM_OFFSET);
	pass->passTime = time;

	/* origin street */
	while ((data = fgetc(passengerReq)) <= '9' && data >= '0')
		LocStreet = LocStreet * 10 + (data - NUM_OFFSET);
	pass->passLocStr = LocStreet;

	/* origin ave */
	while ((data = fgetc(passengerReq)) <= '9' && data >= '0')
		LocAve = LocAve * 10 + (data - NUM_OFFSET);
	pass->passLocAve = LocAve;

	/* dest street */
	while ((data = fgetc(passengerReq)) <= '9' && data >= '0')
		DestStreet = DestStreet * 10 + (data - NUM_OFFSET);
	pass->passDestStr = DestStreet;

	/* dest ave */
	while ((data = fgetc(passengerReq)) <= '9' && data >= '0')
		DestAve = DestAve * 10 + (data - NUM_OFFSET);
	pass->passDestAve = DestAve;

	/* doesn't need a next pass */
	pass->nextPass = NULL;

	/* in valid passenger check */
	if (pass->passTime == 0)
		return NULL;

	return pass;
}

/*
- adds passenger to end of waiting list
*/
void add_PassWait(PASSENGER* passenger)
{
	PASSENGER* curr = waitingPassList;

	if (curr == NULL)		//list is empty
	{
		passenger->nextPass = NULL;
		waitingPassList = passenger;
		return;
	}

	while (curr->nextPass != NULL)		//as soon as NEXT pass is NULL, end of list
	{
		curr = curr->nextPass;
	}
	/* curr holds value of last passenger in list */
	passenger->nextPass = curr->nextPass;
	curr->nextPass = passenger;

	return;
}