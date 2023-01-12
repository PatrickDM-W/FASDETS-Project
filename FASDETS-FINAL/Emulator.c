/****************************************
Nov 17, 2022
Patrick Wain

Emulator.c
emulator function
beefy function

Version 1.1
- bulk of this code copied from old version of project

Version 1.2
- building store is a proper store now

Dec 4
	Reorganize code
	building functions moved to citySupp.c
	Taxi store functions moved to Taxis.c
	Intersection functions in IntersSupp.c (new file)
****************************************/

#include "Taxis.h"
#include "Buildings.h"
#include "Passengers.h"
#include "Menuing.h"

/*
- it is assumed all required files are open at this point
- all files must be read to get required data
- int returned for errors
	this got abandoned a bit
- inactiveTaxis, activeTaxis are both global variables
- Currently this function can only be run once per launch
- this is because there is no check on the passenger store to start at the beginning
	the passenger info function is low complexity and just reads one line and doesn't reposition at all
*/
int runEmulator(int delay_ms)
{
#ifdef DEPRECATED
	/* city size variables */
	/* cityStrees, cityAves have been made global */
	// int cityStreets, cityAves;
	/*
	- gen_build is not strictly necessary, can be redone
	- it should be redone, just unnecessary
	*/
	BUILD* gen_build = malloc(sizeof(BUILD));
#endif

	/* initialize lists */
	activeList = NULL;
	inactiveList = NULL;
	waitingPassList = NULL;

	/* build city from file info */
	readCity();
	/*
	- this can be redone because cityStreets and cityAves are now globals
	- this function broken now :( upgrading the cityStore broke it
	- this function needs to be redone not using a general building
	- use global Buildings pseudo array instead
	*/
	draw_city();

	/*
	- set up intersections
	- this is necessary for rules of the road
	*/
	init_intersections();

	/* taxi setup */
	char data[MAX_CHAR_IN];

	// redundant position to beginning of active taxis
	fseek(activeTaxis, 0, SEEK_SET);
	fgets(data, MAX_CHAR_IN, activeTaxis);
	int total_taxi_cnt = atoi(data);

	/* too many/too few taxi protection */
	if (total_taxi_cnt == 0 || total_taxi_cnt >= ((cityStreets+1) * (cityAves+1)))
	{
		system("cls");
		printf("emulation failed!");
		return -1;
	}

	/* extract rest of data */
	if (!read_activeTaxis(total_taxi_cnt))
	{
		system("cls");
		printf("emulation failed!");
		return -2;
	}

	/* inactive taxis are initialized */
	/* draw at initial locations */
	drawTaxiList(inactiveList);

	/* get first passenger info */
	/* reposition to top */
	fseek(passengerReq, 0, SEEK_SET);


	PASSENGER* gen_pass;
	gen_pass = get_pass_info();

	int currentTime = 0;
	TAXI* gen_taxi;

	/* 
	- time to enter emulator proper
	- while condition needs to be more clever 
		done
	*/
	while (gen_pass || activeList || waitingPassList)
	{
		/* deal with waiting list first */
		while (waitingPassList && inactiveList)		//neither list is empty
		{
			gen_taxi = find_nearestTaxi(waitingPassList->passLocStr, waitingPassList->passLocAve, inactiveList);

			gen_taxi->pass = waitingPassList;		//take from top of list

			/* remove from waiting list */
			waitingPassList = waitingPassList->nextPass;
			gen_taxi->pass->nextPass = NULL;

			/* go to pick up */
			gen_taxi->dest_ave = gen_taxi->pass->passLocAve;
			gen_taxi->dest_street = gen_taxi->pass->passLocStr;
			set_taxi_dir(gen_taxi);

			/* remove from inactive */
			removeTaxi_fromList(gen_taxi->vin, inactiveList);

			/* add to active */
			gen_taxi->next_taxi = activeList;
			activeList = gen_taxi;
		}

		/* currentTime = reqTime? */
		while (gen_pass != NULL && currentTime >= gen_pass->passTime)
		{
			/* find a taxi */
			gen_taxi = find_nearestTaxi(gen_pass->passLocStr, gen_pass->passLocAve, inactiveList);
			if (gen_taxi == NULL)
			{
				/* no available taxis, do something here? */
				/* add to passenger waiting list @ end */
				add_PassWait(gen_pass);
			}
			else
			{
				/* use general taxi, add to active list remove from inactive */
				gen_taxi->pass = gen_pass;
				gen_taxi->dest_ave = gen_taxi->pass->passLocAve;
				gen_taxi->dest_street = gen_taxi->pass->passLocStr;
				set_taxi_dir(gen_taxi);

				removeTaxi_fromList(gen_taxi->vin, inactiveList);

				gen_taxi->next_taxi = activeList;
				activeList = gen_taxi;
			}

			/* keep updating passenger req if there's more */
			gen_pass = get_pass_info();
			/* if gen_pass ever gets the value of NULL, end of passengers */
		}
		/* all current requests should be loaded at this point */
		/* movement time! */

		moveActiveTaxis();

		eraseInactiveTaxis();

		/* 
		- all taxis have moved, check
		- check Rules of Road (RoR)
		*/
		Rules_of_Road();

		/* end things */
		/* 
		- for some reason manual control doesn't work?? idk
		- the getchar() may be messing something up in the 
		- get_pass_info() funct
			Dec 4, manual control worked, unsure why
			not a priority
		*/
		if (delay_ms)
			Sleep(delay_ms);
		else
			getchar();

		currentTime++;
	}

	/* 
	- technically everything needs to be freed now 
	- currently passengers are not freed
	*/
	emptyActiveList();
	emptyInactiveList();
	free(Buildings);
	free(Intersections);
	return 0;
}

