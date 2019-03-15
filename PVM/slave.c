
The code can be downloaded from: slave.c:

#include <stdio.h>
#include <pvm3.h>

int main()
{
  int myTID, masterTID;
  int x = 12;

  myTID = pvm_mytid();
  printf("Slave: TID is %d\n", myTID);

  pvm_recv(-1, -1);
  pvm_upkint(&x, 1, 1);
  printf("Slave has received x=%d\n", x);

  sleep(3);
  x = x + 5;

  pvm_initsend(PvmDataDefault);
  pvm_pkint(&x, 1, 1);

  pvm_send(pvm_parent(), 1);

  pvm_exit();

  return 0;
}

