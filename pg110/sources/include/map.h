/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef MAP_H_
#define MAP_H_

enum cell_type {

	CELL_EMPTY=0x00,   		//  0000 0000  0
	CELL_SCENERY=0x10, 		//  0001 0000  16
	CELL_BOX=0x20,   		//  0010 0000  32
	CELL_DOOR=0x30,                	//  0011 0000  48
	CELL_KEY=0x40,                	//  0100 0000  64 
	CELL_BONUS=0x50, 		//  0101 0000  80
	CELL_MONSTER=0x60, 		//  0110 0000  96
	CELL_ALL_BOMB=0x70, 	        //  0111 0000  112


	EXPLO_BOMB=0x80,
	

};
enum bomb_type {
    
  CELL_BOMB1=0x01,     //1
  CELL_BOMB2=0x02,     //2
  CELL_BOMB3=0x03 ,    //3
  CELL_BOMB4=0x04,     //4
	
};



enum bonus_type {
	BONUS_BOMB_RANGE_DEC=0x01,
	BONUS_BOMB_RANGE_INC= 0x02,
	BONUS_BOMB_NB_DEC= 0x03,
	BONUS_BOMB_NB_INC= 0x04,
	BONUS_MONSTER= 0x05,
	BONUS_LIFE= 0x06,
	
};


enum scenery_type {
	SCENERY_STONE =     0x01,    // 0001 
	SCENERY_TREE  =     0x02,    // 0010 
	SCENERY_PRINCESS =  0x04     // 0100 
};


enum door_type {
  OP_DOOR= 1,
  CL_DOOR= 0,
};

enum compose_type {



          

	CELL_BOMB11  = CELL_ALL_BOMB | CELL_BOMB1 ,
	CELL_BOMB22  = CELL_ALL_BOMB | CELL_BOMB2 ,
	CELL_BOMB33  = CELL_ALL_BOMB | CELL_BOMB3 ,
	CELL_BOMB44  = CELL_ALL_BOMB | CELL_BOMB4 ,


	CELL_TREE     = CELL_SCENERY | SCENERY_TREE,   /*17*/
	CELL_STONE    = CELL_SCENERY | SCENERY_STONE,    /*18*/
	CELL_PRINCESS = CELL_SCENERY | SCENERY_PRINCESS, /*20*/


	CELL_OP_DOOR = CELL_DOOR | OP_DOOR , /*49*/
        CELL_CL_DOOR = CELL_DOOR | CL_DOOR, /*48*/
	

	CELL_BONUS_RD      = CELL_BONUS | BONUS_BOMB_RANGE_DEC, //81
	CELL_BONUS_RI      = CELL_BONUS | BONUS_BOMB_RANGE_INC, //82
	CELL_BONUS_ND      = CELL_BONUS | BONUS_BOMB_NB_DEC,    //83
	CELL_BONUS_NI      = CELL_BONUS | BONUS_BOMB_NB_INC,    //84
        CELL_BONUS_MONSTER = CELL_BONUS | BONUS_MONSTER,        //85
	CELL_BONUS_LIFE    = CELL_BONUS | BONUS_LIFE,           //86 


    
	CELL_BOX_RANGEDEC = CELL_BOX | BONUS_BOMB_RANGE_DEC,    //33
	CELL_BOX_RANGEINC = CELL_BOX | BONUS_BOMB_RANGE_INC,    //34
	CELL_BOX_BOMBINC  = CELL_BOX | BONUS_BOMB_NB_INC,       //36
	CELL_BOX_BOMBDEC  = CELL_BOX | BONUS_BOMB_NB_DEC,       //35
	CELL_BOX_LIFE     = CELL_BOX | BONUS_LIFE,              //37
	CELL_BOX_MONSTER  = CELL_BOX | BONUS_MONSTER,           //36
};

struct map;

// Create a new empty map
struct map* map_new(int width, int height);
void map_free(struct map* map);


// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the type of a cell
enum cell_type map_get_cell_type(struct map* map, int x, int y);

// Return the compose type of a cell
enum compose_type map_get_cell_compose_type(struct map* map, int x, int y);

// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, enum cell_type type);


// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Return a default static map
struct map* map_get_static();
struct map* map_decrypte(int lvl);
// Display the map on the screen
void map_display(struct map* map);
void map_set_com_type(struct map* map, int x, int y, enum compose_type type);
void display_bomb(struct map* map, int x, int  y, unsigned char type) ;
void display_bonus(struct map* map, int x, int y, unsigned char type) ;

#endif /* MAP_H_ */
