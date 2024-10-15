#include "game.h"
#include <stdint.h>

/**
 * This function reads the player data for the current uid
 * from the file.
 * Return -1 if can't find the player uid
 */
int get_player_data(void)
{
	int uid, fd;
	ssize_t read_bytes;
	User entry;

	uid = getuid();
	fd = open(DATAFILE, O_RDONLY);
	if (fd == -1)	//can't open the file, maybe it doesn't exist
		return (-1);
	read_bytes = read(fd, &entry, sizeof(User));	//Read the first User
	while (entry.uid != uid && read_bytes > 0)
		read_bytes = read(fd, &entry, sizeof(User));
	close(fd);
	if (read_bytes < sizeof(User))
		return (-1);
	else
		player = entry;
	return 1;
}

/**
 * Gets users name input and assigns to player name
 */
void input_name(void)
{
	char *name_ptr = player.name, input_char = '\0';

	printf("Enter name: ");
	while (input_char == '\n')	//flush any leftover
		scanf("%c", &input_char);

	while (input_char != '\n' && name_ptr - player.name < sizeof(player.name) - 1) //loop untill new line and checks if there is available space for more characters
	{
		*name_ptr = input_char;
		scanf("%c", &input_char);
		name_ptr++;
	}
	*name_ptr = '\0';
}

/**
 * prints character by characterto the stdout with a time interval
 */
void print_typing_effect(const char *message, int delay) {
    while (*message) { // Loop through each character in the message
        putchar(*message); // Print the current character
        fflush(stdout); // Ensure that the output is displayed immediately
        usleep(delay * 1000000); // Wait for the specified delay (in seconds or milliseconds)
        message++; // Move to the next character
    }
    putchar('\n'); // Print a newline at the end
}

/**
 * Registers a new game player
 * Appends the new player to the file
 */
