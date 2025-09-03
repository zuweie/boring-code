/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-08-22 09:45:30
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-08-31 13:37:06
 * @FilePath: /boring-code/src/reinforce_learning/grid_world.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __GRID_WORLD_H__
#define __GRID_WORLD_H__

#define ID2X(id, cols) ((id)/(cols))
#define ID2Y(id, cols) ((id)%(cols))
#define XY2ID(x, y, cols) ((x)*(clos)+(y))

typedef enum move move_t;

typedef enum cell_clazz {
    e_null = 0,
    e_normel,
    e_forbitten,
    e_target
} cell_clazz_t;

typedef struct cell{
    int id;
    cell_clazz_t cell_type;
} cell_t;

// typedef struct consequence {
//     float enter_reward;
//     int   stayin_id;
// } consequence_t;

typedef struct grid_world {
    int rows;
    int cols;
    cell_t* cells;
} grid_world_t;

int grid_world_load (const char* path, grid_world_t* grid);
int grid_world_save (const char* path, grid_world_t* grid);
int grid_world_display (grid_world_t* grid);
int grid_world_reset (grid_world_t* grid);
float cell_reward(cell_clazz_t cell_type);
//consequence_t grid_world_setp_to(grid_world_t* grid, int from, move_t move);

#endif