/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <time.h>
#include <SDL_ttf.h>
#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <stdio.h> 

struct player {
	int x, y;
	enum direction direction;
	int bombs;
        int life;
        int key;
        int lvl;
      	int portee;
    
 
};
int time_bomb = -5000 ;
int x1=0 , y1=0 , nb_bomb_ex = 0 ;


struct player* player_init(int bombs) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->direction = NORTH;
	player->bombs = bombs;
	player->life = 3;
	player->key = 0;

	player->lvl =0;
	player->portee = 1 ;
    

	return player;
}



void player_set_position(struct player *player, int x, int y) {
	assert(player);
	player->x = x;
	player->y = y;
}


void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

int player_get_lvl(struct player* player){
  assert(player);
  return player->lvl;
}
void player_inc_lvl(struct player* player) {
	assert(player);
	player->lvl += 1;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->direction = way;
}

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->bombs;
}


void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->bombs += 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->bombs -= 1;
}

int player_get_nb_life(struct player* player) {
        assert(player);
        return player->life;
}

void player_inc_life(struct player* player) {
        assert(player);
        player->life += 1;
}

void player_dec_life(struct player* player) {
        assert(player);
        player->life -= 1;
}

int player_get_nb_portee(struct player* player) {
        assert(player);
        return player->portee;
}

void player_inc_portee(struct player* player) {
        assert(player);
        player->portee += 1;
}

void player_dec_portee(struct player* player) {
        assert(player);
        player->portee -= 1;
}


int player_get_nb_key(struct player* player) {
        assert(player);
        return player->key;
}

void player_inc_nb_key(struct player* player) {
        assert(player);
        player->key += 1;
}

void player_dec_nb_key(struct player* player) {
        assert(player);
        player->key -= 1;
}



static int player_move_aux(struct player* player, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;
	 
	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
	  if (map_get_cell_compose_type(map,x,y)== 4)
	    SDL_Quit(); 
	     	return 0; 
		break;

	case CELL_BOX:
		return 1;
		break;

	case CELL_BONUS:
	  if (map_get_cell_compose_type(map, x , y)== CELL_BONUS_ND)
	    player_dec_nb_bomb(player);
	  if (map_get_cell_compose_type(map, x , y)== CELL_BONUS_NI)
	    player_inc_nb_bomb(player);
	  if (map_get_cell_compose_type(map, x , y)== CELL_BONUS_LIFE)
	    player_inc_life(player);
	  if (map_get_cell_compose_type(map, x , y)== CELL_BONUS_RD)
		  player_dec_portee(player);
	  if (map_get_cell_compose_type(map, x , y)== CELL_BONUS_RI)
		  player_inc_portee(player);
		  
		break;
	

	    
	case CELL_KEY:
           player_inc_nb_key(player);
	        break;
	case CELL_MONSTER:
		break;
	case CELL_DOOR:
	 
	  if  ((map_get_cell_compose_type(map, x , y)== CELL_CL_DOOR) && (player->key ==0))
	    return 0;
	  else
	    { 
	     map_set_com_type(map,x,y,CELL_OP_DOOR);
	     player_dec_nb_key(player);	     	    
	     player_inc_lvl(player);
	   
	    }
	  
	  break;

	default:
		break;
	}

	// Player has moved
	return 1;
}

// initialiser le time
void init_time_bomb(int b,struct player *player){	
	if(player->bombs>0){
		if(nb_bomb_ex==0){
		player_dec_nb_bomb(player) ;
    time_bomb = b ;	
    x1 = player->x ;
    y1 = player->y ;
		nb_bomb_ex++ ;
		}
	}

}
int player_bomb(struct map* map , int t2){
	assert(map) ;
	int t1 = time_bomb ;
	int x=x1 ;
	int y=y1 ;
	
	// afichage des bombs
  
  if ((t2-t1)<1000 ){
	    map_set_com_type(map,x,y,CELL_BOMB44);  // affichage de la bomb4
    }
  if ((t2-t1)>1000 && (t2-t1)<2000){
	    map_set_com_type(map,x,y,CELL_BOMB33);		// affichage de la bomb3
    }
  if ((t2-t1)>2000 &&(t2-t1)<3000 ){
 	    map_set_com_type(map,x,y,CELL_BOMB22);	 // affichage de la bomb2
    }
  if ( (t2-t1) > 3000 && (t2-t1) < 4000 ){
	    map_set_com_type(map,x,y,CELL_BOMB11);  // affichage de la bomb1
    }
	  // affichage de l'éxplosion
  if ((t2-t1)>4000 && (t2-t1)<5000 ){
          
	      map_set_com_type(map,x,y,0x80);

			//-----------------------------EXPLOSION OU BONUS DE (X,Y+1)-----------------------------------------------
			if((map_get_cell_type(map,x,y+1) & 0xf0) == CELL_BOX  ){
				map_set_com_type(map, x, y+1,map_get_cell_compose_type(map,x,y+1) + 0x30) ;
				} 
			if(map_get_cell_type(map,x,y+1) == CELL_EMPTY && y+1<11){
				map_set_com_type(map,x,y+1,0x80);
			}
			//------------------------------EXPLOSION OU BONUS DE (X+1,Y)------------------------------------------------
			if((map_get_cell_type(map,x+1,y) & 0xf0) == CELL_BOX ){
				map_set_com_type(map, x+1, y,map_get_cell_compose_type(map,x+1,y) + 0x30) ;
				} 
			if(map_get_cell_type(map,x+1,y) == CELL_EMPTY && x<11 ){
				map_set_com_type(map,x+1,y,0x80);
			}
			//------------------------------EXPLOSION OU BONUS DE (X,Y-1)---------------------------------------------
			if((map_get_cell_type(map,x,y-1) & 0xf0) == CELL_BOX  ){
				map_set_com_type(map, x, y-1,map_get_cell_compose_type(map,x,y-1) + 0x30) ;
				} 
			if(map_get_cell_type(map,x,y-1) == CELL_EMPTY && y>0){
				map_set_com_type(map,x,y-1,0x80);
			}
			//------------------------------EXPLOSION OU BONUS DE (X-1,Y)----------------------------------------------
			if((map_get_cell_type(map,x-1,y) & 0xf0) == CELL_BOX  ){
				map_set_com_type(map, x-1, y,map_get_cell_compose_type(map,x-1,y) + 0x30) ;
				} 
			if(map_get_cell_type(map,x-1,y) == CELL_EMPTY && x-1>0){
				map_set_com_type(map,x-1,y,0x80);
			}
  }
	if ((t2-t1)>5000 && (t2-t1)<6000 ){
    nb_bomb_ex=0;
	    map_set_com_type(map,x,y,0x00);
		if(map_get_cell_type(map,x,y+1)==EXPLO_BOMB){
			map_set_com_type(map,x,y+1,0x00);
		}
		if(map_get_cell_type(map,x+1,y)==EXPLO_BOMB){
			map_set_com_type(map,x+1,y,0x00);
		}
		if(map_get_cell_type(map,x,y-1)==EXPLO_BOMB){
			map_set_com_type(map,x,y-1,0x00);
		}
		if(map_get_cell_type(map,x-1,y)==EXPLO_BOMB){
			map_set_com_type(map,x-1,y,0x00);
		}
		   
	}

return(0) ;
}

