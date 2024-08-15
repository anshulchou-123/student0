/*
 * Implementation of the word_count interface using Pintos lists.
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
#error "PINTOS_LIST must be #define'd when compiling word_count_l.c"
#endif

#include "word_count.h"


/*
The word count definition for word_count_l.c after preprocessor macros are:

typedef struct word_count {
  char* word;
  int count;
  struct list_elem elem;
} word_count_t;

using word_count_list_t = struct list;

*/

void init_words(word_count_list_t* wclist) { /* TODO */
  list_init(wclist);
}

size_t len_words(word_count_list_t* wclist) {
  /* TODO */

  struct list_elem *e;
  int length = 0;

  for(e = list_begin(wclist); e != list_end(wclist); e = list_next(e))
  {
    list_entry(e, struct word_count, elem);
    length++;
  }

  return length;
}

word_count_t* find_word(word_count_list_t* wclist, char* word) {
  struct list_elem *e;

  for(e = list_begin(wclist); e != list_end(wclist); e = list_next(e))
  {
    word_count_t* element = list_entry(e, struct word_count, elem);
    if(strcmp(element->word, word) == 0)
    {
      return element;
    }
  }
  return NULL;
}

word_count_t* add_word(word_count_list_t* wclist, char* word) {
  struct list_elem *e;

  for(e = list_begin(wclist); e != list_end(wclist); e = list_next(e))
  {
    word_count_t* element = list_entry(e, struct word_count, elem);
    if(strcmp(element->word, word) == 0)
    {
      element->count++;
      return element;
    }
  }

  word_count_t* t = malloc(sizeof(word_count_t));
  t->count = 1;
  t->word = word;
  list_push_front(wclist, &t->elem);

  return t;
}

void fprint_words(word_count_list_t* wclist, FILE* outfile) { 
  struct list_elem *e;

  for(e = list_begin(wclist); e != list_end(wclist); e = list_next(e))
  {
    word_count_t* element = list_entry(e, struct word_count, elem);
    fprintf(outfile, "%i\t%s\n", element->count, element->word);
  }
}

static bool less_list(const struct list_elem* ewc1, const struct list_elem* ewc2, void* aux) {
  /* TODO */
  return false;
}

void wordcount_sort(word_count_list_t* wclist,
                    bool less(const word_count_t*, const word_count_t*)) {
  list_sort(wclist, less_list, less);
}
