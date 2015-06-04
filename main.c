#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "bcl/src/lz.h"

#define VERSION_INFO  "Version 1.1"
#define MAX_STR_LEN	  1024

const char* DEFAULT_BLOB_NAME = "BLOB";

char* binary_file = NULL;
char header_file[MAX_STR_LEN + 1] = {0};
char   blob_name[MAX_STR_LEN + 1] = {0};
bool compress = false;


void
printHeader
(void)
{
    printf("bin2h. Binary to C Header file converter. " VERSION_INFO "\n");
    printf("Copyright @ http://www.mitp0sh.de 2013. All rights reserved.\n");
    printf("\n");
}

void
printUsage
(const char* exec_path)
{
    printf("Usage: %s FILE -f STRING -b STRING\n", exec_path);
    printf("       %s FILE -b STRING\n", exec_path);
    printf("       %s FILE -f STRING\n", exec_path);
    printf("\n");
}

void
printCommands
(void)
{
    printf("  Commands:\n");
    printf("\n");
    printf("  -v, --version          output version information and exit\n");
    printf("  -h, --help             display this help and exit\n");
    printf("  -f, --file             [OPTIONAL] - specify location and name of header file\n");
    printf("  -b, --blobname         [OPTIONAL] - use this name as binary blob name\n");
    printf("  -c, --compress         [OPTIONAL] - compress binary with lz77 (gzip)\n");
    printf("\n");
}
void
printBugs
(void)
{
    printf("Report bugs to http://www.mitp0sh.de/");
}

void
printVersionInfo
(void)
{
    printf("  %s\n", VERSION_INFO);
    printf("\n");
}

void
printUnknown
(void)
{
    printf("error - unknown command!\n");
    printf("\n");
}

void
printTooLong
(const char* origin)
{
    printf("error - %s is too long!\n", origin);
    printf("\n");
}

void
printNoBinaryFile
(void)
{
    printf("error - no binary file for bin -> h selected!\n");
    printf("\n");
}

void
printUnableToReadBinary
(void)
{
    printf("error - unable to read binary file!\n");
    printf("\n");
}

void
printUnableToAllocateMemory
(void)
{
    printf("error - unable to allocate memory!\n");
    printf("\n");
}

void
printUnableToCreateHeaderFile
(void)
{
    printf("error - unable to open/create header file!\n");
    printf("\n");
}


void
processCommandLine
(
 int         argc,
 const char* argv[]
)
{
    /* local function variables */
    unsigned int            i = 0;
    bool                  inV = false;
    bool                  inH = false;
    bool                  inF = false;
    bool                  inB = false;
    bool              unknown = false;
    bool            inTooLong = false;
    char*       tooLongOrigin = NULL;
    compress = false;
    /* ---------------------------- */
    
    /* scan all commandline parameters */
    for(i = 1; i < argc; i++)
    {
        /* local variables */
        char* current = NULL;
        /* --------------------- */
        
        /* set current */
        current = (char*)argv[i];
        
        if(inF == true)
        {
            inF = false;
            
            /* nothing to process ? */
            if(i >= argc) continue;
            
            /* check if too long */
            if(strlen(current) >= MAX_STR_LEN)
            {
                inTooLong     = true;
                tooLongOrigin = "header file name";
                break;
            }
            strncpy(header_file, current, MAX_STR_LEN);
        }
        else
        if(inB == true)
        {
            inB = false;
            
            /* nothing to process ? */
            if(i >= argc) continue;
                
            /* check if too long */
            if(strlen(current) >= MAX_STR_LEN)
            {
                inTooLong     = true;
                tooLongOrigin = "blob name";
                break;
            }
                
            strncpy(blob_name, current, MAX_STR_LEN);
        }
        else        
        if(current[0] == '-')
        {
            if((current[1] == 'v' && current[2] == 0) ||
               strcmp(&current[2], "version") == 0)
            {
                inV = true;
                break;
            }
            else
            if((current[1] == 'h' && current[2] == 0) ||
               strcmp(&current[2], "help") == 0)
            {
                inH = true;
                break;
            }
            else
            if((current[1] == 'f' && current[2] == 0) ||
               strcmp(&current[2], "file") == 0)
            {
                inF = true;
            }
            else
            if((current[1] == 'b' && current[2] == 0) ||
               strcmp(&current[2], "blobname") == 0)
            {
                inB = true;
            }
            else
            if((current[1] == 'c' && current[2] == 0) ||
               strcmp(&current[2], "compress") == 0)
            {
                compress = true;
            }
            else
            {
                unknown = true;
                break;
            }
        }
        else
        {            
            /* set binary file */
            binary_file = (char*)current;
        }
    }
    
    if(unknown == true)
    {
        printHeader();
        printUnknown();
        exit(EXIT_FAILURE);
    }
    else
    if(inTooLong == true)
    {
        printHeader();
        printTooLong(tooLongOrigin);
        exit(EXIT_FAILURE);
    }
    else
    if(inV == true)
    {
        printHeader();
        printVersionInfo();
        exit(0);
    }
    else
    if(inH == true)
    {
        printHeader();
        printUsage(argv[0]);
        printCommands();
        exit(0);
    }
    if(binary_file == NULL)
    {
        printHeader();
        printNoBinaryFile();
        exit(EXIT_FAILURE);
    }
    
    if(blob_name[0] == 0)
    {
        /* copy default value */
        strncpy(blob_name, DEFAULT_BLOB_NAME, MAX_STR_LEN);
    }
    
    if(header_file[0] == 0)
    {
        /* copy default value */
        strncpy(header_file, binary_file, MAX_STR_LEN);
        strncat(header_file, "_", MAX_STR_LEN);
        strncat(header_file, blob_name, MAX_STR_LEN);
        strncat(header_file, ".h", MAX_STR_LEN);
    }
}

