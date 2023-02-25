#define _XOPEN_SOURCE 500 /* Habilitar functions (strdup) do linux   */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <limits.h>

struct entry_s {
	char *key;
	char *value;
	struct entry_s *next;
};

typedef struct entry_s entry_t;

struct hashtable_s {
	int size;
	struct entry_s **table;	
};

typedef struct hashtable_s hashtable_t;


/* Criar um novo hashtable. */
hashtable_t *ht_create( int size );


int ht_hash( hashtable_t *hashtable, char *key );
entry_t *ht_newpair( char *key, char *value );
void ht_set( hashtable_t *hashtable, char *key, char *value );
char *ht_get( hashtable_t *hashtable, char *key );