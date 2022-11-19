/*
 * Sawyer Norquist
 * CS202
 * Program 2
 * games.h
 * 7/21/2020
 *
 * This is the header file for my primary game functions.  Includes the ABC for the games, three derived games,
 * the CLL list container, and the game master class to combine players with games.
 */
#ifndef GAMES
#define GAMES

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <chrono>
#include "players.h"


void clear_screen();  //cout's 100 lines
bool to_quit();  //originally intended to help all loop exits, but ended up wanting special verbiage so this was only used once

//ABC for the whole program.  Contains the player and current score as private members as the class will read relevent
//data directly into the player list.  Also includes the next pointer so each derived class can automatically
//be placed on a Game list
class Game
{

    public:
        Game();
        Game(const Game &source);
        virtual ~Game();
        virtual void set_player(char *new_player);  //Sets the player var, virtual because one class was originally intended to set two, but ended up using a different method instead
        virtual void play()=0;  //play function MUST be implemented in derived class, as this is the entire point of the derived class
        virtual void update_score(Player_List &update_player);  //Sets Player_List with new player data
        virtual void rules()=0;  //Rules can only be called from derived classes, given they contain the unique logic
        Game *&get_next();  //getter for ptr
        void set_next(Game *to_set);  //setter for ptr

    protected:
        char *player;  //holds the name of the player using the current object
        int score;  //Total score across the player's game time in this object
        Game *next;  //next pointer for list

};

//First derived object.  Extremely simple interface, only one data member to hold how long the player
//wants to see the string.
class Memory: public Game
{

    private:
        int delay_time;  //Holds how long to display the random string
        
    public:
        Memory();
        Memory(const Memory &source);
        void play();  //Play function for game
        void set_time();  //unique function for downcasting, sets the delay_time var
        void rules();  //Show rules for Memory
        //All other functions can be called directly from base class

};

//My multiplayer game.  Much more complicated due to the multiple elements involved.  Has to handle
//name and score for a second player, along with multiple supporting functions
class Adders: public Game
{

    private:
        char* player_2;  //player 2 name
        int score_2;  //player 2 score
        //helper function to determine which player won a given round
        void calculate_score(bool correct_1, bool correct_2, auto time_1, auto time_2, int score_add);
        void play_round(int &difficulty, int num_rounds);  //controls gameplay loop, set up by play()

    public:
        Adders();
        Adders(const Adders &source);
        ~Adders();
        void play();  //main function for game
        void update_score(Player_List &update_player);  //special implimentation of update_score due to two players
        void set_multiplayer();  //unique function to set up second player
        void rules();  //show rules for Adders

};


//Last derived game.  Somewhat lower complexity though with more data members.  Data members are used to
//control flow of the game and how scoring is determined.
class HeadTail: public Game
{

    private:
        int tries;  //How many times the player needs to call heads or tails
        int num_to_get;  //number of correct answers needed to win
        int score_give;  //the score value of winning at the current difficulty level
        int get_input();  //Helper function for reading user input on heads or tails
        void score_game(int &answer);  //helper function to see if player won

    public:
        HeadTail();  //No dynamic data, so no destructor or copy constructor
        void play();  //Main game loop
        void rules();  //show rules for HeadTail
        void set_difficulty();  //Unique function, used to set up rest of member vars based on chosen difficulty 

};

//implementation of Game CLL.  Contains needed functions for upcasting and downcasting, as well as user interaction
//Holds the rear of the CLL, a current pointer to track current object in list, and the active player
class Game_List
{

    private:
        Game *rear;  //Rear node for CLL
        Game *current;  //pointer for currently active game
        char *player;  //active player
        Game *copy_list(Game *&new_list, Game *source);  //Helper for copy constructor
        Game *copy_node(Game *to_check);  //helper for properly downcasting a source and upcasting to a new node
        void delete_list(Game *&current);  //Helper for destructor and delete_list functions
        void go_next();  //Advances current ptr
        void run_special();  //Properly downcasts to call the correct unique function on the current node
        void show_rules();  //Prompts if user wants to see rules then calls rules function as appropriate
        bool keep_playing();  //Prompts if user wants to keep playing

    public:
        Game_List();
        Game_List(const Game_List &source);
        ~Game_List();
        void play_loop(Player_List &players);  //Main game loop, takes in the Player_List class from calling function
        void add_game();  //Prompts which game the user wants to add, then adds it at the rear of the CLL
        void delete_list();  //Deletes CLL and resets current to NULL
        void set_player();  //Sets the player var

};

//Simple class for holding the Player_List and Game_List classes.  Could have implemented in main,
//but this made it cleaner
class Game_Master
{

    private:
        Player_List players;
        Game_List games;

    public:
        Game_Master();
        Game_Master(const Game_Master &source);
        void menu();  //Basic user interface, holds gateways to functiosn from the two list classes


};

#endif
