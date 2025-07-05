#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int matchhere(char*, char*);
int matchstar(int, char*, char*);

int match(char* re, char* text){
    if (re[0] == '^')
        return matchhere(re+1, text);

    do{  // must look at empty string
        if (matchhere(re, text))
            return 1;
    }while(*text++ != '\0');

  return 0;
}

// matchhere: search for re at beginning of text
int matchhere(char* re, char* text){
  if (re[0] == '\0')
    return 1;

  if (re[1] == '*')
    return matchstar(re[0], re+2, text);

  if (re[0] == '$' && re[1] == '\0')
    return *text == '\0';

  if (*text!='\0' && (re[0]=='.' || re[0]==*text))
    return matchhere(re+1, text+1);

  return 0;
}

// matchstar: search for c*re at beginning of text
int matchstar(int c, char* re, char* text){
  do{  // a * matches zero or more instances
    if (matchhere(re, text))
      return 1;
  }while(*text!='\0' && (*text++==c || c=='.'));

  return 0;
}

void find(char* path, char* target){
    char buf[512];
    char* p;
    int fd;

    struct dirent de;
    struct stat st;

    //Check for opening the directory

    if ((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    //Check for the current status of the directory

    if (fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    //Check the type of the file

    switch (st.type){

        //If is file

        case T_FILE:

            if (match(target, path))
                printf("%s\n", path);
            break;

        case T_DIR:

        /*
            strlen(path): Gets the length of the current path
            +1: Accounts for a path separator character (/)
            + DIRSIZ: Adds space for the maximum filename length (which we know is 14 from the fs.h definition)
            + 1: Accounts for the null terminator (\0) at the end of the string
            sizeof buf: The total size of the buffer that will store the complete path
        */

            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("find: path too long\n");
                break;
            }

            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';

            while (read(fd, &de, sizeof(de)) == sizeof(de)){
                if (de.inum == 0) // In xv6's filesystem, an inode number of 0 indicates an unused directory entry
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;

                if (stat(buf, &st) < 0){
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }

                if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0)
                    find(buf, target);
            }

            break;
    }

    close(fd);
}

int main(int argc, char** argv){
    if (argc < 3)
        exit(0);

    find(argv[1], argv[2]);

    return 0;
}