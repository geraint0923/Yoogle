#include "trie.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


using namespace std;


void trie::insert_entry(const char *str, const int id) {
	if(dissect_string(str, id)) {
		insert_word(str, strlen(str), id);
//		printf("insert:%s\n", str);
	}
}

void trie::prefix_search(const char *prefix, set<int> &result) {
	struct trie_node *pnode = find_node(prefix, strlen(prefix), false);
	printf("pnode:0x%08x\n", pnode);
	if(pnode) {
		dfs_traverse(pnode, add_trie_node_set, (void*)&result);
	}
}

bool trie::is_seperator(char ch) {
	if(ch == ' ' 
			|| ch == '\t'
			|| ch == '\n'
			|| ch == '\r'
			|| ch == 0) {
		return true;
	}
	
	return false;
}

bool trie::dissect_string(const char *str, const int id) {
	int len = strlen(str);
	int acc = 0;
	bool dissect = false;
	for(int i = 0; i < len; ++i) {
		if(is_seperator(str[i]) || i == len - 1) {
			insert_word(str + i - acc, acc, id);
			acc = 0;
			if(i != len - 1) {
				dissect = true;
			}
		} else {
			++acc;
		}
	}
	return dissect;
}


void trie::insert_word(const char *str, int len, const int id) {
	struct trie_node *pnode = find_node(str, len, true);
	if(pnode) {
		pnode->id_set.insert(id);
	} else {
		printf("error!!! insert_word : null word\n");
	}
}


// use back order when traversing the trie
void trie::dfs_traverse(struct trie_node *node, 
		void (*func)(trie *ptrie, struct trie_node *pnode, void *pvoid), void *pvoid) {

	for(int i = 0; i < 256; ++i) {
		if(node->char_map[i]) {
			dfs_traverse(node->char_map[i], func, pvoid);
		}
	}

	func(this, node, pvoid);
}

struct trie_node *trie::find_node(const char *pfx, int len, bool create) {
	struct trie_node *pnode = &trie_root;
	for(int i = 0; i < len; ++i) {
		if(pnode->char_map[(unsigned char)pfx[i]]) {
			pnode = pnode->char_map[(unsigned char)pfx[i]]; 
		} else {
			// TODO : create or return NULL
			if(create) {
				pnode = add_node(pnode, pfx[i]);
			} else {
	//			printf("find null\n");
				return NULL;
			}
		}
	}
	return pnode;
}

struct trie_node *trie::add_node(struct trie_node *parent, char ch_id) {
	struct trie_node *pnode = new trie_node;
	pnode->char_id = ch_id;
	pnode->id_set.clear();

	memset(&(pnode->char_map), 0, 256 * sizeof(struct trie_node*));

	parent->char_map[(unsigned char)ch_id] = pnode;

	return pnode;
}

void trie::delete_trie_node(trie *ptrie, struct trie_node *pnode, void *pvoid) {
	delete pnode;
}

void trie::add_trie_node_set(trie *ptrie, struct trie_node *pnode, void *pvoid) {
	set<int> *presult = (set<int>*) pvoid;
	set<int>::iterator iter = pnode->id_set.begin(), endi = pnode->id_set.end();
	while(iter != endi) {
		presult->insert(*iter);
		++iter;
	}
}


trie::trie() {
	trie_root.char_id = 0;
	trie_root.id_set.clear();
	memset(&(trie_root.char_map), 0, 256 * sizeof(struct trie_node*));
}

trie::~trie() {
	// TODO : delete all the allocated memory
	dfs_traverse(&trie_root, delete_trie_node, NULL);
}
