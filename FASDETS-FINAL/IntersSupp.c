/****************************************
Nov 4, 2022
Patrick Wain

IntersSupp.c
Contains support for intersection class
Rules of Road (RoR)

ChangeLog
Dec 4
	New file for intersection functions
****************************************/

#include "Taxis.h"


/*
- Rules of the Road
- god help me
*/
void Rules_of_Road()
{
	/*
	- go through active taxis and find taxis with intersection condition
	- add to list of (street,ave) pairs w/taxis at them
	- check that list for multiple taxis
	- it is assume that taxis in activeList have at least st_dir or ave_dir
		this solution was not used

	- simpler solution:
		go through intersections and set delays from there
		takes more time b/c more intersection than taxis
		but less complicated to code
		upside is that each intersection can be cleared after it is checked
			this part ^ was not implemented, taxis are removed from intersection when they move in move functs
	*/
	INTERS* gen_inter = Intersections;
	int low_vin, vins[MAX_SMALL_ARRAY];

	for (int i = 0; i < (cityStreets + 1); i++)
	{
		for (int j = 0; j < (cityAves + 1); j++)
		{
			switch (gen_inter->state)
			{
			case EMPTY:
				break;
			case ___N:
				break;
			case __E_:
				break;
			case __EN:
				gen_inter->north->delay += 1;		//delay right taxi
				break;
			case _S__:
				break;
			case _S_N:
				if (gen_inter->south->vin > gen_inter->north->vin)
					gen_inter->south->delay += 1;		//lower vin first
				else
					gen_inter->north->delay += 1;
				break;
			case _SE_:
				gen_inter->east->delay += 1;		//delay right taxi
				break;
			case _SEN:
				/*
				- going to make a function to analyze the intersection and return lowest vin
				- idea:
					create an array with x vins to find lowest
					pass to function that finds lowest int for any array size
						done
				*/
				vins[0] = gen_inter->north->vin;
				vins[1] = gen_inter->east->vin;
				vins[2] = gen_inter->south->vin;
				low_vin = lowest_int(vins, 3);

				if (low_vin == gen_inter->south->vin)
				{
					gen_inter->east->delay += 1;		//these two will be dealt with on next pass
					gen_inter->north->delay += 1;		//this may lead to higher vins waiting longer
				}										//too bad
				else if (low_vin == gen_inter->east->vin)
				{
					gen_inter->north->delay += 1;
					gen_inter->south->delay += 1;
				}
				else if (low_vin == gen_inter->north->vin)
				{
					gen_inter->east->delay += 1;
					gen_inter->south->delay += 1;
				}
			case(W___):
				break;
			case(W__N):
				gen_inter->west->delay += 1;		//delay right taxi
				break;
			case(W_E_):
				if (gen_inter->west->vin < gen_inter->east->vin)
					gen_inter->east->delay += 1;				//lowest vin first
				else
					gen_inter->west->delay += 1;
				break;
			case(W_EN):
				vins[0] = gen_inter->north->vin;
				vins[1] = gen_inter->east->vin;
				vins[2] = gen_inter->west->vin;

				low_vin = lowest_int(vins, 3);

				if (low_vin == gen_inter->west->vin)
				{
					gen_inter->east->delay += 1;
					gen_inter->north->delay += 1;
				}
				else if (low_vin == gen_inter->east->vin)
				{
					gen_inter->west->delay += 1;
					gen_inter->north->delay += 1;
				}
				else if (low_vin == gen_inter->north->vin)
				{
					gen_inter->west->delay += 1;
					gen_inter->east->delay += 1;
				}
				break;
			case(WS__):
				gen_inter->south->delay += 1;		//delay right taxi
				break;
			case(WS_N):
				vins[0] = gen_inter->north->vin;
				vins[1] = gen_inter->south->vin;
				vins[2] = gen_inter->west->vin;

				low_vin = lowest_int(vins, 3);

				if (low_vin == gen_inter->west->vin)
				{
					gen_inter->south->delay += 1;
					gen_inter->north->delay += 1;
				}
				else if (low_vin == gen_inter->south->vin)
				{
					gen_inter->west->delay += 1;
					gen_inter->north->delay += 1;
				}
				else if (low_vin == gen_inter->north->vin)
				{
					gen_inter->west->delay += 1;
					gen_inter->south->delay += 1;
				}
				break;
			case(WSE_):
				vins[0] = gen_inter->east->vin;
				vins[1] = gen_inter->south->vin;
				vins[2] = gen_inter->west->vin;

				low_vin = lowest_int(vins, 3);

				if (low_vin == gen_inter->west->vin)
				{
					gen_inter->south->delay += 1;
					gen_inter->east->delay += 1;
				}
				else if (low_vin == gen_inter->south->vin)
				{
					gen_inter->west->delay += 1;
					gen_inter->east->delay += 1;
				}
				else if (low_vin == gen_inter->east->vin)
				{
					gen_inter->west->delay += 1;
					gen_inter->south->delay += 1;
				}
				break;
			case(WSEN):
				vins[0] = gen_inter->north->vin;
				vins[1] = gen_inter->east->vin;
				vins[2] = gen_inter->south->vin;
				vins[3] = gen_inter->west->vin;

				low_vin = lowest_int(vins, 4);

				if (low_vin == gen_inter->north->vin)		//lowest goes first
				{
					gen_inter->south->delay += 1;
					gen_inter->east->delay += 1;
					gen_inter->west->delay += 1;
				}
				else if (low_vin == gen_inter->east->vin)
				{
					gen_inter->north->delay += 1;
					gen_inter->south->delay += 1;
					gen_inter->west->delay += 1;
				}
				else if (low_vin == gen_inter->south->vin)
				{
					gen_inter->north->delay += 1;
					gen_inter->east->delay += 1;
					gen_inter->west->delay += 1;
				}
				else if (low_vin == gen_inter->west->vin)
				{
					gen_inter->north->delay += 1;
					gen_inter->east->delay += 1;
					gen_inter->south->delay += 1;
				}
				break;
			}
			gen_inter++;
		}
	}
}

