#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/sng_ptr.h"


int main(int argc, char *argv[])
{
    char *line;
    int choice, member;
    ssize_t input_length;
    size_t length = 0;
    long value;
    int errno;
    Tree *tree = malloc(sizeof *tree);
    line = NULL; 
    printf("\n -*- Binary Search Tree -*- \n");
    
    while (true) {
    
        printf("\nPick any of the following choices: [1] insert - ");
        printf("[2] delete - [3] search - [4] print tree - [5] exit\n");
        printf("Enter choice: ");

        input_length = getline(&line, &length, stdin);
        if (input_length != 2) 
            printf("Error: invalid choice selection.\n");
        else {
            choice = line[0] - '0';
            if (choice < 1 || choice > 5) {
                printf("Error: invalid choice selection.\n");
            } else {
                errno = 0;
                switch (choice) {
                
                    case (1):
                        printf("Enter value: ");
                        input_length = getline(&line, &length, stdin);
                        value = strtol(line, NULL, 10);
                        if (errno != 0) 
                            printf("Error: %s\n", strerror(errno));
                        else {
                            if (!(tree->root)) {
                                tree->root = create_leaf();
                                tree->root->value = value;
                                tree->root->parent = NULL;
                            } else 
                                insert(tree->root, value);
                        }
                        break;

                    case (2):
                        printf("Enter value: ");
                        input_length = getline(&line, &length, stdin);
                        value = strtol(line, NULL, 10);
                        if (errno != 0) 
                            printf("Error: %s\n", strerror(errno));
                        else 
                            remove_leaf(tree, value);
                        break;

                    case (3):
                        printf("Enter value: ");
                        input_length = getline(&line, &length, stdin);
                        value = strtol(line, NULL, 10);
                        if (errno != 0) 
                            printf("Error: %s\n", strerror(errno));
                        else {
                            member = search(tree->root, value);
                            if (member) 
                                printf("%ld is in the tree!\n", value);
                            else 
                                printf("%ld is NOT in the tree!\n", value);
                        }
                        break;

                    case (4):
                        dump_tree(tree->root);
                        break;

                    case (5):
                        free_tree(tree->root);
                        free(line);
                        return 0;
                }
            }
        }
    }
}
   
