#ifndef __APP_DEFS_H__
#define __APP_DEFS_H__

#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <limits.h>

#define MAX_U_32_NUM            0xFFFFFFFF
#define CP_UTF8                 65001

#define FLOAT_TO_UNSIGNED(f) ((unsigned long)(((long)((f) - 2147483648.0)) + 2147483647L + 1))
#define UNSIGNED_TO_FLOAT(u) (((double)((long)((u) - 2147483647L - 1))) + 2147483648.0)

#define LOW__BYTE(x) (x & 0x00ff)
#define HIGH_BYTE(x) ((x >> 8) & 0x00ff)

#ifndef HAVE_IEEE754_FLOAT32_T 
typedef float ieee754_float32_t;
#endif

typedef struct blockAlign_struct {
    unsigned long offset;
    unsigned long blockSize;
} blockAlign;

typedef struct IFF_AIFF_struct {
    short   numChannels;
    unsigned long numSampleFrames;
    short   sampleSize;
    double  sampleRate;
    unsigned long sampleType;
    blockAlign blkAlgn;
} IFF_AIFF;

typedef enum sound_file_format_e {
    sf_unknown,
    sf_raw,
    sf_wave,
    sf_aiff,
    sf_mp1,                  /* MPEG Layer 1, aka mpg */
    sf_mp2,                  /* MPEG Layer 2 */
    sf_mp3,                  /* MPEG Layer 3 */
    sf_mp123,                /* MPEG Layer 1,2 or 3; whatever .mp3, .mp2, .mp1 or .mpg contains */
    sf_ogg
} sound_file_format;

typedef struct ReaderConfig {
    sound_file_format input_format;
    int   swapbytes;                /* force byte swapping   default=0 */
    int   swap_channel;             /* 0: no-op, 1: swaps input channels */
    int   input_samplerate;
} ReaderConfig;

typedef struct WriterConfig {
    int   flush_write;
} WriterConfig;

typedef struct UiConfig {
    int   silent;                   /* Verbosity */
    int   brhist;
    int   print_clipping_info;      /* print info whether waveform clips */
    float update_interval;          /* to use Frank's time status display */
} UiConfig;

typedef struct DecoderConfig {
    int   mp3_delay;                /* to adjust the number of samples truncated during decode */
    int   mp3_delay_set;            /* user specified the value of the mp3 encoder delay to assume for decoding */
    int   disable_wav_header;
    mp3data_struct mp3input_data;
} DecoderConfig;

typedef enum ByteOrder { ByteOrderLittleEndian, ByteOrderBigEndian } ByteOrder;

typedef void SNDFILE;

struct PcmBuffer {
    void   *ch[2];           /* buffer for each channel */
    int     w;               /* sample width */
    int     n;               /* number samples allocated */
    int     u;               /* number samples used */
    int     skip_start;      /* number samples to ignore at the beginning */
    int     skip_end;        /* number samples to ignore at the end */
};

typedef struct PcmBuffer PcmBuffer;

typedef struct get_audio_global_data_struct {
    int     count_samples_carefully;
    int     pcmbitwidth;
    int     pcmswapbytes;
    int     pcm_is_unsigned_8bit;
    int     pcm_is_ieee_float;
    unsigned int num_samples_read;
    FILE   *music_in;
    SNDFILE *snd_file;
    hip_t   hip;
    PcmBuffer pcm32;
    PcmBuffer pcm16;
    size_t  in_id3v2_size;
    unsigned char* in_id3v2_tag;
} get_audio_global_data;


typedef struct RawPCMConfig {
    int     in_bitwidth;
    int     in_signed;
    ByteOrder in_endian;
} RawPCMConfig;
#endif//__APP_DEFS_H__
