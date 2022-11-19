/*
 * Sawyer Norquist
 * CS202
 * Program 2
 * players.h
 * 7/21/2020
 *
 * This contains the structures for the Player base class, the derived Player_Node class, and the containing
 * list management class
 */

#ifndef PLAYERS
#define PLAYERS

#include <cstring>
#include <iostream>
//#include "games.h"

static const int MAX_USER_INPUT = 500; //used for get_input_char function

void get_input_char(char *&output);  //Sets 'output' to the user input
int get_input_int();  //outputs int based on user input, checks to ensure user input is a number
void copy_char(char *&destination, const char *source);  //copy char array from source to destination
void append_char(char *&destination, const char *append);  //append char array from append to destination

//core class for handling Player data.  Composed of two primary data members - the gamertag char* and the player's score
class Player
{

    private:
        char* gamertag;  //player name
        int score;  //player score

    public:
        Player();
        Player(const Player &source);
        Player(const char *source, const int &new_score);  //Primary constructor for adding a new player
        ~Player();
        void set_name(); //Prompts input, originally intended as primary means to do so, but not used
        void set_name(const char *&input); //change player name via char array input, not used
        void add_score(const int &to_add); //increments score by given amount
        void display() const;  //show player info
        int score_compare(const Player *comparator) const; //compares score, if argument is lower, return is higher
        int score_compare(const int &comparator) const;  //compares score, if argument is lower, return is higher
        bool is_player(const char *comparator) const;  //compares char array, if match to gamertag is true
        

};

//derived node class for Player
class Player_Node: public Player
{

    private:
        Player_Node *next; //next pointer

    public:
        Player_Node();
        Player_Node(const Player_Node &source);
        Player_Node(const char *source, const int &new_score);
        Player_Node *&get_next();  //next getter
        void set_next(Player_Node *to_set);  //next setter

};

//Class to manage player list.  Contains a Player_Node head and supporting functions
//no user input interface, but does have output functions
class Player_List
{

    private:
        Player_Node *head;  //start of LLL
        //below are supporting recursive functions, see CPP for details
        void add_player(Player_Node *&current, Player_Node *&last, char *&new_player, const int &new_score);
        bool increment_player(Player_Node *&current, const char *player_increment, const int &score_add);
        void copy_player_list(Player_Node *&new_copy, Player_Node *source);
        void delete_list(Player_Node *&current);
        void display(Player_Node *current);
        void sort_list();  //do not want calling class/function to be able to sort, only called when might be needed
        bool sort_list(Player_Node *current, Player_Node *last);

    public:
        Player_List();
        Player_List(const Player_List &source);
        ~Player_List();
        void add_player(char *&new_player, const int &new_score);  //Add player to list, will add at end by default, returns false if player already exists
        bool increment_player(const char *player_increment, const int &score_add);  //increments a player, should have been private, but originally saw potential usage scenario
        void display();  //Display all nodes

};

#endif
