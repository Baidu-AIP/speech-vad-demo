//
// 收集和打印非静音的声音段
// Created by fu on 3/6/18.
//

#ifndef VAD_DEMO_PERIOD_FORMAT_H
#define VAD_DEMO_PERIOD_FORMAT_H

#include "simple_vad.h"

struct periods {
    int is_pervious_active;
    size_t current_frame;
    uint64_t *period_start;
    uint64_t *period_end;
    int size;
    size_t size_allocated;
    int is_end_filled;

};

struct periods *periods_create();
/**
 * @brief
 * @param per
 * @param is_active 是否是有声音的FRAME
 * @param is_last 是否是最后一个FRAME
 * @return
 */
int period_add_vad_activity(struct periods *per, int is_active, int is_last);

void periods_free(struct periods *per);

void periods_print(struct periods *per);

/*
 * 测试例子
 * struct periods *per=periods_create();
    period_add_vad_activity(per,1,0);
    period_add_vad_activity(per,1,0);
    periods_print(per);
    period_add_vad_activity(per,0,0);
    periods_print(per);
    period_add_vad_activity(per,1,0);
    periods_print(per);
    period_add_vad_activity(per,0,0);
    period_add_vad_activity(per,0,0);
    period_add_vad_activity(per,0,0);
    periods_print(per);
    period_add_vad_activity(per,1,0);
    period_add_vad_activity(per,1,1);
    periods_print(per);
    periods_free(per);
 */


#endif //VAD_DEMO_PERIOD_FORMAT_H
