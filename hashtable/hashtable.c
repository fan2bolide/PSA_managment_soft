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


bool buckets_delete(struct buckets **buckets, size_t size){
	if (!buckets)
		return false;
	struct bucket *del;
	struct bucket *curr;
	int i = 0;
	while (i < size){
		if (buckets[i]){
			curr = buckets[i];
			while (curr){
				del = curr;
				curr = curr->next;
				free(del);
			}
		}
		i++;
	}
	free(buckets);
	return true;
}

#define FNV_prime 0x100000001b3
#define FNV_offset_basis 0xcbf29ce484222325

size_t hash(struct hashtable *self, char *key){
    size_t databytes = strlen(key);
	size_t hash = FNV_offset_basis;

	for (size_t i = 0 ; i < databytes ; i++){
		hash ^= key[i];
		hash *= FNV_prime;
	}

	return hash % self->size;
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
	return self->count;
}

size_t hashtable_get_size(const struct hashtable *self) {
	return self->size;
}

char *mystrdup(const char *str)
{
	char *new_str = calloc(1, strlen(str) + 1);
	if (!new_str)
		return (NULL);
	memmove(new_str, str, strlen(str));
	return (new_str);
}

bool hashtable_insert(struct hashtable *self, const char *key, struct value value) {
	int stop = 0;
	//rehash if h/k >= 1/2
	if (!key || !self)
		return false;
	printf("insering\n");
	if (self->count >= self->size / 2){
		hashtable_rehash(self);
	}
	int index = hash(self, key);
	if (index == 39691){
		stop++;
	}
	printf("trying to insert to -> %d\n", index);
	struct bucket *curr = self->buckets[index];
	if (curr){
		while(curr && strcmp(curr->key, key) != 0) {
			curr = curr->next;
		}
		if (curr){
			curr->value = value;
			printf("index already taken, value replaced\n");
			assert(stop != 2);
			return false;
		}
		curr = calloc(1, sizeof(struct bucket));
		curr->value = value;
		curr->key = mystrdup(key);
		self->count++;
		assert(stop != 2);
		return true;
	}
	self->buckets[index] = calloc(1, sizeof(struct bucket));
	self->buckets[index]->value = value;
	self->buckets[index]->key = mystrdup(key);
	self->count++;
	printf("insered -> %d\n", index);
	assert(stop != 2);
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
	struct bucket *curr = self->buckets[hash(self, key)];
	while (curr){
		printf("searching to %d\n", hash(self, key));
		if (strcmp(curr->key, key) == 0){
			printf("found\n");
			return true;
		}
		curr = curr->next;
	}
	return false;
}

void hashtable_rehash(struct hashtable *self) {
	printf("rehashing\n");
	if (!self)
		return;
	self->size *= 2;
	self->count = 0;
	struct bucket **old_buckets = self->buckets;
	self->buckets = calloc(self->size, sizeof(struct bucket *));
	int i = 0;
	struct bucket *curr;
	while (i < self->size / 2){
		if (old_buckets[i]){
			curr = old_buckets[i];
			while (curr){
				hashtable_insert(self, curr->key, curr->value);
				curr = curr->next;
			}
		}
		i++;
	}
	buckets_delete(old_buckets, self->size / 2);
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
	struct bucket *curr = self->buckets[hash(self, key)];
	while (curr){
		if (strcmp(curr->key, key) == 0){
			res = curr->value;
			return res;
		}
		curr = curr->next;
	}
	return res;
}
