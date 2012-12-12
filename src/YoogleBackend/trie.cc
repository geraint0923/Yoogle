#include "trie.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


using namespace std;


void trie::insert_entry(const char *str, const int id) {
}

void trie::prefix_search(const char *prefix, vector<int> &result) {
}


void trie::dissect_string(const char *str, int id) {
}

// use back order when traversing the trie
void trie::dfs_traverse(struct trie_node *node, 
		void (*func)(trie *ptrie, struct trie_node *pnode, void *pvoid)) {
}


void trie::delete_trie_node(trie *ptrie, struct trie_node *pnode, void *pvoid) {
}

void trie::add_trie_node_set(trie *ptrie, struct trie_node *pnode, void *pvoid) {
}


trie::trie() {
	trie_root.char_id = 0;
	trie_root.id_set.clear();
	trie_root.next_node = NULL;
	trie_root.first_child = NULL;
}

trie::~trie() {
	// TODO : delete all the allocated memory
	dfs_traverse(&trie_root, delete_trie_node);
}
