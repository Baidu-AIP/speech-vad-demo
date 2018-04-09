//
// Created by fu on 3/6/18.
//

#ifdef __APPLE__
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif

#include "common.h"
#include "period_format.h"

static int add_period_start(struct periods *per) {
    per->size++;
    if (per->size > per->size_allocated) {
        size_t new_size = per->size_allocated * 2 * sizeof(uint64_t);
        per->period_start = realloc(per->period_start, new_size);
        if (per->period_start == NULL) {
            fprintf(stderr, "per->period_start allocated failed %ld", new_size);
            return 1;
        }
        per->period_end = realloc(per->period_end, new_size);
        if (per->period_end == NULL) {
            fprintf(stderr, "per->period_end allocated failed %ld", new_size);
            return 2;
        }
        per->size_allocated *= 2;
    }
    uint64_t start_time = CAL_FRAME_BY_FRAME(per->current_frame);
    per->period_start[per->size - 1] = start_time;
    per->is_end_filled = 0;
    return 0;
}

static void add_period_end(struct periods *per) {
    size_t end_time = CAL_FRAME_BY_FRAME(per->current_frame);
    per->period_end[per->size - 1] = end_time;
    per->is_end_filled = 1;
}

struct periods *periods_create() {
    struct periods *per = calloc(1, sizeof(struct periods));
    if (per == NULL) {
        fprintf(stderr, "per alloc failed");
        return NULL;
    }
    per->period_start = calloc(PERIODS_SIZE_INITIED, sizeof(uint64_t));
    if (per->period_start == NULL) {
        fprintf(stderr, "per->period_start alloc failed");
    } else {
        per->period_end = calloc(PERIODS_SIZE_INITIED, sizeof(uint64_t));
        if (per->period_end == NULL) {
            fprintf(stderr, "per->period_start alloc failed");
        } else {
            per->size_allocated = PERIODS_SIZE_INITIED;
            return per;
        }
    }
    return NULL;
}


void periods_print(struct periods *per) {
    int size = per->size;
    int is_ended_fill = per->is_end_filled;
    printf("PERIODS SIZE :%d, is_end_filled:%s ", size, is_ended_fill ? "true" : "false");

    int i;
    for (i = 0; i < size; i++) {
        if ((i != size - 1) || is_ended_fill) {
            printf("[%ld, %ld] ", per->period_start[i], per->period_end[i]);
        } else {
            printf("[%ld, N]", per->period_start[size]);
        }
    }

    // printf("size %ld allocated %ld", per->size, per->size_allocated);
    printf("\n");
}

int period_add_vad_activity(struct periods *per, int is_active, int is_last) {
    if (!per->is_pervious_active && is_active) {
        int res = add_period_start(per);
        if (res != 0) {
            return res;
        }
        per->is_pervious_active = is_active;
    } else if (per->is_pervious_active && (!is_active || is_last)) {
        add_period_end(per);
        per->is_pervious_active = is_active;
    }

    per->current_frame++;
    return 0;
}

void periods_free(struct periods *per) {
    free(per->period_start);
    free(per->period_end);
    free(per);
}