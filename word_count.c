/* File to calculate the top 20 words in the book of Moby Dick.              */
/* Limitations stated in accompanying email.                                 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* Orginally based on                                                        */
/* http://commonplacebook.com/art/books/word-count-for-famous-novels/ which  */
/* set the count as ~210000 words, noted that the sample text is not the     */
/* whole text. MS Word suggests 46354 is the theoretical maximum.            */
#define MAXWORDS (46500)
/* Based on https://en.wikipedia.org/wiki/Longest_word_in_English            */
#define MAXWORDLEN (46)

#define PRINTWORDS (20)


struct Word {
    char word[MAXWORDLEN];
    int count;
};

void add_word(struct Word *words, int *n, char *str)
{
    int i;
    /* Perform search on current words in list.                              */
    for (i=0; i<*n; i++)
    {
        if (strcmp (str, words[i].word) == 0)
        {
            words[i].count++;
            return;
        }
    }

    if ((strlen (str) >= MAXWORDLEN) || (*n >= MAXWORDS))
    {
        fprintf (stderr, "Not a real word or too much text!\n");
        exit(1);
    }

    /* Copy into next available spot. Update length of list.                 */
    strcpy(words[*n].word, str);
    words[*n].count = 1;
    (*n)++;
}

/* Comparison to use for quicksort.                                          */
int cmpfunc (struct Word *a, struct Word *b)
{
    return (a->count < b->count) - (a->count > b->count);
}

int main()
{
    struct Word words[MAXWORDS];
    FILE* file;
    file = fopen("mobydick[1]", "r");

    char str[MAXWORDLEN];

    int i, n;
    n = 0;

    if(file == NULL)
    {
        perror("Error opening file");
        return(-1);
    }
    while (fscanf(file, "%s", str) != EOF)
    {
        /* Tackle the problem on a per-word basis.                           */
        /* Check first letter is in the alphabet.                            */
        if (isalpha(str[0]))
        {
            for (i = 0; str[i]; i++)
            {
                /* For each character if it isn't in the alphabet remove it, */
                /* otherwise make sure it's lowercase.                       */
                if (!isalpha(str[i]))
                {
                    int j = i;
                    while (str[j]) {
                        j++;
                        str[j-1] = str[j];
                    }
                    str[j] = 0;
                    /* The next char may also be non-alpha so recheck this   */
                    /* position.                                             */
                    i--;
                }
                else
                {
                    str[i] = tolower(str[i]);
                }
            }

            /* Formatting done, now add the word to our list.                */
            add_word(words, &n, str);
        }
    }
    fclose(file);

    /* File read over. Now sort the list and get top results.                */
    qsort((void *) words,
          n,
          sizeof(struct Word),
          (int (*) (const void *, const void *)) cmpfunc);

    /* The list is now sorted - print out the wanted values to stdout.       */
    for (i=0; i<PRINTWORDS; i++)
    {
        printf ("%d %s\n", words[i].count, words[i].word);
    }
}

