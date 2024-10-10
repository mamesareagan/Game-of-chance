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
    int winning_number = rand() % 20 + 1; // Random number between 1 and 20
    int player_guess;

    printf("####### Pick a Number ######\n");
    printf("This game costs 10 credits to play. Simply pick a number\n");
    printf("between 1 and 20, and if you pick the winning number, you\n");
    printf("will win the jackpot of 100 credits!\n");

    // Deduct credits, check if the player can play
    if (player.credits < 10) {
        printf("You do not have enough credits to play.\n");
        return; // Exit the game if the player can't afford to play
    }

    player.credits -= 10; // Deduct credits
    printf("10 credits have been deducted from your account.\n");

    // Loop until valid input is provided
    while (1) {
        printf("Pick a number between 1 and 20: ");
        if (scanf("%d", &player_guess) != 1) {
            // Clear the buffer if input is not a number
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number between 1 and 20.\n");
            continue; // Ask for input again
        }

        // Validate the player's guess
        if (player_guess < 1 || player_guess > 20) {
            printf("Invalid input. Please pick a number between 1 and 20.\n");
            continue; // Ask for input again
        }

        // Proceed with the game if input is valid
        printf("The winning number is %d\n", winning_number);
        if (player_guess == winning_number)
            printf("Congratulations! You won!\n");
        else
            printf("Sorry, you didn't win.\n");

        printf("You now have %u credits\n", player.credits);
        break; // Exit the loop after a valid guess
    }

    // Prompt to play again
    char play_again;
    while (1) {
        printf("Would you like to play again? (y/n): ");
        scanf(" %c", &play_again); // Space before %c to handle newline

        // Check for valid input
        if (play_again == 'y' || play_again == 'Y') {
            // Call the game again or reset as necessary
            play_the_game();
            return; // Exit the current game loop
        } else if (play_again == 'n' || play_again == 'N') {
            // Just return to the menu
            printf("Returning to the menu...\n");
            return; // Exit the game function and return to the main menu
        } else {
            // Handle invalid input
            printf("Invalid choice. Please enter 'y' or 'n'.\n");
            continue; // Ask again
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

		
