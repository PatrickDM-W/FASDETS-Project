/****************************************
Nov 07, 2022
Patrick Wain

TaxiSup.c
Contains support for taxi class
printing taxi and updating location

ChangeLog

Version 1.0
- Revamp of Project
- set_taxi_activity is now set_taxi_status
- moved passenger functions to separate file
Version 1.1
- find_available_taxi disabled
- needs to be updated with taxi store info
-	taxi store still needs to be done
- removed set_abs_xloc and set_abs_yloc
	integrated into draw_taxi
	and erase_taxi
****************************************/

#include "Taxis.h"
#include "Buildings.h"

//#define SYMBOL_V

/*
- subroutine pair to draw/erase  taxi at specified location
- currently only draws in a corner of an intersection
	no longer true
*/
void draw_taxi(TAXI* taxi)
{
	char ID = taxi->visualID;
	move_cursor(taxi->abs_yloc, taxi->abs_xloc);

#ifndef SYMBOL_V
	DISP(ID);
#endif

#ifdef SYMBOL_V
	DISP(TaxS);
#endif
}
void erase_taxi(TAXI* taxi)
{
	move_cursor(taxi->abs_yloc, taxi->abs_xloc);
	
	DISP(NoTax);
}

/*
- sets taxi location without position offsets
*/
#ifdef DEPRECATED
void set_taxi_loc(TAXI* taxi, int vin, int street, int ave)
{
	taxi->vin = vin;
	taxi->cur_ave = ave;
	taxi->cur_sub_ave = 0;
	taxi->cur_street = street;
	taxi->cur_sub_street = 0;
}
#endif

