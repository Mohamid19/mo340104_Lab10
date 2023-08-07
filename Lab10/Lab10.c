#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Trie

{
    struct Trie* children[26];
    int count;
};
void insert(struct Trie* pTrie, char* word)
{
    struct Trie* node = pTrie;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        int index = word[i] - 'a';
        if (node->children[index] == NULL)
        {
            node->children[index] = (struct Trie*)malloc(sizeof(struct Trie));
            for (int j = 0; j < 26; ++j)
            {
                node->children[index]->children[j] = NULL;
            }
            node->children[index]->count = 0;
        }
        node = node->children[index];
    }
    node->count++;
}
int numberOfOccurances(struct Trie* pTrie, char* word)
{
    struct Trie* node = pTrie;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        int index = word[i] - 'a';
        if (node->children[index] == NULL)
        {
            return 0;
        }
        node = node->children[index];
    }
    return node->count;
}
struct Trie* deallocateTrie(struct Trie* pTrie)
{
    if (pTrie == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < 26; ++i)
    {
        pTrie->children[i] = deallocateTrie(pTrie->children[i]);
    }
    free(pTrie);
    return NULL;
}
struct Trie* createTrie()
{
    struct Trie* pTrie = (struct Trie*)malloc(sizeof(struct Trie));
    for (int i = 0; i < 26; ++i)
    {
        pTrie->children[i] = NULL;
    }
    pTrie->count = 0;
    return pTrie;
}
int readDictionary(char* filename, char** pInWords)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }
    int count;
    if (fscanf(file, "%d", &count) != 1)
    {
        perror("Error reading word count");
        exit(1);
    }
    char word[256];
    for (int i = 0; i < count; ++i)
    {
        if (fscanf(file, "%s", word) != 1)
        {
            perror("Error reading word");
            exit(1);
        }
        pInWords[i] = strdup(word);
    }
    fclose(file);
    return count;
}
int main(void)
{
    char* inWords[256];
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie* pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }
    char* pWords[] = { "notaword", "ucf", "no", "note", "corg" };
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}