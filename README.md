# 🎲 Game of Chance

Welcome to **Game of Chance**, a C-based program where players can test their luck with different games and wager credits to achieve the highest score.

## 📋 Features

- Play multiple games of chance like **Pick a Number** and **Find the Ace**.
- Keep track of the highest score with a leaderboard.
- Secure handling of player data and credits.

## 🚀 Installation

To build and compile the program, navigate to the project directory and run:

```bash
make

This will generate the executable `myprogram`.

## 🔑 Pre-run Setup

Before running the program, you need to set the appropriate ownership and permissions for secure execution:

- Change the ownership of the executable to root:

  ```bash
  sudo chown root:root ./myprogram

- Set SUID permission (this allows the program to run with root privileges when necessary):

  ```bash
  sudo chmod u+s ./myprogram

These commands ensure the program can access necessary files securely and manage permissions properly.

## 🕹️ Running the Program

Once compiled and set up, run the program by executing the following command:

```bash
./myprogram

Here’s your content formatted in Markdown:

markdown

## 🕹️ Running the Program

Once compiled and set up, run the program by executing the following command:

```bash
./myprogram

You'll be greeted with a menu where you can choose from different games, manage your credits, and aim for the high score.

## 🎮 Game Instructions

1. **Pick a Number**
   - **Objective:** Wager your credits and choose a number between 1 and 20. If you guess the correct number, you'll win the jackpot of 100 credits!

2. **Find the Ace**
   - **Objective:** Wager your credits and pick a card among three (two queens and one ace). If you find the ace, you win your wager! After selecting a card, one of the queens will be revealed, and you'll have the chance to change your selection or increase your wager.

3. **View High Score**
   - Track the highest score in the game and see who holds the top spot. If it's you, congratulations!

## 🛠️ Troubleshooting

### Common Issues:

- **Permission Denied:** If you see a permission error, make sure you followed the setup steps to change the ownership and add the SUID permission to the `myprogram` executable.
- **Makefile Errors:** Ensure that `make` is installed on your system. Run `sudo apt install make` if it’s missing.

## 📜 License

This project is intended for educational purposes only. Use responsibly!