/*
- function pair for moving
- moving ON ave means adjusting street values
- sets xloc and yloc
- need to be reworked to add taxis to intersections when 
	they are reached
		done
- need to be reworked to remove taxis from intersections
	when they leave
		done
- need to be reworked, must check if intersection has a taxi before being moved there
	if true, don't move
	not done
- need to rework, check next direction when leaving intersection
	done
- need to rework, UP and LEFT are wrong currently
	done
- need to rework, reduce bloat

- current issue where taxi is not printed properly for 1 eff_tick after reaching destination

- since func parts are identical, refer to comments in move_OnAve for explanation
*/
void move_OnAve(TAXI* taxi)
{
	/* check direction, should not be 0 */
	if (taxi->ave_dir == RIGHT)
	{
		if (taxi->eff_moveTicks == 0)
		{
			taxi->abs_xloc = (taxi->cur_street * (BUI_W + RD_SIZE)) + ST_MIDPOINT;	// +2 for road_w, could be defined if you wanted wider streets
			taxi->abs_yloc = (taxi->cur_ave * (BUI_H + RD_SIZE)) + 1;				// defined now, +1 for right side of road
			taxi->eff_moveTicks++;
		}
		else if (taxi->eff_moveTicks == 1 && intersOccupation(taxi,WEST))		//taxi is approaching inter from WEST
		{
			taxi->abs_xloc = (taxi->cur_street * (BUI_W + RD_SIZE)) + ST_BLD_RIGHT_EDGE;
			taxi->abs_yloc = (taxi->cur_ave * (BUI_H + RD_SIZE)) + 1;
			taxi->eff_moveTicks++;
			add_toInter(taxi, WEST);		//if intersection is occupied, taxi must wait 
		}
		else if (taxi->eff_moveTicks == 2)	// reset eff_Ticks
		{
			//remove from intersection BEFORE moving
			remove_fromInter(taxi, WEST);
			
			/* 
			- check where taxi is going next
			- for proper cornering
			- check dir after incr street
				if ave_dir == RIGHT, existing code
				if ave_dir == NONE, check st_dir
					if st_dir == UP, set correct loc
					if st_dir == DOWN, set correct loc
				theoretically ave_dir != LEFT
			*/
			taxi->cur_street++;
			set_taxi_dir(taxi);

			if (taxi->st_dir == NONE && taxi->ave_dir == NONE)
				at_dest(taxi);

			if (taxi->ave_dir == RIGHT)
			{
				taxi->abs_xloc = (taxi->cur_street * (BUI_W + RD_SIZE)) + ST_BLD_LEFT_EDGE;
				taxi->abs_yloc = (taxi->cur_ave * (BUI_H + RD_SIZE)) + 1;
			}
			else
			{
				if (taxi->st_dir == UP)
				{
					taxi->abs_xloc = (taxi->cur_street * (BUI_W + RD_SIZE)) + 1;	
					taxi->abs_yloc = ((taxi->cur_ave-1) * (BUI_H + RD_SIZE)) + AVE_BLD_BOT_EDGE;	//theoretically cur_ave != 0 if st_dir == UP, system will crash if so
				}
				else if (taxi->st_dir == DOWN)
				{
					taxi->abs_xloc = (taxi->cur_street * (BUI_W + RD_SIZE));
					taxi->abs_yloc = (taxi->cur_ave * (BUI_H + RD_SIZE)) + AVE_BLD_TOP_EDGE;
				}
			}
			taxi->eff_moveTicks = 0;
		}
	}
	else if (taxi->ave_dir == LEFT)
	{
		if (taxi->eff_moveTicks == 0)
		{
			taxi->abs_xloc = ((taxi->cur_street-1) * (BUI_W + RD_SIZE)) + ST_MIDPOINT;		//theoretically cur_street != 0 if ave_dir == LEFT
			taxi->abs_yloc = (taxi->cur_ave * (BUI_H + RD_SIZE));
			taxi->eff_moveTicks++;
		}
		else if (taxi->eff_moveTicks == 1 && intersOccupation(taxi,EAST))
		{
			taxi->abs_xloc = ((taxi->cur_street - 1) * (BUI_W + RD_SIZE)) + ST_BLD_LEFT_EDGE;
			taxi->abs_yloc = (taxi->cur_ave * (BUI_H + RD_SIZE));
			taxi->eff_moveTicks++;
			add_toInter(taxi, EAST);
		}
		else if (taxi->eff_moveTicks == 2)
		{
			remove_fromInter(taxi, EAST);
			taxi->cur_street--;
			/* 
			- update direction
			- if ave_dir == LEFT, existing code
				else check st_dir
					if == UP, move to correct loc
					if == DOWN move to correct loc
			*/
			set_taxi_dir(taxi);

			if (taxi->ave_dir == NONE && taxi->st_dir == NONE)
				at_dest(taxi);

			if (taxi->ave_dir == LEFT)
			{
				taxi->abs_xloc = ((taxi->cur_street-1) * (BUI_W + RD_SIZE)) + ST_BLD_RIGHT_EDGE;
				taxi->abs_yloc = (taxi->cur_ave * (BUI_H + RD_SIZE));
			}
			else
			{
				if (taxi->st_dir == UP)
				{
					taxi->abs_xloc = (taxi->cur_street * (BUI_W + RD_SIZE)) + 1;			//+2 for gap, +1 for right side
					taxi->abs_yloc = ((taxi->cur_ave-1) * (BUI_H + RD_SIZE)) + AVE_BLD_BOT_EDGE;
				}
				else if (taxi->st_dir == DOWN)
				{
					taxi->abs_xloc = (taxi->cur_street * (BUI_W + RD_SIZE));
					taxi->abs_yloc = (taxi->cur_ave * (BUI_H + RD_SIZE)) + AVE_BLD_TOP_EDGE;
				}
			}

			taxi->eff_moveTicks = 0;
		}
	}
	else
	{
		//thought about returning a fail flag here just in case
	}
}
void move_OnSt(TAXI* taxi)
{
	/* check direction, should not be 0/NONE */
	if (taxi->st_dir == UP)
	{
		if (taxi->eff_moveTicks == 0)
		{
			taxi->abs_xloc = (taxi->cur_street * (BUI_W + RD_SIZE)) + 1;
			taxi->abs_yloc = ((taxi->cur_ave-1) * (BUI_H + RD_SIZE)) + AVE_MIDPOINT;
			taxi->eff_moveTicks++;
		}
		else if (taxi->eff_moveTicks == 1 && intersOccupation(taxi,SOUTH))
		{
			taxi->abs_xloc = (taxi->cur_street * (BUI_W + RD_SIZE)) + 1;
			taxi->abs_yloc = ((taxi->cur_ave - 1) * (BUI_H + RD_SIZE)) + AVE_BLD_TOP_EDGE;
			taxi->eff_moveTicks++;
			add_toInter(taxi, SOUTH);
		}
		else if (taxi->eff_moveTicks == 2)
		{
			remove_fromInter(taxi, SOUTH);
			taxi->cur_ave--;
			set_taxi_dir(taxi);

			if (taxi->st_dir == NONE && taxi->ave_dir == NONE)
				at_dest(taxi);
			
			if (taxi->st_dir == UP)
			{
				taxi->abs_xloc = (taxi->cur_street * (BUI_W + RD_SIZE)) + 1;
				taxi->abs_yloc = ((taxi->cur_ave-1) * (BUI_H + RD_SIZE)) + AVE_BLD_BOT_EDGE;
			}
			else
			{
				if (taxi->ave_dir == RIGHT)
				{
					taxi->abs_xloc = (taxi->cur_street * (BUI_W + RD_SIZE)) + ST_BLD_LEFT_EDGE;
					taxi->abs_yloc = (taxi->cur_ave * (BUI_H + RD_SIZE)) + 1;
				}
				else if (taxi->ave_dir == LEFT)
				{
					taxi->abs_xloc = ((taxi->cur_street-1) * (BUI_W + RD_SIZE)) + ST_BLD_RIGHT_EDGE;
					taxi->abs_yloc = (taxi->cur_ave * (BUI_H + RD_SIZE));
				}
			}
			taxi->eff_moveTicks = 0;
		}
	}
	else if (taxi->st_dir == DOWN)
	{
		if (taxi->eff_moveTicks == 0)
		{
			taxi->abs_xloc = (taxi->cur_street * (BUI_W + RD_SIZE));
			taxi->abs_yloc = (taxi->cur_ave * (BUI_H + RD_SIZE)) + AVE_MIDPOINT;
			taxi->eff_moveTicks++;

		}
		else if (taxi->eff_moveTicks == 1 && intersOccupation(taxi,NORTH))
		{
			taxi->abs_xloc = (taxi->cur_street * (BUI_W + RD_SIZE));
			taxi->abs_yloc = (taxi->cur_ave * (BUI_H + RD_SIZE)) + AVE_BLD_BOT_EDGE;
			taxi->eff_moveTicks++;
			add_toInter(taxi, NORTH);
		}
		else if (taxi->eff_moveTicks == 2)
		{
			remove_fromInter(taxi, NORTH);
			taxi->cur_ave++;
			set_taxi_dir(taxi);

			if (taxi->st_dir == NONE && taxi->ave_dir == NONE)
				at_dest(taxi);

			if (taxi->st_dir == DOWN)
			{
				taxi->abs_xloc = (taxi->cur_street * (BUI_W + RD_SIZE));
				taxi->abs_yloc = (taxi->cur_ave * (BUI_H + RD_SIZE)) + AVE_BLD_TOP_EDGE;
			}
			else
			{
				if (taxi->ave_dir == RIGHT)
				{
					taxi->abs_xloc = (taxi->cur_street * (BUI_W + RD_SIZE)) + ST_BLD_LEFT_EDGE;
					taxi->abs_yloc = (taxi->cur_ave * (BUI_H + RD_SIZE)) + 1;		//+1 for right side
				}
				else if (taxi->ave_dir == LEFT)
				{
					taxi->abs_xloc = ((taxi->cur_street-1) * (BUI_W + RD_SIZE)) + ST_BLD_RIGHT_EDGE;
					taxi->abs_yloc = (taxi->cur_ave * (BUI_H + RD_SIZE));
				}
			}

			taxi->eff_moveTicks = 0;
		}
	}
	else
	{
		// fail flag?
	}
}

