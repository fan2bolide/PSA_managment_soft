#include "hashtable.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * value
 */

enum value_kind value_get_kind(const struct value *self) {
	if (value_is_nil(self))
		return VALUE_NIL;
	if (value_is_custom(self))
		return VALUE_CUSTOM;
	if (value_is_boolean(self))
		return VALUE_BOOLEAN;
	if (value_is_real(self))
		return VALUE_REAL;
	return VALUE_INTEGER;
}

bool value_is_nil(const struct value *self) {
	return self->kind == VALUE_NIL;
}

bool value_is_boolean(const struct value *self) {
	return self->kind == VALUE_BOOLEAN;
}

bool value_is_integer(const struct value *self) {
	return self->kind == VALUE_INTEGER;
}

bool value_is_real(const struct value *self) {
	return self->kind == VALUE_REAL;
}

bool value_is_custom(const struct value *self) {
	return self->kind == VALUE_CUSTOM;
}

void value_set_nil(struct value *self) {
	self->kind = VALUE_NIL;
}

void value_set_boolean(struct value *self, bool val) {
	self->kind = VALUE_BOOLEAN;
	self->as.boolean = val;
}

void value_set_integer(struct value *self, int64_t val) {
	self->kind = VALUE_INTEGER;
	self->as.integer = val;
}

void value_set_real(struct value *self, double val) {
	self->kind = VALUE_REAL;
	self->as.real = val;
}

void value_set_custom(struct value *self, void *val) {
	self->kind = VALUE_CUSTOM;
	self->as.custom = val;
}


bool value_get_boolean(const struct value *self) {
	if (value_is_boolean(self))
		return self->as.boolean;
	return false;
}

int64_t value_get_integer(const struct value *self) {
	if (value_is_integer(self))
		return self->as.integer;
	return 0;
}

double value_get_real(const struct value *self) {
	if (value_is_real(self))
		return self->as.real;
	return 0.0;
}

void *value_get_custom(const struct value *self) {
	if (value_is_custom(self))
		return self->as.custom;
	return NULL;
}


struct value value_make_nil() {
	struct value res;
	res.kind = VALUE_NIL;
	return res;
}

struct value value_make_boolean(bool val) {
	struct value res;
	res.kind = VALUE_BOOLEAN;
	res.as.boolean = val;
	return res;
}

struct value value_make_integer(int64_t val) {
	struct value res;
	res.kind = VALUE_INTEGER;
	res.as.integer = val;
	return res;
}

struct value value_make_real(double val) {
	struct value res;
	res.kind = VALUE_REAL;
	res.as.real = val;
	return res;
}

struct value value_make_custom(void *val) {
	struct value res;
	res.kind = VALUE_CUSTOM;
	res.as.custom = val;
	return res;
}

/*
 * hashtable
 */

//void array_grow(struct array *self){
//	size_t capacity = self->capacity * 2;
//	int *data = malloc(capacity * sizeof(int));
//	memcpy(data, self->data, self->size * sizeof(int));
//	free(self->data);
//	self->data = data;
//	self->capacity = capacity;
//}

int hash(struct hashtable *self, char *key){
    int hash = 0;
    int c;
	int i = 0;
    while (key[i]){
        hash += key[i] % self->size;
		i++;
	}
	printf("hashed\n");
    return (hash + strlen(key)) % self->size;
}

void hashtable_grow(struct hashtable *self){
	size_t size = self->size * 2;
	struct bucket **buckets = calloc(size, sizeof(struct bucket*));
	memcpy(buckets, self->buckets, self->size * sizeof(struct bucket *));
	free(self->buckets);
	self->buckets = buckets;
	self->size = size;
}

void hashtable_create(struct hashtable *self) {
	printf("creating\n");
	self->buckets = calloc(HASHTABLE_INITIAL_SIZE, sizeof(struct bucket*)); //lil segfault fix: calloc replaces malloc here
	self->count = 0;
	self->size = HASHTABLE_INITIAL_SIZE;
	printf("created\n");
}

void hashtable_destroy(struct hashtable *self) {
	printf("destroying\n");
	struct bucket *curr;
	struct bucket *del;
	int i = 0;
	while (i < self->size){
		curr = self->buckets[i];
		while (curr){
			del = curr;
			curr = curr->next;
			free (del);
		}
		self->count--;
		i++;
	}
	printf("destroyed\n");
}

size_t hashtable_get_count(const struct hashtable *self) {
	printf("counted\n");
	return self->count;
}

size_t hashtable_get_size(const struct hashtable *self) {
	return self->size;
}

bool hashtable_insert(struct hashtable *self, const char *key, struct value value) {
	//rehash if h/k >= 1/2
	printf("insering\n");
	if (self->count >= self->size / 2){
		hashtable_rehash(self);
	}
	int index = hash(self, key);
	if (self->buckets[index]){
		printf("%d\n", value_get_kind(&self->buckets[index]->value));
		self->buckets[index]->value = value;
		printf("cannot insert, value replaced\n");
		return false;
	}
	self->buckets[index] = calloc(1, sizeof(struct bucket));
	self->buckets[index]->value = value;
	self->buckets[index].key = strdup(key);
	printf("key = %s", self->buckets[index]->key);
	self->count++;
	printf("insered -> %d\n", index);
	return true;
}

bool hashtable_remove(struct hashtable *self, const char *key) {
	printf("removing\n");
	if (!self)
		return false;
	if (hashtable_contains(self, key)){
		free(self->buckets[hash(self, key)]);
		self->buckets[hash(self, key)] = NULL;
		self->count--;
		printf("removed\n");
		return true;
	}
	printf("not removed\n");
	return false;
}

bool hashtable_contains(const struct hashtable *self, const char *key) {
	return (self->buckets[hash(self, key)] != NULL);
}

void hashtable_rehash(struct hashtable *self) {
	printf("rehashing\n");
	self->size *= 2;
	if (!self || self->count == 0){
		return;
	}
	struct bucket **new_buckets;
	new_buckets = calloc(self->size, sizeof(struct bucket *));
	if (!new_buckets){
		return;
	}
	int i = 0;
	while (i < (self->size  / 2)){
		if (self->buckets[i] != NULL){
			printf("moving %d\n", i);
			printf("bug\n");
			new_buckets[hash(self, self->buckets[i]->key)] = self->buckets[i];
		}
		i++;
	}
	printf("rehashed\n");
}

void hashtable_set_nil(struct hashtable *self, const char *key) {
	hashtable_insert(self, key, value_make_nil());
}

void hashtable_set_boolean(struct hashtable *self, const char *key, bool val) {
	hashtable_insert(self, key, value_make_boolean(val));
}

void hashtable_set_integer(struct hashtable *self, const char *key, int64_t val) {
	hashtable_insert(self, key, value_make_integer(val));
}

void hashtable_set_real(struct hashtable *self, const char *key, double val) {
	hashtable_insert(self, key, value_make_real(val));
}

void hashtable_set_custom(struct hashtable *self, const char *key, void *val) {
	hashtable_insert(self, key, value_make_custom(val));
}

struct value hashtable_get(struct hashtable *self, const char *key) {
	struct value res = value_make_nil();
	if (hashtable_contains(self, key))
		res = self->buckets[hash(self, key)]->value;
	return res;
}
