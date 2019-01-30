#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int chkdst(int argc, char **argv);
int isdir(char *path);
int isregular(char *path);
int copyfiles(int argc, char **argv);
int cp1file(char *srcpath, char *dstpath);
char *buildpath(char *src, char *dst);
int filexists(char *src, char *dst);

/*Driver class
* Checks if the last argument is a directory
* using the chkdst method
* If it is a directory then it starts
* the copy file process
*/
int main(int argc, char **argv) {
  if(chkdst(argc, argv)==1){
    copyfiles(argc, argv);
  }
  return 1;
}


/*Checks for correct number of arguments
* and then checks whether the path is a directory
* using isdir function
* Param: takes the command line args
*/
int chkdst(int argc, char **argv) {
  int i;
  char *path = argv[argc-1];
  int retval = isdir(path);

  if(argc <=2){
    fprintf(stderr, "Incorrect number of arguments\n");
    return 0;
  }
  else if(retval != 1){
    fprintf(stderr, "Path is not a directory\n");
    return 0;
  }
  else{
    return 1;
  }
}


/*Checks whether the path is a directory
* Param: path: file path specified in last arg
*/
int isdir(char *path){
  struct stat buf;
  stat(path, &buf);

  if(S_ISDIR(buf.st_mode) != 0){
    return 1;
  }
  else{
    return 0;
  }
}


/*Checks whether the path is a regular file
* Param: path: file path specified in command args
*/
int isregular(char *path){
  struct stat buf;
  stat(path, &buf);

  if(S_ISREG(buf.st_mode) != 0){
    return 1;
  }
  else{
    return 0;
  }
}


/*Iterates through command line args up to last arg
* and then checks if file exists in destination path
* already or if it is a regular file
* Param: command line arguments
*/
int copyfiles(int argc, char **argv){
  int i;
  char *path;

  for(i = 1; i <=argc-2; i++){
    path = argv[i];
    if(filexists(path, argv[argc-1])){
      fprintf(stderr, "%s\t: exists at destination\n", path);
    }
    else if(isregular(path)==1){
      cp1file(path, argv[argc-1]);
      fprintf(stderr, "%s\t: copied\n", path);
    }
    else{
      fprintf(stderr, "%s\t: not file or doesn't exist\n", path);
    }
  }
  return 1;
}


/*Reads the src file and writes it to outpath
* Uses buildpath to create path and allocate memory
* Param: srcpath: file to copy
         dstpath: directory to copy to
*/
int cp1file(char *srcpath, char *dstpath){
  int fdin, fdout, err, flag;
  unsigned char buf[4096];
  char *outpath = buildpath(srcpath, dstpath);

  fdin = open(srcpath, O_RDONLY);
  fdout = open(outpath, O_CREAT | O_WRONLY);
  err = read(fdin, buf, 4096);
  flag = err;
  err = write(fdout, buf, flag);
  if(err == -1){
    fprintf(stderr, "Error writing");
    free((void *) buf);
  }
  close(fdin);
  close(fdout);

  return 1;
}


/* Builds and allocates memory for file path
* Param: srcpath: file to copy
         dstpath: directory to copy to
*/
char *buildpath(char *src, char *dst){
  char *outpath;
  char *fname;

  if(strchr(src, '/') != NULL){
    fname = strrchr(src, '/');
    outpath = malloc(strlen(fname)+1+strlen(dst));
    strcpy(outpath, dst);
    strcat(outpath, "/");
    strcat(outpath, fname);
  }
  else{
    outpath = malloc(strlen(dst)+strlen(src) +1);
    strcpy(outpath, dst);
    strcat(outpath, "/");
    strcat(outpath, src);
  }
  return outpath;
}


/*Checks if the file to copy is already
* in the destination path
* Param: srcpath: file to copy
         dstpath: directory to copy to
*/
int filexists(char *src, char *dst){
  char *fpath;
  char *fname;
  struct stat buffer;

  if(strchr(src, '/') != NULL){
    fname = strrchr(src, '/');
    fpath = malloc(strlen(fname)+1+strlen(dst));
    strcpy(fpath, dst);
    strcat(fpath, "/");
    strcat(fpath, fname);
  }
  else{
    fpath = malloc(strlen(dst)+strlen(src) +1);
    strcpy(fpath, dst);
    strcat(fpath, "/");
    strcat(fpath, src);
  }
  return (stat(fpath, &buffer) ==0);
}
