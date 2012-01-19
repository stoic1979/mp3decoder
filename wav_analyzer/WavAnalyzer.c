#include <stdio.h>

//#define INFO(A, B) printf(A, B);

#define HEADER_LEN         44

//PROTOTYPES ------------------------------------------------------------------
void parseHeader(FILE *fp);
void info(char *msg);
void showPCM(FILE *fp);

//DATA ------------------------------------------------------------------------
unsigned char buf[HEADER_LEN];



//FUNCTIONS -------------------------------------------------------------------
void showHeader() {
    int i;
    unsigned int chk_size, subchk1_size, subchk2_size, audio_format, num_channels;
    unsigned int samp_rate, byte_rate, block_align, bits_per_samp;

    printf("\nChunk ID . . . . . . : ");

    /*first 4 bytes is RIFF*/
    for(i=0; i<4; i++)
        printf("%c",  buf[i]);
    printf("\n");

    /*chunk size*/
    chk_size = buf[7] << 24 | buf[6] << 16 | buf[5] << 8 | buf[4];
    printf("Chunk Size  . . . . .: %d\n",  chk_size);

    /*chunk format*/
    printf("Chunk Format . . . . : ");
    for(i=8; i<12; i++)
        printf("%c",  buf[i]);
    printf("\n");

    /*subchunk1 format*/
    printf("subchunk1 ID . . . . : ");
    for(i=12; i<16; i++)
        printf("%c",  buf[i]);
    printf("\n");

    /*subchunk1 size*/
    subchk1_size = buf[19] << 24 | buf[18] << 16 | buf[17] << 8 | buf[16];
    printf("Sub Chunk1 Size  . . : %d\n",  subchk1_size);

    /*audio format, 1 for PCM, */
    audio_format = buf[21] << 8 | buf[20];
    printf("Audio Format . . . . : %d\n",  audio_format);

    /*no. of channels*/
    num_channels = buf[23] << 8 | buf[22];
    printf("No. Of Channels . . .: %d\n",  num_channels);

    /*sampling rate*/
    samp_rate = buf[27] << 24 | buf[26] << 16 | buf[25] << 8 | buf[24];
    printf("Sampling Rate . . . .: %d\n",  samp_rate);

    /*byte rate*/
    byte_rate = buf[31] << 24 | buf[30] << 16 | buf[29] << 8 | buf[28];
    printf("Byte Rate . . . . . .: %d\n",  byte_rate);

    /*block align*/
    block_align = buf[33] << 8 | buf[32];
    printf("Block Align . . . . .: %d\n",  block_align);

    /*bits per sample*/
    bits_per_samp = buf[35] << 8 | buf[34];
    printf("Bits Per Sample . . .: %d\n",  bits_per_samp);

    /*subchunk2 format*/
    printf("subchunk2 ID . . . . : ");
    for(i=36; i<40; i++)
        printf("%c",  buf[i]);
    printf("\n");

    /*subchunk2 size*/
    subchk2_size = buf[43] << 24 | buf[42] << 16 | buf[41] << 8 | buf[40];
    printf("Sub Chunk2 Size  . . : %d\n",  subchk2_size);

    for(i=44; i<HEADER_LEN; i++) {
        printf("%d ",  buf[i]);
    }
    printf("\n");
}

void parseHeader(FILE *fp) {
    int i, actually_read;

    /*reset buffer*/
    //memset(buf, HEADER_LEN, 0, 0);

    actually_read = fread(buf, 1, HEADER_LEN, fp);
    if(HEADER_LEN != actually_read) {
	    printf("Reading ERROR\n");
	    exit(1);
    }
    printf("Actually read: %d\n", actually_read);
    
    showHeader();
}

void showPCM(FILE *fp) {
    #define LR_SIZE  4
    unsigned char lr_buf[LR_SIZE];
    unsigned short l, r;
    int actually_read;

    /*analyze first 100 samples of PCM*/
    int move_on = 100;

    do{
        actually_read = fread(lr_buf, 1, LR_SIZE, fp);
        if(LR_SIZE != actually_read) {
            printf("Reading ERROR or EOF has been reached...\n");
            break;
        }
        l =  lr_buf[3] << 8 |  lr_buf[2];
        r =  lr_buf[1] << 8 |  lr_buf[0];
        printf("%x %x %x %x, L R : %d, %d\n", lr_buf[0], lr_buf[1], lr_buf[2], lr_buf[3], l, r);
        move_on--;
        if(!move_on) break;
    }while(1);
}

void info(char *msg) {
    printf("[INFO] :: %s\n", msg);
}

int main(int argc, char *argv[]) {
    FILE *fp;

    if(argc<2) {
        printf("USAGE:-\n%s <input-wav-file>\n", argv[0]);
        exit(1);
    }
    info("Analyzing WAV file");

    printf("reading: %s\n", argv[1]);
    if(0 == (fp=fopen(argv[1], "rb"))) {
        info("Failed to open file");
        exit(1);
    }

    /*process header*/
    parseHeader(fp);

    /*show PCM in WAV*/
    showPCM(fp);

    /*cleanup and release resources*/
    fclose(fp);
    return 1;
}
