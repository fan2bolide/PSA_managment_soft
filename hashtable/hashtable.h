#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum value_kind {
  VALUE_NIL,
  VALUE_BOOLEAN,
  VALUE_INTEGER,
  VALUE_REAL,
  VALUE_CUSTOM,
};

struct value {
  enum value_kind kind;
  union {
    bool boolean;
    int64_t integer;
    double real;
    void *custom;
  } as;
};

enum value_kind value_get_kind(const struct value *self);

bool value_is_nil(const struct value *self);
bool value_is_boolean(const struct value *self);
bool value_is_integer(const struct value *self);
bool value_is_real(const struct value *self);
bool value_is_custom(const struct value *self);

void value_set_nil(struct value *self);
void value_set_boolean(struct value *self, bool val);
void value_set_integer(struct value *self, int64_t val);
void value_set_real(struct value *self, double val);
void value_set_custom(struct value *self, void *val);

bool value_get_boolean(const struct value *self);
int64_t value_get_integer(const struct value *self);
double value_get_real(const struct value *self);
void *value_get_custom(const struct value *self);

struct value value_make_nil();
struct value value_make_boolean(bool val);
struct value value_make_integer(int64_t val);
struct value value_make_real(double val);
struct value value_make_custom(void *val);



struct bucket {
  char *key;
  struct value value;
  struct bucket *next;
};

#define HASHTABLE_INITIAL_SIZE 4

struct hashtable {
  struct bucket **buckets;
  size_t count; // number of elements in the table
  size_t size;  // size of the buckets array
};

void hashtable_create(struct hashtable *self);

void hashtable_destroy(struct hashtable *self);

size_t hashtable_get_count(const struct hashtable *self);
size_t hashtable_get_size(const struct hashtable *self);

bool hashtable_insert(struct hashtable *self, const char *key, struct value val);
bool hashtable_remove(struct hashtable *self, const char *key);
bool hashtable_contains(const struct hashtable *self, const char *key);
void hashtable_rehash(struct hashtable *self);

void hashtable_set_nil(struct hashtable *self, const char *key);
void hashtable_set_boolean(struct hashtable *self, const char *key, bool val);
void hashtable_set_integer(struct hashtable *self, const char *key, int64_t val);
void hashtable_set_real(struct hashtable *self, const char *key, double val);
void hashtable_set_custom(struct hashtable *self, const char *key, void *val);

struct value hashtable_get(struct hashtable *self, const char *key);

#ifdef __cplusplus
}
#endif

#endif // HASHTABLE_H