int
main
(
    int         argc,
    const char* argv[]
)
{
    /* local function variables */
    FILE*        fp = NULL;
    long        len = 0;
    char* file_buff = NULL;
    int      status = EXIT_SUCCESS;
    FILE*    fp_out = NULL;
    unsigned int  i = 0;
    /* ------------------------- */
    
    if(argc == 1)
    {
        printHeader();
        printUsage(argv[0]);
        return status;
    }
    
    /* process commandline arguments */
    processCommandLine(argc, argv);
    
    if((fp = fopen(binary_file, "rb")) == NULL)
    {
        printf("öaa");
        printHeader();
        printUnableToReadBinary();
        status = EXIT_FAILURE;
        goto bail;
    }
    
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    /* allocate memory for binary file */
    if((file_buff = (char *)malloc((size_t)len)) == NULL)
    {
        printHeader();
        printUnableToAllocateMemory();
        status = EXIT_FAILURE;
        goto bail_1;
    }
    
    /* read binary */
    if(fread(file_buff, 1, len, fp) != len)
    {
        printHeader();
        printUnableToReadBinary();
        status = EXIT_FAILURE;
        goto bail_2;
    }
    
    printf("\n");
    printf("  [OK] - %s - binary successfully read.\n", binary_file);
    
    if(compress)
    {
    	/* compress */
    	long compress_len;
    	char* compress_buff = (char *)malloc((size_t)len);
    	compress_len = LZ_Compress( (unsigned char*)file_buff, (unsigned char*)compress_buff, len);
    	printf("Compressed %ld to %ld bytes\n", len, compress_len);
    	free(file_buff);
    	len = compress_len;
    	file_buff = compress_buff;
    }
    
    /* create/open output file */
    fp_out = fopen(header_file, "w+b");
    if(fp_out == NULL)
    {
        printHeader();
        printUnableToCreateHeaderFile();
        status = EXIT_FAILURE;
        goto bail_2;
    }
    
    printf("  [OK] - %s - header file ready for processing.\n", header_file);
    
    fprintf(fp_out, "#ifndef __%s_h__\n", blob_name);
    fprintf(fp_out, "#define __%s_h__\n", blob_name);
    fprintf(fp_out, "\n");
    fprintf(fp_out, "static FW_SECTION const bool %s_compress = %s;\n", blob_name, compress ? "true" : "false");
    fprintf(fp_out, "static FW_SECTION const unsigned char %s[] = \n", blob_name);
    fprintf(fp_out, "{\n");
    
    /* process all bytes */
    for(i = 1; i < len + 1; i++)
    {
        if((i - 1) % 8 == 0)
        {
            fprintf(fp_out, "    ");
        }
        
        fprintf(fp_out, "0x%02x", file_buff[i - 1] & 0xFF);
    
        if(i < len)
        {
            fprintf(fp_out, ", ");
        }
        else
        {
            fprintf(fp_out, "\n");
        }
        
        if(i % 8 == 0)
        {
            fprintf(fp_out, "\n");
        }
    }
    
    fprintf(fp_out, "};\n");
    fprintf(fp_out, "\n");
    fprintf(fp_out, "#endif /* __%s_h__ */\n", blob_name);
    fprintf(fp_out, "\n");
    fclose(fp_out);
    
    printf("  [OK] - successfully finished!\n");
    printf("\n");
    
    /* everything went fine */
bail_2:
    free(file_buff);
bail_1:
    fclose(fp);
bail:
    fflush(stdout);
    return EXIT_SUCCESS;
}
