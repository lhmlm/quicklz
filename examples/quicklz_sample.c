/*
 * File : quicklz_test.c
 * this example is a very simple test program for the quicklz library,
 * using non-stream compress and decompress. I
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "quicklz.h"

/*
  quicklz_compress_file using non-stream mode.
 */
static int quicklz_compress_file(FILE *ifile, FILE *ofile)
{
    /* Start to compress file  */
    char *src, *dst;
    qlz_state_compress *state_compress = (qlz_state_compress *)malloc(sizeof(qlz_state_compress));
    size_t len, len2;

    // allocate source buffer and read file
    fseek(ifile, 0, SEEK_END);
    len = ftell(ifile);
    fseek(ifile, 0, SEEK_SET);
    src = (char*) malloc(len);
    fread(src, 1, len, ifile);

    // allocate "uncompressed size" + 400 for the destination buffer
    dst = (char*) malloc(len + 400);

    // compress and write result
    len2 = qlz_compress(src, dst, len, state_compress);
    fwrite(dst, len2, 1, ofile);
    return 0;
}

/*
  quicklz_decompress_file using non-stream mode.
 */
static int quicklz_decompress_file(FILE *ifile, FILE *ofile)
{
    /* Start to decompress file  */
    char *src, *dst, *scratch;
    qlz_state_decompress *state_decompress = (qlz_state_decompress *)malloc(sizeof(qlz_state_decompress));
    unsigned int len;

    // allocate source buffer
    fseek(ifile, 0, SEEK_END);
    len = ftell(ifile);
    fseek(ifile, 0, SEEK_SET);
    src = (char*) malloc(len);

    // read file and allocate destination buffer
    fread(src, 1, len, ifile);
    len = qlz_size_decompressed(src);
    dst = (char*) malloc(len);

    // decompress and write result
    len = qlz_decompress(src, dst, state_decompress);
    fwrite(dst, len, 1, ofile);
    return 0;
}


int main(int argc, char ** argv)
{
    FILE *fd_in = NULL, *fd_out = NULL;
    int ret  = 0;

    if (argc != 4)
    {
        ret = -1;
        goto _print_usage;
    }

    fd_in = fopen(argv[2], "rb");
    if (fd_in < 0)
    {
        printf("[qlz] open the input file : %s error!\n", argv[2]);
        ret = -1;
        goto _exit;
    }

    fd_out = fopen(argv[3], "wb");
    if (fd_out < 0)
    {
        printf("[qlz] open the output file : %s error!\n", argv[3]);
        ret = -1;
        goto _exit;
    }

    if(memcmp("-c", argv[1], strlen(argv[1])) == 0)
    {

        ret = quicklz_compress_file(fd_in, fd_out);
        if(ret < 0)
        {
            printf("[qlz] quciklz compress file error!\n");
        }

    }
    else if(memcmp("-d", argv[1], strlen(argv[1])) == 0)
    {

        ret = quicklz_decompress_file(fd_in, fd_out);
        if(ret < 0)
        {
            printf("[qlz] quciklz decompress file error!\n");
        }
    }
    else
    {
        ret = -1;
        goto _print_usage;
    }
    goto _exit;


_print_usage:
    {
        printf("Usage:\n");
        printf("qlz_test -c [src file] [cmprs_file]      -compress \"src file\" to \"cmprs_file\" \n");
        printf("qlz_test -d [cmprs_file] [dcmprs_file]   -dcompress \"cmprs_file\" to \"dcmprs_file\" \n");
    }

_exit:
    if(fd_in != NULL)
    {
        fclose(fd_in);
    }

    if(fd_out != NULL)
    {
        fclose(fd_out);
    }

    return ret;
}
