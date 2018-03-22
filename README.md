# rsplit

RSPLIT is a command-line program for the GNU operating system that randomly extracts a percentage of lines from a text file.  This program can be useful in processes relating to split-sample quality assurance and interpolation algorithm testing.

- Bin Program: `rsplit`

```
Usage: rpslit [OPTIONS] [infile]

Randomly extract a percentage of lines from a text file.

Options:
  -p, --percent 	The percent of lines to randomly extract 
			from the input.
  -n, --number  	If set, will interpret the -p option as the total number of
                	lines to extract, rather than as a percentage.
  -l, --line-num 	Include line numbers in output.
  -r, --remaining 	Send the lines that aren't extracted to stderr.

  --version 		Print version information and exit.

Example: rsplit test.xyz -p 20 -lr > test_rand20.xyz 2> test_base.xyz
```
