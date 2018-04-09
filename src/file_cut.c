//
// Created by fu on 3/7/18.
//

#include <stdlib.h>
#include "file_cut.h"

// static size_t file_total = 0;

static inline int cut_write_file(struct cut_info *cut, int frames) {
    int size = frames * FRAME_SIZE * sizeof(uint16_t);
    uint16_t buffer[size];
    int readed = fread(buffer, 1, size, cut->fp);
    if (readed > 0) {
        FILE *res_file = fopen(cut->result_filename, "wb+");
        if (res_file == NULL) {
            fprintf(stderr, "file open failed, %s\n", cut->result_filename);
            return 3;
        }
        int written = fwrite(buffer, 1, readed, res_file);
        fclose(res_file);
        if (written != readed) {
            fprintf(stderr, "written is %d, readed is %d\n", written, readed);
            return 2;
        }
        // file_total += written;
        // printf("file write success, %s, written %d\n", cut->result_filename, written);
        return 0;

    } else {
        return 1;
    }

}

static inline int cut_frame(struct cut_info *cut, int last_frame, int force) {
    int frames = last_frame - cut->cut_begin_frame;
    if (force || (frames >= CAL_FRAME_BY_TIME(FILE_CUT_MIN_MS))) {
        if (last_frame == 109) {
            printf("%d", 109);
        }
        //printf("cut file at frame: %d\n", last_frame);
        snprintf(cut->result_filename, sizeof(cut->result_filename),
                 "%s/%s_%ld-%ld_%s.pcm", cut->output_file_dir,
                 cut->output_filename_prefix, CAL_FRAME_BY_FRAME(cut->cut_begin_frame),
                 CAL_FRAME_BY_FRAME(last_frame) - 1, cut->is_contain_active ? "A" : "I");
        cut_write_file(cut, frames);
        cut->is_pervious_active = 0;
        cut->is_contain_active = 0;
        cut->cut_begin_frame = last_frame;
        return 0;
    } else {
        return 1;
    }
}

static inline int add_continued(struct cut_info *cut, int is_active) {
    if (!is_active && cut->is_contain_active) {
        // 有响声，之后连续静音
        int diff = cut->previous_along_frames - CAL_FRAME_BY_TIME(FILE_CUT_SILENCE_AFTER_ACTIVE_MS);
        if (diff >= 0) {
            int res = cut_frame(cut, cut->current_frame, 0);
            if (res == 0) {
                int frame = -1 * (cut->current_frame);
                cut->previous_along_frames = 1;
                return frame;
            }
        }
    }
    cut->previous_along_frames++;
    return 0;
}


static inline int add_changed(struct cut_info *cut, int is_active) {
    int frame = 0;
    if (is_active) {
        // 连续静音，之后遇到响声
        if (cut->previous_along_frames > CAL_FRAME_BY_TIME(FILE_CUT_SILENCE_BEFORE_ACTIVE_MS)) {
            int c_frames =
                    cut->current_frame - CAL_FRAME_BY_TIME(FILE_CUT_SILENCE_BEFORE_ACTIVE_MS);
            int res = cut_frame(cut, c_frames, 0);
            if (res == 0) {
                frame = -1 * (c_frames);
            }
            // cut->previous_along_frames = FILE_CUT_SILENCE_AFTER_ACTIVE_MS;
        }
    }

    cut->previous_along_frames = 1;
    return frame;
}

struct cut_info *cut_info_create(FILE *fp) {
    struct cut_info *cut = calloc(1, sizeof(struct cut_info));
    cut->fp = fp;
    return cut;
}


int cut_add_vad_activity(struct cut_info *cut, int is_active, int is_last) {
    int res;
    if (is_last ||
        (cut->current_frame - cut->cut_begin_frame == CAL_FRAME_BY_TIME(FILE_CUT_MAX_MS))) {
        cut_frame(cut, cut->current_frame, is_last);
        res = -1 * cut->current_frame;
    } else if (cut->is_pervious_active == is_active) {
        res = add_continued(cut, is_active);
    } else {
        res = add_changed(cut, is_active);
        if (is_active) {
            cut->is_contain_active = 1;
        }
    }
    cut->is_pervious_active = is_active;
    cut->current_frame++;
    return res;
}

void cut_info_free(struct cut_info *cut) {
    free(cut);
}


void cut_info_print(struct cut_info *cut) {
    printf("%s, %s， %d, frame %d\n", cut->is_pervious_active ? "PA" : "PI",
           cut->is_contain_active ? "CA" : "CI", cut->previous_along_frames, cut->current_frame);
}
