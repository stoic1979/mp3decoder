/*
 * Copyright (c) 2011 onwards by WeaveBytes InfoTech Pvt. Ltd.
 * 
 * Please reports bugs at weavebytes@gmail.com
 * 
 * This file may be distributed and/or modified under the terms of the 
 * GNU General Public License version 2 as published by the Free Software 
 * Foundation. (See COPYING.GPL for details.)
 * 
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 * 
 */
#ifndef __APP_H__
#define __APP_H__

#include "lame.h"

#include <wchar.h>
//#include <mbstring.h>

#include "app_defs.h"
#include "app_utils.h"
#include "app.h"
#include "app_fft.h"

//PROTOTYPES -------------------------------------------------------------------------------------
static int     lame_decode_fromfile(FILE * fd, short pcm_l[], short pcm_r[], mp3data_struct * mp3data);
static void    close_infile(void);
static void    put_audio16(FILE * outf, short Buffer[2][1152], int iread, int nch);
static int     get_audio16(lame_t gfp, short buffer[2][1152]);
static int     get_audio_common(lame_t gfp, int buffer[2][1152], short buffer16[2][1152]);
static int     takePcmBuffer(PcmBuffer * b, void *a0, void *a1, int a_n, int mm);
static int     addPcmBuffer(PcmBuffer * b, void *a0, void *a1, int read);
static int     unpack_read_samples(const int samples_to_read, const int bytes_per_sample, const int swap_order, int *sample_buffer, FILE * pcm_in);
static int     WriteWaveHeader(FILE * const fp, int pcmbytes, int freq, int channels, int bits);
static int     read_samples_mp3(lame_t gfp, FILE * musicin, short int mpg123pcm[2][1152]);
static int     read_samples_pcm(FILE * musicin, int sample_buffer[2304], int samples_to_read);
static int     samples_to_skip_at_start(void);
static int     samples_to_skip_at_end(void);
static char*   lame_getenv(char const* var);
static FILE*   init_files(lame_global_flags * gf, char const *inPath, char const *outPath);
int            init_infile(lame_t gfp, char const *inPath);
FILE*          init_outfile(char const *outPath, int decode);
int            is_mpeg_file_format(int input_file_format);
static FILE*   open_mpeg_file(lame_t gfp, char const *inPath, int *enc_delay, int *enc_padding);
static FILE*   open_wave_file(lame_t gfp, char const *inPath);
static int     close_input_file(FILE * musicin);
static void    freePcmBuffer(PcmBuffer * b);
static void    initPcmBuffer(PcmBuffer * b, int w);
static off_t   lame_get_file_size(FILE * fp);
static int     lame_set_stream_binary_mode(FILE * const fp);
static int     parse_file_header(lame_global_flags * gfp, FILE * sf);
static void    setSkipStartAndEnd(lame_t gfp, int enc_delay, int enc_padding);
static int     parse_wave_header(lame_global_flags * gfp, FILE * sf);
static int     parse_aiff_header(lame_global_flags * gfp, FILE * sf);
static int     aiff_check2(IFF_AIFF * const pcm_aiff_data);
static double  read_ieee_extended_high_low(FILE * fp);
static int     fskip(FILE * fp, long offset, int whence);
static size_t  min_size_t(size_t a, size_t b);
static FILE*   lame_fopen(char const* file, char const* mode);
static int     lame_decoder(lame_t gfp, FILE * outf, char *inPath, char *outPath);


static unsigned long calcNumBlocks(unsigned long samples, int pcm_samples_per_frame);
static unsigned long calcEndPadding(unsigned long samples, int pcm_samples_per_frame);


void processPCM(char data[2 * 1152 * 2]);
void computeFft4Buf(char data[2 * 1152 * 2]);

#ifndef STANDALONE_APPLICATION
int decoder_lib_main(char *src, char* des);
#endif
#endif//__APP_H__
