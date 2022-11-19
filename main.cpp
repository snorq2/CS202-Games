
/*
 * Sawyer Norquist
 * CS202
 * Program 2
 * main.h
 * 7/21/2020
 *
 * Primary caller of the Game_Master object and entry into the games
 */
//#include "players.h"
#include "games.h"
using namespace::std;

int main()
{

    Game_Master master_play;

    master_play.menu();

    return 0;

}
