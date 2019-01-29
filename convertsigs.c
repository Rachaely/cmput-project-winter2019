#define _GNU_SOURCE
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_TEXT 20


int ascii, element_pos = 0, start = 0;
char res[MAX_TEXT];
//ascii is the ascii number of the letter sent though signals, 8 bit
//element_pos points the 8 bit of binary ascii from left to right
void handler(int signal)
{

    if (signal == SIGUSR1)
    {

        ascii += 1 << (7 - element_pos);
    }
    else if (signal == SIGUSR2)
    {
    }
    if (++element_pos == 8)
    {
        element_pos = 0;
    }

    //printf("pointer is %d, ascii is %d\n",element_pos, ascii);
    if (element_pos == 0)
    {
        res[start] = ascii;
        start++;

        
        if (ascii == 10)
        {
            printf("!%s\n",res);
            start = 0;
            for(int i = 0;i<strlen(res) +1;i++){
                res[i] = 0;
            }
            ascii = 0;
            element_pos = 0;
        }
        ascii = 0;
        element_pos = 0;
        
    }
}

int main(int argc, char const *argv[])
{

    printf("Own PID: %d\n", getpid());
    // print pid of this process

    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_flags = SA_NODEFER;
    //copy form lab

    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
        #if defined(SINGLE)
        int single_mode = 1;
    #else
        int single_mode = 0;
    #endif

    int sent = 0;

    pid_t pid;
    scanf("%d", &pid);
    //get the pid want to commnicate with

    while (1)
    {

        char sent_message[MAX_TEXT];

        fgets(sent_message,MAX_TEXT,stdin);

        //check if user enter a word
        if (sent_message[0] >= 33)
        {
            //strcat(sent_message, "\n");

            for (int i = 0; i < strlen(sent_message); i++)

            //for each letter in the message
            {

                for (int j = 7; j >= 0; --j)
                //encode letter into binary, order of bit is from left to right
                {
                    if (sent_message[i] & (1 << j))
                    {
                        //printf("1");
                        kill(pid, SIGUSR1);
                    }
                    else
                    {
                        //printf("0");
                        kill(pid, SIGUSR2);
                    }
                    
                    usleep(300);

                    //putchar((sent_message[i]&(1<<j))?'1':'0');
                }
                
            }
            
            
        }
        for(int i = 0; i <strlen(sent_message);i++){
                sent_message[i] = 0;
            }
    }

    return 0;
}
