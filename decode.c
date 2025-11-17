#include<stdio.h>
#include<string.h>
#include"decode.h"
#include"encode.h"
#include"types.h"


Status open_files_decode(DecodeInfo *decInfo){  //opening files
    decInfo->fptr_stego_image=fopen(decInfo->stego_image_fname,"r");
      
    if(decInfo->fptr_stego_image==NULL)
    {
        printf("stego file not present");
        return e_failure;

    }
    
    decInfo->secret_fname =decInfo->output_name;
    decInfo->fptr_secret=fopen(decInfo->secret_fname,"w");
     
   
    return e_success;
    

}

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
   
    if((strstr(argv[2],".bmp"))!=NULL)
    {
        printf("bmp file is present\n");
        decInfo->stego_image_fname=argv[2];
    }
    else{
        printf("enter proper arguments witn .bmp \n");
        return e_failure;
    }
    if(argv[3]!=NULL){
         if(strstr(argv[3],".txt")!=NULL)
        {
        printf("outout.txt is present\n");
        strcpy(decInfo -> output_name,argv[3]);
        }
     

    }
    else
    {
        decInfo -> secret_fname="output.txt"; //assigning name to secret file
        strcpy(decInfo->output_name,decInfo->secret_fname);
        
    }
    return e_success;

}
Status do_decoding(DecodeInfo *decInfo,EncodeInfo *encInfo)
{
    if(open_files_decode(decInfo)==e_success)
    {
        printf("all files are opened successfully\n");
         printf("enter the magic string\n");
        scanf("%[^\n]",decInfo->read_magic);

    }
    else{
        printf("files not opened\n");
        return e_failure;
    }
    if(decode_magic_string_len(decInfo)==e_success)
   {
    printf("decode magic string len successful\n");
   }
   else{
    printf("decode magic string len unsuccessful\n");

    return e_failure;
   }
 
   
    
   printf("entered magic string is %s\n",decInfo->read_magic);//magic string
   if(decode_magic_string(decInfo,decInfo->read_magic)==e_success)
   {
    printf("decode magic string successful\n");
   }
   else{
    printf("decode magic string unsuccessful\n");

    return e_failure;
   }
   if(decode_secret_extension_size(decInfo)==e_success)
   {
    printf("decode secret ext size successful\n");
   }
   else{
    printf("decode secret ext size unsuccessful\n");

    return e_failure;
   }
    if(decode_secret_extension(decInfo)==e_success)
   {
    printf("decode secret ext successful\n");
   }
   else{
    printf("decode secret ext unsuccessful\n");

    return e_failure;
   }
   if(secret_data_size_decode(decInfo)==e_success)
   {
    printf("decode secret data size successful\n");
   }
   else{
    printf("decode secret data size unsuccessful\n");

    return e_failure;
   }
   
   if(secret_data_decode(decInfo->size_secret_file,decInfo)==e_success)
   {
    printf("decode secret data successful\n");
   }
   else{
    printf("decode secret data unsuccessful\n");

    return e_failure;
   }

   






return e_success;



}
Status decode_magic_string_len(DecodeInfo *decInfo)				// Function to decode magic string length
{
    int ch = 0;
    char buff[32];
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    fread(buff, 1, 32, decInfo->fptr_stego_image);
    for (int i = 0; i < 32; i++)
    {
        
            ch |= ((buff[i] & 1) << i);
    }
	
    decInfo->size_magic = ch;
    if(strlen(decInfo->read_magic)!=ch){
        printf("Error:invalid magic string\n");
        return e_failure;
    }

    return e_success;
}



Status decode_magic_string(DecodeInfo *decInfo,char *magic_string)		// Function to decode magic string
{
    char buff[8], str[50];int i,j;
    char ch;
    
    for ( i = 0; i < strlen(decInfo->read_magic); i++)
    {
	    ch = 0;
	    fread(buff, 1, 8, decInfo->fptr_stego_image);
	    for ( j = 0; j < 8; j++)
	        ch |= ((buff[j] & 1) << j);
	    
	    str[i] = ch;
    }
    str[i]='\0';
    
    
    if(strcmp(magic_string,str) !=0)
    {
        return e_failure;
    }
    
    return e_success;
}
  

Status decode_secret_extension_size(DecodeInfo *decInfo)
{
    char buffer[32];
    unsigned int ch=0;


    int n=fread(buffer,1,32,decInfo->fptr_stego_image);

    for(int i=0;i<32;i++){  //decoding size 
        ch |= ((buffer[i]&1)<<i);

    }
    decInfo->size_extension=ch;
    
    return e_success;
}

Status decode_secret_extension(DecodeInfo *decInfo)
{
    char buffer[8],str[10];
    for(int i=0;i<decInfo->size_extension;i++)
    {
        char ch=0;
        fread(buffer,1,8,decInfo->fptr_stego_image); //decoding character oe by one
        for(int j=0;j<8;j++)
        {
            ch |= ((buffer[j]&1)<<j);

        }
        
        str[i]=ch;
    }
    str[decInfo->size_extension]='\0';
      
     strcpy(decInfo->secret_fname, decInfo->output_name);
    strcat(decInfo->secret_fname,str);
    
    return e_success;
}

Status secret_data_size_decode(DecodeInfo *decInfo)	//decoding secret data size	
{
    char buff[32],ch = 0;
    fread(buff, 1, 32, decInfo->fptr_stego_image);
    for (int i = 0; i < 32; i++)
	    ch |= ((buff[i] & 1) << i);
    decInfo->size_secret_file = ch;
    
    return e_success;
}

Status secret_data_decode(long size, DecodeInfo *decInfo)	//decoding secret data	
{
    char buff[8];
    for (int i = 0; i < size; i++)
    {
	char ch = 0;
	fread(buff, 1, 8, decInfo->fptr_stego_image);
	for (int j = 0; j < 8; j++)
	    ch |= ((buff[j] & 1) << j);
	fwrite(&ch, 1, 1, decInfo->fptr_secret);
    }
    return e_success;
}

