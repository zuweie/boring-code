/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-08-22 09:45:39
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-08-24 17:11:32
 * @FilePath: /boring-code/src/reinforce_learning/grid_world.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "grid_world.h"

static int __read_grid_size(FILE* file, grid_world_t* grid) 
{
    const int LINE_LENGTH = 32;
    char line[LINE_LENGTH];

    fgets(line, sizeof(line), file);

    if (strlen(line) <=1 ) return -1;
    
    char* token;
    char* last;

    token = strtok_r(line, " \t\n", &last);

    if (token) {
        grid->rows = atoi(token);
    }

    token = strtok_r(NULL, " \t\n", &last);

    if (token) {
        grid->cols = atoi(token);
    }
    return grid->rows > 0 && grid->cols > 0;
}

static int __build_grid_world(FILE* file, grid_world_t* grid) 
{
    if (__read_grid_size(file, grid)) {

        grid->cells = (cell_t*) malloc (grid->rows * grid->cols * sizeof(cell_t));
        const int LINE_LENGTH = 256;
        char line[LINE_LENGTH];
        char* token;
        char* last;
        int row = 0;
        int col = 0;

        while(fgets(line, sizeof(line), file)) {

            if (strlen(line) <= 1) continue;

            token = strtok_r(line, " \t\n", &last);
            col   = 0;
            while(token) {
                // 获取第 i 行，第 j 列的 cell，将数据填入 cell 中。
                
                cell_t* cell = &grid->cells[row * grid->cols + col];

                cell->x = row;
                cell->y = col;
                cell->e_type = atoi(token);

                token = strtok_r(NULL, " \t\n", &last);

                col++;
            }
            row ++;
        }

        return 0;
    }
    
    return -1;

}

/**
 * @brief 从文件中读取 grid world 的数据。
 * 
 * @param path 
 * @param grid 
 * @return int 
 */
int grid_world_load(const char* path, grid_world_t* grid)
{
    // 将 grid 的对象清空。

    FILE* file = fopen(path, "r");
    int ret = -1;
    if (file) {
        ret = __build_grid_world(file, grid);
    } else {
        printf("file open error: %s \n",  strerror(errno));
    }
    fclose(file);
    return ret;
}

int grid_world_save(const char* path, grid_world_t* grid)
{
    int ret = -1;
    FILE* file = fopen(path, "a");
    if (file) {
        fprintf(file, "%d %d\n", grid->rows, grid->cols);
        int total = grid->rows * grid->cols;

        for(int i=0; i<grid->rows; ++i) {
            for (int j=0; j<grid->cols; ++j) {

                fprintf(file, "%d ", grid->cells[i * grid->cols + j].e_type);
            }
            fprintf(file, "\n");
        }

        
        ret = 0;
    } else {
        printf("file open error: %s \n", strerror(errno));
    }
    fclose(file);

    return ret;
}

int grid_world_display(grid_world_t* grid)
{
    for (int i=0; i<grid->rows; ++i) {
        for (int j=0; j<grid->cols; ++j) {
            cell_t* cell = &grid->cells[i * grid->cols +j];
            printf("%d ", cell->e_type);
        }
        printf("\n");
    }
    return 0;
}

int grid_world_reset(grid_world_t* grid)
{
    grid->rows = 0;
    grid->cols = 0;
    if (grid->cells){
        free(grid->cells);
        grid->cells = NULL;
    } 
    return 0;
}