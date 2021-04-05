#include "head.h"

char *get_value(char *path, char *target) {
    FILE *fp = NULL;
    if ((fp = fopen(path, "r")) == NULL) {
        perror("fopen");
        exit(1);
    }
    char buff[512] = {0};
    int ind = 0;
    while (fgets(buff, 1024, fp)) {
        char *temp = (char *)malloc(sizeof(1024));
        char *index = NULL;
        if ((index = strstr(buff, target)) == NULL) continue;
        strcpy(temp, index + strlen(target) + 1);
        temp[strlen(temp) - 1] = '\0';
        return temp;
    }
    return NULL;
}


