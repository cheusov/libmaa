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
      err_fatal(__func__, "fatal error #%d\n", 56);
      break;
   case 2:
      err_fatal(NULL, "fatal error #%d\n", 711);
      break;
   case 3:
      err_set_program_name(NULL);
      err_fatal(NULL, "fatal error #%d\n", 75);
      break;
   case 4:
      read(100, NULL, 0);
      err_fatal_errno(__func__, "fatal error #%d\n", 17);
      break;
   case 5:
      err_warning(__func__, "warning #%d\n", 11);
      break;
   case 6:
      err_warning(NULL, "warning #%d\n", 11);
      break;
   case 7:
      err_internal(NULL, "internal error #%d\n", 71);
      break;
   case 8:
      err_internal(NULL, "internal error #%d\n", 71);
      break;
   default:
      fprintf(stderr, "incorrect argument '%s' was specified\n", argv[1]);
      return 2;
   }

   maa_shutdown();

   return 0;
}
