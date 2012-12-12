#ifndef _YDB_H_
#define _YDB_H_

#include <node.h>
#include <v8.h>
#include <vector>
#include "trie.h"

using namespace std;
using namespace v8;
using namespace node;

struct entry {
	int id;
	char *addr;
	long long date;
	double lat_coord;
	double lng_coord;
	char *name;
	int post_code;
	char *url;
};

class ydb {
public:
	static Handle<Value> insert_record(const Arguments &args);

	static ydb *get_instance();

	ydb() {}
	~ydb();

private:
	static ydb *instance;
	
	vector<entry> entry_list;

	trie word_trie;
	

	double cal_distance(double x1, double y1, double x2, double y2);
	void process_entry(struct entry &ent);



};


#endif
