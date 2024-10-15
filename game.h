#ifndef GAME_H
#define GAME_H
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


// Define the user struct with typedef
typedef struct {
    int uid;               // User ID
    unsigned int credits;           // User credits
    unsigned int highscore;         // User high score
    char name[30];        // User name
    int (*current_game)(void); // Pointer to current game function
} User;


// Define the path to the user data file
#define DATAFILE "/var/chance.data" // File to store user data

// Function prototypes
int get_player_data();
void register_new_player();
void update_player_data();
void show_highscore();
void jackpot();
void input_name();
void print_cards(char *, char *, int);
int take_wager(int, int);
void play_the_game(void);
int pick_a_number(void);
int dealer_no_match();
int find_the_ace();

void print_typing_effect(const char *message, int delay);
// Declare the global variable
extern User player;

// Function to display an error message and then exit
void fatal(const char *message);
// An error-checked malloc() wrapper function
void *ec_malloc(unsigned int size);
#endif
