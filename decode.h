 #include<stdio.h>
 #include"encode.h"
 #ifndef DECODE_H
 #define DECODE_H
 #include"types.h"
 

 #define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4
#define MAX_FILE_OUTPUT_SUFFIX 20

typedef struct _DecodeInfo

{
    //stego image info
    char *stego_image_fname;
    FILE *fptr_stego_image;

    //secret dat info
    char *secret_fname;
    char output_name[MAX_FILE_OUTPUT_SUFFIX];
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;
    long size_extension;
    long size_magic;//magic string size
    int extn_size;
    char read_magic[20];//magic string
    



}DecodeInfo;
Status open_files_decode(DecodeInfo *decInfo);

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo,EncodeInfo *encInfo);

Status secret_data_decode(long size, DecodeInfo *decInfo);

Status secret_data_size_decode(DecodeInfo *decInfo);

Status decode_magic_string_len(DecodeInfo *decInfo);

Status decode_magic_string(DecodeInfo *decInfo, char *magic_string);

Status decode_secret_extension_size(DecodeInfo *decInfo);

Status decode_secret_extension(DecodeInfo *decInfo);
#endif


