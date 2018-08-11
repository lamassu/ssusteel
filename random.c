#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "wordlist.h"

char *alphabet = "abcdefghijklmnopqrstuvwxyz";
char lookup[words_len * 4 * 2];

int char_cmp(const void *a, const void *b) {
    const char *ia = (const char *)a;
    const char *ib = (const char *)b;
    return *ia  - *ib;
}

void build_config(char *config) {
  char ab[27];
  char tmp[27];

  strcpy(ab, alphabet);

  for (int i = 0; i < 5; i++) {
    int letter_count = i == 4 ? 2 : 6;
    char ring_letters[letter_count];

    for (int j = 0; j < letter_count; j++) {
      int len = strlen(ab);
      int n = rand() / (RAND_MAX / len + 1);
      ring_letters[j] = ab[n];
      memcpy(tmp, ab, n);
      memcpy(tmp + n, ab + n + 1, len - (n+1));
      memcpy(ab, tmp, len - 1);
      ab[len - 1] = '\0';
    }

    qsort(ring_letters, letter_count, 1, char_cmp);
    memcpy(config + i*6, ring_letters, letter_count);
  }
}

void update_lookup(char letter, int word) {
  for (char i = 0; i < 4; i++) {
    char rec[2];
    memcpy(rec, lookup + word * 8 + 2*i, 2);

    if (rec[0] == letter) {
      *(lookup + word*8 + 2*i + 1) = rec[1] + 1;
    }
  }
}

char lookup_letter(int word, char letter) {
  for (char i = 0; i < 4; i++) {
    char rec[2];
    memcpy(rec, lookup + word*8 + 2*i, 2);

    if (rec[0] == letter) {
      return rec[1];
    }
  }

  return 0;
}

void create_lookup(void) {
  for (int i = 0; i < words_len; i++) {
    const char *word = words[i];
    char *word_lookup;
    int word_len = strlen(word);

    for (int j = 0; j < word_len; j++) {
      char letter = word[j];
      update_lookup(letter, i);
    }

    memcpy(lookup + i*8, word_lookup, 8);
  }
}

int compute_ring(char *ring, int len) {
  int max_occurances = 0;

  for (int i = 0; i < words_len; i++) {
    int occurances = 0;

    for (int j = 0; j < len; j++) {
      char ring_letter = ring[j];
      int letter_count = (int)lookup_letter(i, ring_letter);

      // printf("ring_letter: %c; letter_count: %d\n", ring_letter, letter_count);
      occurances += letter_count;
    }

    max_occurances = occurances > max_occurances ?
      occurances :
      max_occurances;
  }

  return max_occurances;
}

int compute_config(char *rings) {
  int sum = 0;

  for (int i = 0; i < 5; i++) {
    int ring_len = i == 4 ? 2 : 6;

    if (ring_len == 2) {
      printf("ring: %.2s\n", rings + i*6);
    } else {
      printf("ring: %.6s\n", rings + i*6);
    }

    int ring_count = compute_ring(rings + i * 6, ring_len);
    printf("ring_count [%d]: %d\n", i, ring_count);
    sum += ring_count;
  }

  return 12 * sum;
}

int main(void) {
  char config[26];
  create_lookup();
  build_config(config);
  printf("%.26s\n", config);

  int sum = compute_config(config);
  printf("%d", sum);
}
