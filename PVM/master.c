
#include <stdio.h>
#include <stdlib.h>
#include <pvm3.h>

int main()
{
  int myTID;
  int x = 12;
  int children[10];
  int res;

  myTID = pvm_mytid();
  printf("Master: TID is %d\n", myTID);

  res = pvm_spawn("slave", NULL, PvmTaskDefault, "", 1, children);
  if (res<1) {
    printf("Master: pvm_spawn error\n");
    pvm_exit();
    exit(1);
  }

  pvm_initsend(PvmDataDefault);
  pvm_pkint(&x, 1, 1);

  pvm_send(children[0], 1);

  pvm_recv(-1, -1);
  pvm_upkint(&x, 1, 1);

  printf("Master has received x=%d\n", x);

  pvm_exit();

  return 0;
}

