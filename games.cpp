/*
 * Sawyer Norquist
 * CS202
 * Program 2
 * games.cpp
 * 7/21/2020
 *
 * Implementation for all functions in games.h.  
 *
 */

#include "games.h"
using namespace::std;

//Just outputs 100 blank lines
void clear_screen()
{

    for(int i = 0; i < 100; ++i)
        cout << endl;

    return;

}

//Helper for an exit loop.  Intended to be called in the while of a do-while loop
//Returns true if user input indicates they want to quit
bool to_quit()
{

    //
    char response = 'a';
    
    cout << "Do you want to play again? (y/n): ";
    cin >> response;
    response = toupper(response);

    while(response != 'Y'  && response != 'N')
    {

        cin.clear();
        cin.ignore(500, '\n');
        cout << "Invalid response" << endl;
        cout << "Do you want to play again? (y/n): ";
        cin >> response;
        response = toupper(response);

    }

    cin.clear();
    cin.ignore(500, '\n');

    if(response == 'Y')
        return true;
    else
        return false;

}

Game::Game():player(NULL), score(0){}

Game::Game(const Game &source):score(source.score)
{

    copy_char(player, source.player);

}

Game::~Game()
{

    delete[] player;

}

//Takes in a char* array to set the player inside the Game object
void Game::set_player(char *new_player)
{

    copy_char(player, new_player);

    return;

}

//Simple function to load the player and score from the Game class
//Takes in a Player_List and calls the update function using the Game class vars
void Game::update_score(Player_List &update_player)
{

    update_player.add_player(player, score);
    return;

}

//next pointer getter
Game *&Game::get_next()
{

    return next;

}

//next pointer setter - takes in a Game ptr
void Game::set_next(Game *to_set)
{

    next = to_set;
    return;

}

//constructor for the Memory game
//Defaults to no delay - not playable, but not intended to be by default
Memory::Memory():delay_time(0){}

Memory::Memory(const Memory &source):Game(source), delay_time(source.delay_time){}

//Main game loop.  Takes no args, but uses a randomizer and a set of dynamically allocated
//char arrays to generate a string of random letters
void Memory::play()
{

    char *holder = NULL;  //holds a single char, fed by the randomizer
    char *guess = NULL;  //the user's guess
    char *to_guess = NULL;  //appended string formed from the holder char

    srand(time(NULL));

    holder = new char('a' + rand() % 26);  //randomizer code

    copy_char(to_guess, holder);


    //primary loop - on each execution it adds a random char to to_guess
    //until the player can't remember any more.  A known issue is that it doesn't
    //clear the buffer before reading in, so you can cheat by typing in the characters
    //while they're still displayed
    do
    {

        *holder = 'a' + rand() % 26;
        append_char(to_guess, holder);

        cout << "Memorize..." << endl;
        cout << to_guess << endl;
        sleep(delay_time);  //pauses based on delay_time
        clear_screen();
        cout << "What is the series of letters? ";
        get_input_char(guess);

    }while(strcmp(guess, to_guess) == 0);  //loop continues as long as guess and to_guess match

    score = strlen(to_guess) - 1;  //Calculates score.  Also doesn't do it properly as you'll get a score even if you fail first time, but I ran out of time to make it better

    cout << "Your score is " << score << endl;


    //clear all the dynamic memory - NO LEAKS!
    //oh, and don't delete a null pointer either
    if(holder)
        delete holder;
    
    if(guess)
    {

        delete[] guess;
        guess = NULL;

    }

    
    if(to_guess)
    {

        delete[] to_guess;
        to_guess = NULL;

    }
    

    return;

}

//Solicits user input to determine how long they want to see the characters
//Sets delay_time
void Memory::set_time()
{

    cout << "How long do you want the letters displayed? ";
    cin >> delay_time;

    if(!cin.good())
    {
        cin.clear();
        cin.ignore(500, '\n');

        cout << "Input not recognized - please input a number" << endl;
        cout << "How long do you want the letters displayed? ";
        cin >> delay_time;

    }

    cin.clear();
    cin.ignore(500, '\n');

    cout << endl << "Letters will be displayed for " << delay_time << " seconds during play";
    cout << endl;

    return;

}

