/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef PLAYER_H_
#define PLAYER_H_


#include <map.h>
#include <constant.h>

struct player;

// Creates a new player with a given number of available bombs
struct player* player_init(int bomb_number);
void   player_free(struct player* player);

// Set the position of the player
void player_set_position(struct player *player, int x, int y);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);
int player_get_lvl(struct player* player);
// Set the direction of the next move of the player
void player_set_current_way(struct player * player, enum direction direction);

// Set, Increase, Decrease the number of bomb that player can put
int  player_get_nb_bomb(struct player * player);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);

// Set Increase decrease number of life
int player_get_nb_life(struct player* player);
void player_inc_life(struct player* player);
void player_dec_life(struct player* player);

// Set, Increase Decrease number of key
int player_get_nb_key(struct player* player);
void player_inc_nb_key(struct player* player);
void player_dec_nb_key(struct player* player);
void player_inc_lvl(struct player* player);

// affichage 
int player_bomb(struct map* map , int t2);
void init_time_bomb(int b,struct player *player) ;
// Move the player according to the current direction
int player_move(struct player* player, struct map* map);

// Display the player on the screen
void player_display(struct player* player);
int player_get_nb_portee(struct player* player);
void player_inc_portee(struct player* player);
void player_dec_portee(struct player* player);

#endif /* PLAYER_H_ */
