#include <node.h>
#include <v8.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <set>
#include <string>

#include <unistd.h>
#include <sys/time.h>

#include "ydb.h"


using namespace std;
using namespace node;
using namespace v8;

ydb* ydb::instance = NULL;

static char *init_str(const Arguments &args, int idx) {
	String::Utf8Value value(args[idx]);
	int len = strlen(*value);
	char *buffer = new char[len + 1];
	strcpy(buffer, *value);
	return buffer;
}

Handle<Value> ydb::insert_record(const Arguments &args) {
	ydb *db = get_instance();

	if(db) {
		struct entry ent;
		string str;
		ent.id = args[0]->NumberValue();
		ent.date = args[2]->NumberValue();
		ent.lat_coord = args[3]->NumberValue();
		ent.lng_coord = args[4]->NumberValue();
		ent.post_code = args[6]->NumberValue();

		/*
		str = string(*String::Utf8Value(args[1]));
		ent.addr = new char[str.size()+1];
		strcpy(ent.addr, str.c_str());
		*/
		ent.addr = init_str(args, 1);

		/*
		str = string(*String::Utf8Value(args[7]));
		ent.url = new char[str.size()+1];
		strcpy(ent.url, str.c_str());
		*/
		ent.url = init_str(args, 7);

		/*
		str = string(*String::Utf8Value(args[5]));
		ent.name = new char[str.size()+1];
		strcpy(ent.name, str.c_str());
		printf("ent.name:0x%08x\n", ent.name);
		*/
		ent.name = init_str(args, 5);
				/*
		if(args[1]->IsString()) {
			printf("is number!\n");
		}
		printf("DB not null, 0x%08x addr:%s\n", db, ent.addr);
		*/

		db->process_keyword(ent.name, db->entry_list.size());

		db->entry_list.push_back(ent);
		
		//printf("%d\n", db->entry_list.size());
	}

	return Undefined();
}

Handle<Value> ydb::set_position(double lat, double lng) {
	ydb *db = get_instance();

	db->lat_coord = lat;
	db->lng_coord = lng;

	return Undefined();
}


Handle<Value> ydb::prefix_search(const Arguments &args) {
	ydb *db = get_instance();

	char *buffer = init_str(args, 0);

	/* large -> small */
	double dist_list[] = {999999, 999999, 999999, 999999, 999999, 999999};
	int idx_list[] = {-1, -1, -1, -1, -1};

	set<int> res;

	double sec;
	struct timeval start_val, end_val;
	gettimeofday(&start_val, NULL);

	db->word_trie.prefix_search(buffer, res);

	gettimeofday(&end_val, NULL);

	end_val.tv_sec -= start_val.tv_sec;
	end_val.tv_usec -= start_val.tv_usec;

	sec = end_val.tv_sec + (double)end_val.tv_usec / 1000000.0;

	set<int>::iterator iter = res.begin(), endi = res.end();
	while(iter != endi) {
		int idx = *iter;
		++iter;

		double dist = cal_distance(db->lat_coord, db->lng_coord, 
				db->entry_list[idx].lat_coord, db->entry_list[idx].lng_coord);
		int max_i = -1, i;
		for(i = 0; i < 5; ++i) {
			if(dist < dist_list[i]) {
				if(max_i == -1) {
					max_i = i;
				} else {
					if(dist_list[i] > dist_list[max_i]) {
						max_i = i;
					}
				}
			}
		}
		if(max_i != -1) {
			dist_list[max_i] = dist;
			idx_list[max_i] = idx;
		}
		printf("%d------%s     %lf\n", idx, db->entry_list[idx].name, dist);
	}
	printf("top 5:\n");
	
	vector<int> res_idx;
	for(int i = 0; i < 5; ++i) {
		int min_i = -1;
		double min_d = 99999;
		for(int j = 0; j < 5; ++j) {
			if(idx_list[j] != -1) {
				if(dist_list[j] < min_d) {
					min_i = j;
					min_d = dist_list[j];
				}
			}
		}
		if(min_i == -1)
			break;
		res_idx.push_back(idx_list[min_i]);
		idx_list[min_i] = -1;
		printf("%d------%s, %lf\n", res_idx[i], db->entry_list[res_idx[i]].name, dist_list[min_i]);
	}
	printf("finish?  %lf\n", sec);


	delete buffer;

	return Undefined();
}

ydb *ydb::get_instance() {
	if(instance == NULL) {
		instance = new ydb;
	}
	return instance;
}

double ydb::cal_distance(double x1, double y1, double x2, double y2) {
	double dx = x1 - x2, dy = y1 - y2;
	return sqrt(dx * dx + dy * dy);
}

void ydb::process_keyword(char *key, const int id) {
	char buffer[1024];
	int len = strlen(key);
	for(int i = 0; i < len; ++i) {
		buffer[i] = tolower(key[i]);
	}
	buffer[len] = 0;
	word_trie.insert_entry(buffer, id);
}



ydb::~ydb() {
	int len = entry_list.size();
	for(int i = 0; i < len; ++i) {
		struct entry &ent = entry_list[i];
		delete ent.addr;
		delete ent.name;
		delete ent.url;
	}
	entry_list.clear();

	instance = NULL;
}


extern "C" {
	static void init(Handle<Object> target) {
		NODE_SET_METHOD(target, "insert_record", ydb::insert_record);
		NODE_SET_METHOD(target, "prefix_search", ydb::prefix_search);
	}
	
	NODE_MODULE(ydb, init);
}
