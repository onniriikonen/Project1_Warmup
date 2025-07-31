#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Dynaaminen taulukko rivien tallentamiseen
char **read_lines(FILE *input, int *line_count)
{
    char **lines = NULL;
    char *line = NULL;
    size_t len = 0;
    int count = 0;

    while (getline(&line, &len, input) != -1)
    {
        char *stored = malloc(strlen(line) + 1);
        if (stored == NULL)
        {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        strcpy(stored, line);

        char **temp = realloc(lines, (count + 1) * sizeof(char *));
        if (temp == NULL)
        {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }

        lines = temp;
        lines[count++] = stored;
    }

    free(line);
    *line_count = count;
    return lines;
}

// Vapauttaa muistissa olevat rivit
void free_lines(char **lines, int count)
{
    for (int i = 0; i < count; i++)
    {
        free(lines[i]);
    }
    free(lines);
}

int main(int argc, char *argv[])
{
    FILE *input = stdin;
    FILE *output = stdout;

    // Liikaa argumentteja
    if (argc > 3)
    {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    // Vain syötetiedosto annettu
    if (argc >= 2)
    {
        input = fopen(argv[1], "r");
        if (input == NULL)
        {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    }

    // Syöte- ja tulostetiedosto annettu
    if (argc == 3)
    {
        // Samanlaiset tiedostot
        if (strcmp(argv[1], argv[2]) == 0)
        {
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }

        output = fopen(argv[2], "w");
        if (output == NULL)
        {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            exit(1);
        }
    }

    // Lukee rivit ja tulostaa ne käänteisessä järjestyksessä
    int count = 0;
    char **lines = read_lines(input, &count);

    for (int i = count - 1; i >= 0; i--)
    {
        fprintf(output, "%s", lines[i]);
    }

    free_lines(lines, count);

    if (input != stdin)
        fclose(input);
    if (output != stdout)
        fclose(output);

    return 0;
}
