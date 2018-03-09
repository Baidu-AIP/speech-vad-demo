//
// Created by fu on 3/9/18.
//

#include "common.h"

uint64_t inline cal_time_by_frame(int frame) {
    return MULTI * frame * 10;
}

int inline cal_frame_by_time(uint64_t time) {
    return time / (MULTI * 10);
}