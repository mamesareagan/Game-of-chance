#include "game.h"
#include <time.h>

User player; 

int main(void)
{
	//int choice, last_game;

	srand(time(0));	/*seed the randomizer with the current time*/
	if (get_player_data() == -1)
		register_new_player();


}
