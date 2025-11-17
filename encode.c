#include<stdio.h>
#include<string.h>
#include"types.h"
#include"encode.h"


Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strstr(argv[2],".bmp")!=NULL)        //reading and validation of encode arguments
    {
        printf(".bmp is present\n");
        encInfo -> src_image_fname=argv[2];
    }
    else
    {
        printf(".bmp is not present\n");
        return e_failure;

    }
     if(strstr(argv[3],".txt")!=NULL)
    {
        printf(".txt is present\n");
        encInfo -> secret_fname=argv[3];
    }
    else
    {
        printf(".txt is not present\n");
        return e_failure;

    }
    if(argv[4]!=NULL){
         if(strstr(argv[4],".bmp")!=NULL)
        {
        printf("stego.bmp is present\n");
        encInfo -> stego_image_fname=argv[4];
        }
     

    }
    else
    {
        encInfo -> stego_image_fname="stego.bmp";
        
        
    }
    return e_success;
    

    
}
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo)==e_success)
    {
        printf("all file are opened successfully\n");
    }
    else
    {
        printf("files not opened\n");
        return e_failure;
    }
    if(check_capacity(encInfo)==e_success)
    {
        printf("check capacity is successful\n");
    }
    else
    {
        printf("check capacity is unsuccessful\n");
        return e_failure;
    }
    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    {
        printf("header copied successfully\n");
    }
    else
    {
        printf("header notcopied successfully\n");
        return e_failure;
    }
    if(encode_magic_string_size(strlen(encInfo->magic),encInfo)==e_success)
    {
        printf("magic string size encoded successfully\n");
    }
    else
    {
        printf("magic string size not encoded successfully\n");
        return e_failure;
    }
   
    if(encode_magic_string(encInfo)==e_success)
    {
        printf("magic string encoded successfully\n");
    }
    else
    {
        printf("magic string not encoded successfully\n");
        return e_failure;
    }
    
    int size=strlen(strchr(encInfo->secret_fname,'.'));
    if( encode_size_to_lsb(size,encInfo)==e_success)
    {
        printf("size of extn encoded successfully\n");
       
    }
    else{
        printf("size of extn encoded unsuccessfully\n");
        return e_failure;

    }

    if(encode_secret_file_extn(strchr(encInfo->secret_fname,'.'),encInfo)==e_success){
        printf("extn encoded successfully\n");
    }
    else{
        printf("extn encoded unsuccessfully\n");
        return e_failure;

    }
    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success){
        printf("secret size encoded successfully\n");
    }
    else{
         printf("secret size encoded unsuccessfully\n");
         return e_failure;

    }
    if(encode_secret_file_data(encInfo)==e_success){
        printf("secret data encoded successfully\n");

    }
    else{
        printf("secret data encoded unsuccessfully\n");
        return e_failure;

    }
    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success){
        printf("remaining data copied \n");
    
    }
    else{
        printf("remaining data not copied \n");
        return e_failure;

    }













    return e_success;

}
Status open_files(EncodeInfo *encInfo)      //opening files
{
    encInfo->fptr_src_image=fopen((encInfo->src_image_fname),"r");
    if(encInfo->fptr_src_image==NULL)
    {
        printf("source file is not present\n");
        return e_failure;
    }
     encInfo->fptr_secret=fopen((encInfo->secret_fname),"r");
    if(encInfo->fptr_secret==NULL)
    {
        printf("secret file is not present\n");
        return e_failure;
    }
     encInfo->fptr_stego_image=fopen((encInfo->stego_image_fname),"w");
     
    
     return e_success;
    
}

Status check_capacity(EncodeInfo *encInfo){
    encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);

    printf("enter the magic string\n");
    scanf("%[^\n]",encInfo->magic);

     if(encInfo->image_capacity>(strlen(encInfo->magic)*8+32+32+32+encInfo->size_secret_file*8))//checking for capacity
     {
        return e_success;
     }
     else
     {
        printf("image capacity failed\n");
        return e_failure;   
     }
    


    return e_success;
    
}
uint get_image_size_for_bmp(FILE *fptr_image){
    //getting size from header
    uint width,height;
    fseek(fptr_image,18,SEEK_SET);
    fread(&width,4,1,fptr_image);
    printf("width %d\n",width);
    fread(&height,4,1,fptr_image);
    printf("height %d\n",height);

    return width*height*3;


}
uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    rewind(fptr_src_image);
    char buffer[54];
    fread(buffer,54,1,fptr_src_image);
    fwrite(buffer,54,1,fptr_dest_image);
    return e_success;


}
Status encode_magic_string_size(long file_size, EncodeInfo *encInfo)
{
    //encoding magic string length
     encode_size_to_lsb(file_size,encInfo);
     printf("magic string len %ld\n",file_size);
     return e_success;


}


Status encode_magic_string(EncodeInfo *encInfo){    
    if(encode_data_to_image(encInfo->magic,strlen(encInfo->magic),encInfo)==e_success)
    {
        printf("magic string is encoded successfully\n");

    }
    else
    {
          printf("magic string is not encoded successfully\n");   
          return e_failure;
    }
    return e_success;
}
Status encode_data_to_image(char *data, int size,EncodeInfo *encInfo ) //encoding data to image
{
    for(int i=0;i<size;i++){
        fread(encInfo->image_data,8,1,encInfo->fptr_src_image);
        encode_byte_to_lsb(data[i],encInfo->image_data);
        fwrite(encInfo->image_data,8,1,encInfo->fptr_stego_image);

    }
    
    return e_success;
}
Status encode_byte_to_lsb(char data, char *image_buffer){           //encoding string to image lsb
    for(int i=0;i<8;i++)
    {
        image_buffer[i]=(image_buffer[i] & 0xFE ) | ((data>>i)&1);
    }

}
Status encode_size_to_lsb(int size,EncodeInfo *encInfo)	//encoding int to lsb
{
     char buffer[32];
        fread(buffer,32,1,encInfo->fptr_src_image);
        for(int i=0;i<32;i++){
            buffer[i]=(buffer[i] & 0xFE) | ((size >>i)&1);
        }
        fwrite(buffer,32,1,encInfo->fptr_stego_image);
        return e_success;
}

Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo){ //encoding secret file extn
    if(encode_data_to_image(file_extn,strlen(file_extn),encInfo)==e_success)
    {
        printf("successfull\n");
        return e_success;
    }
    else{
        printf("unsuccessful\n");
        return e_failure;
    }
}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo){
 
    encode_size_to_lsb(file_size,encInfo);
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo){    //encoding secret file data
    char buffer[encInfo->size_secret_file];
    rewind(encInfo->fptr_secret);
    fread(buffer,encInfo->size_secret_file,1,encInfo->fptr_secret);
    encode_data_to_image(buffer,encInfo->size_secret_file,encInfo);
    return e_success;
    
        
    

}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest){ //copying remaining data
    char ch;
    while(fread(&ch,1,1,fptr_src)){
        fwrite(&ch,1,1,fptr_dest);
    }
    return e_success;

}