/*
- removes taxi from given list
*/
void removeTaxi_fromList(int vin, TAXI* list)
{
	TAXI* curr;
	TAXI* prev;

	curr = list;
	prev = list;

	while (curr->vin != vin)
	{
		prev = curr;
		curr = curr->next_taxi;
	}
	/* curr now holds value of taxi to remove */
	if (curr == prev)
	{		// start of list
		if (list == activeList)
			activeList = curr->next_taxi;
		else if (list == inactiveList)
			inactiveList = curr->next_taxi;
	}
	else
	{
		prev->next_taxi = curr->next_taxi;
	}
}

/*
- check and set taxi direction
*/
void set_taxi_dir(TAXI* taxi)
{
	if (taxi->dest_street != NO_DEST)
	{
		if (taxi->cur_street < taxi->dest_street)
			taxi->ave_dir = RIGHT;
		else if (taxi->cur_street > taxi->dest_street)
			taxi->ave_dir = LEFT;
		else
			taxi->ave_dir = NONE;
	}
	else
		taxi->ave_dir = NONE;

	if (taxi->dest_ave != NO_DEST)
	{
		if (taxi->cur_ave < taxi->dest_ave)
			taxi->st_dir = DOWN;
		else if (taxi->cur_ave > taxi->dest_ave)
			taxi->st_dir = UP;
		else
			taxi->st_dir = NONE;
	}
	else
		taxi->st_dir = NONE;
}

