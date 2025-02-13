#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function to remove a word from a string
void removeWord(char *str, const char *word) {
    char *pos, temp[1000];
    int len = strlen(word);
    while ((pos = strstr(str, word)) != NULL) {
        strcpy(temp, pos + len);
        if (*(pos + len) != '\0' && *(pos + len) != ' ') {
            continue;
        }
        if (pos != str && *(pos - 1) == ' ') {
            pos--;
        }
        *pos = '\0';
        strcat(str, temp);
    }
}

// Function to reverse each word in a string
void reverseWords(char *str) {
    char *word_start = str, *temp = str;
    while (*temp) {
        while (*temp && *temp != ' ') temp++;
        char *word_end = temp - 1;
        while (word_start < word_end) {
            char tempChar = *word_start;
            *word_start++ = *word_end;
            *word_end-- = tempChar;
        }
        if (*temp) temp++;
        word_start = temp;
    }
}

// Function to find the largest and smallest word in a string
void findLargestSmallestWord(char *str, char *largest, char *smallest) {
    char word[100], *ptr = str;
    int len, minLen = 1000, maxLen = 0;
    while (sscanf(ptr, "%s", word) == 1) {
        len = strlen(word);
        if (len > maxLen) {
            maxLen = len;
            strcpy(largest, word);
        }
        if (len < minLen) {
            minLen = len;
            strcpy(smallest, word);
        }
        ptr += len;
        while (*ptr == ' ') ptr++;
    }
}

// Function to count number of words in a string
int countWords(char *str) {
    int count = 0;
    char *ptr = str;
    while (*ptr) {
        while (*ptr == ' ') ptr++;
        if (*ptr) {
            count++;
            while (*ptr && *ptr != ' ') ptr++;
        }
    }
    return count;
}

// Function to capitalize first letter of each word
void capitalizeWords(char *str) {
    int newWord = 1;
    while (*str) {
        if (isspace(*str)) {
            newWord = 1;
        } else if (newWord) {
            *str = toupper(*str);
            newWord = 0;
        }
        str++;
    }
}

int main() {
    char str[1000], str1[1000], str2[1000], str3[1000], str4[1000], largest[100], smallest[100];
    
    printf("Enter a string: ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = 0;
    
    // Removing a word
    strcpy(str1, str);
    removeWord(str1, "is");
    printf("After removing 'is': %s\n", str1);
    
    // Reversing each word
    strcpy(str2, str);
    reverseWords(str2);
    printf("After reversing each word: %s\n", str2);
    
    // Finding largest and smallest word
    strcpy(str3, str);
    findLargestSmallestWord(str3, largest, smallest);
    printf("Largest word: %s, Smallest word: %s\n", largest, smallest);
    
    // Counting words
    strcpy(str4, str);
    printf("Number of words: %d\n", countWords(str4));
    
    // Capitalizing first letter of each word
    capitalizeWords(str);
    printf("After capitalizing: %s\n", str);
    
    return 0;
}
