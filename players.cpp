/*
 * Sawyer Norquist
 * CS202
 * Program 2
 * players.cpp
 * 7/21/2020
 *
 * Implementations of players.h
 */

#include "players.h"

using namespace::std;

//helper function for assigning user input as a new array to a char ptr
//Takes in a char* as reference argument to be modified
//No prompt, intended to support prompts in other relevent functions
void get_input_char(char *&output)
{

    char *user_input = new char[MAX_USER_INPUT + 1];  //Holder for initial user input

    cin.get(user_input, MAX_USER_INPUT);

    cin.clear();
    cin.ignore(500, '\n');

    //delete output if it contains data. IMPORTANT - calling function must ensure output owns
    //memory (always initialize) or this will cause problems
    if(output)
        delete[] output;

    //rightsize output based on actual user input and copy in
    output = new char[strlen(user_input) + 1];
    strcpy(output, user_input);

    delete[] user_input;  //In the paraphrased words of Edna Mode - NO LEAKS!!!

    return;

}

//similar to get_input_char, but no args and returns a single int
//no initial prompt, but does prompt if wrong input is detected
int get_input_int()
{

    int input = 0;  //user input

    cin >> input;

    //verify the input is actually an int and repromt if needed
    if(!cin.good())
    {

        cin.clear();
        cin.ignore(500, '\n');
        cout << "I'm sorry, I'm expecting a number - please try again: ";
        cin >> input;

    }

    cin.clear();
    cin.ignore(500, '\n');

    return input;

}

//Takes in a char*& and char*, copies the latter to the former, creating a new char[] as needed
//deletes the char*& if data exists
//Helps several other functions
void copy_char(char *&destination, const char *origin)
{

    //Set destination to null if origin has no data
    if(!origin)
    {

        delete destination;
        destination = NULL;

    }

    //clear destination if it has data.  IMPORTANT - calling function must initialize pointer properly
    //if it has garbage data, bad things happen here
    if(destination)
        delete[] destination;

    //set up new char array and copy over
    destination = new char[strlen(origin) + 1];
    strcpy(destination, origin);

    return;

}

//similar to copy_char, but appends to the char*& instead
//Used only in the Memory game, could have gone there but originally thought I might use this elsewhere
//this function assumes char*& has data, should have been fixed but ran out of time.  works properly in the game context
void append_char(char *&destination, const char *append)
{

    char *new_char = new char[strlen(destination) + strlen(append) + 1];  //set up holder char with new array sized to hold all needed data

    //copy origin data first, then append new data second
    //delete original destination then copy address over from new_char
    strcpy(new_char, destination);
    strcat(new_char, append);
    delete[] destination;
    destination = new_char;

    return;

}



Player::Player():gamertag(NULL), score(0){}

Player::Player(const Player &source): gamertag(NULL), score(source.score)
{

    copy_char(gamertag, source.gamertag);

}

//special constructor to set up a new player.  Primary constructor used.
Player::Player(const char *source, const int &new_score = 0):gamertag(NULL), score(new_score)
{

    copy_char(gamertag, source);

}

Player::~Player()
{

    if(gamertag)
        delete[] gamertag;

}

//Originally intended to have the Player class set its own name, ended up with different method
//Prompts user for name and saves it to gamertag
void Player::set_name()
{

    cout << "Please input gamertag: ";
    get_input_char(gamertag);

    return;

}

//Another iteration on setting the player
//Takes in char*& (in error, but function not used so I didn't change) and copies to gamertag
void Player::set_name(const char *& input)
{

    if(!input)
        return;

    copy_char(gamertag, input);

    return;

}

//Increment player score
void Player::add_score(const int &to_add)
{

    score = score + to_add;

    return;

}

//output player name and score to screen
void Player::display() const
{

    cout << "Gamertag: " << gamertag << endl;
    cout << "Score : " << score << endl;

    return;

}

//takes in int of score, returns comparison to internal var
//Not used, ended up using the next function instead
int Player::score_compare(const int &comparator) const
{

    return comparator - score;

}

//Takes in another player object and compares score
//Used to help sort list in the list class
int Player::score_compare(const Player *comparator) const
{

    return score_compare(comparator->score);

}

//takes in a name, returns true if the name matches gamertag
bool Player::is_player(const char *comparator) const
{

    if(strcmp(gamertag, comparator) == 0)
        return true;

    return false;

}

Player_Node::Player_Node():next(NULL){}

Player_Node::Player_Node(const Player_Node &source):Player(source), next(NULL){}

//makes sure correct copy constructor for the Player class can be called when a new node is made
Player_Node::Player_Node(const char *source, const int &new_score = 0):Player(source, new_score), next(NULL){}

//getter for next
Player_Node *&Player_Node::get_next()
{

    return next;

}

//setter for next
//should have been const, but no time to verify and test
void Player_Node::set_next(Player_Node *to_set)
{

    next = to_set;
    return;

}

Player_List::Player_List():head(NULL){}

//copy constructor, head MUST be null or bad things happen later
Player_List::Player_List(const Player_List &source)
{

    head = NULL;
    copy_player_list(head, source.head);

}

