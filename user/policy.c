#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Make sure you run the program with one argument!");
    return -1;
  }
  int policy_code = atoi(argv[1]);
  int result = set_policy(policy_code);

  if (result == -1) 
    printf("Error setting policy\n");
   else  if (result == 0) 
    printf("Policy set successfully\n");
  
  exit(0,"");
}