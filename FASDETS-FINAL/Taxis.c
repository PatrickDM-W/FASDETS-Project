/****************************************
Nov 08, 2022
Patrick Wain

Taxis.c
functions for taxi store management

ChangeLog

Version 1.0
Revamp of Project
Version 1.1
- I think these functions all need to be ripped out :/
- functions fixed
- file access works
****************************************/

#include "Taxis.h"

/*
- function for beginning store
- code adapted from code provided by Dr. Hughes
- OPENS FILE
- CLOSES WHEN FINISHED
*/
void init_taxis()
{
	/* header initialization */
	TAX_HEAD t_head;
	char fname[FILENAMELEN];

	printf("initializing taxi store\n");

	t_head.first_vin = 1000;

	/* create new file (initialization) */
	printf("enter name of taxi store\n");
	fgets(fname, FILENAMELEN, stdin);
	TRUNCATE(fname);

	/* set access mode and create */
	printf("creating store\n");
	_set_fmode(_O_BINARY);
	fopen_s(&taxiStore, fname, "w+");

	/* redundancy, reposition to beginning */
	fseek(taxiStore, 0, SEEK_SET);

	/* write the header */
	fwrite(&t_head, sizeof(TAX_HEAD), 1, taxiStore);

	printf("taxi store initialized\n");
	fclose(taxiStore);
}

/*
- updating info taxis to store
- code adapted from code provided by Dr. Hughes
- assumes file is open already
- need to fix
*/
#ifdef DEBUG
void update_taxis()
{
	TAX_HEAD t_head;
	int choice;
	int vin;

	/* open to header, read header */
	fseek(taxiStore, 0, SEEK_SET);
	fread(&t_head, sizeof(TAX_HEAD), 1, taxiStore);

	do
	{
		printf("Enter vin (0 to end)\n");
		scanf_s("%d", &vin);
		getchar();		//clear buffer

		if (vin < 1000 || vin >= t_head.first_vin)
			printf("Invalid vin %u\n", vin);
		else if (vin >= 1000)
		{
			/* position to vin */
			fseek(taxiStore, ((vin - 999) * sizeof(TF_REC)), SEEK_SET);
			/* read info to change */
			fread(&taxis_rec, sizeof(TF_REC), 1, taxiStore);

			printf("Taxi %4d found\n", vin);
			display_tax_info(taxis_rec.trec);

			/* what to do? */
			printf("What to change?\n");
			printf("0: Stop, 1: etc, 2: etc");

			scanf_s("%d", &choice);
			while (choice != 0)
			{
				getc(stdin);		//clear buffer

				switch (choice)
				{
				case 1: /* whatever it is */
					printf("Enter new [info]\n");
					/* enter new info */
					break;
				case 2: /* whatever it is */
					printf("Enter new [info]\n");
					/* new info */
					break;
				default:
					printf("unknown field: %d\n", choice);
				}

				printf("What to change?\n");
				printf("0: Stop, 1: etc, 2: etc\n");
				scanf_s("%d", &choice);
			}
			/* overwrite with new info*/
			fseek(taxiStore, ((vin - 999) * sizeof(TF_REC)), SEEK_SET);
			fwrite(&taxis_rec, sizeof(TF_REC), 1, taxiStore);
		}
	} while (vin > 0);
}
#endif // DEBUG

/*
- display current taxiStore
- ASSUMES FILE IS OPEN already
- need to rework
*/
void displaytaxiStore(void)
{
	TAX_HEAD t_head;
	TAX_REC t_rec;
	int rec_no;

	/* read header to find limit */
	fseek(taxiStore, 0, SEEK_SET);
	fread(&t_head, sizeof(TAX_HEAD), 1, taxiStore);

	printf("Header:\n");
	printf("First available vin : % u\n", t_head.first_vin);

	if (t_head.first_vin == 1000)
	{
		printf("No taxis currently stored\n");
		return;
	}

	/* position to first record */
	fseek(taxiStore, sizeof(TAX_HEAD), SEEK_SET);

	/*
	- read data
	- start at rec 1
	- up to first vin
	*/
	rec_no = 1;
	while (rec_no < t_head.first_vin - 1000 && fread(&t_rec, sizeof(TAX_REC), 1, taxiStore))
	{
		/* it should have own function in future */
		printf("vin: %4d\t", t_rec.vin);
		printf("max charge: %d\t", t_rec.max_charge);
		printf("charge rate: %d\n", t_rec.charge_rate);
	}
}

