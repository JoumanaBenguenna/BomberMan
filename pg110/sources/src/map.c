/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>

struct map {
	int width;
	int height;
	unsigned char* grid;
};

#define CELL(i,j) ( (i) + (j) * map->width)

struct map* map_new(int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
	  for (j = 0; j < height; j++)
	    map->grid[CELL(i,j)] = CELL_EMPTY;

	return map;
}

int map_is_inside(struct map* map, int x, int y)
{
	assert(map);
	return 1;
}

void map_free(struct map *map)
{
	if (map == NULL )
		return;
	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0xf0 ;
}

enum compose_type map_get_cell_compose_type(struct map* map, int x, int y)
{ 
  assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)];
}

void map_set_com_type(struct map* map, int x, int y, enum compose_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}


void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}


void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type & 0x0f) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_DEC), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
		break;
	case BONUS_LIFE:
	  window_display_image(sprite_get_bonus(BONUS_LIFE), x, y);
	  break;
	}
}

void display_door(struct map* map, int x, int y, unsigned char type)
{
	switch (type & 0x0f) {
	case OP_DOOR:
	      	window_display_image(sprite_get_door_opened(), x, y);
		break;

	case CL_DOOR:
		window_display_image(sprite_get_door_closed(), x, y);
		break;

}
}
void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	case SCENERY_PRINCESS:
	  	window_display_image(sprite_get_princ(), x, y);
		break;
	}
}


void display_bomb(struct map* map, int x, int  y, unsigned char type){
	
  switch(type & 0x0f) {
		  
			case CELL_BOMB1:
	      // AFFICHAGE DE LA BOMB1
				window_display_image(sprite_get_banner_bomb1(), x, y) ;
	      break ;
			case CELL_BOMB2:
	      // AFFICHAGE DE LA BOMB2
	      window_display_image(sprite_get_banner_bomb2(), x, y);
	      break ;
			case CELL_BOMB3:
	      // AFFICHAGE DE LA BOMB3
	      window_display_image(sprite_get_banner_bomb(), x, y);
	      break ;
			case CELL_BOMB4:
	      // AFFICHAGE DE LA BOMB4
				window_display_image(sprite_get_banner_bomb4(), x, y);
	}
	
}

void map_display(struct map* map)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
	    x = i * SIZE_BLOC;
	    y = j * SIZE_BLOC;
	
	    unsigned char type = map->grid[CELL(i,j)];
	    
	    switch (type & 0xf0) {
		  case CELL_SCENERY:
		  display_scenery(map, x, y, type);
		    break;
	    case CELL_BOX:
			  window_display_image(sprite_get_box(), x, y);
	      break;

	    case CELL_BONUS:
	      display_bonus(map, x, y, type);
	      break;
	    case CELL_KEY:
	      window_display_image(sprite_get_key(), x, y);
	      break;
	    case CELL_DOOR:
	      display_door(map,x,y,type);
	      break;
			case CELL_ALL_BOMB :
				display_bomb(map, x, y, type);
				break ;
			case EXPLO_BOMB :
				window_display_image(sprite_get_banner_explosion(), x, y);
	    break ;

	    }
	  }
	}
}

/*struct map* map_get_static(void)
{
	struct map* map = map_new(STATIC_MAP_WIDTH, STATIC_MAP_HEIGHT);

	unsigned char themap[STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT] = {
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_KEY, CELL_BONUS_LIFE , CELL_EMPTY, CELL_BONUS_NI, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
	  CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
<<<<<<< HEAD
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY, 
	  CELL_BOX, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,  
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
=======
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX_RANGEDEC, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_BOX, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_BOX_LIFE, CELL_BOX_RANGEDEC, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_BOX_LIFE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_BOX_RANGEDEC, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
>>>>>>> edee5173bb5b7afc2422c1b2124591ee40a068e2
	  CELL_EMPTY, CELL_TREE, CELL_BOX_RANGEDEC, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_TREE, CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_BOX_RANGEINC, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_BOX, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,  CELL_BOX, CELL_EMPTY,
	  CELL_BOX,  CELL_EMPTY, CELL_CL_DOOR, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
		};

	for (int i = 0; i < STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT; i++)
		map->grid[i] = themap[i];

	return map;
	} */


struct map* map_decrypte(int lvl){
 
  int niveau= lvl;
  char fin[25];
  sprintf(fin, "src/map/map_%i", niveau);


  FILE* fic = fopen(fin,"r");
  int w,h,j;
  
  fscanf(fic,"%i:%i",&w,&h);

  struct map* map1 = map_new(w,h);
  
  
  for (int i=0; i<w*h; i++)
    {
      fscanf(fic,"%i_",&j);
      map1->grid[i]=j;
    }
  
    return map1;
}

