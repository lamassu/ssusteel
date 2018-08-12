#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "wordlist.h"

char *alphabet = "abcdefghijklmnopqrstuvwxyz";
char lookup[1024 * 4 * 2];

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
  int last_rec_idx = 0;

  for (char i = 0; i < 4; i++) {
    char rec[2];
    memcpy(rec, lookup + word * 8 + 2*i, 2);

    if (rec[0] == '\0') {
      last_rec_idx = i;
      break;
    } else if (rec[0] == letter) {
      *(lookup + word*8 + 2*i + 1) = rec[1] + 1;
      return;
    }
  }

  *(lookup + word*8 + 2*last_rec_idx + 0) = letter;
  *(lookup + word*8 + 2*last_rec_idx + 1) = 1;
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
    char *word = words[i];
    int raw_word_len = strlen(word);
    int word_len = raw_word_len > 4 ? 4 : raw_word_len;

    for (int j = 0; j < word_len; j++) {
      char letter = word[j];
      update_lookup(letter, i);
    }
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
    int ring_count = compute_ring(rings + i * 6, ring_len);
    sum += ring_count;
  }

  return 12 * sum;
}

void print_config(char *rings) {
  for (int i = 0; i < 5; i++) {
    int ring_len = i == 4 ? 2 : 6;

    if (ring_len == 2) {
      printf("%.2s\n", rings + i*6);
    } else {
      printf("%.6s\n", rings + i*6);
    }
  }
}

void print_lookup(void) {
  for (int i = 0; i < words_len; i++) {
    for (int j = 0; j < 8; j++) {
      char c = *(lookup + i*8 + j);

      if (j % 2 == 0) {
        printf("%c", c == '\0' ? 'N' : c);
      } else {
        printf("%d", c);
      }
    }
  }

  printf("\n\n");
}

int is_sorted(char *config) {
  return config[0] < config[5] &&
    config[5] < config[10] &&
    config[10] < config[15] &&
    config[15] < config[20];
}

int main(int argc, char *argv[]) {
  char *arg = argv[1];
  printf("arg: %s\n", arg);
  int seed = atoi(arg);
  printf("seed: %d\n", seed);
  srand(seed);
  memset(lookup, '\0', words_len * 4 * 2);
  create_lookup();

  int min = 1000;
  int i = 0;

  while (1) {
    char config[26];
    build_config(config);

    int sum = compute_config(config);

    if (sum < min) {
      min = sum;
      printf("[%d][%d] sum: %d\n", seed, i, sum);
      print_config(config);
      printf("\n");
    }

    i++;
  }
}
