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
	struct trie_node *char_map[256];
};

class trie {
public:
	trie();
	~trie();

	void insert_entry(const char *str, const int id);

	void prefix_search(const char *prefix, set<int> &result);

private:
	struct trie_node trie_root;

	void insert_word(const char *str, int len, int id);

	bool dissect_string(const char *str, int id);

	void dfs_traverse(struct trie_node *node, void (*func)(trie *ptrie, struct trie_node *pnode, void *pvoid), void *pvoid);

	struct trie_node *add_node(struct trie_node *parent, char ch_id);

	struct trie_node *find_node(const char *pfx, int len, bool create);

	bool is_seperator(char ch);

	static void delete_trie_node(trie *ptrie, struct trie_node *pnode, void *pvoid);
	
	static void add_trie_node_set(trie *ptrie, struct trie_node *pnode, void *pvoid);
};

#endif
