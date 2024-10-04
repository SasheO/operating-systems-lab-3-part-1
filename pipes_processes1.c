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
    to do: read input string in child, pass both input str and concat str to p1, print out

    - write from child, send signal (parent should have signal listener registered)
    - write from child again, concatenate and print
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
        close(fd2[1]); // Close writing end of pipe that will be use to read from child process
        close(fd1[0]);  // Close reading end of pipe used to write to child process
        
  
        // Write input string
        write(fd1[1], input_str, strlen(input_str)+1); 
  
        // Wait for child to send a string 
        wait(NULL); 
        
        read(fd2[0], output_str, 100); // read string from P2/child process

        // concatenate with fixed string
        int k = strlen(output_str);
        int i; 
        for (i=0; i<strlen(input_str2); i++) 
            output_str[k++] = input_str2[i]; 
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
        scanf("%s", input_str); 

        // Close remaining open ends of pipes
        close(fd1[0]); 
        close(fd2[1]); 

        // exit process
        exit(0); 
    } 
} 