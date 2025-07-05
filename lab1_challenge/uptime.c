#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){

    printf("%d\n", uptime());

    return 0;
}