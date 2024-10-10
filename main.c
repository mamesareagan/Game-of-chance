#include "game.h"
#include <time.h>

User player; 

int main(void)
{
	int choice = 0, last_game = 0;

	srand(time(0));	/*seed the randomizer with the current time*/
	if (get_player_data() == -1)
		register_new_player();
	printf("[DEBUG] Address of pick_a_number: %p\n", (void*)pick_a_number);
	while (choice != 7)
	{
		printf("-~~~[Game of Chance Menu]~~~-\n");
		printf("1 - Play the Pick a Number Game\n");
		printf("2 - Play the No Match Dealer game\n");
		printf("3 - Play the Find the Ace game\n");
		printf("4 - View current high score\n");
		printf("5 - Change your user name\n");
		printf("6 - Reset your account at 100 credits\n");
		printf("7 - Quit\n");
		printf("[Name: %s]\n", player.name);
		printf("[You have %u credits] -> ", player.credits);
		if (scanf("%d", &choice) != 1)
		{
			while (getchar() != '\n')
				printf("Invalid input. Please enter a number.\n");
			continue;
		}

		if ((choice < 1) || (choice > 7))
			printf("\n[!!] The number %d is an invalid selection.\n\n", choice);
		else if (choice < 4)
		{
			if(choice != last_game)	//if the function ptr in the struct isnt set yet
			{
				if (choice == 1)
				{
					printf("[DEBUG] Address of pick_a_number: %p\n", (void*)pick_a_number);
					player.current_game = &pick_a_number;
					printf("Current game function pointer: %p\n", (void*)player.current_game);

					last_game = choice;
				}
			}
			play_the_game();
			update_player_data();
		}
		
	}
	update_player_data();
	printf("\nThanks for playing! Bye.\n");
}
