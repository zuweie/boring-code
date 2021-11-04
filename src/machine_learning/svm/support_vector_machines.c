/*
 * @Author: your name
 * @Date: 2021-05-10 13:15:21
 * @LastEditTime: 2021-11-04 11:35:50
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm.c
 */
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "container/cn.h"
#include "container/entity.h"
#include "ultra_array/ultra_router.h"
#include "ultra_array/ultra_array.h"
#include "support_vector_machines.h"
#include "svm_problem.h"

// svc 的 svm 支持向量机的实现
// X 为数据
// Y 为标志量
// M 为输出的 model

// 数据预处理：
// 统计一下那个 _Y 中 class 的类别，以及各种类别所占有的比例。
// 只用于 c_svc 与 nu_svc
int svm_classify_problem(u_array_t* _X, u_array_t* _Y, CN svm_problems)
{
    
    size_t len_Y    = UA_length(_Y);
    vfloat_t* Y_ptr = UA_data_ptr(_Y);
    
    size_t len_Xc             = UA_shape_axis(_X, 1);
    vfloat_t (*X_ptr)[len_Xc] = UA_data_ptr(_X);

    //List counting_list = _LeList(Entity_is_key_equal);
    CN counting_map = CN_create(HASH_MAP, vf_t, int_t);
    
    for (size_t i=0; i<len_Y; ++i) {
        vfloat_t y = Y_ptr[i];

        // It it = LeCN_find(&counting_list, f2t(y));
        // List* class_list = NULL;
        if (!CN_has(counting_map, y)) {
            // 没找到这个 float 
            // class_list = malloc(sizeof(List));
            // *class_list = _List(NULL);
            // CN_add(class_list, i2t(i));
            // LeCN_add2(&counting_list, f2t(y), p2t(class_list));
            CN class_list = CN_create(LIST, int_t);
            CN_add(class_list, i);
            CN_set(counting_map, y, class_list);
        } else {
            // 找到了这个 float
            // Entity* ent = LeCN_get_entity(&counting_list, it);
            // List* class_list = t2p(ent->tv[1]);
            // CN_add(class_list, i2t(i));
            CN class_list = CN_get(counting_map, y);
            CN_add(class_list, i);
        }
    }

    // 组成配对。
    int class_nr = CN_size(counting_map);
    if (class_nr > 2) {
        // 三个以上的class
        for(It firstA=CN_first(counting_map); !It_equal(firstA, CN_last(counting_map)); It_next(firstA)) {

            It firstB = firstA;
            It_next(firstB);
            
            for (;!It_equal(firstB, CN_tail(counting_map)); It_next(firstB)) {
                
                entity_t* entity_A = It_ptr(firstA);
                entity_t* entity_B = It_ptr(firstB);

                svm_classify_problem_t* problem = malloc(sizeof(svm_classify_problem_t));
                
                problem->tagA = ef_vft(entity_A, 0);//t2f(entity_A->tv[0]);
                problem->class_ls_A = ef_int(entity_A, 1);//t2p(entity_A->tv[1]);
                
                // weight 要使用外来输入，这里暂时用1就 ok 了
                //problem->c_weight_A = (double)CN_size(problem->class_ls_A) / (double)len_Y;
                problem->c_weight_A = 1.f;
                
                problem->tagB = ef_vft(entity_B, 0);//t2f(entity_B->tv[0]);
                problem->class_ls_B = ef_int(entity_B, 1);//t2p(entity_B->tv[1]);
                problem->c_weight_B = 1.f;

                //problem->c_weight_B = (double)CN_size(problem->class_ls_B) / (double)len_Y;

                CN_add(svm_problems, problem);
            }
        }

    } else if ( class_nr == 2 ) {
        // 两个 class
        entity_t* entity_A = It_ptr(CN_first(counting_map));
        entity_t* entity_B = It_ptr(CN_last(counting_map));


        svm_classify_problem_t* problem = malloc(sizeof(svm_classify_problem_t));

        problem->tagA = ef_vft(entity_A, 0);//t2f(entity_A->tv[0]);
        problem->class_ls_A = ef_int(entity_A, 1);//t2p(entity_A->tv[1]);
        problem->c_weight_A = 1.f;

        problem->tagB = ef_vft(entity_B, 0);//t2f(entity_B->tv[0]);
        problem->class_ls_B = ef_int(entity_B, 1);//t2p(entity_B->tv[1]);
        problem->c_weight_B = 1.f;

        CN_add(svm_problems, problem);

    } 
    CN_finalize(counting_map, NULL);
    return class_nr;
}

