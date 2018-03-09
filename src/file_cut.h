//
// Created by fu on 3/7/18.
//

#ifndef VAD_DEMO_FILE_CUT_H
#define VAD_DEMO_FILE_CUT_H

#include <stdio.h>
#include "common.h"
#include "stdint.h"

struct cut_info {
    int is_pervious_active;
    int is_contain_active;
    int previous_along_frames;
    FILE *fp;
    int current_frame;
    int cut_begin_frame;
    char result_filename[200];
    char output_file_dir[100];
    char output_filename_prefix[100];

};

struct cut_info *cut_info_create(FILE *fp);

int cut_add_vad_activity(struct cut_info *cut, int is_active, int is_last);

void cut_info_free(struct cut_info *cut);

void cut_info_print(struct cut_info *cut);

#endif //VAD_DEMO_FILE_CUT_H
/**

    struct cut_info *cut = cut_create_info(NULL);
    int acts[] = {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0};
    int len = sizeof(acts) / sizeof(int);
    int is_cuts[len];
    printf(" acts length: %d\n", len);

    int j, k;
    for (k = 0; k < len; k++) {
        printf("%d ", acts[k]);
        is_cuts[k] = 0;
    }
    printf("\n");
    for (k = 0; k < len; k++) {

        printf("\n");
        int res = cut_add_vad_activity(cut, acts[k], k == len - 1);
        cut_info_print(cut);
        if (res < 0) {
            is_cuts[-1 * res] = 1;
        }
        for (j = 0; j <= k; j++) {
            if (is_cuts[j]) {
                printf("| ");
            }
            printf("%d ", acts[j]);

        }
    }
    printf("\n");
    for (j = 0; j <= k; j++) {
        if (is_cuts[j]) {
            printf("| ");
        }
        printf("%d ", acts[j]);

    }
    printf("\n");
    printf("PROGRAM FINISH\n");
 **/