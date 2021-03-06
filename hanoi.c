#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include "stack.h"

#define NUM_DISKS 5
#define NUM_TOWERS 3

// ============================================================================
// Tower-related functions
typedef stack_t tower_t;

// Creates a tower
tower_t* make_tower(void) {
    return make_stack();
}

// Frees the tower
void free_tower(tower_t *src) {
    free_stack(src);
}

// Checks whether there is a valid move between the source
// and destination tower
// Rule: Player can't place a bigger disk on top of a smaller one
bool has_valid_move(tower_t *src, tower_t *dest) {
    if (stack_size(src) == 0) {
        return false;
    } else if (stack_size(dest) == 0) {
        return true;
    } else {
        return stack_peek(src) < stack_peek(dest);
    }
}

// Moves a disk from the source to the destination tower
void move_disk(tower_t *src, tower_t *dest) {
    stack_push(dest, stack_pop(src));
}

// Checks wheter the tower is complete (i.e. same size as the original
// tower and disks are sorted in an ascending order)
bool is_complete_tower(tower_t *src, int num_total_disks) {
    return (stack_size(src) == num_total_disks) & is_stack_ascending_order(src);
}

// ============================================================================
// Hanoi-related functions
typedef struct {
    tower_t* towers[NUM_TOWERS];
    int moves;
} hanoi_t;

// Makes a Tower of Hanoi game
hanoi_t* make_hanoi(int num_total_disks) {
    hanoi_t* ret = (hanoi_t*) malloc(sizeof(hanoi_t));
    tower_t* t0 = make_tower();
    for (int i = num_total_disks; i > 0; i--) {
        stack_push(t0, i);
    }
    ret->towers[0] = t0;
    for (int j = 1; j < NUM_TOWERS; j++) {
        ret->towers[j] = make_tower();
    }
    ret->moves = 0;
    return ret;
}

// Frees a Tower of Hanoi game
void free_hanoi(hanoi_t *game) {
    for (int i = 0; i < NUM_TOWERS; i++) {
        free_tower(game->towers[i]);
    }
    free(game);
}

// Checks wheter the game is won (i.e. a tower is complete, not
// considering the original tower)
bool is_won_game(hanoi_t *game, int num_total_disks) {
    bool ret = 0;
    for (int i = 1; i < NUM_TOWERS; i++) {
      ret += is_complete_tower(game->towers[i], num_total_disks);
    }
    return ret;
}

// ============================================================================
// Rendering functions
// Prints a string a number of times
void print(char *str, int num) {
    for (int i = 0; i < num; i++) {
            printf(str);
    }   
}

// Prints a disk of the given size
void print_disk(int disk, int num_total_disks) {
    int num_spcs = num_total_disks - disk;
    print(" ", num_spcs);
    print("-", disk);
    printf("|");
    print("-", disk);
    print(" ", num_spcs);
}

// Prints the tower
void print_tower(tower_t *t, int num_total_disks) {
    int num_disks = stack_size(t);
    for (int i = 0; i < num_total_disks - num_disks; i++) {
        print(" ", num_total_disks);
        printf("|");
        print(" ", num_total_disks);
        printf("\n");
    }
    for (int j = 0; j < num_disks; j++) {
      print_disk(list_get(t, j), num_total_disks);
        printf("\n");
    }
    print("=", 2 * num_total_disks + 1);
}

// Prints the Tower of Hanoi game
void print_hanoi(hanoi_t *game, int num_total_disks) {
  printf("Move: %d\n", game->moves);
    for (int i = 0; i < NUM_TOWERS; i++) {
        printf("Tower %d:\n", i);
        print_tower(game->towers[i], num_total_disks);
        printf("\n");
    }
}

// ============================================================================
// Main function
int main(void) {
    int num_total_disks;
    bool winning_game = false;  
    int src, dest;
    src = dest = 0;

    printf("Welcome to Tower of Hanoi!\n"
        "To play, please enter the number of disks: ");
    scanf("%d", &num_total_disks);
    hanoi_t *game = make_hanoi(num_total_disks);
    printf("\nThe game starts now!\n\n");
    sleep(1);
    print_hanoi(game, num_total_disks);

    while (!winning_game) {
        printf("\nEnter a source tower: ");
        scanf("%d", &src);
        printf("Enter a destination tower: ");
        scanf("%d", &dest);
        printf("\n");

        if (src < 0 || src >= NUM_TOWERS) {
            printf("Invalid source tower index!\n\n");
            sleep(1);
        } else if (dest < 0 || dest >= NUM_TOWERS) {
            printf("Invalid destination tower index!\n\n");
            sleep(1);
        } else if (has_valid_move(game->towers[src], game->towers[dest])) {
            move_disk(game->towers[src], game->towers[dest]);
            game->moves += 1;
        } else {
            printf("Invalid move between source and destination!\n\n");
            sleep(1);
        }

        winning_game = is_won_game(game, num_total_disks);
        print_hanoi(game, num_total_disks);
    }

    printf("You won in %d steps!\n", game->moves);
    free_hanoi(game);

    return 0;
}
