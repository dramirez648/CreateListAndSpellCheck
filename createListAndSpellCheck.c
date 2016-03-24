/*-------------------------------------------------------------------------*
 *---									---*
 *---		createListAndSpellCheck.c				---*
 *---									---*
 *---	    This file defines a C program that lets a user create,	---*
 *---	print, list and spell-check text files.				---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1.0		2016 February 23	Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>	// strchr(), strstr()
#include	<dirent.h>	// opendir(), readdir(), closedir()
#include	<sys/types.h>	// open(), read(), write(), close(), stat()
#include	<sys/stat.h>	// open(), read(), write(), close(), stat()
#include	<fcntl.h>	// open(), read(), write(), close()
#include	<unistd.h>	// stat()
#include	<ctype.h>	// toupper()


#define		BUFFER_LEN		256
#define		SPELLER_PROGNAME	"/usr/bin/aspell"
#define		SPELLER_PROG_OPTION	"list"
#define		ENDING_TEXT		"xxxyyyzzz"

extern int errno;

//  PURPOSE:  To ask the user the yes/no question given in 'questionText',
//	get a legal (y)es or (n)o reply, and to return '1' if the user entered
//	yes or '0' otherwise.
int		yesNo		(const char*	questionText
				)
{
  //  I.  Application validity check:

  //  II.  Repeatedly ask until get valid response:
  char	text[BUFFER_LEN];

  do
  {
    printf("%s (Y/N)? ",questionText);
    fgets(text,BUFFER_LEN,stdin);
    text[0]	= toupper(text[0]);
  }
  while  ( (text[0] != 'Y')  &&  (text[0] != 'N') );

  //  III.  Finished:
  return(text[0] == 'Y');
}


//  PURPOSE:  To let the user enter a filepath into a static buffer of length
//	'BUFFER_LEN', to remove the ending '\n', and to return the static
//	address.
char*		getFilePath	()
{
  //  I.  Application validity check:

  //  II.  Ask for file path:
  static
  char	filePath[BUFFER_LEN];

  //  II.A.  Ask for and get file path:
  printf("File path? ");
  fgets(filePath,BUFFER_LEN,stdin);

  //  II.B.  Remove ending '\n':
  char*	cPtr	= strchr(filePath,'\n');

  if  (cPtr != NULL)
    *cPtr = '\0';

  //  III.  Finished:
  return(filePath);
}


//  PURPOSE:  To open the current directory ("."), and the names of all files
//	and directories in it.  Prints "Could not open current directory\n"
//	to 'stderr' and immediately returns if current directory cannot be
//	opened.  No return value.

void		listFiles	()
{
  //  I.  Application validity check:

  //  II.  List items in directory ".":

  char buffer[BUFFER_LEN];
  DIR *fd = opendir(".");
   
  struct dirent* dir; 
  while ((dir=readdir(fd)) != NULL) {
 	printf("%s\n", dir->d_name);
  }
  closedir(fd);
  //  III.  Finished:
}


//  PURPOSE:  To print the contents of the file whose path is given by
//	'filePath'.  If 'filePath' cannot be opened then prints
//	"Could not read <filePath>\n" to 'stderr' and immediately returns.
//	No return value.
void		printFile	(char*	filePath
				)
{
  //  I.  Application validity check:
  if  (filePath == NULL)
  {
    fprintf(stderr,"BUG: NULL ptr to printFile()\n");
    exit(EXIT_FAILURE);
  }

  //  II.  Print file:
  
  char buffer[BUFFER_LEN];
  int fd = open(filePath, O_RDONLY, 0440);
  
  int i;
  do {
	  i = read(fd, buffer, BUFFER_LEN);
	  printf("%s", buffer);
          }
  while (i == BUFFER_LEN);
  if(i == -1) {
  exit(EXIT_FAILURE);
  }	
  close(fd);
  exit(EXIT_SUCCESS);
  //  III.  Finished:
}


//  PURPOSE:  To let the user enter text to write (or over-write) the text
//	file 'filePath'.  The user signifies they are finished entering lines
//	by entering a blank line.
//	(1) If 'filePath' does *not* exist then the file may be created.
//	(2) If 'filePath' does exist *and* is a "regular file" then
//	    over-writing the file may only procede if the user verifies
//	    they are willing to over-wrirte that file (use 'yesNo()').
//	(3) If 'filePath' does exist *but* it is *not* a "regular file"
//	    (e.g. it is a directory) then
//	    "Attempt to overwrite a non-file!\n" is print to 'stderr' and
//	    it immediately returns.
//	Prints "Could not write to <filePath>.\n" to 'stderr' if could not
//	write to file.  No return value.
//
//	HINTS:
//	For (1): A blank line will have '\n' as its first char.
//	For (2) and (3): Use 'stat()' and 'S_ISREG()'.
void    enterFile (char*  filePath
        )
{
  //  I.  Application validity check:
  if  (filePath == NULL)
  {
    fprintf(stderr,"BUG: NULL ptr to enterFile()\n");
    exit(EXIT_FAILURE);
  }

        
  //  II.  Enter file:
  //printf("%s",filePath);
  //  YOUR CODE HERE
  char buffer[BUFFER_LEN];  
  // int file = open(filePath, O_WRONLY | O_CREAT | O_TRUNC, 0660);
  struct stat statbuf;
  memset(&statbuf,0,sizeof(struct stat));
  
  int test = stat(filePath, &statbuf);
  //printf("%d\n",test);
  
  if(test == -1){
    
    printf("Please type the lines of the file.\n");
    printf("Enter a blank line to quit:\n");
    int file = open(filePath, O_WRONLY | O_CREAT | O_TRUNC, 0660);
    
    int chrctr;
    while((chrctr = fgetc(stdin)) != '\n'){
      if(chrctr == '\n'){
        break;
      }
      while(fgets(buffer, BUFFER_LEN-1, stdin) != NULL){
        write(file,buffer, strlen(buffer));
      close(file);
      return;
      }
    }
    
    //close(file);
  }
  
  else{
    if(S_ISDIR(statbuf.st_mode)){
      fprintf(stderr, "Attempt to overwrite a non-file!\n");
      return;
    }

    if(S_ISREG(statbuf.st_mode));
      int choice = yesNo("WARNING: That files exists!  Overwrite it?");
      
      if(choice == 1){
        int file = open(filePath, O_WRONLY | O_CREAT | O_TRUNC, 0660);
  write(file, buffer, BUFFER_LEN);
        close(file);
      }
      if(choice == 0){
        fprintf(stderr, "Did not overwrite!\n");
        return;
      }
    
   
  }
  //  III.  Finished:
}
//  PURPOSE:  This function opens 'filePath' as a file descriptor in
//  read-write mode.  It does this to put 'ENDING_TEXT' (a purposeful
//   misspelling) at the end of the file.  This function also opens a pipe
//  for the child process to communicate back to the parent.  If opening
//  either 'filePath' or the pipe fails then it prints
//  "Error opening <filePath> or creating pipes\n" to 'stderr' and returns.
//    This function then 'fork()'s a child process.  The child process
//  has its input re-directed from the 'filePath' file descriptor, and its
//  output re-directed to the output side of the pipe.  Then, the child
//  process executes program 'SPELLER_PROGNAME' with command line argument
//  'SPELLER_PROG_OPTION'.
//    If the child process cannot execute the program, then it prints
//  "Could not find <SPELLER_PROGNAME>\n" to 'stderr' and does:
//    exit(EXIT_FAILURE);
//
//    Meanwhile, the parent process continually reads from the input end
//  of the pipe if it finds 'ENDING_TEXT' it knows to print up to that
//  occurrence, and then stop the loop.
//    The parent after the reading loop, the parent process should
//  'wait()' for the child process to end, remove 'ENDING_TEXT' from
//  the 'filePath' file descriptor, and close that file descriptor and the
//  pipe.
//
//  HINTS:
//  (*) To look for one string inside of another, use 'strstr()'
void    spellCheckFile  (char*  filePath
        )
{
  //  I.  Application validity check:
  if  (filePath == NULL)
  {
    fprintf(stderr,"BUG: NULL ptr to spellCheckFile()\n");
    exit(EXIT_FAILURE);
  }

  //  II.  Spell-check file:
  //  II.A.  Open file and pipe to get output of spell-checking process:
  int fromSpeller[2];
  int inFileFd;

  //  YOUR CODE HERE
  inFileFd = open(filePath, O_RDWR, 0660);
  pipe(fromSpeller);

  if((inFileFd < 0) || (pipe(fromSpeller) < 0 )){
    
    fprintf(stderr, "Error opening <%s> or creating pipes\n", filePath);
    return;
  
  }

  
  //  II.B.  Append 'ENDING_TEXT' (a purposefully mispelling) to end of file
  //       so parent process can detect end-of-output of child process:
  long  length     = lseek(inFileFd,0,SEEK_END);
  int status     = write(inFileFd,ENDING_TEXT,sizeof(ENDING_TEXT)-1);

  //  II.C.  Create spell-checking process and have it run spell-checker:

  pid_t spellerPid;

  //  YOUR CODE HERE
  spellerPid = fork();

  if  (spellerPid == 0)
  {
    lseek(inFileFd,0,SEEK_SET);
    close(0);
    dup(inFileFd);
    close(1);
    dup(fromSpeller[1]);
    //  YOUR CODE HERE
    
    execl(SPELLER_PROGNAME, SPELLER_PROGNAME, SPELLER_PROG_OPTION, NULL);
    fprintf(stderr, "Could not find <SPELLER_PROGNAME>\n");
    exit(EXIT_FAILURE); 
  }

  //  II.D.  Read response from spell-checking child process:

  //  YOUR CODE HERE
  while(1){
    char buffer[250];
    read(fromSpeller[0], buffer, 250);
    char* substring;
    substring = strstr(filePath, ENDING_TEXT);
    if(substring){
      break;
    }
  
  printf("The misspellings of %s are:\n", filePath);
  printf(buffer);
  return;  
  }
      
  //  II.E.  Wait for child process to officially end:

  //  YOUR CODE HERE
  int pid = wait(&status);
  return;
  //  II.F.  Remove 'ENDING_TEXT' from end of file:
  ftruncate(inFileFd,length);

  //  II.G.  Close file and pipe:

  //  YOUR CODE HERE
  close(inFileFd);
  close(fromSpeller[2]);
  
  //  III.  Finished:
}
//  PURPOSE:  To let the user decide what they want to do, and to call the
//  appropriate function to do it.  Ignores command line arguments.
//  Returns 'EXIT_SUCCESS' to OS.
int   main    ()
{
  //  I.  Application validity check:

  //  II.  Do commands of user until they decide to quit:
  int option  = !0;
  enterFile("test.txt");
//  while  (option != 0)
  while (0) 
  {

    //  II.A.  Obtain a legal command from user:
    do
    {
      char  optionText[BUFFER_LEN];

      printf
  ("\n"
   "(1) List files\n"
   "(2) Print a file\n"
   "(3) Enter a file\n"
   "(4) Spell check a file\n"
   "(0) Quit\n"
   "Your choice? "
  );
      fgets(optionText,BUFFER_LEN,stdin);
      option  = atoi(optionText);
    }
    while  ( (option < 0)  ||  (option > 4) );

    //  II.B.  Take action decided by user:
    switch  (option)
    {
    case 1 :
      listFiles();
      break;
    case 2 :
      printFile(getFilePath());
      break;
    case 3 :
      enterFile(getFilePath());
      break;
    case 4 :
      spellCheckFile(getFilePath());
      break;
    }
  }

  //  III.  Finished:
}

