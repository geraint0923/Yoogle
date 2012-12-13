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

	static Handle<Value> prefix_search(const Arguments &args);

	static Handle<Value> set_position(double lat, double lng);

	ydb() { lat_coord = 1.3; lng_coord = 103; entry_count = 0; count_list = NULL; }
	~ydb();

private:
	static ydb *instance;
	
	vector<entry> entry_list;

	trie word_trie;
	double lat_coord;
	double lng_coord;
	int *count_list;
	int entry_count;
	
	static ydb *get_instance();

	static double cal_distance(double x1, double y1, double x2, double y2);
	void process_keyword(char *key, int id);



};


#endif