//simple display void to show the rules
void Memory::rules()
{

    cout << "Welcome to Memory!" << endl;
    cout << "This game is simple!  A series of random letters will be displayed for" << endl;
    cout << "a predetermine number of seconds.  You will enter as many letters as" << endl;
    cout << "you can remember.  If you get it correct, new random letters will be" << endl;
    cout << "added and you try again!  This continues until you are unable to remember" << endl;
    cout << "the sequence.  Your final score will be the number of letters you remembered." << endl << endl;

    return;

}

//set up Adder game
Adders::Adders():player_2(NULL), score_2(0){}

Adders::Adders(const Adders &source): Game(source), score_2(source.score_2)
{

    copy_char(player_2, source.player_2);

}

Adders::~Adders()
{

    if(player_2)
        delete[] player_2;

}

//main game loop for Adders
//no args or returns
void Adders::play()
{

    int difficulty = 0;  //holds the selected difficulty level
    int rounds = 0;  //how many rounds the users want to play against each other
    
    //need to make sure a second player has been entered
    if(!player_2)
    {

        cout << "Player 2 is not set - please add Player 2 and try again" << endl;
        return;

    }


    cout << "Welcome to Adder!" << endl;

    //main game loop, supported by a few dedicated functions
    //this loop primarily drives the main game menu
    do
    {

        cout << "What difficulty level would you like to play?" << endl;
        cout << "1.  Easy" << endl;
        cout << "2.  Medium" << endl;
        cout << "3.  Hard" << endl;
        cout << "Select: ";

        difficulty = get_input_int();

        cout << "How many rounds do you want to play? (max 10): ";

        rounds = get_input_int();

        //ensure user hasn't done anything silly
        while(rounds < 1 || rounds > 10)
        {

            if(rounds < 1)
                cout << "You need at least one round!" << endl;

            if(rounds > 10)
                cout << "That's too many, you can only play up to ten rounds!" << endl;

            cout << "How many rounds do you want to play? (max 10): ";
            rounds = get_input_int();

        }

        //second game loop, actually plays the game
        play_round(difficulty, rounds);

    }while(to_quit());

    return;
       

}

//the heart of Adders
//Takes in an int to determine the difficulty level, and an int to determine number of rounds
void Adders::play_round(int &difficulty, int num_rounds)
{

    int base_int = 0;  //limits the random function output
    int first_int = 0;  //First integer to add
    int second_int = 0;  //second integer to add
    int score_add = 0;  //holds the score to add to the winning player
    bool correct_1 = false;  //bool to determine if player 1 calculated correctly
    bool correct_2 = false;  //bool to determine if player 2 calculated correctly
    auto start_1 = chrono::steady_clock::now();  //start time for player 1
    auto end_1 = chrono::steady_clock::now();  //end time for player 1
    auto start_2 = chrono::steady_clock::now();  //start time for player 1
    auto end_2 = chrono::steady_clock::now();  //end time for player 1

    srand(time(NULL));

    //sets up base_int and score_add based on input difficulty
    //default case is rather hard
    switch(difficulty)
    {
        case 1:
            base_int = 10;
            score_add = 1;
            break;

        case 2:
            base_int = 100;
            score_add = 3;
            break;

        case 3:
            base_int = 1000;
            score_add = 5;
            break;

        default:
            base_int = 1000000;
            break;

    }

    //loop executes num_rounds time
    for(int i = 0; i < num_rounds; ++i)
    {

        first_int = rand() % base_int;  //random first int
        second_int = rand() % base_int;  //random second int

        cout << player << ", please calculate the following:" << endl;
        cout << first_int << " + " << second_int << endl << "Answer: ";

        start_1 = chrono::steady_clock::now();  //start clock on user input
        //gets user input, immediately checks if calculation is correct and sets bool appropriately
        if(get_input_int() == first_int + second_int)
            correct_1 = true;
        else
            correct_1 = false;
        end_1 = chrono::steady_clock::now();  //end clock

        //rest as above, except for second player
        first_int = rand() % base_int;
        second_int = rand() % base_int;

        cout << player_2 << ", please calculate the following:" << endl;
        cout << first_int << " + " << second_int << endl << "Answer: ";

        start_2 = chrono::steady_clock::now();
        if(get_input_int() == first_int + second_int)
            correct_2 = true;
        else
            correct_2 = false;
        end_2 = chrono::steady_clock::now();

        //helper function to determine winner and add to correct score
        calculate_score(correct_1, correct_2, end_1 - start_1, end_2 - start_2, score_add);

    }

    return;

}


