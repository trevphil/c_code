#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "solver.h"
#include "hashmap.h"
#include "queue.h"

static char char_colors[NUM_COLORS] = { 'R', 'B', 'O', 'G' };

char *initial, *solution;
map_t pred, moves_pred;
Queue *q, *all_states;

// ****** UTILITY FUNCTIONS ******
int dex(int row, int col) { return row * NUM_ROWS + col; }
bool in_bound(int v, int max) { return (v >= 0) && (v < max); }
bool isGoal(char *state) {
    long l = strlen(state);
    for (int i = 0; i < l; i++) {
        if (state[i] != EMPTY) return false;
    }
    return true;
}
char at(char *state, int r, int c) {
    if (in_bound(r, NUM_ROWS) && in_bound(c, NUM_COLS)) {
        return state[dex(r, c)];
    } else {
        return OOB;
    }
}
int count_spaces(char *state, int r, int c, int dr, int dc) {
    int k = 1;
    while (at(state, r + k * dr, c + k * dc) == EMPTY) { k++; }
    return k - 1;
}
void prettify(char *state) {
    if (strlen(state) < NUM_ROWS * NUM_COLS) return;
    
    for (int j = 0; j < KEY_LENGTH; j++) {
        if (j > 0 && j % NUM_COLS == 0) printf("\n");
        printf("%c", state[j]);
    }
    printf("\n");
}
bool is_solvable(char *state) {
    long len = strlen(state);
    int num_counts_of_three = 0;
    int num_counts_of_zero = 0;
    for (int i = 0; i < NUM_COLORS; i++) {
        char c = char_colors[i];
        int current_count = 0;
        for (int j = 0; j < len; j++) {
            if (state[j] == c) current_count++;
        }
        if (current_count == 1) {
            return false;
        } else if (current_count == 3) {
            num_counts_of_three++;
        } else if (current_count == 0) {
            num_counts_of_zero++;
        }
    }
    if (num_counts_of_three == 1 && num_counts_of_zero == (NUM_COLORS - 1)) {
        return false;
    }
    return true;
}

// ****** HELPER FUNCTIONS ******
void trace_back(char *cur) {
	int step = 0;
    char current[KEY_LENGTH];
    char *prev;
    int *move;
	snprintf(current, KEY_LENGTH, "%s", cur);
	
	char move_str[MOVE_LEN + 1] = "";
    while (hashmap_get(pred, current, (void**)(&prev)) == MAP_OK) {
        if (hashmap_get(moves_pred, current, (void**)(&move)) == MAP_OK) {
	        printf("%d\n", step++);
	        printf("(%d %d) -> (%d %d)\n", move[0], move[1], move[2], move[3]);
	        prettify(current);
			snprintf(current, KEY_LENGTH, "%s", prev);
		
			snprintf(move_str, MOVE_LEN + 1, "%d %d %d %d ", move[0], move[1], move[2], move[3]);
			if (step == 1) {
				solution = calloc(MOVE_LEN + 1, sizeof(char));
				strncpy(solution, move_str, MOVE_LEN);
			} else {
				solution = realloc(solution, sizeof(char) * (MOVE_LEN * step + 1));
				strncat(solution, move_str, MOVE_LEN);
			}
        }
    }
}

void propose(char *next, char *prev, int r1, int c1, int r2, int c2) {
    if (prev != NULL && strcmp(initial, next) == 0) {
		free(next); 
		return;
	}
    char value[KEY_LENGTH];
    int e = hashmap_get(pred, next, (void**)(&value));
    if (e == MAP_MISSING) {
        hashmap_put(pred, next, prev);
        add_last(all_states, next);
        if (is_solvable(next)) {
            int *move = malloc(sizeof(int) * 4);
            move[0] = r1; move[1] = c1; move[2] = r2; move[3] = c2;
            hashmap_put(moves_pred, next, move);
            add_last(q, next);
        }
    } else if (e == MAP_OK) {
        free(next);
    }
}

void removeLinks(char *new_state, char *before_slide, int r1, int c1, int r2, int c2) {
    bool linked = false;
    char v = at(new_state, r2, c2);
    if (at(new_state, r2 - 1, c2) == v) {
        new_state[dex(r2 - 1, c2)] = EMPTY;
        linked = true;
    }
    if (at(new_state, r2 + 1, c2) == v) {
        new_state[dex(r2 + 1, c2)] = EMPTY;
        linked = true;
    }
    if (at(new_state, r2, c2 - 1) == v) {
        new_state[dex(r2, c2 - 1)] = EMPTY;
        linked = true;
    }
    if (at(new_state, r2, c2 + 1) == v) {
        new_state[dex(r2, c2 + 1)] = EMPTY;
        linked = true;
    }
    if (linked) new_state[dex(r2, c2)] = EMPTY;
    propose(new_state, before_slide, r1, c1, r2, c2);
}

void slide(char *current, int r, int c, int dr, int dc) {
    if (dr == 0 && dc == 0) return;
    char temp = at(current, r, c);
    if (temp == EMPTY || temp == OOB) {
        printf("ERROR:\tcannot slide empty or void position\n");
        return;
    }
    
    char *new_state = malloc(sizeof(char) * KEY_LENGTH);
    memmove(new_state, current, sizeof(char) * KEY_LENGTH);
    new_state[dex(r, c)] = EMPTY;
    new_state[dex(r + dr, c + dc)] = temp;
    removeLinks(new_state, current, r, c, r + dr, c + dc);
}

void explore(char *current) {
    for (int r = 0; r < NUM_ROWS; r++) {
        for (int c = 0; c < NUM_COLS; c++) {
            if (at(current, r, c) == EMPTY) continue;
            int nU = count_spaces(current, r, c, -1, 0);
            int nD = count_spaces(current, r, c, +1, 0);
            int nL = count_spaces(current, r, c, 0, -1);
            int nR = count_spaces(current, r, c, 0, +1);
            slide(current, r, c, -nU, 0);
            slide(current, r, c, +nD, 0);
            slide(current, r, c, 0, -nL);
            slide(current, r, c, 0, +nR);
        }
    }
}

char * solve(char *pattern) {
    solution = "DNE";
    
    initial = strdup(pattern);
    pred = hashmap_new();
    moves_pred = hashmap_new();
    q = queue_new();
    all_states = queue_new();
    
    propose(initial, NULL, 0, 0, 0, 0);
    bool solved = false;
    while (q->size > 0) {
        Node *n = remove_first(q);
        char *current = n->state;
        node_free(n);
        if (isGoal(current) && !solved) {
            solved = true;
            trace_back(current);
            break;
        } else {
            explore(current);
        }
    }
    printf("%d explored\n", hashmap_length(pred));
    
    // free up memory
    int *move;
    while (all_states->size > 0) {
        Node *n = remove_first(all_states);
        hashmap_remove(pred, n->state);
		
        if (hashmap_get(moves_pred, n->state, (void**)(&move)) == MAP_OK) {
            hashmap_remove(moves_pred, n->state);
            free(move);
        }
		
        free(n->state);
        node_free(n);
    }

    hashmap_free(pred);
    hashmap_free(moves_pred);
    queue_free(all_states);
    queue_free(q);
    
    return solution;
}

void clean() {
	free(solution);
}

int main(int argc, char *argv[]) {
	// GOGB.BOGOGROROBR
	// ORBGBGRBGOGOBG.G
	// BGOBG.RGROGOGBRG
	printf("%s\n", solve(argv[1]));
	return 0;
}
