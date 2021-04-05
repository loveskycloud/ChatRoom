#include "head.h"

char path[] = "../conf/server.conf";

int main()
{
    printf("***main***\n");
    printf("%s", get_value(path, (char *)"Server_Name"));
    return 0;
}

