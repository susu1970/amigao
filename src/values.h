#ifndef AMIGAO_VALUES_H_
#define AMIGAO_VALUES_H_

#include<unistd.h>

static const int WORKERS=sysconf(_SC_NPROCESSORS_CONF);

#define DB_DICTIONARY_WORD_SIZE 768
#define REVERSE_VALUATE_STRATEGY_WORD_SIZE 255

#define URL_SIZE 2048//the maximum size of url address
#define HTML_SIZE 1048576//the maximum size of html page

#define HTML_NUMS 60
#define URL_NUMS 20000

//search_server
#define WORD_SIZE 255
#define DEFAULT_RESULTS_PER_PAGE 10
#define MAX_RESULTS_PER_PAGE 50
#define MAX_SEARCH_SENTENCE_SIZE 90
#define MAX_CONTENTS_SIZE 355
#define DFL_PAGES 7

#endif
