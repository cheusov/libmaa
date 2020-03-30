#include "maaP.h"

int main(int argc, char **argv)
{
   int num;
   int n;

   if (argc == 1){
      fprintf(stderr, "argument is expected\n");
      return 2;
   }

   num = atoi(argv[1]);
   fprintf(stderr, "=== errtest %d\n", num);

   maa_init(argv [0]);

   log_file ("errtest", "log.log");

   switch (num){
   case 1:
      err_fatal(__func__, "fatal error #%d", 56);
      break;
   case 2:
      err_fatal(NULL, "fatal error #%d", 711);
      break;
   case 3:
      err_set_program_name(NULL);
      err_fatal(NULL, "fatal error #%d", 75);
      break;
   case 4:
      n = read(100, NULL, 0);
      if (n == -1)
	 err_fatal_errno(__func__, "fatal error #%d", 17);
      break;
   case 5:
      err_warning(__func__, "warning #%d", 11);
      break;
   case 6:
      err_warning(NULL, "warning #%d", 11);
      break;
   case 7:
      err_internal(NULL, "internal error #%d", 71);
      break;
   case 8:
      err_internal(NULL, "internal error #%d", 71);
      break;
   case 11:
      err_fatal(__func__, "fatal error #%s", "56");
      break;
   case 12:
      err_fatal(NULL, "fatal error #%s", "711");
      break;
   case 13:
      err_set_program_name(NULL);
      err_fatal(NULL, "fatal error #%s", "75");
      break;
   case 14:
      read(100, NULL, 0);
      err_fatal_errno(__func__, "fatal error #%s", "17");
      break;
   case 15:
      err_warning(__func__, "warning #%s", "11");
      break;
   case 16:
      err_warning(NULL, "warning #%s", "11");
      break;
   case 17:
      err_internal(NULL, "internal error #%s", "71");
      break;
   case 18:
      err_internal(NULL, "internal error #%s", "71");
      break;
   default:
      fprintf(stderr, "incorrect argument '%s' was specified", argv[1]);
      return 2;
   }

   maa_shutdown();

   return 0;
}