int svm_classify_problem_finalize(CN problems)
{
    int problems_nr = CN_size(problems);
    int i = 0;
    int j = 2;
    // 这里有个问题，若果是两个怎么办，
    // 
    for (It last=CN_last(problems); !It_equal(last, CN_head(problems)); It_prev(last)) {

        svm_classify_problem_t* problem = It_ptr(last);
        int c_nr = j * (j -1) / 2;
        if (i == c_nr-1) {

            if (i == 0) {
                CN_finalize(problem->class_ls_B, NULL);
            }
            CN_finalize(problem->class_ls_A, NULL);
            j ++;
        }
        free(problem);
        i++;
    }
    return 0;
}


/**
 * 
 */
int svm_solve_generic(solver_t* solver)
{
    // 准备一堆变量
    size_t len_alpha = UA_length(&solver->alpha);
    size_t len_Qc    = UA_shape_axis(&solver->Q, 1);
    
        // 一堆变脸个
    vfloat_t C_i, C_j;
    vfloat_t old_alpha_i, old_alpha_j, alpha_i, alpha_j;
    vfloat_t delta_alpha_i, delta_alpha_j;

    vfloat_t* alpha_ptr = UA_data_ptr(&solver->alpha);
    vfloat_t* Y_ptr     = UA_data_ptr(solver->Y);
    vfloat_t* C_ptr     = UA_data_ptr(solver->C);

    vfloat_t (*Qc_ptr)[len_Qc] = UA_data_ptr(&solver->Q);
    
    //TODO: 1 这里计算 deta f(Bate) = Q dot Beta + P
    vfloat_t* G_ptr = UA_data_ptr(&solver->G);
    // 先把 Q 复制给 G
    UA_load(&solver->G, UA_data_ptr(&solver->Q));

    // G dot alpha
    UA_dot(&solver->G, &solver->alpha);

    // G sum P
    UA_sum_uar(&solver->G, &solver->P);
    
    // Debug
    //printf("\n G is \n");
    //UA_display(&solver->G);
    
    int selected_i, selected_j;
    int iter = 0;
    
    for (;;) {

        //TODO: 2 通过计算获取两个需要优化的 Bate，找不到或者循环次数大于最大的循环次数，则退出循环。
        if (solver->select_working_set(solver, &selected_i, &selected_j) != 0 || iter++ > solver->max_iter ) 
        break;
        
        printf(" select i: %d, j: %d, iter: %d \n", selected_i, selected_j, iter);

        //TODO: 3 更新这两个 Bate。
        vfloat_t* Qi_ptr = Qc_ptr[selected_i];
        vfloat_t* Qj_ptr = Qc_ptr[selected_j];
        

        C_i = C_ptr[selected_i];
        C_j = C_ptr[selected_j];

        alpha_i = old_alpha_i = alpha_ptr[selected_i];
        alpha_j = old_alpha_j = alpha_ptr[selected_j];
        
        
        if (Y_ptr[selected_i] != Y_ptr[selected_j]) { // 当 zi <> zj 

            double denom = Qi_ptr[selected_i] + Qj_ptr[selected_j] + 2 * Qi_ptr[selected_j];
            double delta = (-G_ptr[selected_i] - G_ptr[selected_j]) / SVM_MAX(fabs(denom), FLT_EPSILON);
            double diff = alpha_i - alpha_j;
            // 更新 alpha 值
            alpha_i += delta;
            alpha_j += delta;

            // 把 alpha_i 与 alpha_j 的值做限制。
            if ( diff > 0 && alpha_j < 0 ) { // 区域 III

                alpha_j = 0;
                alpha_i = diff;

            } else if (diff <= 0 && alpha_i < 0 ) { // 区域 IV

                alpha_i = 0;
                alpha_j = -diff;

            } 
            
            if (diff > C_i - C_j && alpha_i > C_i) { // 区域 I

                alpha_i = C_i;
                alpha_j = C_i - diff;

            }else if ( diff <= C_i - C_j && alpha_j > C_j){ // 区域 II s
                alpha_i = C_j;
                alpha_j = C_j + diff;
            }

        } else {
            // 当 zi == zj

            double denom = Qi_ptr[selected_i] + Qj_ptr[selected_j] - 2*Qi_ptr[selected_j];
            double delta = (G_ptr[selected_i] - G_ptr[selected_j]) / SVM_MAX(fabs(denom), FLT_EPSILON);
            double sum = alpha_i + alpha_j;

            alpha_i -= delta;
            alpha_j += delta;

            if (sum > C_i && alpha_i > C_i) { // 区域 I
                alpha_i = C_i;
                alpha_j = sum - C_i;
            } else if (sum <= C_i && alpha_j < 0 ) { // 区域 II
                alpha_j = 0;
                alpha_i = sum;
            }
            
            if (sum > C_j && alpha_j > C_j) { // 区域 III
                alpha_j = C_j;
                alpha_i = sum - C_j;
            } else if ( sum <= C_j && alpha_j < 0) { // 区域 IV
                alpha_i = 0;
                alpha_j = sum;
            }
        }
        //TODO: 4 把这两个 Bate 限定在可用范围内。

        alpha_ptr[selected_i] = alpha_i;
        alpha_ptr[selected_j] = alpha_j;
        
        // TODO : update alpha status ? 为什么要做 ？ 直接从 alpha 的值中得到不就完了么。

        // 更新 G 
        delta_alpha_i = alpha_i - old_alpha_i;
        delta_alpha_j = alpha_j - old_alpha_j;

        // Debug
        printf("\n update G:\n");
        for (size_t k=0; k<len_alpha; ++k) {
            printf("Qi_%d_ptr[%d]: %f, delta_alpha_i: %f, Qj_%d_ptr[%d]: %f, delta_alpha_j: %f ",selected_i, k, Qi_ptr[k], delta_alpha_i, selected_j, k,  Qj_ptr[k], delta_alpha_j);
            G_ptr[k] += Qi_ptr[k] * delta_alpha_i + Qj_ptr[k] * delta_alpha_j;
            // 
            printf("   G[%d]: %lf \n ", k, G_ptr[k]);
        }
        // Debug
        printf("\n\n\n");
    }

    // 计算 b。将来用作 预测函数上
    solver->calc_rho(solver, &solver->rho, &solver->r);
    return 0;
    
}