/*
- function to walk through active taxi list and move them
- returns NULL if empty list
	not currently true
- needs to check for delay on taxis
*/
void moveActiveTaxis()
{
	/* first taxi */
	TAXI* curr = activeList;
	TAXI* next = activeList;

	while (curr != NULL)			//List condition
	{
		next = curr->next_taxi;		//save next taxi b/c Move functions change this info
		erase_taxi(curr);

		if (curr->delay)
			curr->delay--;
		else
		{

			/* this entire if else structure should be updated to use direction fields */
			/* location checks need to be a touch more complex for which bld corners */
			/* move to set taxi dir? I think so */
			if (curr->ave_dir == NONE && curr->st_dir == NONE)
				at_dest(curr);

			else if (curr->ave_dir)		// ave_dir = LEFT/RIGHT
			{	//needs to get to street
				move_OnAve(curr);
			}
			else
			{	// ave_dir = NONE		// needs to get to ave
				move_OnSt(curr);
			}
		}
		draw_taxi(curr);

#ifdef DEPRECATED
		/*
		- this will probably be moved,
		- only need to set dir after leaving intersection
		- remove bloat, also corner properly
		*/
		set_taxi_dir(curr);
#endif

		curr = next;
	}

}

/*
- clears visual clutter from inactive taxis
*/
void eraseInactiveTaxis()
{
	TAXI* curr = inactiveList;

	while (curr != NULL)
	{
		erase_taxi(curr);
		curr = curr->next_taxi;
	}

}

/*
- function to be carried out whenever a taxi reaches its destination
- st_dir = ave_dir == NONE
- cur_street == dest_street
- cur_ave == dest_ave
*/
void at_dest(TAXI* taxi)
{
	/* is passenger pick up? */
	if (taxi->cur_ave == taxi->pass->passLocAve && taxi->cur_street == taxi->pass->passLocStr)
	{	// @passenger
		/* pick up */
		taxi->dest_ave = taxi->pass->passDestAve;
		taxi->dest_street = taxi->pass->passDestStr;

#ifdef DEBUG
		move_cursor(5, (cityStreets * (BUI_W + RD_SIZE)) + 5);
		printf("taxi:%d\tpick up @: %d,%d", taxi->vin, taxi->cur_street, taxi->cur_ave);
#endif

	}
	else if (taxi->cur_ave == taxi->pass->passDestAve && taxi->cur_street == taxi->pass->passDestStr)
	{	// @drop off
		/* drop off */
		/* this needs code! */
		removeTaxi_fromList(taxi->vin, activeList);
		taxi->next_taxi = inactiveList;
		inactiveList = taxi;
		/* change destination */
		taxi->dest_ave = NO_DEST;
		taxi->dest_street = NO_DEST;

#ifdef DEBUG
		move_cursor(5, (cityStreets * (BUI_W + RD_SIZE)) + 5);
		printf("taxi:%d\tdrop off @: %d,%d", taxi->vin, taxi->cur_street, taxi->cur_ave);
#endif

		/* placeholder ^, actually need to go park/charge */
	}
	else
	{
		/* charing spot or parking spot */
		/* this will need code */
		return;
	}
	set_taxi_dir(taxi);
}

#ifdef DEBUG
/*
- finds available taxi for trip

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
int find_available_taxi(int taxiNum)
{
	int inactiveTaxiAddress = 65;
	int cnt = 0;
	int inactiveFound = FALSE;
	int num = taxiNum;

	while (cnt < num && inactiveFound == FALSE)
	{

	}
}
#endif		//DEBUG