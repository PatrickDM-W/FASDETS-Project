/****************************************
Nov 08, 2022
Patrick Wain

FASDETS.c
Main program containing entry point

Version 1.1
- added a menu
- theoretically allows for initializing taxis
- adding taxis
****************************************/

#include "taxis.h"
#include "Menuing.h"

/*
- define HARDC_FILES
- allows for debugging with the given files hard coded in,
- files must be stored in same directory as the source code
*/
#define HARDC_FILES

int main(int argc, char* argv[])
{
#ifdef HARDC_FILES
	fopen_s(&activeTaxis, "Active Taxis.txt", "r");
	fopen_s(&cityStore, "CityStore.txt", "r");
	fopen_s(&passengerReq, "c Passengers.txt", "r");
	fopen_s(&maintainHist, "maintenance History.dat", "r+");
	_set_fmode(_O_BINARY);
	fopen_s(&taxiStore, "taxiStore.dat", "r+");
#endif

	/* resize screen */
	COORD scrnsize;
	Console = GetStdHandle(STD_OUTPUT_HANDLE);

	scrnsize = GetLargestConsoleWindowSize(Console);
	SetConsoleScreenBufferSize(Console, scrnsize);

	/* 
	- initialize file pointers to NULL
	- if FILE* has value, no need to ask again
	*/
#ifndef HARDC_FILES
	cityStore = NULL;
	taxiStore = NULL;
	passengerReq = NULL;
	activeTaxis = NULL;
#endif

	/* user input variable */
	int input;

	do
	{
		main_menu();

		/* get choice */
		scanf_s("%d", &input);
		getchar();		//clear buffer

		switch (input)
		{
		case(RUN):
			/* check files provided, open as required */
#ifndef HARDC_FILES
			if (argc < 4)
			{
				printf("you have not provided enough files (3 are required)\n");
			}
			else
			{
				if (!cityStore)	//cityStore not selected yet
				{
					system("cls");		//this function clears the window
					display_given_files(argc, argv);
					printf("please indicate which file is the cityStore\n");

					int file_choice;

					scanf_s("%d", &file_choice);
					getchar();		//clear buffer

					/*
					- cityStore should be opened as text
					- cityStore does not need to edited "r"
					*/
					_set_fmode(_O_TEXT);
					fopen_s(&cityStore, argv[file_choice], "r");
				}
				if (!taxiStore)
				{
					system("cls");		//this function clears the window
					display_given_files(argc, argv);
					printf("Please indicate which file is the taxiStore\n");
					int file_choice;

					scanf_s("%d", &file_choice);
					getchar();		//clear buffer

					/*
					- need to open file in binary mode
					- taxiStore can be edited "r+"
					*/
					_set_fmode(_O_BINARY);
					fopen_s(&taxiStore, argv[file_choice], "r+");
				}
				if (!passengerReq)
				{
					system("cls");		//this function clears the window
					display_given_files(argc, argv);
					printf("Please indicate which file is the passengerRequest file\n");
					int file_choice;

					scanf_s("%d", &file_choice);
					getchar();		//clear buffer

					/*
					- need to open file in text mode
					- no need to edit "r"
					*/
					_set_fmode(_O_TEXT);
					fopen_s(&passengerReq, argv[file_choice], "r");
				}
				if (!activeTaxis)
				{
					system("cls");
					display_given_files(argc, argv);
					printf("Please indicate which file is the activeTaxis file\n");
					int file_choice;

					scanf_s("%d", &file_choice);
					getchar();		//clear buffer

					/*
					- need to open file in text mode
					- no need to edit "r"
					*/
					_set_fmode(_O_TEXT);
					fopen_s(&activeTaxis, argv[file_choice], "r");
				}
#endif
				/* delay control */
				printf("Enter time (in ms) delay between cycles:\n");
				printf("Time of 0 gives manual control\n");

				int delay_ms;

				scanf_s("%d", &delay_ms);
				getchar();			//clear buffer

				system("cls");		//this function clears the window

				/* all files should be open at this point, proceed to run */
				int runFlag = runEmulator(delay_ms);

				/*
				- move cursor to bottom and print runFlag ?
				- possibly no space at bottom, move to side
				*/
				move_cursor(5, (cityStreets * (BUI_W + RD_SIZE)) + 5);
				printf("exit code: %d", runFlag);
				move_cursor(6, (cityStreets * (BUI_W + RD_SIZE)) + 5);
#ifndef HARDC_FILES
			}
#endif

			printf("press any key to return to main menu\n");
			getchar();
			break;

		case(INIT):
			init_taxis();

			printf("press any key to return to main menu\n");
			getchar();
			break;

		case(ADD):
			/* check provided files, get if necessary */
#ifndef HARDC_FILES
			if (argc == 1)
				printf("you have not provided enough files (1 is required)\n");
			else
			{
				/* get taxiStore */
				if (!taxiStore)	//file not provided yet
				{
					display_given_files(argc, argv);
					printf("Please indicate which file is the taxiStore\n");
					int file_choice;

					scanf_s("%d", &file_choice);
					getchar();		//clear buffer

					/*
					- need to open file in binary mode
					- taxiStore can be edited "r+"
					*/
					_set_fmode(_O_BINARY);
					fopen_s(&taxiStore, argv[file_choice], "r+");
				}
#endif
				add_taxis();
#ifndef HARDC_FILES
			}
#endif

			printf("press any key to return to main menu");
			getchar();
			break;

		case(VIEW):
			/* check files, get if necessary */
#ifndef HARDC_FILES
			if (argc == 1)
				printf("you have not provided enough files (1 is required)\n");
			else
			{
				if (!taxiStore)		//file not provided yet
				{
					system("cls");
					/* get taxiStore */
					display_given_files(argc, argv);
					printf("Please indicate which file is the taxiStore\n");
					//printf("If file is not listed, enter 0 to quit\n");
					int file_choice;

					scanf_s("%d", &file_choice);
					getchar();		//clear buffer

					/*
					- need to open file in Binary mode
					- taxiStore can be edited "r+"
					*/
					_set_fmode(_O_BINARY);
					fopen_s(&taxiStore, argv[file_choice], "r+");
				}
#endif
				displaytaxiStore();
#ifndef HARDC_FILES
			}
#endif

			printf("press any key to return to main menu");
			getchar();
			break;

		case(SERVICE):
#ifndef HARDC_FILES
			if (argc < 3)
				printf("you have not provided enough files (2 are required)\n");
			else
			{
				if (!maintainHist)		//file not provided yet
				{
					system("cls");
					/* get taxiStore */
					display_given_files(argc, argv);
					printf("Please indicate which file is the Maintenance Record\n");
					//printf("If file is not listed, enter 0 to quit\n");
					int file_choice;

					scanf_s("%d", &file_choice);
					getchar();		//clear buffer

					/*
					- need to open file in Binary mode
					- taxiStore can be edited "r+"
					*/
					_set_fmode(_O_TEXT);
					fopen_s(&maintainHist, argv[file_choice], "r+");
				}
				if (!taxiStore)
				{
					system("cls");
					display_given_files(argc, argv);
					printf("Please indicate which file is the Taxi Store\n");
					int file_choice;

					scanf_s("%d", &file_choice);
					getchar();

					_set_fmode(_O_BINARY);
					fopen_s(&taxiStore, argv[file_choice], "r+");
				}
#endif
				add_maintenance();
#ifndef HARDC_FILES
			}
#endif

			printf("press any key to return to main menu");
			getchar();

			break;
		case(QUIT):
			break;

		default:
			printf("unknown field: %d\n", input);
			printf("press any key to return to main menu\n");
			getchar();
			break;
		}

	} while (input);

	/* check files and close */
	MY_CLOSE(taxiStore);
	MY_CLOSE(cityStore);
	MY_CLOSE(passengerReq);
	MY_CLOSE(activeTaxis);

	return 0;
}