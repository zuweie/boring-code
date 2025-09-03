#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#include "policy.h"


int policy_load(FILE* path, policy_t* policy)
{
    if (policy->rows == 0 || policy->cols == 0) return -1;

    policy->actions = (action_t**) malloc (policy->rows * policy->cols * sizeof (action_t*));
    memset(policy->actions, (void*)0, policy->rows*policy->cols*sizeof(action_t*));

    FILE* file = fopen(path, "r");
    int ret    = -1;
    if (file) {
        char* token1;
        char* lasts1;
        char* token2;
        char* lasts2;
        char* token3;
        char* lasts3;

        const int LINE_LENGTH = 128;
        char  line[LINE_LENGTH];
        int row = 0, col = 0;
        float probability;
        move_t move;
        while(fgets(line, sizeof(line), file) ) {

            col = 0;
            if (strlen(line) <=1) continue;

            token1 = strtok_r(line, " \t\n", &lasts1);

            while (token1) {

                token2 = strtok_r(token1, ";", &lasts2);

                while ( token2 ) {

                    token3 = strtok_r( token2, "@", &lasts3);
                    
                    if (token3) {
                        move = atoi(token3);
                    } else {
                        return -1;
                    }

                    token3 = strtok_r( NULL, "@", &lasts3);

                    if (token3) {
                        probability = atof(token3);
                    } else {
                        return -1;
                    }

                    action_t** link  = &policy->actions[row * policy->cols + col];
                    action_t* action = (action_t*) malloc (sizeof(action_t));

                    action->probability = probability;
                    action->move        = move;
                    // 把新的 action 挂到 link 上面去。
                    action->next = *link;
                    *link        = action;

                    // 
                    token2 = strtok_r(NULL, ";", &lasts2);
                }

                token1 = strtok_r(NULL, " \t\n", &lasts1);

                col ++;
            }
            row ++;
        }
        ret = 0;
    } else {
        printf(" file error %s \n", strerror(errno));
    }

    fclose(file);
    return ret;
}

int policy_reset(policy_t* policy)
{ 
    if (policy->actions) {

        for (int i=0; i<policy->rows; ++i) {
            for (int j=0; j<policy->cols; ++j) {

                action_t* first = policy->actions[i*policy->cols+j];
                action_t* del;
                while (first){   
                    
                    del = first;
                    first = first->next;
                    free(del);

                }
            }
        }

        free(policy->actions);
    }
    policy->rows = 0;
    policy->cols = 0;
    return 0;
}

int policy_display(policy_t* policy)
{
    setlocale(LC_ALL, "");

    for (int i=0; i<policy->rows; ++i) {
        for (int j=0; j<policy->cols; ++j) {

            action_t* first = policy->actions[i*policy->cols +j];
            while(first) {
                
                switch (first->move)
                {
                case e_go_up:
                    /* code */
                    wprintf(L"\u2191@%0.2f;", first->probability);
                    break;
                case e_go_right:
                    wprintf(L"\u2192@%0.2f;", first->probability);
                    break;
                case e_go_down:
                    wprintf(L"\u2193@%0.2f;", first->probability);
                    break;
                case e_go_left:
                    wprintf(L"\u2190@%0.2f;", first->probability);
                    break;
                case e_stay:
                    printf("o@%0.2f;", first->probability);
                    break;
                default:
                    printf("**");
                    break;
                }

                first = first->next;
            } 
            printf(" ");
        }
        printf("\n");
    }
    return 0;
}