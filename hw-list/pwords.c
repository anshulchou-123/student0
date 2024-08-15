/*
 * Word count application with one thread per input file.
 *
 * You may modify this file in any way you like, and are expected to modify it.
 * Your solution must read each input file from a separate thread. We encourage
 * you to make as few changes as necessary.
 */

/*
 * Copyright © 2021 University of California, Berkeley
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <pthread.h>

#include "word_count.h"
#include "word_helpers.h"

typedef struct {
    word_count_list_t* word_counts;
    char* file;
} shared_data_t;


void* threadfun(void* argc) {
  shared_data_t* shared_data = (shared_data_t*)(argc);
  FILE* infile = NULL;
  infile = fopen(shared_data->file, "r");
  if (infile == NULL) {
    perror("Error opening file");
  }
  
  int ch;

  char* buffer = malloc(100);
  int index = 0;
  while ((ch = fgetc(infile)) != EOF) {
    if ((char)ch == '\n' || (char)ch == ' ') {
      if (index != 0) {
        buffer[index] = '\0';
        // printf("%s ", buffer);
        // printf("Lock acquired");
        // pthread_mutex_lock(&(word_counts.lock));
        add_word(shared_data->word_counts, buffer);
        buffer = (char*)(malloc(65));
        // pthread_mutex_unlock(&(word_counts.lock));
        // printf("Lock is released");
        // printf("Word added is = %s\n", whead->word);
      }
      // printf("Index is 0");
      memset(buffer, '\0', 65);
      index = 0;
    } else {
      buffer[index++] = (char)ch;
    }
  }
  // printf("A");
  pthread_exit(NULL);
}

/*
 * main - handle command line, spawning one thread per file.
 */
int main(int argc, char* argv[]) {
  /* Create the empty data structure. */
  shared_data_t shared_data;
  word_count_list_t word_counts;
  shared_data.word_counts = &word_counts;
  init_words(&word_counts);
  int nthreads = argc - 1;
  pthread_t threads[nthreads];
  if (argc <= 1) {
    /* Process stdin in a single thread. */
    count_words(&word_counts, stdin);
  } else {
    /* TODO */
    for (int i = 1; i < argc; i++) {
      shared_data.file = argv[i];
      int rc = pthread_create(&threads[i-1], NULL, threadfun, (void*)(&shared_data));
      if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
      }
    }
  }

  for (int t = 0; t < nthreads; t++) {
    int rc = pthread_join(threads[t], NULL);
    if (rc) {
      printf("ERROR; return code from pthread_join() is %d\n", rc);
      exit(-1);
    }
  }

  /* Output final result of all threads' work. */
  wordcount_sort(&word_counts, less_count);
  fprint_words(&word_counts, stdout);
  return 0;
}
