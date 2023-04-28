/*
 * This is a simple program used to generate a table of opcodes given a file
 * describing the opcode table.
 * The input file should generally be optable.tab.
 * Output file (table.h) should be in jameboy/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char (*tab)[100];
static int tablen;

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

int ahtoi(char c)
{
    if ('0' <= c && c <= '9') {
        return c - '0';
    } else if ('a' <= c && c <= 'f') {
        return c - 'a' + 10;
    }
}

int h2int(char *hex)
{
    int n = strlen(hex);
    int res = 0;
    int pow = 1;
    int i;
    for (i = 0; i < n; i++, pow *= 16) {
        res += ahtoi(hex[n - i - 1]) * pow;
    }
    
    return res;
}

void build_table(FILE *fp)
{
    char linebuf[100] = {0};
    read_line(linebuf, fp);
    tablen = atoi(linebuf);
    tab = calloc(tablen, sizeof(char) * 100);
    printf("%d\n", tablen * sizeof(char) * 100);
    printf("%d\n", sizeof(tab[0]));

    int i = 0;
    while (read_line(linebuf, fp) != -1) {
        // linebuf contains the correct string
        char ib[3] = {0};
        while (*ib != ';') {
            if (read_line(ib, fp) == -1) {
                break;
            }

            if (*ib == ';')
                break;
            strcpy(tab[h2int(ib)], linebuf);
        }
    }
}

void print_table()
{
    int i;
    for (i = 0; i < tablen; i++) {
        printf("%x: %s\n", i, tab[i]);
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
    print_table();
    return 0;
}