// 开始计算分类 svm 中最简单的分类 C_SVC
int svm_solve_c_svc( 
        u_array_t* X, u_array_t* Y, 
        SVM_kernel svm_kernel, 
        double C, double _gammer, 
        double _coef, double _degree, 
        double eps, 
        int max_iter,
        CN classify_models)
{

    size_t len_Xc = UA_shape_axis(X, 1);
    size_t len_Xr = UA_shape_axis(X, 0);
    size_t len_Y  = UA_length(Y);
    
    vfloat_t (*X_ptr)[len_Xc] = UA_data_ptr(X);
    vfloat_t *Y_ptr           = UA_data_ptr(Y);

    // 这个用于临时罐装数据。
    u_array_t _X = _UArray2d(len_Xr/2, len_Xc);
    u_array_t _Y = _UArray1d(len_Y/2);
    u_array_t _C = _UArray1d(len_Y/2);
    //u_array_t _C = _UArray1d(len_Y/2);
    solver_t solver;

    // 归类各种类比嗯
    CN problems = CN_create(LIST, ptr_t);
    svm_classify_problem(X, Y, problems);
    
    // 初始化 solver
    solver_initialize(&solver, C_SVC, svm_kernel, _gammer, _coef, _degree, eps, max_iter);

    size_t problem_size = CN_size(&problems);

    for ( It first = CN_first(&problems); !It_equal(first, CN_tail(&problems)); It_next(first) ) {

        svm_classify_problem_t* problem = It_ptr(first);
        
        size_t len_class_A = CN_size(problem->class_ls_A);
        size_t len_class_B = CN_size(problem->class_ls_B);

        size_t total = len_class_A + len_class_B;
        
        // 调整 _X, _Y, _C 大小，准备装载数据。
        UA_reshape_dots(&_X, 2, total, len_Xc);
        UA_reshape_dots(&_Y, 1, total);
        UA_reshape_dots(&_C, 1, total);
        
        vfloat_t (*_X_ptr)[len_Xc] = UA_data_ptr(&_X);
        vfloat_t *_Y_ptr           = UA_data_ptr(&_Y);
        vfloat_t *_C_ptr           = UA_data_ptr(&_C);
        
        // 配置 X Y C
        // 把数据罐装到 _X 与 _Y _C 中去。
        // tabA 为 1, tabB 为 -1
        int i=0, j=0, k=0;


        for (It it_a=CN_first(problem->class_ls_A); !It_equal(it_a, CN_tail(problem->class_ls_A)); It_next(it_a)) {
            size_t index_a = It_int(it_a);
            memcpy(_X_ptr[i++], X_ptr[index_a], sizeof(vfloat_t) * len_Xc);
            _Y_ptr[j++] = 1.f;
            //_C_ptr[k++] = problem->c_weight_A * C;
            _C_ptr[k++] = C;
        }
        
        for (It it_b=CN_first(problem->class_ls_B); !It_equal(it_b, CN_tail(problem->class_ls_B)); It_next(it_b)) {
            size_t index_b = It_int(it_b);
            memcpy(_X_ptr[i++], X_ptr[index_b], sizeof(vfloat_t) * len_Xc);
            _Y_ptr[j++] =  -1.f;
            //_C_ptr[k++] = problem->c_weight_B * C;
            _C_ptr[k++] = C;
        }

        // 设置 X Y C 到 solver 中
        solver_set_calculating_dataset(&solver, &_X, &_Y, &_C);

        //2 初始化 solver 的 alpha, P, Q;
        UA_ones(&solver.alpha, 0);
        UA_ones(&solver.P, -1);
        
        solver.build_Q(&solver);
        // // Debug :
        // printf("\n tagA is %lf, %c, tagB is %lf, %c", problem->tagA, (int)problem->tagA, problem->tagB, (int)problem->tagB);
        //  // Debug:
        // printf("\n X data : \n");
        // UA_display(&_X);
        // // Debug
        // printf("\n Q aixs0: %d, axis1: %d", UA_shape_axis(&solver.Q, 0), UA_shape_axis(&solver.Q, 1));
        // printf("\n Q : \n");
        // UA_display(&solver.Q);
        // printf("\n end Q \n");
        // end debug
        
        // 计算拉格朗日因子：
        svm_solve_generic(&solver);

        /* TODO： 计算完毕，将把有用的信息 solver 中，有用的信息 copy 到 svm model 中去。 ********/
        svm_model_t* model = malloc(sizeof(svm_model_t));
        int sv_count = 0;
        
        vfloat_t* solver_alpha_ptr = UA_data_ptr(&solver.alpha);
        vfloat_t* solver_Y_ptr     = UA_data_ptr(solver.Y);
        size_t len_solver_Xc       = UA_shape_axis(solver.X, 1);
        vfloat_t (*solver_X_ptr)[len_solver_Xc] = UA_data_ptr(solver.X);


        size_t    len_solver_alpha = UA_length(&solver.alpha);

        for (int i=0; i<len_solver_alpha; ++i) {
            // Debug;
            if (solver_alpha_ptr[i] > 0.f) {
                sv_count++;
            }
        }


        model->_star_alpha       = _UArray1d(sv_count);
        vfloat_t* star_alpha_ptr = UA_data_ptr(&model->_star_alpha);

        model->_star_Y        = _UArray1d(sv_count);
        vfloat_t* star_Y_ptr  = UA_data_ptr(&model->_star_Y);

        model->_star_X                 = _UArray2d(sv_count, len_Xc);
        vfloat_t (*star_X_ptr)[len_Xc] = UA_data_ptr(&model->_star_X);

        for (int i=0, j=0; i<len_solver_alpha; ++i) {

            // Debug
            if (solver_alpha_ptr[i] > 0.f) {
            
                star_alpha_ptr[j] = solver_alpha_ptr[i];
                star_Y_ptr[j]     = solver_Y_ptr[i];
                memcpy(star_X_ptr[j], solver_X_ptr[i], sizeof(vfloat_t)*len_solver_Xc);
                j++;
            }
        }
    
        model->_star_rho = solver.rho;
        model->_star_r   = solver.r;
        model->sv_count = sv_count;
        model->k_param = solver.kernel_param;
        
        model->tagA = problem->tagA;
        model->tagB = problem->tagB;
        model->kernel = svm_kernel;
        model->type   = C_SVC;

        switch (svm_kernel)
        {
        case LINEAR:
            /* code */
            model->calculate_kernel = &svm_model_calculate_liner;
            break;
        case POLY:
            model->calculate_kernel = &svm_model_calculate_poly;
            break;
        case SIGMOID:
            model->calculate_kernel = &svm_model_calculate_sigmoid;
            break;
        case RBF:
            model->calculate_kernel = &svm_model_calculate_rbf;
            break;
        default:
            break;
        }

        /* 完成复制信息到计算 model中 */ 

        // 获取劳动果实
        CN_add(classify_models, model);
    }

    // 完了就释放内存。
    // TODO : 释放 problems
    svm_classify_problem_finalize(&problems);

    // TODO : 这里释放 solver 的内容
    solver_finalize(&solver);

    // TODO : 这里释放 _X 与 _Y 的内存。
    UArray_(&_X);
    UArray_(&_Y);
}


