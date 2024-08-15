/*
 * Implementation of the word_count interface using Pintos lists and pthreads.
 *
 * You may modify this file, and are expected to modify it.
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

#ifndef PINTOS_LIST
#error "PINTOS_LIST must be #define'd when compiling word_count_lp.c"
#endif

#ifndef PTHREADS
#error "PTHREADS must be #define'd when compiling word_count_lp.c"
#endif

#include "word_count.h"

/*
The word count definition for word_count_l.c after preprocessor macros are:

typedef struct word_count {
  char* word;
  int count;
  struct list_elem elem;
} word_count_t;

typedef struct word_count_list {
  struct list lst;
  pthread_mutex_t lock;
} word_count_list_t;

using word_count_list_t = struct word_count_list;

*/

void init_words(word_count_list_t* wclist) { /* TODO */
  list_init(&wclist->lst);
  pthread_mutex_init(&wclist->lock, NULL);
}

size_t len_words(word_count_list_t* wclist) {
  /* TODO */
  
  pthread_mutex_lock(&wclist->lock);
  struct list_elem *e;
  int length = 0;

  for(e = list_begin(&wclist->lst); e != list_end(&wclist->lst); e = list_next(e))
  {
    list_entry(e, struct word_count, elem);
    length++;
  }
  pthread_mutex_unlock(&wclist->lock);

  return length;
}

word_count_t* find_word(word_count_list_t* wclist, char* word) {
  
  pthread_mutex_lock(&wclist->lock);
  struct list_elem *e;

  for(e = list_begin(&wclist->lst); e != list_end(&wclist->lst); e = list_next(e))
  {
    word_count_t* element = list_entry(e, struct word_count, elem);
    if(strcmp(element->word, word) == 0)
    {
      pthread_mutex_unlock(&wclist->lock);
      return element;
    }
  }
  pthread_mutex_unlock(&wclist->lock);
  return NULL;
}

word_count_t* add_word(word_count_list_t* wclist, char* word) {
  struct list_elem *e;
  pthread_mutex_lock(&wclist->lock);
  for(e = list_begin(&wclist->lst); e != list_end(&wclist->lst); e = list_next(e))
  {
    word_count_t* element = list_entry(e, struct word_count, elem);
    if(strcmp(element->word, word) == 0)
    {
      element->count++;
      pthread_mutex_unlock(&wclist->lock);
      return element;
    }
  }

  word_count_t* t = malloc(sizeof(word_count_t));
  t->count = 1;
  // char* words = malloc(strlen(word));
  // strcpy(words, word);
  t->word = word;
  list_push_front(&wclist->lst, &t->elem);
  pthread_mutex_unlock(&wclist->lock);
  return t;
}

void fprint_words(word_count_list_t* wclist, FILE* outfile) { 
  pthread_mutex_lock(&wclist->lock);
  struct list_elem *e;

  for(e = list_begin(&wclist->lst); e != list_end(&wclist->lst); e = list_next(e))
  {
    word_count_t* element = list_entry(e, struct word_count, elem);
    fprintf(outfile, "%i\t%s\n", element->count, element->word);
  }
  pthread_mutex_unlock(&wclist->lock);
}

struct list_elem* findMin(word_count_list_t* wclist,
                    bool less(const word_count_t*, const word_count_t*))
{
  struct list_elem *e = list_begin(&wclist->lst);
  struct list_elem *currentmin = list_begin(&wclist->lst);
  for(e = list_begin(&wclist->lst); e != list_end(&wclist->lst); e = list_next(e))
  {
    word_count_t* current_min = list_entry(currentmin, struct word_count, elem);
    word_count_t* element = list_entry(e, struct word_count, elem);
    if(less(element, current_min))
    {
      currentmin = e;
    }
  }
  return currentmin;
}


void wordcount_sort(word_count_list_t* wclist,
                    bool less(const word_count_t*, const word_count_t*)) {
  /* TODO */
  struct list sorted_list;
  list_init(&sorted_list);
  
  while (!list_empty(&wclist->lst)) {
    struct list_elem* min = findMin(wclist, less);
    if (min != NULL) {
      list_remove(min);
      list_push_back(&sorted_list, min);
    }
  }

  // Move sorted elements back to the original list
  while (!list_empty(&sorted_list)) {
    struct list_elem* e = list_pop_front(&sorted_list);
    list_push_back(&wclist->lst, e);
  }

}
