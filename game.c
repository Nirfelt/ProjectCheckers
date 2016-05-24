#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

#define BOARD_SIZE 64

/*Struct for handling checker info*/
typedef struct
{
   int player;
   int king;
} checker;

/*Struct for handling box info*/
typedef struct
{
   int black;
   checker *ch;
} box;

/*Functionprototypes*/
void draw_board(box *p, int player, int *p1, int *p2);
void initiate_board(box *p);
int check_moves(box *p, int *move, int move_length, int player);
int move(box *p, char *s, int player);
static checker *create_checker(int player, int k);
void set_kings(box *p, int player);
void end_game(box *p);

/*
* Starts a new game. Sets a new board with 24 checkers.
* Loops turns untill one player is out of checkers.
* Checks for draw.*/
void new_game()
{
	box board[BOARD_SIZE];
	box *p;
	p = board;
	initiate_board(p);
	int p1ch = 12;
	int p2ch = 12;
	int player = 1;
	do{
		draw_board(p, player, &p1ch, &p2ch);
		if(p1ch == 0 || p2ch == 0) break;
		while(1)
		{
			char word[256];
			printf("Enter move: ");
			scanf("%s" , word);
			if(strcmp(word, "draw") == 0)
			{
				p1ch = 0;
				p2ch = 0;
				break;
			}
			char *e;
			e = word;
			if(move(p, e, player))
			{
				if(player) player = 0;
				else player = 1;
				break;
			}
		}
	}while(p1ch > 0 && p2ch > 0);
	clear_screen();
	if(p1ch == 0 && p2ch == 0) printf("Draw!\n\n");
	else if(p1ch == 0) printf("Player 2 wins!\n\n");
	else printf("Player 1 wins!\n\n");
	end_game(p);
}

/*Prints the board to the console and counts the checkers.*/
void draw_board(box *p, int player, int *p1, int *p2)
{
	clear_screen();
	box *pi;
	int counter = 0;
	*p1 = 0;
	*p2 = 0;
	for(pi = p; pi < (p + BOARD_SIZE); pi++)
	{
		if(pi->black && pi->ch)
		{
			if(pi->ch->player)
			{
				if(pi->ch->king) printf(" X ");
				else printf(" x ");
				*p1 = *p1+1;
			}
			else
			{
				if(pi->ch->king) printf(" O ");
				else printf(" o ");
				*p2=*p2+1;
			}
		}
		else if(pi->black && !(pi->ch)) printf(" _ ");
		else printf(" # ");
		counter++;
		if(counter == 8)
		{
			printf("\n\n");
			counter = 0;
		}
	}
	printf("For draw type: draw\n");
	if(player) printf("Player 1! (x)\n");
	else printf("Player 2! (o)\n");
	box *po;
	int v = 1;
	printf("Checker positions:");
	for(po = p; po < (p + BOARD_SIZE); po++)
	{
		if(po->ch) if(po->ch->player == player) printf(" %d", v);
		v++;
	}
	printf(".\n");
}

/*Sets a new board and creates new checkers.*/
void initiate_board(box *p)
{
	box *pi;
	pi = p;
	int i;
	for(i = 0; i < 4; i++)
	{
		int c;
		for(c = 0; c < 8; c++)
		{
			pi->black = 0;
			pi->ch = 0;
			if((c % 2) == 0)
			{
				pi->black = 1;
				if(i <= 1) pi->ch = create_checker(0, 0);
				if(i == 3) pi->ch = create_checker(1, 0);
			}
			else pi->ch = 0;
			pi++;
		}
		int v;
		for(v = 0; v < 8; v++)
		{
			pi->black = 0;
			pi->ch = 0;
			if(v % 2)
			{
				pi->black = 1;
				if(i >= 2) pi->ch = create_checker(1, 0);
				if(i == 0) pi->ch = create_checker(0, 0);
			}
			else pi->ch = 0;
			pi++;
		}
	}
}

/*Creates a single checkers from specifications. Returns pointer to that checker.*/
static checker *create_checker(int player, int k)
{
	checker *chref;
	chref = (checker *) malloc (sizeof(checker));
	chref->player = player;
	chref->king = k;
	return chref;
}

/*Types breaklines to keep screen clear*/
void clear_screen()
{
  int i;
  for (i = 0; i < 15; i++) putchar('\n');
}

