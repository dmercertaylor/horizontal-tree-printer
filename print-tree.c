#include "print-tree.h"

int main(int argc, char **argv)
{
    srand((unsigned) time(NULL));
    int depth;
    if(argc < 2)
    {
        depth = (rand() % 5) + 1;
    }
    else if (argc == 2)
    {
        for(char *c = argv[1]; *c != '\0'; c++)
        {
            if((*c < '0' || *c > '9') && *c != '-')
            {
            fprintf(stderr, "Depth must be a number\n");
            return 1;
            }
        }
        depth = atoi(argv[1]);
        if(depth < 0)
        {
            depth *= -1;
        }
        if(depth == 0)
        {
            fprintf(stderr, "Depth must be at least 1\n");
            return 1;
        }
    }
    else
    {
        fprintf(stderr, "Usage: tree-printer [depth]\n");
        return 1;
    }
    Node *tree = generate_random_tree(depth, 6);
    char *str = draw_tree(tree);
    printf("\n\n%s\n\n", str);
    free(str);
    return 0;
}
/*
             .- g
       .- b -|
       |     '- f
    a -|
       |     .- d
       '- c -|
             '- e
*/

char *draw_tree(Node *root)
{
    int root_depth = node_depth(root);
    int *data_widths = calloc(sizeof(int), root_depth);
    node_data_widths(root, 0, data_widths);
    int screen_height = node_endpoints(root) * 2 - 1;
    int screen_width = data_widths[0];
    for(int i = 1; i < root_depth; i++)
    {
        screen_width += data_widths[i] + 5;
    }
    char **screen = create_screen(screen_width, screen_height);
    int start_row = max(0, node_endpoints_left(root) * 2 - 1);
    draw_node(root, screen, data_widths, 0, start_row);
    char *out = *screen;
    free(screen);
    return out;
}

int draw_node(Node *root, char **screen, int *data_widths, int col, int row)
{
    if(root == NULL) return row;
    int data_len = root->data ? strlen(root->data) : 0;

    if(!root->left && !root->right)
    {
        if(col > 0)
        {
            draw_on_screen(screen, row, col - 2, "- ");
        }
        draw_on_screen(screen, row, col, root->data);
        return row;
    }

    int data_end = col + *data_widths;
    int write_line = row;
    if(root->left && root->right)
    {
        int row_left = row - max(1, node_endpoints_right(root->left) * 2);
        int row_right = row + max(1, node_endpoints_left(root->right) * 2);
        int line_left = draw_node(root->left, screen, data_widths + 1, data_end + 5, row_left);
        int line_right = draw_node(root->right, screen, data_widths + 1, data_end + 5, row_right);
        for(int i = line_left + 1; i < line_right; i++)
        {
            screen[i][data_end + 2] = '|';
        }
        screen[line_left][data_end + 2] = '.';
        screen[line_right][data_end + 2] = '\'';
        write_line = (line_left + line_right) / 2;
        if(row > write_line && (line_left + line_right) % 2) ++write_line;
    }
    else
    {
        Node *next_node = root->left ? root->left : root->right;
        write_line = draw_node(next_node, screen, data_widths + 1, data_end + 5, row);
        int direction = root->left ? -1 : 1;
        screen[write_line][data_end + 2] = '-';
    }
    /* draw root->data and brackets */
    if(root->data && data_len > 0) draw_on_screen(screen, write_line, col, root->data);
    else screen[write_line][col] = '-';
    /* draw padding dashes if data is short */
    for(int i = col + data_len + 1; i < data_end + 2; i++)
    {
        screen[write_line][i] = '-';
    }
    if(col > 0)
    {
        if(root->data && data_len > 0)
        {
            draw_on_screen(screen, write_line, col - 2, "- ");
        }
        else
        {
            draw_on_screen(screen, write_line, col - 2, "--");
        }
    }
    return write_line;
}

char **create_screen(int width, int height)
{
    char *output = malloc((width + 1) * height);
    char **screen = malloc(sizeof(char *) * height);
    for(int i = 0; i < height; i++)
    {
        screen[i] = output + ((width + 1) * i);
        screen[i][width] = '\n';
        for(int j = 0; j < width; j++)
        {
            screen[i][j] = ' ';
        }
    }
    screen[height - 1][width] = '\0';
    return screen;
}

void draw_on_screen(char **screen, int row, int col, char *str)
{
    if(str == NULL) return;
    for(int i = 0; str[i] != '\0'; ++i)
    {
        screen[row][col + i] = str[i];
    }
}

int node_endpoints(Node *root)
{
    if(root == NULL) return 0;
    if(!root->left && !root->right) return 1;
    return node_endpoints(root->left) + node_endpoints(root->right);
}

int node_endpoints_left(Node *root)
{
    if(root->left && root->right)
    {
        return node_endpoints(root->left);
    }
    else if(root->left)
    {
        return node_endpoints_left(root->left);
    }
    else if(root->right)
    {
        return node_endpoints_left(root->right);
    }
    return 0;
}

int node_endpoints_right(Node *root)
{
    if(root->left && root->right)
    {
        return node_endpoints(root->right);
    }
    else if(root->left)
    {
        return node_endpoints_right(root->left);
    }
    else if(root->right)
    {
        return node_endpoints_right(root->right);
    }
    return 0;
}

void node_data_widths(Node *root, int index, int *widths)
{
    if(root == NULL) return;
    int data_len = root->data ? strlen(root->data) : 0;
    if(data_len > widths[index])
    {
        widths[index] = data_len;
    }
    node_data_widths(root->left, index + 1, widths);
    node_data_widths(root->right, index + 1, widths);
}

int node_depth(Node *root)
{
    if(root == NULL) return 0;
    return 1 + max(node_depth(root->left), node_depth(root->right));
}

int max(int a, int b)
{
    return a > b ? a : b;
}

Node *generate_node(char * data)
{
    Node *out = malloc(sizeof(Node));
    out->left = NULL;
    out->right = NULL;
    out->data = data;
    return out;
}

void generate_leaves(Node *root, char *left_data, char *right_data)
{
    root->left = generate_node(left_data);
    root->right = generate_node(right_data);
}

Node *generate_with_leaves(char *root_data, char *left_data, char *right_data)
{
    Node *root = generate_node(root_data);
    generate_leaves(root, left_data, right_data);
    return root;
}

Node *generate_random_tree(int depth, int str_len)
{
    if(depth == 0) return NULL;
    Node *out = generate_node(NULL);
    if(rand() % 6)
    {
        out->left = generate_random_tree(depth - 1, str_len);
    }
    if(rand() % 6)
    {
        out->right = generate_random_tree(depth - 1, str_len);
    }
    if(out->right || out->left)
    {
        out->data = generate_random_str(rand() % str_len);
    }
    else
    {
        out->data = generate_random_str((rand() % (str_len - 1)) + 2);
    }
    return out;
}

char *generate_random_str(int str_len)
{
    char *out = malloc(str_len + 1);
    out[str_len] = '\0';
    for(int i = 0; i < str_len; i++)
    {
        switch(rand() % 3)
        {
            case 0:
                out[i] = rand() % 10 + '0';
                break;
            case 1:
                out[i] = rand() % 26 + 'A';
                break;
            default:
                out[i] = rand() % 26 + 'a';
        }
    }
    return out;
}


void free_tree(Node *root)
{
    if(root == NULL) return;
    if(root->left) free_tree(root->left);
    if(root->right) free_tree(root->right);
    if(root->data) free(root->data);
    free(root);
}