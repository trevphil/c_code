#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// creating constants
#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
	int id;
	int set;
	// defining size for these arrays makes the Address struct a fixed size
	char name[MAX_DATA];
	char email[MAX_DATA];
};

struct Database {
	// the Database struct is also a fixed size
	struct Address rows[MAX_ROWS];
};

struct Connection {
	// "FILE" is a struct defined by the C standard library
	FILE *file;
	struct Database *db;
};

// forward declaration of Database_close()
void Database_close(struct Connection *conn);

// function to kill the program if there's anything wrong
void die(const char *message, struct Connection *conn) {
	// errno stores an int value of the error code
	if(errno) {
		// perror() will print "message": system error message
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}
	Database_close(conn);
	exit(1);
}

void Address_print(struct Address *addr) {
	printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn) {
	// fread(ptr, size, numItems, FILE stream)
	// reads numItems objects, each 'size' bytes long, from the
	// stream pointed to by stream, storing them at the location given by ptr
	// fread() returns the number of items read
	int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
	if (rc != 1) die("Failed to load database.", conn);
}

struct Connection *Database_open(const char *filename, char mode) {
	struct Connection *conn = malloc(sizeof(struct Connection));
	// if-statement is equivalent to if(conn == NULL)
	if (!conn) die("Memory error", conn);

	conn->db = malloc(sizeof(struct Database));
	// a NULL value of "if(conn->db)" will evaluate to false...
	// so "if(!conn->db)" will evaluate to true if db = NULL
	if (!conn->db) die("Memory error", conn);

	if (mode == 'c') {
		// fopen(filename, mode)
		// mode "w" creates a file for writing; stream is positioned at the start
		// fopen() returns a pointer to the file in question
		conn->file = fopen(filename, "w");
	} else {
		// mode "r+" opens a file for both reading and writing
		conn->file = fopen(filename, "r+");

		if (conn->file) {
			Database_load(conn);
		}
	}

	if (!conn->file) die("Failed to open the file", conn);

	return conn;
}

void Database_close(struct Connection *conn) {
	if (conn) {
		// fclose(FILE *stream)
		// fclose() dissociates the named stream from its underlying file
		// upon successful completion 0 is returned
		if (conn->file) fclose(conn->file);
		if (conn->db) free(conn->db);
		free(conn);
	}
}

void Database_write(struct Connection *conn) {
	// rewind(FILE *stream)
	// rewind() sets the file position indicator for the stream pointed
	// to by *stream to the beginning of the file; does not return a value
	rewind(conn->file);
	// fwrite(ptr, size, numItems, FILE stream)
	// writes 'numItems' objects (each 'size' bytes) to 'stream'
	// the objects to write are obtained from the location of 'ptr'
	// returns the number of objects written
	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if (rc != 1) die("Failed to write database.", conn);
	
	// fflush(FILE *stream)
	// forces a write of all buffered data for the given output
	// via the stream's underlying write() function
	// return 0 after successful completion
	rc = fflush(conn->file);
	if (rc == -1) die("Cannot flush database.", conn);
}

void Database_create(struct Connection *conn) {
	int i = 0;
	for (i = 0; i < MAX_ROWS; i++) {
		// make a prototype to initialize it
		struct Address addr = {.id = i, .set = 0};
		// then just assign it
		conn->db->rows[i] = addr;
	}
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email) {
	// get the i element of rows, which is in db, which is in conn...
	// then get the address of it (with &)
	struct Address *addr = &conn->db->rows[id];
	if (addr->set) die("Already set, delete it first.", conn);
	
	addr->set = 1;
	// strncpy(char destination, char src, size n)
	// copies at most 'n' characters from src to destination
	// if src is less than 'n' chars, the rest of destination becomes '\0'
	// if src is more than 'n' chars, the destination string IS NOT terminated with '\0'
	// returns 'destination'
	char *res = strncpy(addr->name, name, MAX_DATA);
	res[MAX_DATA - 1] = '\0';
	if (!res) die("Name copy failed", conn);
	
	res = strncpy(addr->email, email, MAX_DATA);
	res[MAX_DATA - 1] = '\0';
	if (!res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id) {
	struct Address *addr = &conn->db->rows[id];
	
	if (addr->set) {
		Address_print(addr);
	} else {
		die("ID is not set", conn);
	}
}

void Database_delete(struct Connection *conn, int id) {
	struct Address addr = {.id = id, .set = 0};
	conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn) {
	int i = 0;
	struct Database *db = conn->db;
	for (i = 0; i < MAX_ROWS; i++) {
		struct Address *cur = &db->rows[i];

		if (cur->set) {
			Address_print(cur);
		}
	}
}

void Database_find(struct Connection *conn, const char *keyword) {
	int i = 0;
	int found = 0;
	struct Database *db = conn->db;
	for (i = 0; i < MAX_ROWS; i++) {
		struct Address *cur = &db->rows[i];

		// strcmp(char *s1, char *s2) returns 0 if strings match
		// returns >0 if s1 is greater than s2
		// returns <0 if s1 is less than s2
		if (cur->set && (strcmp(cur->name, keyword) == 0 || strcmp(cur->email, keyword) == 0)) {
			Address_print(cur);
			found = 1;
		}
	}
	if (!found) die("Name or email not found", conn);
}

int main(int argc, char *argv[]) {
	if (argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);
	
	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);
	int id = 0;

	// atoi(char *str) converts the string to an int
	if (argc > 3) id = atoi(argv[3]);
	if (id >= MAX_ROWS) die("There's not that many records.", conn);
	
	switch(action) {
		case 'c':
			Database_create(conn);
			Database_write(conn);
			break;
		case 'f':
            		if (argc < 4) die("Need name or email to find", conn);
            
			Database_find(conn, argv[3]);
			break;
		case 'g':
			if (argc != 4) die("Need a id to get", conn);
			
			Database_get(conn, id);
			break;
		case 's':
			if (argc != 6) die("Need id, name, email to set", conn);
			
			Database_set(conn, id, argv[4], argv[5]);
			Database_write(conn);
			break;
		case 'd':
			if (argc != 4) die("Need id to delete", conn);
			
			Database_delete(conn, id);
			Database_write(conn);
			break;
		case 'l':
			Database_list(conn);
			break;
		default:
			die("Invalid action, only: c=create, f=find, g=get, s=set, d=del, l=list", conn);
	}

	Database_close(conn);
	
	return 0;
}