// 需要劳动成果来做测试的时候到了。
// 实现 svc 投票判断。
double svm_c_svc_predict(CN classify_models, u_array_t* sample)
{
    // 1 确定类别的数量
    // 2 为每个类别生成一个投票站
    // 3 开始使用判断函数进行判断投票。

    CN vote = CN_create(HASH_MAP, db_t, int_t); //_LeList(Entity_is_key_equal);
    int i = 0;
    int j = 2;


    for (It last=CN_last(classify_models); !It_equal(last, CN_head(classify_models)); It_prev(last)) {

        svm_model_t * model = It_ptr(last);
        
        int c_nr = j * (j -1) / 2;
        if (i == c_nr-1) {
            if (i == 0) {
                //LeCN_add2(&vote, f2t(model->tagB), i2t(0));
                CN_set(vote, model->tagB, 0);
            }
            //LeCN_add2(&vote, f2t(model->tagA), i2t(0));
            CN_set(vote, model->tagA, 0);
            j ++;
        }
        i++;
    }
    // // do the prediction 
    // Debug: 
    // printf("\n");
    for (It first=CN_first(classify_models); !It_equal(first, CN_tail(classify_models)); It_next(first)){
        svm_model_t* model = It_ptr(first);
        // Debug:
        // printf(" model->tagA is %f, model->tagB is %f \n", model->tagA, model->tagB);
        double tag = svm_c_svm_predict_one(model, sample);
        // Debug:
        // printf(" calculate tag is %f \n", tag);
        // It it = LeCN_find(&vote, f2t(tag));
        int ticket = CN_get(vote, tag);

        //Entity* entity = LeCN_get_entity(&vote, it);
        // 增加一票
        //int v_count = t2i(entity->tv[1]) + 1;
        //entity->tv[1] = i2t(v_count);
        CN_set(vote, tag, ticket+1);
    }

    // // 选出票数最多的那个 tag 并返回。
    // Debug:
    // printf("\n");
    double winner_tag = -999.f;
    int count_vote = -1;
    for (It first=CN_first(&vote); !It_equal(first, CN_tail(&vote)); It_next(first)) {
        //Entity* entity = LeCN_get_entity(&vote, first);
        entity_t* ent = It_ptr(first);

        // Debug:
        // printf(" entity tv0: %f, tag: %c, tv1: %d \n", t2f(entity->tv[0]), (char)t2f(entity->tv[0]), t2i(entity->tv[1]));
        int ticket = ef_int(ent, 1);//t2i(entity->tv[1]);
        if (count_vote < ticket) {
            //winner_tag = t2f(entity->tv[0]);
            winner_tag = ef_double(ent, 0);
        }
    }
    
    //LeList_(&vote);
    CN_finalize(vote, NULL);
    return winner_tag;
}

