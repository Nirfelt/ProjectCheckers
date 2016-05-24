#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "checkers_game.h"
#include "game.h"

/* Prints menu */
void show_menu()
{
	clear_screen();
	printf("\n*****   Menu   *****\n");
	printf("    1. Start new game\n");
	printf("    2. Show instructions\n");
	printf("    3. Exit\n");
}

/* Handles user selection, checks input */
int get_selection()
{
	int choice;
	do{
		printf("\n\nChoose 1-3: ");
		scanf("%d", &choice);
	}while(choice < 1 || choice > 3);/*Checks if input is within range*/
	return choice;
}
/* Run correct selection from input */
void run_selection(int selection)
{
	switch(selection){
	    case 1  :
	       	new_game();
	       	break;
	    case 2  :
	       	show_instructions();
	       	break;
	    case 3  :
	       	exit(0);
	}
}

/* Pauses the program. */
int pause()
{
	printf("\nPress enter to continue!");
	int c;
	while((c = getchar()) != '\n' && c != EOF); 
	getchar();
	return 1;
}
/* Prints instructions */
void show_instructions()
{
	clear_screen();
	printf("\n*****   Instructions   *****\n");
	printf("\n");
	printf("Player 1 i represented py x (X if king)\n");
	printf("Player 2 i represented py o (O if king)\n");
	printf("Move format: 23-30 for single moves or 19-37-55 for longer moves.\n");
	printf("\n");
	printf("Rules:\n");
	printf("Checkers can only move in one direction unless it's a king.\n");
	printf("Checkers that reach the last row become kings.\n");
	printf("Moves can be either to a free box next to the checker \nor jumping over another checker.\n");
	printf("If the opponents checker is jumped over it will be removed.\n");
	printf("Game is over when one player is out of checkers.\n");
	printf("Players can request a draw at any time.\n");
	printf("\n");
	printf("\n");	
}

