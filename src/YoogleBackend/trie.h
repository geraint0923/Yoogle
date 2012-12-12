#ifndef _TRIE_H_
#define _TRIE_H_

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>

using namespace std;

struct trie_node {
	char char_id;
	set<int> id_set;
	struct trie_node *next_node;
	struct trie_node *first_child;
};

class trie {
public:
	trie();
	~trie();

	void insert_entry(const char *str, const int id);

	void prefix_search(const char *prefix, vector<int> &result);

private:
	struct trie_node trie_root;

	void dissect_string(const char *str, int id);

	void dfs_traverse(struct trie_node *node, void (*func)(trie *ptrie, struct trie_node *pnode, void *pvoid));

	static void delete_trie_node(trie *ptrie, struct trie_node *pnode, void *pvoid);
	
	static void add_trie_node_set(trie *ptrie, struct trie_node *pnode, void *pvoid);
};

#endif
