/*------------------------------------------------------------
 * rsplit.c : version 0.2.6 : mrl
 * 
 * Copyright (c) 2011 - 2016 Matthew Love <matthew.love@colorado.edu>
 * This file is liscensed under the GPL v.2 or later and 
 * is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details. 
 * <http://www.gnu.org/licenses/> 
 *--------------------------------------------------------------*/

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <getopt.h>
#include <time.h>

static char rsplit_version[] = "0.2.6";

/* Flag set by `--version'. */
static int version_flag;
//---

/*------------
 * Line-Count 
/*------------*/

int
linecnt(char* infile)
{
  char tmp[1024] = {0x0};
  int recordcnt = 0;
  FILE *in = fopen(infile, "r");
  
  if(in == NULL) {
    perror("File open error");
    exit(EXIT_FAILURE);
  }

  /* read a record and count the lines */
  while(fgets(tmp, sizeof(tmp), in) != 0) recordcnt++;

  fclose(in);
  return recordcnt; 
}

/*-----------
 * Read-File 
/*-----------*/

int
read_file(char* infile, int rperc, int want_percent, int prline, int remain) {
  FILE *in = fopen(infile, "r");
  char tmp[1024] = {0x0};
  char **strs;
  int recordcnt = 0, fcnt = linecnt(infile), rnum = rperc, bline = 0, brflag = 0;
  int i, j, rline;

  /* Let's check to make sure user entered rational values */
  if (want_percent == 0 && rnum >= fcnt) {
    fprintf(stderr,"rsplit: Error, you are attempting to extract more lines (%d) than are available (%d) in this file\n", rnum, fcnt);
    exit(EXIT_FAILURE);
  }
  else if (want_percent == 1 && rnum >= 100) {
    fprintf(stderr,"rsplit: Error, you are attempting to extract 100 percent or more lines from this file\n");
    exit(EXIT_FAILURE);
  }

  strs = malloc(fcnt * sizeof(*strs));

  /* Read in the file */
  if(in == NULL) {
    perror("File open error");
    exit(EXIT_FAILURE);
  }

  /* read a record */
  while(fgets(tmp, sizeof(tmp), in) !=0) {
    strs[recordcnt] = malloc(strlen(tmp) + 1);
    strcpy(strs[recordcnt], tmp);
    recordcnt++;
  }

  /* Write out the random lines */
  if (want_percent == 1) rnum = (fcnt * (rperc * 0.01));

  int rgot[rnum];

  srand(time(0));

  for (i = 0; i < rnum; i++) 
    {
      rline = rand() % fcnt;
      rgot[i] = rline;
      do {
	rline = rand() % fcnt;
      } while (strncmp(strs[rline], "none", 3) == 0);
      if (prline == 1) printf("%d %s", rgot[i], strs[rline]);
      else printf("%s", strs[rline]);
      strncpy(strs[rline], "none\n", strlen(strs[rline]));
  }

  /* Print the remaining lines to stderr if requested */
  if (remain == 1) {
    for (i = 0; i < fcnt; i++) {
      if (strncmp(strs[i], "none", 3) != 0) {
	if (prline == 1) {
	  fprintf(stderr, "%d %s", i, strs[i]);
	}
	else {
	  fprintf(stderr, "%s", strs[i]);
	}
      }
      else {
	brflag = 0;
      }
    }
  }
  fclose(in);
  free(strs);
  return 1;
}

void
usage()
{
  printf("Usage: rpslit [OPTIONS] [infile]\n\n\
Randomly extract a percentage of lines from a text file.\n\n\
Options:\n\
  -p, --percent \tThe percent of lines to randomly extract \n\t\t\tfrom the input.\n\
  -n, --number  \tIf set, will interpret the -p option as the total number of\n\
                \tlines to extract, rather than as a percentage.\n\
  -l, --line-num \tInclude line numbers in output.\n\
  -r, --remaining \tSend the lines that aren\'t extracted to stderr.\n\n\
  --version \t\tPrint version information and exit.\n\n\
Example: rsplit test.xyz -p 20 -lr > test_rand20.xyz 2> test_base.xyz\n\n\
");
}
//---

/*-----------
 * Main-Line 
/*-----------*/

//---
int
main (int argc, char **argv) {
  int c;
  char* fn;  
  int inflag = 0, pline_flag = 0, remaining_flag = 0,  percent_flag = 1;
  double rp=10;
  
  while (1) {
    static struct option long_options[] =
      {
	/* These options set a flag. */
	{"version", no_argument, &version_flag, 1},
	/* These options don't set a flag.
	   We distinguish them by their indices. */
	{"help", no_argument, 0, 'h'},
	{"percent", required_argument, 0, 'p'},
	{"number", required_argument, 0, 'n'},
	{"line-num", no_argument, 0, 'l'},
	{"remaining", no_argument, 0, 'r'},
	{0, 0, 0, 0}
      };
    /* getopt_long stores the option index here. */
    int option_index = 0;
    
    c = getopt_long(argc, argv, "hp:nlr",
		    long_options, &option_index);
    
    /* Detect the end of the options. */
    if (c == -1)
      break;
    
    switch (c) {
    case 0:
      /* If this option set a flag, do nothing else now. */
      if (long_options[option_index].flag != 0)
	break;
      printf("option %s", long_options[option_index].name);
      if (optarg)
	printf(" with arg %s", optarg);
      printf("\n");
      break;
    case 'h':
      usage();
      exit(1);
    case 'p':
      rp = atof(optarg);
      break;
    case 'n':
      percent_flag=0;
      break;
    case 'l':
      pline_flag=1;
      break;
    case 'r':
      remaining_flag=1;
      break;
    case '?':
      /* getopt_long already printed an error message. */
      
      break;	  
    default:
      abort();
    }
  }
     
  /* Instead of --
     -- as they are encountered,
     we report the final status resulting from them. */
  if (version_flag) {
    printf("rsplit, version %s\n\
Copyright (C) 2011 - 2016 Matthew Love <matth.love@colorado.edu>\n\
This file is liscensed under the GPL v.2 or later and\n\
is distributed in the hope that it will be useful,\n\
but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\
GNU General Public License for more details.\n\
<http://www.gnu.org/licenses/> \n\
", rsplit_version);
    exit (1);
  }
  
  /* Print any remaining command line arguments (not options). */
  if (optind < argc) {
    if (inflag == 0) {
      fn = (argv[optind++]);
      inflag++;
    }
    else {
      printf  ("non-option ARGV-elements: " );
      while (optind < argc) printf ("%s ", argv[ optind++ ]);
      putchar ('\n');
    }
  }
  if (inflag > 0) {
    read_file(fn, rp, percent_flag, pline_flag, remaining_flag);
  }
  else {
    usage();
    exit(EXIT_FAILURE);
  }
  exit(1);
}
//---END