int player_move(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->direction) {

	case NORTH:
	  if (player_move_aux(player, map, x, y - 1) && y > 0 && map_get_cell_type(map,x,y-1) != CELL_BOX) /*on traite les cas ou la cellule suivante n'est pas une boite*/
	    {
	      player->y--; /*si les conditions sont réunies le joueur avance*/
	       	move = 1;
	    }

	  if (y>1 && (map_get_cell_type(map,x,y-1) && (0xf0)) == CELL_BOX && map_get_cell_type(map,x,y-2)== CELL_EMPTY) /*on traite le cas ou la cellule suivante est une boite*/
	    {
	      map_set_cell_type(map,x,y-1,CELL_EMPTY); /*la cellule ou se deplace le joueur devient vide*/
	      map_set_cell_type(map,x,y-2,CELL_BOX); /*la boite est poussée par le joueur*/
	      player->y--;                     /*le joueur avance*/
		move = 1;
	    }
		break;
  

	case SOUTH:
	  if (player_move_aux(player, map, x, y + 1) && y <STATIC_MAP_HEIGHT-1 && map_get_cell_type(map,x,y+1) != CELL_BOX )
	    {
	       	player->y++;
       		move = 1;
	    }
	 
	  if ( y <STATIC_MAP_HEIGHT-2   &&  map_get_cell_type(map,x,y+1) == CELL_BOX   &&   map_get_cell_type(map,x,y+2)== CELL_EMPTY )
	    {
	        map_set_cell_type(map,x,y+1,CELL_EMPTY);
		map_set_cell_type(map,x,y+2,CELL_BOX);
	    	player->y++;
		move = 1;
	    }
		break;

	case WEST:
	  if (player_move_aux(player, map, x - 1, y)   &&   x>0   &&   map_get_cell_type(map,x-1,y) != CELL_BOX)
	    {
	       	player->x--;
       		move = 1;
	    }
	 
	  if ( x>1   &&   map_get_cell_type(map,x-1,y) == CELL_BOX   &&   map_get_cell_type(map,x-2,y)== CELL_EMPTY)
	    {
	        map_set_cell_type(map,x-1,y,CELL_EMPTY);
	        map_set_cell_type(map,x-2,y,CELL_BOX);
	        player->x--;
		move = 1;
	    }
		break;

	case EAST:
		if (player_move_aux(player, map, x + 1, y) && x <STATIC_MAP_WIDTH-1 && map_get_cell_type(map,x+1,y) != CELL_BOX) {
			player->x++;
			move = 1;
		}
		if ( x <STATIC_MAP_WIDTH-2 && map_get_cell_type(map,x+1,y) == CELL_BOX && map_get_cell_type(map,x+2,y)== CELL_EMPTY){
		  map_set_cell_type(map,x+1,y,CELL_EMPTY);
	          map_set_cell_type(map,x+2,y,CELL_BOX);
		  player->x++;
		  move = 1;}
		break;
	}
int a = 0 ;	
  if(map_get_cell_type(map,x,y) == 0x70 ) {
		map_display(map);
	  a++ ;	
	}
	if(map_get_cell_type(map,x,y) == 0x80 ){
		map_set_com_type(map,x,y,0x80);
		a++ ;
	}
	if (move){
	  if ((a==0) & (map_get_cell_type(map,x,y)!= CELL_DOOR))

		  map_set_cell_type(map, x, y,CELL_EMPTY); 
		}

		
  
	return move;
}

	

void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}

