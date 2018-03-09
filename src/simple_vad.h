//
// 包装webrtc 的gmm vad算法
//
// Created by fu on 3/6/18.
//

#ifndef VAD_DEMO_FILE_READ_H
#define VAD_DEMO_FILE_READ_H

#include <stdio.h>
#include <stdint.h>
#include <common_audio/vad/include/webrtc_vad.h>
#include "common.h"

// 一个FRAME代表的时长。MULTI=1： 10ms；MULTI=2： 20ms；MULTI=3： 30ms；
// FRAME_SIZE = 160 * MULTI = SAMPLE_RATE  /1000 * MULTI ,


typedef VadInst simple_vad;

/**
 * @brief 读取int16_t的数组
 * @param fp
 * @param output
 * @return 0 正常 1 文件结束 , 1000+ferror : 文件错误
 */
int read_int16_bytes(FILE *fp, int16_t *output);

/**
 * @brief
 * @return NULL 创建失败
 */
simple_vad *simple_vad_create();

/**
 * @brief
 * @param inst
 * @param data 例如read_int16_bytes读取的数据
 * @return
 */
int process_vad(VadInst *inst, int16_t *data);

/**
 * @brief
 * @param inst
 */
void simple_vad_free(simple_vad *inst);

#endif //VAD_DEMO_FILE_READ_H
