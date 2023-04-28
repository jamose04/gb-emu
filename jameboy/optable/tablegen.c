/*
 * This is a simple program used to generate a table of opcodes given a file
 * describing the opcode table.
 * The input file should generally be optable.tab.
 * Output file (table.h) should be in jameboy/
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * Read the next line in file
 */
void read_line(char *line, FILE *fp)
{
    char c;
    while ((c = getc(fp)) != '\n') {
        *(line++) = c;
    }
}

/*
 * Main function
 */
int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "please provide one arg...\n");
        exit(1);
    }

    FILE *fp = fopen(argv[1], "r");
    return 0;
}