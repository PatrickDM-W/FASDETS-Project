/****************************************
Nov 07, 2022
Patrick Wain

Buildings.h
Header file containing definitions for
buildings and function decelerations

Version 1.0

Version 1.1
- no changes thus far
****************************************/

#pragma once
#ifndef BUILDINGS_H
#define BUILDINGS_H

#include "FASDETS.h"

/* Char codes for buildings */
#define UL 201
#define UR 187
#define LL 200
#define LR 188
#define VBAR 186
#define HBAR 205

/* building constants */
#define BUI_H 6
#define BUI_W 10
#define RD_SIZE 2

/* structures */
struct building
{
	int street, ave;
	int xloc, yloc;
	int prk_space, chrg_space, free_chrg_space, free_prk_space;
};
typedef struct building BUILD;

/* store functions */
/*
- this function has no definition so far
- need a more complicated building store
	these have been completed
	in CitySup.c
*/
extern void readCity();
extern void getSpecBuildInfo(int specBuild_cnt);

/* drawing functions */
extern void draw_build(BUILD* build);
extern void draw_city(void);



/* globals */
int cityStreets, cityAves;
BUILD* Buildings;			// this name sucks, base pointer for array

#endif // !BUILDINGS_H