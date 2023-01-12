/****************************************
Nov 07, 2022
Patrick Wain

Taxis.h
Header file containing definitions for
taxis and function decelerations

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ChangeLog

Version 1.0
- created new header for taxi functions and decelerations specifically
- enum for taxi status
- enum for taxi activity
- defined taxi as int (previously '#')
- renamed taxi symbol to TaxS
Version 1.1
- No changes so far
- will attempt store management
- removed taxi_status enum
- removed taxi_activity enum
- added taxi_rec
	different from taxi
	taxi_rec is the actual file record
	includes
		vin
		max charge
		charge rate
	taxi is what is stored in system
- taxis now carry a passenger struct
****************************************/

#pragma once
#ifndef TAXIS_H
#define TAXIS_H

#include "FASDETS.h"
#include "Passengers.h"
#include "Buildings.h"

/* taxi symbol */
#define TaxS 35
#define NoTax 32

/* taxi constants */

/* everything here should be deprecated now */
#ifdef DEPRECATED
/* position constants */
/* locations of steps along avenue
		|____|
	  WA_X_Y__Z
*/
#define INTERSECTION_AVE 0	// W
#define FIRST_AVE 3			// X
#define SECOND_AVE 5		// Y
#define AVENUE_RIGHT 1		// A

/* locations of steps along street
		W
		A
	+	_
	|	X
	|	Y
	+	_
		Z
*/
#define INTERSECTION_STREET 0	//W
#define FIRST_STREET 3			//X
#define SECOND_STREET 4			//Y
#define STREET_RIGHT 1			//A
#endif

/* I reworked pos consts */
enum AVE_POS {AVE_MIDPOINT = (int)(BUI_H/2) + 2, AVE_BLD_TOP_EDGE = 2, AVE_BLD_BOT_EDGE = 1 + BUI_H};
enum ST_POS {ST_MIDPOINT = (int)(BUI_W/2) + 2, ST_BLD_RIGHT_EDGE = 1 + BUI_W, ST_BLD_LEFT_EDGE = 2 };
/* destination constants */
#define NO_DEST -1

/* direction constants */
enum DIR {LEFT = -2, UP = -1, NONE = 0, DOWN = 1, RIGHT = 2};

/* structures */
struct taxi			//this is stored in the system
{
	int vin;								// vehicle identification number
	int cur_street, cur_ave;				// current address
	int dest_street, dest_ave;				// destination address
#ifdef DEPRECATED
	int cur_sub_street, cur_sub_ave;		// sub values for street and avenue
	int next_dest_street, next_dest_ave;	// used to store the final destination of the passenger
											// these may not be necessary anymore
#endif
	enum DIR st_dir, ave_dir;
	int eff_moveTicks;						// for movement functions
	int delay;								// for RulesOfRoad
	int abs_xloc, abs_yloc;					// absolute console locations

	struct taxi* next_taxi;
	PASSENGER* pass;

	/* battery stuff */
	int max_charge;
	int charge_rate;

	char visualID;
};
typedef struct taxi TAXI;

struct taxi_header	//this is header of the records
{
	int first_vin;							//first vin available
};
typedef struct taxi_header TAX_HEAD;

struct taxi_rec		//this is what is stored in the store
{
	int vin;
	int max_charge;
	int charge_rate;
	/* will have more info as project updates */
};
typedef struct taxi_rec TAX_REC;

/* 
- store management functions
- I want to rebuild these functions
	done
- these are in Taxis.c
*/
extern void init_taxis(void);
extern void update_taxis(void);		//this is the only one that doesn't work
extern void add_taxis(void);
extern void displaytaxiStore(void);
extern void add_maintenance(void);

/* general functions */
extern void draw_taxi(TAXI* taxi);
extern void erase_taxi(TAXI* taxi);
#ifdef DEPRECATED
extern void set_taxi_loc(TAXI* taxi, int x_offset, int y_offset);
#endif
extern int find_available_taxi(int taxiNum);
extern void set_taxi_dir(TAXI* taxi);

#ifdef DEBUG
extern void display_tax_info(TAXI taxi);	//unsure about this function
#endif

/* movement functions */
#ifdef DEPRECATED
extern void increase_ave(TAXI* taxi);
extern void decrease_ave(TAXI* taxi);
extern void increase_street(TAXI* taxi);
extern void decrease_street(TAXI* taxi);
#endif
extern void move_OnAve(TAXI* taxi);
extern void move_OnSt(TAXI* taxi);
extern void at_dest(TAXI* taxi);

/* global lists */
TAXI* activeList;
TAXI* inactiveList;

/* list functions */
extern void drawTaxiList(TAXI*);
extern TAXI* find_nearestTaxi(int street, int ave, TAXI*);
extern void moveActiveTaxis();
extern void eraseInactiveTaxis();
extern void removeTaxi_fromList(int vin, TAXI* list);

/* 
- rules of the road things 
- in new file IntersSupp.c
*/
extern void Rules_of_Road();
extern void init_intersections();
/* 
- this enum has to do with bitshifting for intersection state 
- state is stored
	0bWSEN
*/
enum card_dir{NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};
enum inters_state { EMPTY, ___N, __E_, __EN, _S__, _S_N, _SE_, _SEN, W___, W__N, W_E_, W_EN, WS__, WS_N, WSE_, WSEN };
extern void add_toInter(TAXI* taxi, enum card_dir dir);		//currently in emulator.c
extern void remove_fromInter(TAXI* taxi, enum card_dir dir);
extern int intersOccupation(TAXI* taxi, enum card_dir dir);


struct intersections
{
	int str, ave;
	char state;									//state is stored 0bWSEN
	TAXI* north, * east, * south, * west;		//taxi for each direction
};
typedef struct intersections INTERS;
INTERS* Intersections;

#endif // !TAXIS_H