//pulls in a bool/auto var set for the first player, and another bool/auto var set for the second player
//Also takes in an int for the total score to add
//Function checks first for who got the correct answer.  If both were correct, check who was fastest
//if neither were correct, no score is awarded
void Adders::calculate_score(bool correct_1, bool correct_2, auto time_1, auto time_2, int score_add)
{

    //first check for tie, and then check against times
    if(correct_1 && correct_2)
    {

        if(time_1 > time_2)
        {
            cout << "You were both correct, but " << player << " was faster and wins the point" << endl;
            score = score + score_add;
        }else
        {
            cout << "You were both correct, but " << player_2 << " was faster and wins the point" << endl;
            score_2 = score_2 + score_add;

        }

        return;

    }

    //then if not tie, check if either won
    if(correct_1)
    {

        cout << player << " had the correct answer and wins the point" << endl;
        score = score + score_add;

        return;

    }

    if(correct_2)
    {

        cout << player_2 << " had the correct answer and wins the point" << endl;
        score_2 = score_2 + score_add;

        return;

    }

    //if neither were correct
    cout << "Neither player had the right answer" <<endl;
    return;

}

//takes in a Player_List object
//First calls the base class update_score function
//Then updates Player_List for the second player
void Adders::update_score(Player_List &update_player)
{

    Game::update_score(update_player);
    update_player.add_player(player_2, score_2);

    return;

}

//unique function for Adders
//Prompts and sets the second player
void Adders::set_multiplayer()
{

    cout << "This is a two person game!" << endl;
    cout << "What is the gamertag for the second person? ";

    get_input_char(player_2);

    cout << "Welcome " << player_2 << ".  We're ready to play now!" << endl;

    return;

}

//simple rule display function for Adders
void Adders::rules()
{

    cout << "Welcome to Adders!"  << endl;
    cout << "Pit your addition prowess against your friend!  In this game" << endl;
    cout << "you and your friend will both add two sets of numbers together." << endl;
    cout << "The player who gets the number correct wins!  If it's a" << endl;
    cout << "tie, the player who completed quickest wins!  A tie awards" << endl;
    cout << "no points.  There are three difficulty levels:" << endl;
    cout << "Easy - add two numbers up to ten, worth 1 point." << endl;
    cout << "Medium - add two numbers up to one hundred, worth 3 points." << endl;
    cout << "Hard - add two numbers up to a thousand, worth 5 points." << endl;
    cout << "You will also be able to choose how many rounds you want to play" << endl;
    cout << "up to 10 rounds." << endl;

    return;

}

HeadTail::HeadTail():tries(0), num_to_get(0), score_give(0){}


//unique function for HeadTail
//Prompts for a difficulty level then sets class var members based on input
void HeadTail::set_difficulty()
{

    int input = 0; //holds user input

    //menu display with input catch and validation
    //Should be do while, but I hadn't remembered that technique when I wrote this
    cout << "1.  Easy, 2 out of 3, 1 point earned" << endl;
    cout << "2.  Medium, 4 out of 5, 3 points earned" << endl;
    cout << "3.  Hard, 6 out of 7, 5 points earned" << endl;
    cout << "Choose difficulty level: ";

    input = get_input_int();

    while(input < 1 || input > 3)
    {

        cout << "I'm sorry, I'm expecting input from 1 to 3" << endl;
        cout << "1.  Easy, 2 out of 3, 1 point earned" << endl;
        cout << "2.  Medium, 4 out of 5, 3 points earned" << endl;
        cout << "3.  Hard, 6 out of 7, 5 points earned" << endl;
        cout << "Choose difficulty level: ";

        input = get_input_int();

    }

    //switch statement to set up the class vars
    switch(input)
    {
         case 1:
            tries = 3;
            num_to_get = 2;
            score_give = 1;
            break;

        case 2:
            tries = 5;
            num_to_get = 4;
            score_give = 3;
            break;

        case 3:
            tries = 7;
            num_to_get = 6;
            score_give = 5;
            break;
    
    }

    return;

}

