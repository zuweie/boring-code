/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-08-22 09:45:30
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-08-26 14:51:43
 * @FilePath: /boring-code/src/reinforce_learning/grid_world.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __GRID_WORLD_H__
#define __GRID_WORLD_H__

typedef enum cell_type {

    e_normel = 0,
    e_forbitten,
    e_target

} cell_type_t;

typedef struct cell{
    int x;
    int y;
    cell_type_t e_type;
} cell_t;

typedef struct grid_world {
    int rows;
    int cols;
    cell_t* cells;
} grid_world_t;

int grid_world_load (const char* path, grid_world_t* grid);
int grid_world_save (const char* path, grid_world_t* grid);
int grid_world_display (grid_world_t* grid);
int grid_world_reset (grid_world_t* grid);

#endif