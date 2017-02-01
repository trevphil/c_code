#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "ex19.h"

int Monster_attack(void *self, int damage) {
	assert(self != NULL);
	Monster *monster = self;
	
	printf("You attack %s!\n", monster->_(description));

	monster->hit_points -= damage;

	if (monster->hit_points > 0) {
		printf("It is still alive.\n");
		return 0;
	} else {
		printf("It is dead!\n");
		return 1;
	}
}

int Monster_init(void *self) {
	assert(self != NULL);
	Monster *monster = self;
	monster->hit_points = 10;
	return 1;
}

// changing default methods to custom methods
Object MonsterProto = {
	.init = Monster_init,
	.attack = Monster_attack
};

void *Room_move(void *self, Direction direction) {
	assert(self != NULL);
	Room *room = self;
	Room *next = NULL;

	if (direction == NORTH && room->north) {
		printf("You go north, into:\n");
		next = room->north;
	} else if (direction == SOUTH && room->south) {
		printf("You go south, into:\n");
		next = room->south;
	} else if (direction == EAST && room->east) {
		printf("You go east, into:\n");
		next = room->east;
	} else if (direction == WEST && room->west) {
		printf("You go west, into:\n");
		next = room->west;
	} else {
		printf("You can't go that direction.");
		next = NULL;
	}

	if (next) {
		next->_(describe)(next);
		// equivalent to next->proto.describe(next)
	}

	return next;
}

int Room_attack(void *self, int damage) {
	assert(self != NULL);
	Room *room = self;
	Monster *monster = room->bad_guy;

	if (monster) {
		monster->_(attack)(monster, damage);
		return 1;
	} else {
		printf("You flail in the air at nothing. Idiot.\n");
		return 0;
	}
}

Object RoomProto = {
	.move = Room_move,
	.attack = Room_attack
};

void *Map_move(void *self, Direction direction) {
	assert(self != NULL);
	Map *map = self;
	Room *location = map->location;
	Room *next = NULL;

	next = location->_(move)(location, direction);
	
	if (next) {
		map->location = next;
	}

	return next;
}

int Map_attack(void *self, int damage) {
	assert(self != NULL);
	Map *map = self;
	Room *location = map->location;
	
	return location->_(attack)(location, damage);
	// equivalent to location->proto.attack(location, damage)
}

int Map_init(void *self) {
	assert(self != NULL);
	Map *map = self;

	// make some rooms for a small map
	// equivalent to Object_new(sizeof(Room), RoomProto, "G.H.");
	// "RoomProto" argument is cast as type "Object"
	// the object returned will contain "Object"'s default methods
	// except for RoomProto.move and RoomProto.attack, which were custom
	// the return (Object *el) will point to the beginning of the memory
	// allocation for Room
	// the "hall" will still have enough space for its "Room" datatype:
	// (Object proto, Monster bad_guy, Room north, ... Room west)
	Room *hall = NEW(Room, "The great Hall");
	Room *throne = NEW(Room, "The throne room");
	Room *arena = NEW(Room, "The arena, with the minotaur");
	//Room *kitchen = NEW(Room, "Kitchen, you have the knife now");
	Room *kitchen = Object_new(sizeof(Room), RoomProto, "Kitchen... with knife!");
	Room *dungeon = NEW(Room, "The dungeon, with a minotaur");

	// put the bad guy in the arena
	arena->bad_guy = NEW(Monster, "The evil minotaur");
	// and in the dungeon
	dungeon->bad_guy = NEW(Monster, "The terrible minotaur");

	// setup the map rooms
	hall->north = throne;
	hall->south = dungeon;	

	dungeon->north = hall;

	throne->west = arena;
	throne->east = kitchen;
	throne->south = hall;

	arena->east = throne;
	kitchen->west = throne;

	// start the map and the character off in the hall
	map->start = hall;
	map->location = hall;

	return 1;
}

Object MapProto = {
	.init = Map_init,
	.move = Map_move,
	.attack = Map_attack
};

int process_input(Map *game) {
	assert(game != NULL);
	printf("\n> ");
	
	// getchar() returns the next char from the standard input
	char ch = getchar();
	if (ch == 10) {
		printf("Choose n, s, e, w, or l\n");
		return 0;
	}
	getchar(); // eat ENTER

	// rand() returns a number between 0 and RAND_MAX
	// v1 = rand() % 100 gives a range from 0 to 99
	// v2 = rand() % 100 + 1 gives range 1 to 100
	// v3 = rand() % 30 + 1985 gives range 1985 to 2014
	int damage = rand() % 4;

	switch(ch) {
		case -1:
			printf("Giving up? You suck.\n");
			return 0;
			break;
		case 'n':
			// equivalent to game->proto.move(game, NORTH)
			game->_(move)(game, NORTH);
			break;
		case 's':
			game->proto.move(game, SOUTH);
			break;
		case 'e':
			game->_(move)(game, EAST);
			break;
		case 'w':
			game->_(move)(game, WEST);
			break;
		case 'a':
			game->_(attack)(game, damage);
			break;
		case 'l':
			printf("You can go:\n");
			if (game->location->north) printf("NORTH\n");
			if (game->location->south) printf("SOUTH\n");
			if (game->location->east) printf("EAST\n");
			if (game->location->west) printf("WEST\n");
			break;
		default:
			printf("What? %d\n", ch);
	}
	return 1;
}

int main(int argc, char *argv[]) {
	// simple way to setup the randomness
	// srand "seeds" the RNG
	// time(NULL) returns the num seconds since Jan. 1970
	// passing it a pointer will set the pointer to the current time
	srand(time(NULL));

	// make our map to work with
	Map *game = NEW(Map, "The Hall of the Minotaur.");
	
	printf("You enter the ");
	game->location->_(describe)(game->location);
	// equal to game->location->proto.describe(game->location)
	// location->description was set in NEW()

	while(process_input(game)) {
	}
	
	return 0;
}