Player_List::~Player_List()
{

    delete_list(head);

}

//Supports the constructor, head recursive function for building the LLL
void Player_List::copy_player_list(Player_Node *&new_copy, Player_Node *source)
{

    if(!source)
        return;

    copy_player_list(new_copy, source->get_next());

    Player_Node *to_add = new Player_Node(*source);

    to_add->set_next(new_copy);
    new_copy = to_add;

    return;

}

//head recursive function to delete list
void Player_List::delete_list(Player_Node *&current)
{

    if(!current)
        return;

    delete_list(current->get_next());

    delete current;

    current = NULL;

    return;

}

//adds a player to the list using char*& as new player and int as new score
//Should have been const char* as player, but had issues and no time to fix
//does double-duty to append player data in addition to adding a new one
void Player_List::add_player(char *&new_player, const int &new_score = 0)
{

    //make sure a player exists, or don't add anything.  Score is allowed to be zero
    if(!new_player)
        return;

    //if no players exist, easy to add node.  Node constructor sets next to null by default
    if(!head)
    {

        head = new Player_Node(new_player, new_score);
        return;

    }

    //checks if player exists, if not add player
    if(!increment_player(new_player, new_score))
    {


        //need to make sure player doesn't belong at top of list before calling recursive function
        if(head->score_compare(new_score) >= 0)
        {
             
            Player_Node *new_node = new Player_Node(new_player, new_score);
            new_node->set_next(head);
            head = new_node;
            return;

        }

        add_player(head->get_next(), head, new_player, new_score);

    }else
        
        //if a player was incrememented, need to make sure they move to the right spot
        sort_list();

    return;

}

//recursive function to support adding a new player
//takes in a current pointer to check, the last pointer so its next can be changed, and new player data
void Player_List::add_player(Player_Node *&current, Player_Node *&last, char *&new_player, const int &new_score)
{

    //if we've reached the end of the list, no lower scores exist and we add here
    if(!current)
    {

        current = new Player_Node(new_player, new_score);
        return;

    }

    //if the current score is smaller but last score is smaller, this is where we add
    if(current->score_compare(new_score) >= 0 && last->score_compare(new_score) <= 0)
    {

        Player_Node *new_node = new Player_Node(new_player, new_score);
        new_node->set_next(current);
        last->set_next(new_node);

        return;

    }

    //recurse!
    add_player(current->get_next(), current, new_player, new_score);

    return;

}

//wrapper function to check if player exists and increment score accordingly
//char* is player to find, int is the score to add, return is true if the player existed
bool Player_List::increment_player(const char *player_increment, const int &score_add)
{

    //if head is empty, player doesn't exist!
    if(!head)
        return false;

    return increment_player(head, player_increment, score_add);

}

//recursive score increment function
//Takes in the current list position and remaining player data
bool Player_List::increment_player(Player_Node *&current, const char *player_increment, const int &score_add)
{

    //if we've reached the end of the list, player doesn't exist
    if(!current)
        return false;

    //if player match found, increment score here and return true
    if(current->is_player(player_increment))
    {

        current->add_score(score_add);
        return true;

    }

    return increment_player(current->get_next(), player_increment, score_add);

}

//wrapper for display output
//Returns message if list is empty
void Player_List::display()
{

    if(!head)
        cout << endl << "No players on the board yet!" << endl << endl;
    else
        display(head);

    return;

}

//traverses lists calling display function on all nodes
void Player_List::display(Player_Node *current)
{

    if(!current)
        return;

    current->display();
    display(current->get_next());
    return;

}

//wrapper for sorting list
//catches several single-interation scenarios
void Player_List::sort_list()
{

    bool list_sorted = false;  //holder to make sure the recursive function continues until list is sorted

    //if list is empty or only one item on list, no need to sort
    if(!head || !head->get_next())
        return;

    //if only two items on list, directly check order and switch if needed
    if(!head->get_next()->get_next())
    {

        if(head->score_compare(head->get_next()) > 0)
        {

            head->get_next()->set_next(head);
            head = head->get_next();
            head->get_next()->set_next(NULL);

            return;
        }else
            return;

    }

    //loop executes as long as list is not sorted, only called if list has at least three nodes
    while(!list_sorted)
        list_sorted = sort_list(head, head);

    return;

}

//recursive portion of player list
//Takes in the current position and last position so appropriate pointers can be changed without too many
//look aheads.  Returns true when entire list is traversed with no swaps
bool Player_List::sort_list(Player_Node *current, Player_Node *last)
{

    //base case - if we're here, list is sorted
    //stops just short of end, because current is checking one node ahead for swap
    if(!current->get_next())
        return true;

    //checks current and next current to see if they need to switch
    if(current->score_compare(current->get_next()) > 0)
    {

        //make sure we're not at head, requires head to be reset if so.  Else just swap node.
        if(current == head)
        {

            head = current->get_next();
            current->set_next(head->get_next());
            head->set_next(current);

            return false;
        }

        last->set_next(current->get_next());
        current->set_next(last->get_next()->get_next());
        last->get_next()->set_next(current);

        return false;

    }

    return sort_list(current->get_next(), current);

}
