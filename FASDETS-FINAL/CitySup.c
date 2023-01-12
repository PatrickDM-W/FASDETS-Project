/****************************************
Nov 07, 2022
Patrick Wain

CitySup.c
support functions for building structure

ChangeLog

Version 1.0
- revamp of project and all files
Version 1.1
- reorganization
Dec 4
	uses building pseudoarray
	Buildings is base of array
	added more general functions
	readCity()
	spec build info()
****************************************/

#include "Buildings.h"

/****************************************
function to draw a building
passed building contains a size of building
and a location
****************************************/
void draw_build(BUILD* build)
{
	/* top row */
	move_cursor(build->yloc, build->xloc);
	DISP(UL);
	for (int relx = 0; relx < BUI_W - 2; relx++)		// -2 for the corners
		DISP(HBAR);
	DISP(UR);

	/* sides */
	for (int rely = 1; rely < BUI_H - 1; rely++)		// rely = 1 for corner, -1 for corner
	{
		move_cursor(build->yloc + rely, build->xloc);
		DISP(VBAR);
		move_cursor(build->yloc + rely, build->xloc + BUI_W - 1);	//-1 for bottom corners
		DISP(VBAR);
	}

	/* bot row */
	move_cursor(build->yloc + BUI_H - 1, build->xloc);		// -1 for offset
	DISP(LL);
	for (int relx = 0; relx < BUI_W - 2; relx++)		// -2 for the corners
		DISP(HBAR);
	DISP(LR);

	/* building number */
	move_cursor(build->yloc + 1, build->xloc + 1);		//+1 +1 to be just below UL corner
	printf("s:%d", build->street);
	move_cursor(build->yloc + 2, build->xloc + 1);		//+2 +1 to be below prev
	printf("a:%d", build->ave);
	/* parking info */
	move_cursor(build->yloc + 3, build->xloc + 1);		//+3 below prev
	printf("p:%d", build->free_prk_space);
	move_cursor(build->yloc + 4, build->xloc + 1);		//+4 below prev
	printf("c:%d", build->free_chrg_space);

	return;
}

/****************************************
function to draw the city
Calls draw_build for each building
passed building contains a size of the building
number of streets and avenues is passed
- needs to be done w/ pseudo-array
	in progress
	complete
****************************************/
void draw_city()
{
	BUILD* gen_build = Buildings;

	for (int i = 0; i < cityStreets; i++)				// i is y index
	{
		for (int j = 0; j < cityAves; j++)				// j is x index
		{
			draw_build(gen_build);

			gen_build++;
		}
	}
}

/*
- extracts data from cityStore
- updates cityStreets/Aves
- needs to be updated in future
- will need to create a array
- also sets up intersections for rules of road
	this is not true, done external to this
*/
void readCity()
{
	char c_streets[MAX_CHAR_IN], c_aves[MAX_CHAR_IN];

	/* redundant, ensure file is open to beginning */
	fseek(cityStore, 0, SEEK_SET);

	fgets(c_streets, MAX_CHAR_IN, cityStore);
	fgets(c_aves, MAX_CHAR_IN, cityStore);

	cityStreets = atoi(c_streets);
	cityAves = atoi(c_aves);

	Buildings = malloc(cityStreets * cityAves * sizeof(BUILD));	// buildings is global

	/* copy the actual buildings */
	BUILD* gen_build;
	int ptr_offset;

	/* this loads default settings*/
	for (int i = 0; i < cityStreets; i++)			// i is street - 1
	{
		for (int j = 0; j < cityAves; j++)			// j is ave - 1
		{
			ptr_offset = (i * cityStreets) + j;
			gen_build = Buildings + ptr_offset;

			gen_build->ave = j + 1;
			gen_build->street = i + 1;
			/* set x and y here as well */
			gen_build->yloc = ((BUI_H + RD_SIZE) * i) + 2;	//+2 for street width, +2 for top gap
			gen_build->xloc = ((BUI_W + RD_SIZE) * j) + 2;	//+2 for street width, +2 for side gap

			/* everything here could be chucked in a function but its so simple */
			gen_build->chrg_space = 0;
			gen_build->free_chrg_space = 0;
			gen_build->prk_space = 0;
			gen_build->free_prk_space = 0;
		}
	}

	/* load specifics */
	char spec_builds[MAX_CHAR_IN];
	fgets(spec_builds, MAX_CHAR_IN, cityStore);
	getSpecBuildInfo(atoi(spec_builds));

	return;
}

/*
- extracts special building info
*/
void getSpecBuildInfo(int specBuildCnt)
{
	char data;
	int street, ave, prk, chrg;
	BUILD* gen_build;
	int ptr_offset;

	for (int i = 0; i < specBuildCnt; i++)
	{
		street = 0;
		ave = 0;
		prk = 0;
		chrg = 0;
		while ((data = fgetc(cityStore)) <= '9' && data >= '0')		//this should probably be a
			street = street * 10 + (data - NUM_OFFSET);				//funct for how often its done
		while ((data = fgetc(cityStore)) <= '9' && data >= '0')
			ave = ave * 10 + (data - NUM_OFFSET);
		while ((data = fgetc(cityStore)) <= '9' && data >= '0')
			prk = prk * 10 + (data - NUM_OFFSET);
		while ((data = fgetc(cityStore)) <= '9' && data >= '0')
			chrg = chrg * 10 + (data - NUM_OFFSET);

		ptr_offset = ((street - 1) * cityStreets) + (ave - 1);
		gen_build = Buildings + ptr_offset;

		gen_build->chrg_space = chrg;
		gen_build->prk_space = prk;
		gen_build->free_chrg_space = chrg;
		gen_build->free_prk_space = prk;
	}
}