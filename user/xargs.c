#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void run(char* program, char** args){
    if (fork() == 0)
        exec(program, args);

    return;
}

int main(int argc, char** argv){


    // char* argsbuf[]: buffer for the individual parameter

    char buf[2048];
    char* p = buf, *last_p = buf;
    char* argsbuf[128];

    char** args = argsbuf;


    for (int i = 1; i < argc; ++i){
        *args = argv[i];
        //Note that argv[1] is stored for the value of the program
        args++;         
    }

    char** current_p = args; // Current parameter (initial)

    //Note here the while loop indicates the process of reading the command from scratch

    /*Like: echo hello too ...*/

    /*To emphasize, p in the loop refers to "hello too" in the example above*/

    while (read(0, p, 1) != 0){

        if (*p == ' ' || *p == '\n'){
            char temp = *p;
            *p = '\0';

            *(current_p++) = last_p;

            if (temp == '\n'){
                *current_p = 0;
                run(argv[1], argsbuf);
                current_p = args;
            }

            last_p = p+1;
        }

        p++;
    }

    //The case where current_p != args can be triggered
    //in this example: echo -n "hello world" | xargs echo foo

    if (current_p != args){
        *p = '\0';
        *(current_p++) = last_p;
        *current_p = 0;
        run(argv[1], argsbuf);
    }

    while (wait(0) != -1);

    return 0;
}