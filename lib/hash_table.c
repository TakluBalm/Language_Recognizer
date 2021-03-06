#include "include.h"

struct hash_node{
	void* data;
	char* key;
	hash_node_p next;
};

static bool isEqual(char* a, char* b){
	while(*a != '\0' && *b != '\0'){
		if(*(a++) != *(b++))	return false;
	}
	if(*a == *b)	return true;
	return false;
}

static size_t hash_function(char* str){
	size_t hash = 0;
	int c;
	while(c = *str++){
		hash = ((hash << 5) + hash)^c;
	}
	return hash;
}

static void hash_expand(htable_p ht){
	size_t new_size = (ht->size << 1);
	hash_node_p* new_tb = calloc(new_size,  sizeof(*new_tb)), n;
	for(int i = 0; i < ht->size; i++){
		n = ht->tb[i];
		while(n != NULL){
			int new_i = hash_function(n->key) % new_size;
			hash_node_p temp = n->next;
			n->next = new_tb[i];
			new_tb[i] = n;
			n = temp;
		}
	}
	free(ht->tb);
	ht->tb = new_tb;
	ht->size = new_size;
}

static void* HASH_REMOVE(char* key, htable_p ht){
	int i = hash_function(key) % ht->size;
	void* data = NULL;
	hash_node_p current, prev = NULL;
	for(current = ht->tb[i]; current != NULL; current = current->next){
		if(isEqual(current->key, key)){
			if(prev == NULL)	ht->tb[i] = current->next;
			else				prev->next = current->next;
			data = current->data;
			ht->stored--;
			free(current);
			break;
		}
		prev = current;
	}
	return data;
}

static void* HASH_SEARCH(char* key, htable_p ht){
	if(ht->size == 0)	return NULL;
	int i = hash_function(key) % ht->size;
	hash_node_p n = ht->tb[i];
	while(n != NULL && !isEqual(key, n->key)){
		n = n->next;
	}
	if(n == NULL)	return NULL;
	return n->data;
}

static void HASH_INSERT(char* key, void* data, htable_p ht){
	if(ht->size == 0){
		ht->size = TB_INIT_SIZE;
		ht->tb = calloc(TB_INIT_SIZE, sizeof(*(ht->tb)));
	}
	double load_factor = (double) (ht->stored)/(ht->size);
	
	if(load_factor >= 0.8){
		hash_expand(ht);
	}

	int i = hash_function(key) % ht->size;
	hash_node_p n = malloc(sizeof(*n));
	n->data = data;
	n->key = key;
	n->next = ht->tb[i];
	ht->tb[i] = n;
	ht->stored++;
}

struct HashTable HashTableInit(){
	struct HashTable ht;
	ht.size = 0;
	ht.search = HASH_SEARCH;
	ht.insert = HASH_INSERT;
	ht.remove = HASH_REMOVE;
	ht.stored = 0;
	ht.tb = NULL;
	return ht;
}

struct HashTable DelHTable(bool freeData, struct HashTable ht){
	for(int i = 0; i < ht.size; i++){
		hash_node_p n = ht.tb[i];
		while(n != NULL){
			hash_node_p temp = n->next;
			if(n->data != NULL && freeData)	free(n->data);
			free(n);
			n = temp;
		}
	}
	free(ht.tb);
	return HashTableInit();
}