double svm_c_svm_predict_one(svm_model_t* model, u_array_t* sample)
{
    double sum = model->_star_rho;
    // 计算核函数
    u_array_t kernel_X = model->calculate_kernel(model, sample);

    int len_start_alpha = UA_length(&model->_star_alpha);
    int len_start_Y     = UA_length(&model->_star_Y);
    int len_kernel_X    = UA_length(&kernel_X);
    
    vfloat_t* _start_alpha_ptr = UA_data_ptr(&model->_star_alpha);
    vfloat_t* _start_Y_ptr     = UA_data_ptr(&model->_star_X);
    vfloat_t* kernel_X_ptr     = UA_data_ptr(&kernel_X);
    
    for (int i=0; i<len_start_alpha; ++i) {
        
        sum += _start_alpha_ptr[i] * _start_Y_ptr[i] * kernel_X_ptr[i];

    }
    double tag = sum > 0 ? model->tagA : model->tagB;
    UArray_(&kernel_X);
    return tag;
}

int svm_models_finalize(CN models)
{
    for(It first=CN_first(models); !It_equal(first, CN_tail(models)); It_next(first)) {
        svm_model_t* model = It_ptr(first);
        svm_model_finalize(model);
        free(model);
    }
    return 0;
}

int svm_models_export(CN models)
{
    /* TODO: 把模型 export 到文本文件中去，以备重复使用 */

}
