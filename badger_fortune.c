#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char **argv){

  // Check if number of command-line arguments is correct.
  if(argc < 5){
    printf("USAGE: \n\tbadger-fortune -f <file> -n <number> (optionally: -o <output file>) \n\t\t OR \n\tbadger-fortune -f <file> -b <batch file> (optionally: -o <output file>)\n");
    exit(1);  
  }

  int fortune_num = 0; // the number of fortune from the fortune file
  int fortune_maxChar = 0; // the maximum number of fortune
  FILE *fortune_fp = NULL;
  FILE *batch_fp = NULL;
  FILE *output_fp = NULL;  
  int n_num = 0; // if specified first, becomes the number 
  int b_num = 0; // if specified first, becomes 1
  int o_num = 0; // if specified, becomes 1
  int f_exist = 0; // if exists, becoems 1;
  int flag = 0;
  char *line1 = NULL;
  size_t len1 = 0;
  extern int opterr;
  opterr = 0;
  char* b_arg = NULL;
  char* n_arg = NULL;
  char* f_arg = NULL;
  ssize_t read;
  ssize_t read2;
  char* fortune2 = NULL;
  char** matrix = NULL;

   // Check each argument one by one
   while ((flag = getopt(argc, argv, "f:b:o:n:")) != -1){

     switch(flag){

       case 'f':

                 f_arg = optarg;

                 // Fortune File Doesn't Exist
                 fortune_fp = fopen(f_arg, "r");
                 if (fortune_fp == NULL) {
                   printf("ERROR: Can't open fortune file\n");
                   if(output_fp != NULL){
                     fclose(output_fp);
                   }
                   if(batch_fp != NULL){
                     fclose(batch_fp); 
                   }

                   exit(1);
                 }

                 // Empty fortune file
                 fseek(fortune_fp, 0, SEEK_END);
                 int size = 0;
                 size = ftell(fortune_fp);
				 rewind(fortune_fp);
                 if(size == 0) {
                   printf("ERROR: Fortune File Empty\n");
                   fclose(fortune_fp);
                   if(output_fp != NULL){
                     fclose(output_fp);
                   }
                   if(batch_fp != NULL){
                     fclose(batch_fp);
                   }
                   exit(1);
                  }

                 f_exist = 1; // f exists
                 if ((read = getline(&line1, &len1, fortune_fp)) != -1) {
                   fortune_num = atoi(line1);
                 }

                 if ((read = getline(&line1, &len1, fortune_fp)) != -1) {
                   fortune_maxChar = atoi(line1);
                 } 

                 free(line1);
                 line1 = NULL; 
                 break;

      case 'b':

               b_num = 1;

               // n comes first
               if(n_arg != NULL){
                 printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                 if(fortune_fp != NULL){
                   fclose(fortune_fp);
                 }
                 if(output_fp != NULL){
                   fclose(output_fp);
                 }
                 exit(1);  
               }
               b_arg = optarg;

               // Batch File Doesn't Exist
               if(b_arg != NULL){
                 batch_fp = fopen(b_arg, "r");
                 if (batch_fp == NULL) {
                   printf("ERROR: Can't open batch file\n");
                   if(fortune_fp != NULL){
                     fclose(fortune_fp);
                   }
                   if(output_fp != NULL){
                     fclose(output_fp);
                   }
                   exit(1);
                   }

               // Empty batch file
               fseek(batch_fp, 0, SEEK_END);
               int size = ftell(batch_fp);
               rewind(batch_fp);
               if(size == 0) {
                 printf("ERROR: Batch File Empty\n");
                 fclose(batch_fp);
                 if(fortune_fp != NULL){
                   fclose(fortune_fp);
                 }
                 if(output_fp != NULL){
                   fclose(output_fp);
                 }
                 exit(1);
                }

                 }  
               break;

      case 'n':

               if(b_arg != NULL){
                 printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                 if(fortune_fp != NULL){
                   fclose(fortune_fp);
                 }
                 if(output_fp != NULL){
                   fclose(output_fp);
                 }
                 if(batch_fp != NULL){
                   fclose(batch_fp); 
                 }
                 exit(1);
               }
               n_arg = optarg;

               // check n
               if(n_arg != NULL){
                 n_num = atoi(n_arg);

                 // -n followed by number <= 0
                 if(n_num <= 0){
                   printf("ERROR: Invalid Fortune Number\n");
                   if(fortune_fp != NULL){
                     fclose(fortune_fp);
                   }
                   if(output_fp != NULL){
                     fclose(output_fp);
                   }
                   exit(1);
                 }

                // -n followed by number > number of fortunes
                if(f_arg != NULL && n_num > fortune_num){
                  printf("ERROR: Invalid Fortune Number\n");
                  if(fortune_fp != NULL){
                    fclose(fortune_fp);
                  }
                  if(output_fp != NULL){
                    fclose(output_fp);
                  }
                  exit(1);
                }
               }
               break; 

      case 'o':
               output_fp = fopen(optarg, "w");
               if (output_fp == NULL) {
                 if(fortune_fp != NULL){
                   fclose(fortune_fp);
                 }
                 if(batch_fp != NULL){
                   fclose(batch_fp); 
                 }
                 exit(1);
                }
               o_num = 1;
               break;

      // invalid flags
      default:

               if(strcmp(argv[argc - 1], "-f") == 0){
                 printf("ERROR: No fortune file was provided\n");
                 if(output_fp != NULL){
                   fclose(output_fp);
                 }
                 if(batch_fp != NULL){
                   fclose(batch_fp);
                 }
                 exit(1);  
               }

               printf("ERROR: Invalid Flag Types\n");
               if(fortune_fp != NULL){
                 fclose(fortune_fp);
               }
               if(output_fp != NULL){
                 fclose(output_fp);
               }
               if(batch_fp != NULL){
                 fclose(batch_fp); 
               }
               exit(1); 

    } 

  }

  if(f_exist == 0){
    // Fortune File not Specified
    if(f_arg == NULL){
      printf("ERROR: No fortune file was provided\n");
      if(batch_fp != NULL){
        fclose(batch_fp);
      }
      if(output_fp != NULL){
        fclose(output_fp);
      }
      exit(1);
    }
  }

  // check n
  if(n_arg != NULL){
                n_num = atoi(n_arg);

                // -n followed by number <= 0
                if(n_num <= 0){
                  printf("ERROR: Invalid Fortune Number\n");
                  if(fortune_fp != NULL){
                    fclose(fortune_fp);
                  }
                  if(output_fp != NULL){
                    fclose(output_fp);
                  }
                  exit(1);
                }

                // -n followed by number > number of fortunes
                if(n_num > fortune_num){
                  printf("ERROR: Invalid Fortune Number\n");
                  if(fortune_fp != NULL){
                    fclose(fortune_fp);
                  }
                  if(output_fp != NULL){
                    fclose(output_fp);
                  }
                  exit(1);
                }
  }

  // save fortune file
  matrix = malloc(fortune_num * sizeof(char*));
  for(int i = 0; i < fortune_num; i ++){
    *(matrix + i) = malloc((fortune_maxChar + 1) * sizeof(char)); 
  }

  fortune2 = malloc((fortune_maxChar + 1) * sizeof(char)); 
  int i = 0;
  char* buffer = NULL;
  fgets(fortune2, 100, fortune_fp);
  while((buffer = fgets(fortune2, 100, fortune_fp)) != NULL){
    if(strcmp(fortune2, "%\n") == 0){
      i ++;
      continue;
    }else{
      strcat(*(matrix + i), fortune2);}
    }

  free(buffer);
  buffer = NULL;

  // Case1) if number
  if(n_num != 0){

    // Case1.a) print out to file
    if(o_num == 1){

      fputs(*(matrix + n_num - 1), output_fp); 

    }else{ // Case1.b) print out to console  

      printf("%s", *(matrix + n_num - 1));

    }

  }

  // Case2) if batch file
  if(b_num != 0){

    // Case2.a) print out to file
    if(o_num == 1){

      char *line2 = NULL;
      size_t len2 = 0;
      while(getline(&line2, &len2, batch_fp) != -1){

        if(atoi(line2) <= 0 || atoi(line2) > fortune_num){
          printf("ERROR: Invalid Fortune Number\n\n");
        }else{
          fputs(*(matrix + atoi(line2) - 1), output_fp);
          fputs("\n\n", output_fp);
        }
     }
     free(line2);
     line2 = NULL; 
 
    }else{ // Case2.b) print out to console  

      char *line2 = NULL;
      size_t len2 = 0;
      while((read2 = getline(&line2, &len2, batch_fp)) != -1){

        if(atoi(line2) <= 0 || atoi(line2) > fortune_num){
          printf("ERROR: Invalid Fortune Number\n\n");
        }else{

          printf("%s", *(matrix + atoi(line2) - 1));
          printf("\n\n");
        } 
      }
        free(line2);
        line2 = NULL;
    }

  }

  // free and close everything
  if(batch_fp != NULL){
    fclose(batch_fp);
  }

  if(fortune_fp != NULL){
    fclose(fortune_fp);
  }

  if(output_fp != NULL){
    fclose(output_fp);
  }

  free(fortune2);
  fortune2 = NULL;

  for(int i = 0; i < fortune_num; i ++){
    free(*(matrix + i));
    *(matrix + i) = NULL;
  }

  free(matrix);
  matrix = NULL;

  b_arg = NULL;
  n_arg = NULL;
  f_arg = NULL;

  fortune_fp = NULL;
  batch_fp = NULL;
  output_fp = NULL;

  return 0;
}

//complete