//main game loop for HeadTail
void HeadTail::play()
{

    int coin = 0;  //Coin value - no specific value for head or tails needed for the function to work
    int answer = 0;  //user input var
    int num_correct = 0;  //tracks number of correct guesses

    srand(time(NULL));

    //primary do while loop
    do
    {
    
        //loops the number of tries based on selected difficulty
        for(int i = 0; i < tries; ++i)
        {
             
            coin = rand() % 2;  //Random number, 0 or 1
            cout << "Heads or tails? ";
            answer = get_input();

            //if user input matches coin, your correct answer is added to num_correct
            if(coin == answer)
            {

                cout << "Correct!" << endl << endl;
                ++num_correct;

            }else
                cout << "Incorrect" << endl << endl;

        }

        //once the number of tries is completed, pass the number of correct tries into another
        //function for evaluation
        score_game(num_correct);
        num_correct = 0;  //reset in case player wants to go again in this loop

    }while(to_quit());

    return;   

}

//helper function for soliciting user input
//should have had a better name to differentiate between other input functions
//Returns an int converted from user input into a 0 or a 1
int HeadTail::get_input()
{

    char input = 'a';  //user input var

    cin >> input;
    input = toupper(input);

    //reprompt if incorrect input
    while(input != 'H' && input != 'T')
    {
            
        cin.clear();
        cin.ignore(500, '\n');

        cout << "Expecting a Head or a Tails" << endl;
        cout << "Head or tails? ";
        cin >> input;
        input = toupper(input);

    }

    cin.clear();
    cin.ignore(500, '\n');

    //check which user input matches
    if(input == 'T')
        return 0;

    if(input == 'H')
        return 1;

    //should never get to this point, but needed just in case
    return 0;

}

//Takes in int, checks it against the current number needed to win the game
//Updates score var for later extraction
void HeadTail::score_game(int &answer)
{

    //compare number correct against number needed to win
    if(answer >= num_to_get)
    {

        cout << "You won the round!  " << score_give << " points awarded." << endl;
        score = score + score_give;

    }else
        cout << "You missed by " << num_to_get - answer << " guesses - try again!" << endl;

    return;

}

//simple display function to show the rules for HeadTail
void HeadTail::rules()
{

    cout << "Welcome to Heads or Tails!" << endl;
    cout << "Guess whether the coin will land heads or tails to earn points." << endl;
    cout << "There are three difficulty levels:" << endl;
    cout << "Easy - 2 out of 3, 1 point earned" << endl;
    cout << "Medium - 4 out of 5, 3 points earned" << endl;
    cout << "Hard - 6 out of 7, 5 points earned" << endl;
    cout << "Choose wisely, and have fun!" << endl;

    return;

}

Game_List::Game_List():rear(NULL), current(NULL), player(NULL){}

Game_List::Game_List(const Game_List &source)
{

    //verify source has a valid rear
    //If so, set up first node then call recursive copy function
    if(!source.rear)
        rear = NULL;
    else
    {
        Game *head = copy_list(rear, source.rear->get_next());
        rear->set_next(head);

    }

    if(source.player)
        copy_char(player, source.player);

}

Game_List::~Game_List()
{

    if(player)
        delete[] player;

    //checks condition of rear ptr, and calls delete functions as needed
    if(rear)
    {
        
        if(rear->get_next() != rear)
        {

            Game *next_node = rear->get_next();
            rear->set_next(NULL);
            delete_list(next_node);

        }else
            delete rear;

    }

}

//recursive function to assist with the CLL copy
//requires calling function set up first node
//first arg is the new CLL, second is the source CLL
Game *Game_List::copy_list(Game *&new_list, Game *source)
{

    //base case - rear has already been set up, so we need to terminate and create
    //the last copied node as this is a tail recursive function
    if(!source->get_next())
    {

        Game *new_node = copy_node(source);
        new_node->set_next(NULL);
        new_list = new_node;

        return new_node;

    }

    Game *new_node = copy_node(source);  //due to dynamic casing, the new pointer is set up by a helper function
    new_node->set_next(copy_list(new_list, source->get_next()));

    return new_node;

}

