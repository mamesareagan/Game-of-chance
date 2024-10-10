#include "game.h"

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

		
