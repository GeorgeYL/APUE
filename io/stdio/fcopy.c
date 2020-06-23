
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char**argv)
{
   FILE* fps;
   FILE* fpd;
   
   if(argc < 3)
   {
      fprintf(stderr, "Usage: %s <sourceFile> <DestFile>\n", argv[0]);
      exit(1);
   }

   fps= fopen(argv[1], "r");
   if(fps == NULL)
   {
    	perror("fopen():");
        fprintf(stderr, "fopen() failes:%s\n", strerror(errno));
        exit(1);
   }
   fpd = fopen(argv[2], "w");
   if(fpd == NULL)
   {
    	perror("fopen():");
        fprintf(stderr, "fopen() failes:%s\n", strerror(errno));

        fclose(fps);
        exit(1);
   } 
   char buf[1024];
   int cnt;
   while((cnt = fread(buf, 1, 1024, fps)) > 0)
       fwrite(buf, 1, cnt, fpd);
   
   fclose(fpd);
   fclose(fps);


    exit(0);
}







