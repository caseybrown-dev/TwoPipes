#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define SIZE 100
#define WRITE_END   1
#define READ_END     0

int main(void)
{
    char Write_Msg[SIZE];
    char Read_Msg[SIZE];
    char Mod_Msg[SIZE];
    int fd1[2];
    int fd2[2];
    pid_t pid;
    /* Create a message */
    printf ("Please input a string: \n");
    fgets (Write_Msg, SIZE, stdin);
    
    /* Create pipes */
    if (pipe(fd1) == -1)
    {
        fprintf(stderr,"Pipe 1 failed");
        return 1;
    }
    fprintf(stderr,"Pipe 1 created\n");
    if (pipe(fd2) == -1)
    {
        fprintf(stderr,"Pipe 2 failed");
        return 1;
    }
    fprintf(stderr,"Pipe 2 created\n");
    
    /* Fork a child process */
    fflush(0);
    pid = fork();
    
    if (pid < 0)
    {
        fprintf(stderr, "Fork failed");
        return 1;
    }
    /* The parent process */
    if (pid > 0)
    {
        close(fd1[READ_END]);
        close(fd2[WRITE_END]);
        
        printf ("Parent: Write message to pipe 1\n");
        write (fd1[WRITE_END], Write_Msg,
               strlen(Write_Msg)+1);
        
        close (fd1[WRITE_END]);
        wait(NULL);
        
        read (fd2[READ_END], Mod_Msg, SIZE);
        printf ("Parent: Read message from pipe 2:%s", Mod_Msg);
        close(fd2[READ_END]);
    }
    /* The child process */
    else
    {
        close(fd1[WRITE_END]);
        read (fd1[READ_END], Read_Msg, SIZE);
        printf ("Child: Read message from pipe 1:%s", Read_Msg);
        /* Reverse Case */
        int i=0;
        while(Read_Msg[i]!='\0'){
            if(Read_Msg[i]>='a'&&Read_Msg[i]<='z'){
                Read_Msg[i]-=32;
            }
            else if(Read_Msg[i]>='A'&&Read_Msg[i]<='Z'){
                Read_Msg[i]+=32;
            }
            i++;
        }
        close(fd2[READ_END]);
        printf("Child: Write message to pipe 2\n");
        write (fd2[WRITE_END], Read_Msg,strlen(Read_Msg)+1);
        close(fd2[WRITE_END]);
        close (fd1[READ_END]);
    }
            
    return 0;
    
}
