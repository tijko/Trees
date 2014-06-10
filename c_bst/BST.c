#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lib/BST_actions.h"


int main(void) {

    char *line;
    int choice;
    ssize_t input_length;
    size_t length = 0;
    long value;
    int errno;
    struct leaf *tree = NULL;

    printf ("\n -*- Binary Search Tree -*- \n");

    while ( true ) {

        printf ("\nPick any of the following choices: [1] insert - [2] delete - [3] search - [4] print tree - [5] exit\n");
        printf ("Enter choice: ");

        input_length = getline(&line, &length, stdin);
        if (input_length != 2) {
            printf("Error: invalid choice selection.\n");
        } else {
            choice = line[0] - '0';
            if (choice < 1 || choice > 5) {
                printf("Error: invalid choice selection.\n");
            } else {
                errno = 0;
                switch (choice) {

                    case (1):

                        printf ("Enter value: ");
                        input_length = getline(&line, &length, stdin);
                        value = strtol(line, NULL, 10);
                        if (errno != 0) {
                            printf("Error: %s\n", strerror(errno));
                        } else { 
                            if (!(tree)) {
                                tree = create_leaf(); 
                                tree->value = value;
                            }    
                            else {
                                insert(&tree, value);
                            }
                        }
                        break;
                    
                    case (2):

                        printf ("Enter value: ");
                        input_length = getline(&line, &length, stdin);
                        value = strtol(line, NULL, 10);
                        if (errno != 0) {
                            printf("Error: %s\n", strerror(errno));
                        } else {
                            delete(&tree, value);
                        }
                        break;

                    case (3):

                        printf ("Enter value: ");
                        input_length = getline(&line, &length, stdin);
                        value = strtol(line, NULL, 10);
                        if (errno != 0) {
                            printf("Error: %s\n", strerror(errno));
                        } else {
                            search(tree, value);
                        }
                        break;

                    case (4):

                        dump_tree(tree); 

                        break;

                    case (5):
                        return 0;
                } 
            }                 
        }
    }
}