//Support function for ensuring the correct class object is created and return
//takes in the pointer to create a new class of, returns a pointer to that new class object
Game *Game_List::copy_node(Game *to_check)
{

    Game *return_ptr = NULL;  //this will hold the new class object
    Memory *memory_ptr = dynamic_cast<Memory *>(to_check);  //dyncast for Memory class
    Adders *adders_ptr = dynamic_cast<Adders *>(to_check);  //dyncast for Adders class
    HeadTail *headtail_ptr = dynamic_cast<HeadTail *>(to_check);  //dyncast for HeadTail class

    //checks for which of the dynamic cast pointers is good
    //Not if else because only one can be true
    //Creates a new node of the appropriate type with the address stored in return_ptr
    if(memory_ptr)
        return_ptr = new Memory(*memory_ptr);

    if(adders_ptr)
        return_ptr = new Adders(*adders_ptr);

    if(headtail_ptr)
        return_ptr = new HeadTail(*headtail_ptr);

    return return_ptr;

}

//recursive function for deleting the list
//Takes in a Game ptr, does a head recursive delete
void Game_List::delete_list(Game *&current)
{

    if(!current)
        return;

    delete_list(current->get_next());
    delete current;

    return;

}

//set up game CLL based on user input
void Game_List::add_game()
{

    int input = 0; //catch user input
    Game *new_node = NULL;  //holds new class object for adding to list

    //player needs to exist, as the game objects will hold the player name independently
    if(!player)
    {

        cout << "Player not set - please set player and try again" << endl;
        return;

    }

    //Looping menu - allows games to be added until the user chooses to leave the loop
    do
    {

        cout << "Which game would you like to play?" << endl;
        cout << "1.  Memory - how many random letters can YOU remember?" << endl;
        cout << "2.  Adders - go head-to-head with a friend as you race to solve math problems!" << endl;
        cout << "3.  Heads or Tails - can you guess what the next flip will be?" << endl;
        cout << "0.  Quit adding games" << endl;
        cout << "Select: ";

        input = get_input_int();

        while(input < 0 || input > 3)
        {

            cout << "I'm sorry, that's not a valid selection" << endl;
            cout << "Which game would you like to play?" << endl;
            cout << "1.  Memory - how many random letters can YOU remember?" << endl;
            cout << "2.  Adders - go head-to-head with a friend as you race to solve math problems!" << endl;
            cout << "3.  Heads or Tails - can you guess what the next flip will be?" << endl;
            cout << "0.  Quit adding games" << endl;
            cout << "Select: ";

            input = get_input_int();

        }

        //interprets input and sets new_node with the appropriate upcast object
        switch(input)
        {
            case 1:
                new_node = new Memory();
                break;

            case 2:
                new_node = new Adders();
                break;

            case 3:
                new_node = new HeadTail();
                break;

        }

        //checks state of the user input and the rear ptr
        //if user input isn't for an exit, one of the two will execute
        //if rear is null, sets the node pointing to itself
        //if there's already an object on rear, does a rear insert
        if(!rear && input != 0)
        {

            rear = new_node;
            rear->set_next(rear);
            rear->set_player(player);
            new_node = NULL;

        }else if(input != 0)
        {

            new_node->set_player(player);
            new_node->set_next(rear->get_next());
            rear->set_next(new_node);
            rear = new_node;
            new_node = NULL;

        }

    }while(input != 0);

}

//set player name for the current iteration of Game_List
void Game_List::set_player()
{

    cout << "What is your gamertag? ";

    get_input_char(player);

    cout << "Welcome, " << player << "!" << endl;

    return;

}

//advances the current pointer for the Game CLL
void Game_List::go_next()
{

    if(!current)
        current = rear->get_next();
    else
        current = current->get_next();

    return;

}

//delete function to support when a new player joins
void Game_List::delete_list()
{

    //nothing to do if CLL is already empty
    if(!rear)
        return;

    //if only one node, just need to delete rear and set to NULL
    if(rear == rear->get_next())
    {

        delete rear;
        rear = NULL;
        current = NULL;
        return;

    }

    //if multiple objects on list, turn into LLL, delete LLL, and set rear AND current to NULL
    //Bad things happen if you don't set current to NULL
    Game *next_node = rear->get_next();
    rear->set_next(NULL);
    delete_list(next_node);
    rear = NULL;
    current = NULL;

    return;

}