/*Checks if move is ok before executing. Makes sure that all rules are followed.*/
int check_moves(box *p, int *move, int move_length, int player)
{
	if(move_length < 2) return 0;
	int *mp;
	int *tp;
	box *po;
	mp = move;
	tp = mp+1;
	po = p+*mp;
	if(!po->ch) return 0;
	int test = *mp-*tp;
	//Single move
	if(move_length == 2 && test < 10 && test > -10)
	{
		if(*mp < 0 || *mp > 63 || *tp < 0 || *tp > 63) return 0;
		if((player || po->ch->king != 0) && test > 0)
		{
			po = p+*tp;
			if(test == 7 && !po->ch && po->black) return 1;
			else if(test == 9 && !po->ch && po->black) return 1;
			else return 0;

		}
		else if((!player || po->ch->king != 0) && test < 0)
		{
			po = p+*tp;
			if(abs(test) == 7 && !po->ch && po->black) return 1;
			else if(abs(test) == 9 && !po->ch && po->black) return 1;
			else return 0;
		}
		else return 0;
	}
	//Multiple moves
	box *pos;
	pos = p+*mp;
	if(pos->ch->player != player) return 0;
	int i;
	for(i = 0; i < move_length-1; i++)
	{
		test = *mp-*tp;
		po = p+*mp;
		if(*mp < 0 || *mp > 63 || *tp < 0 || *tp > 63) return 0;
		if((player || (!player && pos->ch->king)) && *mp > *tp)
		{
			po = p+*tp;
			if(test == 14 && !po->ch && po->black)
			{
				po = p+*mp-7;
				if(!po->ch) return 0;
			} 
			else if(test == 18 && !po->ch && po->black)
			{
				po = p+*mp-9;
				if(!po->ch) return 0;
			} 
			else return 0;
		}
		else if((!player || (player && pos->ch->king)) && *mp < *tp)
		{
			po = p+*tp;
			if(abs(test) == 14 && !po->ch && po->black)
			{
				po = p+*mp+7;
				if(!po->ch) return 0;
			} 
			else if(abs(test) == 18 && !po->ch && po->black)
			{
				po = p+*mp+9;
				if(!po->ch) return 0;
			} 
			else return 0;
		}
		else return 0;
		mp++;
		tp++;
	}
	return 1;
}

/*
* Breaks down the input and adds moves to a list. 
* If moves pass the check it executes.
* Checks for new kings. */
int move(box *p, char *s, int player)
{
	int moves[12];
	int moves_length = 0;
	int *mp;
	int *tp;
	mp = moves;
	tp = moves;
	box *po1;
	box *po2;
	char *st;
	st = s;
	int ok = 0;
	//Separates input
	do
	{
		ok = 0;
		char *e;
		int index;
		e = strchr(st, '-');
		if(e)
		{
			index = (int)(e - st);
			char sub[index];
			strncpy(sub, st, index);
			sub[index] = '\0';
			st = st + index + 1;
			*mp = atoi(sub)-1;
			mp++;
			moves_length++;
			ok = 1;
		}
		else
		{
			*mp = atoi(st)-1;
			mp++;
			moves_length++;
		}
	}while(ok);
	mp = moves;
	tp = mp+1;
	//Execute move
	if(check_moves(p, mp, moves_length, player))
	{
		//Single move
		if(abs(*mp-*tp) < 10)
		{
			po1 = p+*mp;
			po2 = p+*tp;
			po2->ch = po1->ch;
			po1->ch = 0;
			set_kings(p, player);
			return 1;
		}
		//Multiple moves
		int i;
		for(i = 0; i < moves_length-1; i++)
		{
			po1 = p+*mp;
			po2 = p+*tp;
			po2->ch = po1->ch;
			po1->ch = 0;
			int rem = *tp+((*mp-*tp)/2);
			printf("%d\n", rem);
			box *rp;
			rp = p+rem;
			if(po2->ch->player != rp->ch->player)
			{
				checker *tmp;
				tmp = rp->ch;
				free(tmp);
				rp->ch = 0;
			} 
			mp++;
			tp++;
		}
		set_kings(p, player);
		return 1;
	}
	printf("BAD MOVE!\n");
	return 0;
}

/*Updates kings. */
void set_kings(box *p, int player)
{
	box *kp;
		kp = p;
		int c;
		for(c = 0; c < 8; c++){
			if(kp->ch)
			{
				if(kp->ch->player) kp->ch->king = 1;
			}
			kp++;
		} 
		kp = p+56;
		for(c = 56; c < BOARD_SIZE; c++){
			if(kp->ch)
			{
				if(!kp->ch->player) kp->ch->king = 1;
			}
			kp++;
		} 
}

/*Free up used memory*/
void end_game(box *p)
{
	box *rc;
	rc = p;
	int i;
	for(i = 0; i < BOARD_SIZE; i++)
	{
		if(rc->ch)
		{
			checker *ch;
			ch = rc->ch;
			free(ch);
		}
	}
}

