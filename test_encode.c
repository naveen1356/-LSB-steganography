/*
Name : Naveen B
Project Name : Steganography
*/

#include<stdio.h>
#include<string.h>
#include"encode.h"
#include"decode.h"
#include"types.h"


int main(int argc,char *argv[]){
    int res;
    
        res=check_operation_type(argv);

    if(res==e_encode)
    {
        if(argc >=4){                               //checcking for correct arguments
            printf("correct arguments provided\n");
        
        }

        else{
                printf("provide correct arguments\n");
        return 0;
        }
        
        printf("You have choosed encoding\n");
        EncodeInfo encInfo;
        if(read_and_validate_encode_args(argv ,&encInfo)==e_success)
        {
            printf("read and validate is successful\n");
           
            if(do_encoding(&encInfo)==e_success)
            {
                printf("encoding is successful\n");
                
            }
            else
            {
                printf("encoding is unsuccessful\n");
                return e_failure;

            }

        }
        else
        {
              printf("read and validate is unsuccessful\n");
              return e_failure;
        }
    }
    else if(res==e_decode)
    {
        if(argc >=3){
            printf("correct arguments provided\n");     //checking for correct arguments
        
        }

        else{
                printf("provide correct arguments\n");
        return 0;
        }
        
        printf("You have choosed decoding\n");
        DecodeInfo decInfo;
         EncodeInfo encInfo;
        
        
        if(read_and_validate_decode_args(argv,&decInfo)==e_success){
            printf("read and validate is successful\n");
             if(do_decoding(&decInfo,&encInfo)==e_success)
            {
                printf("decoding is successful\n");
            }
            else
            {
                printf("decoding is unsuccessful\n");
                return e_failure;

            }

        }

    }
    else
    {
        printf("pass correct arguments\n");
        printf("./a.out -e beautiful.bmp secret.txt --> for encoding\n");
         printf("./a.out -d stego.bmp --> for decoding\n");
    }
    return e_success;


}
OperationType check_operation_type(char *argv[]){  //checking for operation type
    if(argv[1]==NULL)
    {
        printf("provide valid arguments\n");
        return e_unsupported;
    }
    else{
        if(strcmp(argv[1],"-e")==0)
        return e_encode;
        
    else if(strcmp(argv[1],"-d")==0)
        return e_decode;
    }
    

    

}