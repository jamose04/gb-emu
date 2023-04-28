/*
 * This is a simple program used to generate a table of opcodes given a file
 * describing the opcode table.
 * The input file should generally be optable.tab.
 * Output file (table.h) should be in jameboy/
 */

#include <stdio.h>
#include <stdlib.h>

static char (*tab)[100];

/*
 * Read the next line in file
 */
int read_line(char *line, FILE *fp)
{
    char c;
    while ((c = getc(fp)) != '\n') {
        if (c == EOF)
            return -1;
        *(line++) = c;
    }
    *line = 0;
    return 0;
}

void build_table(FILE *fp)
{
    char linebuf[100] = {0};
    read_line(linebuf, fp);
    tab = malloc(sizeof(char) * 100 * atoi(linebuf));

    int i = 0;
    while (read_line(linebuf, fp) != -1) {
        
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
    build_table(fp);
    return 0;
}