/*
 *  shell.c
 *  Example program used to demonstrate how to call O/S
 *  commands from PL/SQL using external procedures. 
 */  

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int mailx(char *to, char *subject, char *message) {

    char command[5000];

    strcpy(command, "echo \"");
    strcat(command, message);
    strcat(command, "\" | mailx -s \"");
    strcat(command, subject);
    strcat(command, "\" ");
    strcat(command, to);

    return system(command);
}

char *stresc(char *str, char chr)
{
    static char buffer[4096];
    buffer[0] = '\0';
    const char rep[3] = {'\\', chr, '\0'};
    const char del[2] = {chr, '\0'};
    size_t len = strlen(str);

    //if (str[strlen(str)-1] == chr)
    //    strcat(str, "\x04");  

    char *next = strchr(str, chr);
    char *pos = str;
    //printf( "str=%s del=%s next=%s len=%d \n", str, del, next, strlen(pos));
    while (next)
    {
        int len = next-pos;

        //printf( "pos=%.*s len=%d\n", len, pos, len);
        if (len)
            sprintf(buffer+strlen(buffer), "%s%.*s", (pos>str ? rep:""), len, pos);

        pos = next + 1;
        if (pos[0] == '\0')
        {
            strcat(buffer, rep);
            next = NULL;
        }
        else
        {
            next=strchr(pos, chr);
            if (!next)
                sprintf(buffer+strlen(buffer), "%s%s", rep, pos);
        }
    }

    return (strlen(buffer) ? buffer:str);
}

void shell(char *command, char *msg) 
{
    int ret = system(command);
    /*strcpy(msg, "$(date +%y%m%d_%H%M%S)");*/

    sprintf(msg, "echo \"$(/bin/date +%%y%%m%%d_%%H%%M%%S): command=%s ret=%d\" >> /tmp/extproc.tmp ", stresc(command, '"'), ret);
    system(msg);
}

int main(int argc, char **argv)
{
    char *tmp = strdup((const char*)argv[1]);
    
    printf("stresc(%s)=++%s++\n", argv[1], stresc(tmp, '"')); 

    return 0;
}
