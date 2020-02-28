#ifndef PRINT_TREE_H
#define PRINT_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Node
{
    char *data;
    struct Node *left;
    struct Node *right;
} Node;

char *draw_tree(Node *root);
void draw_node(Node *root, char **screen, int *data_width, int col, int row);

/*
 * Generates a string of whitespaces with newlines at every width+1.
 *  Returns an array of char *'s pointing at the start of each new line.
 */
char **create_screen(int width, int height);
/* copies string to screen, but not null char */
void draw_on_screen(char **screen, int row, int col, char *str);

/* returns the number of endpoints a node has */
int node_endpoints(Node *root);
int node_endpoints_left(Node *root);
int node_endpoints_right(Node *root);
/* fills *widths with the max strlen of every data field at a given layer*/
void node_data_widths(Node *root, int index, int *widths);

/* returns number of nodes from root to the longest branch */
int node_depth(Node *root);

/* returns Node with NULL children */
Node *generate_node(char * data);

/* adds children to root, with data left_data and right_data */
void generate_leaves(Node *root, char *left_data, char *right_data);

/* generates Node with children */
Node *generate_with_leaves(char *root_data, char *left_data, char *right_data);

/* randomized things */
Node *generate_random_tree(int depth, int str_len);
char *generate_random_str(int str_len);

/* recursively free nodes. Also frees data */
void free_tree(Node *root);

/* random helpers */
int max(int a, int b);

#endif