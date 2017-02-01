// "if not defined" prevents defining this multiple times
#ifndef _object_h
#define _object_h

// enum is useful for making constants
typedef enum {NORTH, SOUTH, EAST, WEST} Direction;

// creating a type 'Object' with function pointers
// they don't point to anything yet
typedef struct {
	char *description;
	int (*init)(void *self);
	void (*describe)(void *self);
	void (*destroy)(void *self);
	void *(*move)(void *self, Direction direction);
	int (*attack)(void *self, int damage);
} Object;

// these are the functions that the 'Object' function pointers point to
int Object_init(void *self);
void Object_destroy(void *self);
void Object_describe(void *self);
void *Object_move(void *self, Direction direction);
int Object_attack(void *self, int damage);
// size_t is a fundamental unsigned integer type
// it represents the size of any object in bytes
void *Object_new(size_t size, Object proto, char *description);

// this makes a macro, which works like a template...
// it spits out the code on the right, injecting 'T' and 'N'
// a macro is a single instuction that expands automatically into
// a set of instructions
// syntax T##Proto means "concat Proto at the end of T"
// e.g. NEW(Room, "Hello.") makes "RoomProto"
#define NEW(T, N) Object_new(sizeof(T), T##Proto, N)
// this is "syntactic sugar" to avoid having to write "obj->proto.blah"
// instead allows you to write "obj->_(blah)"
#define _(N) proto.N

// end the if statement from #ifndef
#endif