/*
- sets up intersections for rules of the road
- for this to work, cityStreets and cityAves need values
*/
void init_intersections()
{
	/*
	- get some memory space for the intersections
	- because of our setup, cities actually have
		1 + street num, 1 + ave num
		streets/aves
	*/
	Intersections = malloc((cityStreets + 1) * (cityAves + 1) * sizeof(INTERS));

	/* build actual intersection */
	INTERS* gen_inter = Intersections;

	for (int i = 0; i < (cityStreets + 1); i++)
	{
		for (int j = 0; j < (cityAves + 1); j++)
		{
			gen_inter->ave = j;
			gen_inter->str = i;
			gen_inter->east = NULL;
			gen_inter->north = NULL;
			gen_inter->south = NULL;
			gen_inter->west = NULL;
			gen_inter->state = 0b0000;

			gen_inter++;
		}
	}
}

/*
- goes to the intersection of taxi and makes intersection point to taxi
- also updates the intersection state
- uses bit shifting for state
- state info is stored 0bWSEN
- remove function does the opposite
*/
void add_toInter(TAXI* taxi, enum card_dir dir)
{
	INTERS* gen_inter;
	int ptr_offset;

	switch (dir)
	{
	case(NORTH):
		ptr_offset = ((taxi->cur_street * (cityStreets + 1))) + (taxi->cur_ave+1);
		gen_inter = Intersections + ptr_offset;

		gen_inter->north = taxi;
		gen_inter->state |= (1 << NORTH);
		break;
	case(EAST):
		ptr_offset = ((taxi->cur_street * (cityStreets + 1))) + (taxi->cur_ave);
		gen_inter = Intersections + ptr_offset;

		gen_inter->east = taxi;
		gen_inter->state |= (1 << EAST);
		break;
	case(SOUTH):
		ptr_offset = ((taxi->cur_street * (cityStreets + 1))) + (taxi->cur_ave);
		gen_inter = Intersections + ptr_offset;

		gen_inter->south = taxi;
		gen_inter->state |= (1 << SOUTH);
		break;
	case(WEST):
		ptr_offset = (((taxi->cur_street+1) * (cityStreets + 1))) + (taxi->cur_ave);
		gen_inter = Intersections + ptr_offset;

		gen_inter->west = taxi;
		gen_inter->state |= (1 << WEST);
		break;
	}
}
void remove_fromInter(TAXI* taxi, enum card_dir dir)
{
	INTERS* gen_inter;
	int ptr_offset;

	switch (dir)
	{
	case(NORTH):
		ptr_offset = ((taxi->cur_street * (cityStreets + 1))) + (taxi->cur_ave+1);
		gen_inter = Intersections + ptr_offset;

		gen_inter->north = NULL;
		gen_inter->state &= ~(1 << NORTH);
		break;
	case(EAST):
		ptr_offset = ((taxi->cur_street * (cityStreets + 1))) + (taxi->cur_ave);
		gen_inter = Intersections + ptr_offset;

		gen_inter->east = NULL;
		gen_inter->state &= ~(1 << EAST);
		break;
	case(SOUTH):
		ptr_offset = ((taxi->cur_street * (cityStreets + 1))) + (taxi->cur_ave);
		gen_inter = Intersections + ptr_offset;

		gen_inter->south = NULL;
		gen_inter->state &= ~(1 << SOUTH);
		break;
	case(WEST):
		ptr_offset = (((taxi->cur_street+1) * (cityStreets + 1))) + (taxi->cur_ave);
		gen_inter = Intersections + ptr_offset;

		gen_inter->west = NULL;
		gen_inter->state &= ~(1 << WEST);
		break;
	}
}

/*
- Checks if intersection is occupied from 
- given direction, return false if occupied, true if not
*/
int intersOccupation(TAXI* taxi, enum card_dir dir)
{
	INTERS* gen_inter;
	int ptr_offset;

	ptr_offset = ((taxi->cur_street * (cityStreets + 1))) + (taxi->cur_ave);
	gen_inter = Intersections + ptr_offset;

	switch (dir)
	{
	case(NORTH):
		return (!(gen_inter->north));
	case(EAST):
		return (!(gen_inter->east));
	case(SOUTH):
		return (!(gen_inter->south));
	case(WEST):
		return (!(gen_inter->west));
	}
}