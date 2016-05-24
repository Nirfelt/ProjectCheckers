#include <stdio.h>
#include "checkers_game.h"

int main()
{
	do{
		clear_screen();
		show_menu();
		run_selection(get_selection());
	}while(pause());
	return 0;
}
