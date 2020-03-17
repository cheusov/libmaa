#include "maaP.h"

int main(int argc, char **argv)
{
   int num;

   if (argc == 1){
      fprintf(stderr, "argument is expected\n");
      return 2;
   }

   num = atoi(argv[1]);

   maa_init(argv [0]);

   switch (num){
   case 1:
      err_fatal(__func__, "qqq\n");
      break;
   default:
      fprintf(stderr, "incorrect argument '%s' was specified\n", argv[1]);
      return 2;
   }

   maa_shutdown();

   return 0;
}