void register_new_player(void)
{
	int fd, delay = 0.95;
	char * user_greeting = "Hello Buddy, first have a name!.";
	
	printf("----------[New player registration]----------\n");
	print_typing_effect(user_greeting, delay);
	input_name();

	player.uid = getuid();
	player.highscore = player.credits = 100;

	fd = open(DATAFILE, O_WRONLY| O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
	if (fd == -1)
		fatal("User registration while opening file");
	write(fd, &player, sizeof(User));
	printf("Welcome to the game %s\n", player.name);
	close(fd);
}

/**
 * Pick number game
 * Returns -1 if player doesnt have enough credits
 * else 0
 */
int pick_a_number(void)
{
	int pick, winning_number;

	printf("\n####### Pick a Number ######\n");
	printf("This game costs 10 credits to play. Simply pick a number\n");
	printf("between 1 and 20, and if you pick the winning number, you\n");
	printf("will win the jackpot of 100 credits!\n\n");
	winning_number = (rand() % 20) + 1;	//pick a number between 1 and 20
	if (player.credits < 10)
	{
		printf("You only have %d credits. That's not enough to play!\n\n", player.credits);
		return -1; // Not enough credits to play
	}
	player.credits -=10;	//Deduct 10 credits
	printf("10 credits have been deducted from your account.\n");
	printf("Pick a number between 1 and 20: ");
	scanf("%d", &pick);
	
	printf("The winning number is %d\n", winning_number);
	if (pick == winning_number)
		jackpot();
	else
		printf("Sorry, you didn't win.\n");
	return 0;
}
	
/**
 * Awards the jackpoint for the pick a number game
 */
void jackpot(void)
{
	printf("*+*+*+*  JACKPOT  *+*+*+*\n");
	printf("You have won the jackpot of 100 credits!\n");
	player.credits += 100;
}

/**
 * Allows te current game to be played again
 * Writes te new credit totals to file
 *
 */
void play_the_game()
{ 
	int play_again = 1;
    	char selection;

	while (play_again)
	{
        	printf("\n[DEBUG] current_game pointer @ %p\n", (void*)player.current_game);

        	// Check if the current game plays successfully
        	if (player.current_game() != -1)
		{
            		// If the game plays without error, check if a new high score is achieved
            		if (player.credits > player.highscore) 
                		player.highscore = player.credits;  // Update high score          		
			printf("\nYou now have %u credits\n", player.credits);
            		update_player_data();  // Save the new credit total to file

            		// Ask the player if they want to play again
            		printf("Would you like to play again? (y/n) ");
            		selection = '\n';  // Initialize selection with newline

            		// Clear any extra newlines or invalid input
            		while (selection == '\n') 
			{
                		scanf(" %c", &selection);  // Skip leading whitespaces
            		}

            		// Exit the loop if the player does not want to play again
           		if (selection == 'n' || selection == 'N') 
			{
                		play_again = 0;
            		}
        	}
		else 
		{
            		// If the game returns an error, exit the loop
            		play_again = 0;
        	}
	}
}
/**
 * Writes the current player data to the file.
 * Used for updates
 */
void update_player_data(void)
{
	int fd;
 	User temp_user; // Temporary user struct to read data
	fd = open(DATAFILE, O_RDWR);
	if (fd == -1) 
	{ // If open fails here, something is really wrong.
        	fatal("in update_player_data() while opening file");
    	}

	while (read(fd, &temp_user, sizeof(User)) == sizeof(User))
	{
        	if (temp_user.uid == player.uid) 
		{// Update the existing user's data
			lseek(fd, -((off_t)sizeof(User)), SEEK_CUR); // Move the cursor back to overwrite
		        write(fd, &player, sizeof(User)); // Write the entire updated player struct
            		close(fd);
            		return;
        	}
    }

    close(fd);
    printf("User ID %d not found!\n", player.uid);
}

/**
 * This function inputs wagers .It expects the available credits and previous wagers as arguments
 * Returns -1 if wager is too big or too little otherwise returns the wager amount
 */
int take_wager(int available_credits, int previous_wager)
{
	int wager, total_wager;

	printf("How many of your %d credits would you like to wager? ", available_credits);
	scanf("%d", &wager);

	if (wager < 1)
	{
		printf("Nice try, but you must wager a positive number!\n");
		return -1;
	}
	total_wager = previous_wager + wager;
	if (total_wager > available_credits)
	{
		printf("Your total wager of %d is more than you have!\n", total_wager);		
		printf("You only have %d available credits, try again.\n", available_credits);
		return -1;
	}
	return wager;
}

/**
 * This is the No Match Dealer game
 * returns -1 if the player has 0 credits
 */
int dealer_no_match()
{
	int i, j, numbers[16], wager = -1, match = -1;
	
	printf("\n::::::: No Match Dealer :::::::\n");
	printf("In this game, you can wager up to all of your credits.\n");
	printf("The dealer will deal out 16 random numbers between 0 and 99.\n");
	printf("If there are no matches among them, you double your money!\n\n");

	if (player.credits == 0)
	{
		printf("You don't have any credits to wager!\n\n");
		return -1;
	}
	while (wager == -1)
		wager = take_wager(player.credits, 0);
	printf("\t\t::: Dealing out 16 random numbers :::\n");
	//assign random numbers to numbers
	for (i = 0; i < 16; i++)
	{
		numbers[i] = rand() % 100;	//pick a number between 0 and 99
		printf("%2d\t", numbers[i]);
		if (i % 8 == 7)
			printf("\n");
	}
	//loop looking for matches
	for (i = 0; i < 15; i++)
	{
		j = i + 1;
		while (j < 16)
		{
			if (numbers[i] == numbers[j])
				match = numbers[i];
			j++;
		}
	}
	if (match != -1)
	{
		printf("The dealer matched the number %d\n", match);
		printf("You lose %d credits\n", wager);
		player.credits -= wager;
	}
	else
	{
		printf("There were no matches! You win %d credits!\n", wager);
		player.credits += wager;
	}
	return 0;
}
