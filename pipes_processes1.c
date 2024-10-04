// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 


int main() 
{ 
    /*
    todo: 
    - ensure you can read text with spaces (scanf can't atm)
    - read more than once from child process without having to wait(NULL) in main process

    */

    // We use two pipes  
    int fd1[2];  // file descriptor for first pipe. Used to to send input string from parent 
    int fd2[2];  // file descriptor for second pipe. Used to send concatenated string from child 
  
    char fixed_str[] = "howard.edu"; 
    char input_str[100]; 
    char input_str2[100]; 
    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("Input: ");
    scanf("%s", input_str); 
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    
    else if (p > 0) { // P1, Parent process  
        char output_str[100];
        char output_str2[100];         
        close(fd2[1]); // Close writing end of pipe that will be use to read from child process
        close(fd1[0]);  // Close reading end of pipe used to write to child process


        // Write input string
        write(fd1[1], input_str, strlen(input_str)+1); 
        read(fd2[0], output_str, 100); // read concat string from P2/child process
        
        wait(NULL); // wait for child process to end
        read(fd2[0], output_str2, 100); // read second input string from P2/child process

        // concatenate with fixed string
        int k = strlen(output_str);
        int i; 
        for (i=0; i<strlen(output_str2); i++) 
            output_str[k++] = output_str2[i]; 
        output_str[k] = '\0';   // string ends with '\0' 

        // print new string
        printf("Output: %s\n", output_str);

        // Close remaining open ends of pipes
        close(fd2[0]); 
        close(fd1[1]); 
    } 
  
    // child process 
    else
    { 
        close(fd1[1]); //  Close writing end of pipe for reading from P1/parent process
        close(fd2[0]);  //  Close reading end of pipe for writing to P1/parent process
      
        // Read a string using first pipe 
        char concat_str[100]; 
        read(fd1[0], concat_str, 100); 
  
        // Concatenate a fixed string with it 
        int k = strlen(concat_str); 
        int i; 
        for (i=0; i<strlen(fixed_str); i++) 
            concat_str[k++] = fixed_str[i]; 
        concat_str[k] = '\0';   // string ends with '\0' 
  
        printf("Output: %s\n", concat_str); // print output
        write(fd2[1], concat_str, strlen(concat_str)+1); // write concatenated string to pipe so that P1/parent process can read it 
        
        printf("Input: ");
        scanf("%s", input_str2); 
        input_str2[strlen(input_str2)] = '\0';
        write(fd2[1], input_str2, strlen(input_str2)+1); // write concatenated string to pipe so that P1/parent process can read it 

        // Close remaining open ends of pipes
        close(fd1[0]); 
        close(fd2[1]); 

        // exit process
        exit(0); 
    } 
} 