/*
- function to add taxi to store
- ASSUMES FILE IS OPEN
- DOES NOT CLOSE
*/
void add_taxis(void)
{
	TAX_HEAD t_head;
	TAX_REC t_rec;

	/* open to beginning, find next vin */
	fseek(taxiStore, 0, SEEK_SET);
	fread(&t_head, sizeof(TAX_HEAD), 1, taxiStore);

	printf("new taxi vin: %4d\n", t_head.first_vin);
	
	/* 
	- get new taxi info
	- in future will be
		max battery
		charge rate
		etc
		for now no info is needed
	*/

	int max_charge, charge_rate;

	printf("enter max battery level:\n");
	scanf_s("%d", &max_charge);
	getchar();
	printf("enter charge rate:\n");
	scanf_s("%d", &charge_rate);
	
	/* set new t_rec info, write to file */
	t_rec.vin = t_head.first_vin;
	t_rec.max_charge = max_charge;
	t_rec.charge_rate = charge_rate;


	/* reposition to correct location */
	fseek(taxiStore, ((t_rec.vin - 1000) * sizeof(TAX_REC)) + sizeof(TAX_HEAD), SEEK_SET);
	fwrite(&t_rec, sizeof(TAX_REC), 1, taxiStore);

	printf("taxi added\n");

	/* update header */
	t_head.first_vin++;
	fseek(taxiStore, 0, SEEK_SET);
	fwrite(&t_head, sizeof(TAX_HEAD), 1, taxiStore);
	printf("header updated\n");

	return;
}


/*
- extracts data from activeTaxis
- returns an pass/fail of the vins
- edits taxi_cnt to have the number of taxis
*/
int read_activeTaxis(int total_taxis)
{
	char data[MAX_CHAR_IN];
	int vin;
	TAX_REC t_rec;
	TAX_HEAD t_head;
	TAXI* curr;

	/* for default start locations */
	int street = 0;
	int ave = 0;

	char ID = 'A';

	/* redundant file positioning */
	fseek(activeTaxis, 0, SEEK_SET);
	fgets(data, MAX_CHAR_IN, activeTaxis);		//this line can be disregarded, info retrieved

	/* get limit of vins */
	fseek(taxiStore, 0, SEEK_SET);
	fread(&t_head, sizeof(TAX_HEAD), 1, taxiStore);

	for (int i = 0; i < total_taxis; i++)
	{
		fgets(data, MAX_CHAR_IN, activeTaxis);
		vin = atoi(data);

		/* valid vin? */
		if (vin > t_head.first_vin)
			return FALSE;

		/* go to taxiStore and extract info */
		fseek(taxiStore, (vin - 1000) * sizeof(TAX_REC) + sizeof(TAX_HEAD), SEEK_SET);
		fread(&t_rec, sizeof(TAX_REC), 1, taxiStore);

		/* convert trec to taxi, this will need to be updated */
		/* malloc for taxi */
		curr = malloc(sizeof(TAXI));
		curr->vin = t_rec.vin;
		/* more lines here, bat level etc */
		curr->max_charge = t_rec.max_charge;
		curr->charge_rate = t_rec.charge_rate;


		curr->pass = NULL;
		curr->dest_ave = NO_DEST;
		curr->dest_street = NO_DEST;
		curr->eff_moveTicks = 0;
		curr->delay = 0;
		curr->ave_dir = NONE;
		curr->st_dir = NONE;

		/* default start location */
		if (ave > cityAves)
		{
			ave = 0;
			street++;

			/* too many taxis check (redundant) */
			if (street > cityStreets)
				return FALSE;
		}
		curr->cur_ave = ave;
		curr->cur_street = street;
#ifdef DEPRECATED
		curr->cur_sub_ave = INTERSECTION_AVE;
		curr->cur_sub_street = INTERSECTION_STREET;
#endif
		curr->abs_xloc = (curr->cur_street * (BUI_W + 2)) + ST_BLD_LEFT_EDGE;	// +2 for road_w, could be defined if you wanted wider streets
		curr->abs_yloc = (curr->cur_ave * (BUI_H + 2)) + 1;			// +2 for road_w, +1 for right side of road

		ave++;

		curr->visualID = ID;
		ID++;

		/* update list */
		curr->next_taxi = inactiveList;
		inactiveList = curr;
	}

	return TRUE;
}


/*
- add maintenance
- assumes files are open
*/
void add_maintenance(void)
{
	system("cls");

	TAX_HEAD t_head;
	int vin, date;
	char comment[COMMENT_LEN];

	/* move to start of taxiStore, get limit */
	fseek(taxiStore, 0, SEEK_SET);
	fread(&t_head, sizeof(TAX_HEAD), 1, taxiStore);

	do
	{
		printf("Enter the vin for the taxi you intend to service (0 to quit)\n");

		scanf_s("%d", &vin);
		getchar();

		if (vin == 0)
			return;

		if (vin >= t_head.first_vin || vin < 1000)
			printf("Invalid vin\n");
		else if (vin != 0)
		{
			printf("Enter the date of maintenance (yyyymmdd)\n");
			scanf_s("%d", &date);
			getchar();

			printf("Enter maintenance comment\n");
			fgets(comment, COMMENT_LEN, stdin);
			TRUNCATE(comment);

			fseek(maintainHist, 0, SEEK_END);
			fprintf_s(maintainHist, "%d\t", vin);
			fprintf_s(maintainHist, "%d\t", date);
			fprintf_s(maintainHist, "%s\n", comment);

			printf("Record entered\n");
		}
		printf("press any key to continue\n");
		getchar();
	} while (vin);
}