//function to perform downcasting and unique function call for the derived class objects
void Game_List::run_special()
{

    //first dynamic cast for each time
    Memory *memory_ptr = dynamic_cast<Memory *>(current);
    Adders *adders_ptr = dynamic_cast<Adders *>(current);
    HeadTail *headtail_ptr = dynamic_cast<HeadTail *>(current);

    //then evaluate which pointer is good and call the correct object function
    if(memory_ptr)
        memory_ptr->set_time();

    if(adders_ptr)
        adders_ptr->set_multiplayer();

    if(headtail_ptr)
        headtail_ptr->set_difficulty();

    return;

}

//plays through the CLL and manages the required interactions between Game objects
//Takes in the list of players for the Game objects to update
void Game_List::play_loop(Player_List &players)
{

    //leave function if games haven't been set
    if(!rear)
    {
        
        cout << "No games on list yet!  Please add games and try again." << endl;
        return;

    }

    //leave function if there's no player
    if(!player)
    {

        cout << "No player set yet!  Please create a player and try again." << endl;
        return;

    }

    //if current hasn't been set, put on beginning of CLL
    if(!current)
        go_next();

    //primary game look
    do
    {

        show_rules();  //wrapper function to the dynamic rules call, checks if user wants to see the rules   
        run_special();  //call the special downcasting function
        current->play();  //dynamic call to the object play function
        current->update_score(players);  //dynamic call to the player list update function
        go_next(); //advance to the next pointer, but nothing will happen unless the player keeps the loop going
        
    }while(keep_playing());  //check if user wants to play the next game on the loop

    return;

}

//wrapper for the dynamic rules function call
//checks if user wants to see the rules
void Game_List::show_rules()
{

    char input = 'a';  //user input

    //here's the data validating do while loop
    do
    {

        cout << "Would you like to read the rules? (y/n): ";
        cin >> input;
        input = toupper(input);

        cin.clear();
        cin.ignore(500, '\n');

        if(input != 'Y' && input != 'N')
            cout << "Invalid input" << endl;

    }while(input != 'Y' && input != 'N');

    //if yes, dynamic call time
    if(input == 'Y')
        current->rules();

    return;

}

//Helper function to determine if player wants to continue the loop - returns yes if so 
bool Game_List::keep_playing()
{

    char input = 'a'; //user input

    do
    {

        cout << "Would you like to play the next game? (y/n): ";
        cin >> input;
        input = toupper(input);

        cin.clear();
        cin.ignore(500, '\n');

        if(input != 'Y' && input != 'N')
            cout << "Invalid input" << endl;

    }while(input != 'Y' && input != 'N');

    if(input == 'Y')
        return true;
    else
        return false;

}

Game_Master::Game_Master(){}

Game_Master::Game_Master(const Game_Master &source):players(source.players), games(source.games){}

//main menu for managing the player list and game list
void Game_Master::menu()
{

    int input = 0;  //user input
    bool continue_program = true;  //bool to control main loop

    cout << "Welcome to Dynamic Games!" << endl;
    cout << "The games aren't dynamic, but the the casting sure is!" << endl; //I'm so funny

    do
    {

        cout << "Main Menu" << endl;
        cout << "1.  New player" << endl;
        cout << "2.  Set game list" << endl;
        cout << "3.  Start play"  << endl;
        cout << "4.  Show player scores" << endl;
        cout << "0.  Quit program" << endl;
        cout << "Select: ";

        input = get_input_int();

        if(input < 0 || input > 4)
            cout << "Invalid input" << endl << endl;

        //interpret user input
        //cases are mixed up because I needed to fiddle with menu order
        //and didn't want to risk breaking something at the 11th hour
        switch(input)
        {

            case 3:
                games.play_loop(players);
                break;

            case 2:
                games.add_game();
                break;

            case 1:
                games.delete_list();
                games.set_player();
                break;

            case 4:
                players.display();
                break;

            case 0:
                continue_program = false;
                break;

            default:
                continue_program = true;

        }

    }while(continue_program);

}

