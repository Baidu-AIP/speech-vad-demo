//
// Created by fu on 3/6/18.
//

#include "simple_vad.h"
#include <string.h>

static int check_end_file(FILE *fp) {
    if (feof(fp)) {
        return 0;
    }
    return ferror(fp);
}

int read_int16_bytes(FILE *fp, int16_t *output) {
    if (feof(fp)) {
        return 1;
    }

    size_t readed = fread(output, sizeof(int16_t), FRAME_SIZE, fp);
    if (readed <= 0) {
        int res = check_end_file(fp);
        return (res == 0) ? 1 : 1000 + res;
    }
    if (readed < FRAME_SIZE) {
        memset(output + readed, 0, (FRAME_SIZE - readed) * sizeof(int16_t));
        // printf("only %ld bits, will refill to %ld\n", readed, length);
    }
    return 0;

}

simple_vad *simple_vad_create() {
    VadInst *inst = WebRtcVad_Create();
    int res = WebRtcVad_Init(inst);
    if (res != 0) {
        fprintf(stderr, "WebRtcVad_Init failed %d", res);
        return NULL;
    }
    res = WebRtcVad_set_mode(inst, WEBRTC_VAD_MODE);
    if (res != 0) {
        fprintf(stderr, "WebRtcVad_set_mode failed %d", res);
        WebRtcVad_Free(inst);
        return NULL;
    }
    return inst;
}

int process_vad(VadInst *inst, int16_t *data) {
    int res = WebRtcVad_Process(inst, SAMPLE_RATE, data, FRAME_SIZE);
    return res;
}

void simple_vad_free(simple_vad *inst) {
    WebRtcVad_Free(inst);
}
