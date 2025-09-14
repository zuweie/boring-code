#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <float.h>
#include "policy.h"


int policy_load(const char* path, policy_t* policy)
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

int policy_display(policy_t* policy, int disp_all)
{
    setlocale(LC_ALL, "");
    action_t* first;

    for (int i=0; i<policy->rows; ++i) {
        for (int j=0; j<policy->cols; ++j) {
            first = policy->actions[i * policy->cols + j];
            if (disp_all) {
                while (first){
                    action_display(first);
                    first = first->next;
                }
            } else {
                action_t* max = action_get_max(first);
                action_display(max);
            }
            printf(" ");
        }
        printf("\n");
    }
    return 0;
}

action_t* action_get_max(action_t* first) 
{
    action_t* max_act = NULL;
    float     max_p   = -FLT_MAX;

    while (first) {

        if (first->probability > max_p) {
            max_p   = first->probability;
            max_act = first;
        }
        first = first->next;
    }
    return max_act;
}

/**
 * @brief 根据各个 action 之间的概率返回 move。
 * 
 * @param act_link 
 * @return move_t 
 */
move_t policy_take_action(action_t* act_link)
{
    float p         = (double) rand() / (double)RAND_MAX;
    float range     = 0.f;
    action_t* first = act_link;

    while(first) {

        range += first->probability;

        if (range - p >=0 ) 
            return first->move;

        first = first->next;
    }
    return e_idle;
}

int action_display(action_t* act)
{
    switch (act->move)
    {
    case e_go_up:
        /* code */
        wprintf(L"\u2191@%0.2f;", act->probability);
        break;
    case e_go_right:
        wprintf(L"\u2192@%0.2f;", act->probability);
        break;
    case e_go_down:
        wprintf(L"\u2193@%0.2f;", act->probability);
        break;
    case e_go_left:
        wprintf(L"\u2190@%0.2f;", act->probability);
        break;
    case e_stay:
        printf("o@%0.2f;", act->probability);
        break;
    default:
        printf("**");
        break;
    }
    return 0;
}