/*
- draws the taxi list specified by the func arg
- uses draw_taxi(TAXI)
*/
void drawTaxiList(TAXI* list)
{
	TAXI* curr = list;

	while (curr != NULL)
	{
		draw_taxi(curr);
		curr = curr->next_taxi;
	}
}

/*
- Finds the nearest taxi based on a passed location
- and taxi list
- returns NULL if given list is empty
*/
TAXI* find_nearestTaxi(int street, int ave, TAXI* list)
{
	/* check for empty list */
	if (list == NULL)
		return NULL;

	int street_dist, ave_dist, tdist, lowest_dist;
	TAXI* curr = list;
	TAXI* closestTaxi;

	street_dist = abs(street - curr->cur_street);
	ave_dist = abs(ave - curr->cur_ave);
	tdist = street_dist + ave_dist;

	lowest_dist = tdist;
	closestTaxi = curr;

	curr = curr->next_taxi;

	while (curr != NULL)
	{
		street_dist = abs(street - curr->cur_street);
		ave_dist = abs(ave - curr->cur_ave);
		tdist = street_dist + ave_dist;

		if (tdist < lowest_dist)
			closestTaxi = curr;

		curr = curr->next_taxi;
	}

	return closestTaxi;
}

/*
- functions to free memory
*/
void emptyActiveList()
{
	TAXI* curr;
	TAXI* prev;
	curr = activeList;
	prev = activeList;

	while (curr != NULL)
	{
		prev = curr;
		curr = curr->next_taxi;
		free(prev);
	}
}
void emptyInactiveList()
{
	TAXI* curr;
	TAXI* prev;
	curr = inactiveList;
	prev = inactiveList;

	while (curr != NULL)
	{
		prev = curr;
		curr = curr->next_taxi;
		free(prev);
	}
}
#ifdef DEPRECATED
void emptyBuildings()
{
	int ptr_offset;
	BUILD* gen_build;

	for (int i = 0; i < cityStreets; i++)
	{
		for (int j = 0; j < cityAves; j++)
		{
			ptr_offset = (i * cityStreets * sizeof(BUILD)) + (j * sizeof(BUILD));
			gen_build = Buildings + ptr_offset;

			free(gen_build);
		}
	}
}
#endif

/*
- not sure where this goes cause its a general function
- returns lowest int in a given array
*/
int lowest_int(int arr[], int size)
{
	int lowest = arr[0];
	
	for (int i = 1; i < size; i++)
	{
		if (arr[i] < lowest)
			lowest = arr[i];
	}
	return lowest;
}