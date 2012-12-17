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

		++(db->entry_count);
		
		//printf("%d\n", db->entry_list.size());
	}

	return Undefined();
}

Handle<Value> ydb::set_position(const Arguments &args) {
	ydb *db = get_instance();

	db->lat_coord = args[0]->NumberValue();
	db->lng_coord = args[1]->NumberValue();

	return Undefined();
}


Handle<Value> ydb::prefix_search(const Arguments &args) {
	HandleScope scope;

	ydb *db = get_instance();

	set_position(args);

	char *buffer = init_str(args, 2);

	int buffer_len = strlen(buffer);
	for(int i = 0; i < buffer_len; ++i) {
		buffer[i] = tolower(buffer[i]);
	}

	/* large -> small */
	double dist_list[] = {999999, 999999, 999999, 999999, 999999, 999999};
	int idx_list[] = {-1, -1, -1, -1, -1};

	vector<int> res;
	vector<int> tmp_res;

	if(!db->count_list) {
		db->count_list = new int[db->entry_count];
	}
	memset(db->count_list, 0, sizeof(int) * db->entry_count);

	double sec;
	struct timeval start_val, end_val;

	gettimeofday(&start_val, NULL);


	int origin_len = strlen(buffer) + 1;
	int seg_count = 0;
	
	bool start = false;
	char *pbegin = NULL;
	for(int i = 0; i < origin_len; ++i) {
		if(trie::is_seperator(buffer[i])) {
			if(start) {
				start = false;
				buffer[i] = 0;
				tmp_res.clear();
				res.clear();
				++seg_count;
				db->word_trie.prefix_search((const char*)pbegin, tmp_res);
				
				//printf("pbegin:%s count:%d\n", pbegin, tmp_res.size());


				vector<int>::iterator iter = tmp_res.begin(), endi = tmp_res.end();
				while(iter != endi) {
					++(db->count_list[*iter]);
					if(db->count_list[*iter] == seg_count) {
						res.push_back(*iter);
					}
					++iter;
				}
				//printf("OK!!");
			} else {
				continue;
			}
		} else {
			if(!start)
				pbegin = buffer + i;
			start = true;
		}
	}



	vector<int>::iterator iter = res.begin(), endi = res.end();
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
		//printf("%d------%s     %lf\n", idx, db->entry_list[idx].name, dist);
	}
	printf("top 5:\n");
	
	int ec = 0;
	for(int i = 0; i < 5; ++i) {
		if(idx_list[i] != -1) {
			++ec;
		}
	}
	Local<Array> ret_result = Array::New(ec);
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
		
		Local<Object> res_ent = Object::New();
		struct entry *pent = &(db->entry_list[res_idx[i]]);
		res_ent->Set(String::New("id"), Integer::New(pent->id));
		res_ent->Set(String::New("addr"), String::New(pent->addr));
		//res_ent->Set(String::New("date"), Integer::New(pent->date));
		res_ent->Set(String::New("lat_coord"), Number::New(pent->lat_coord));
		res_ent->Set(String::New("lng_coord"), Number::New(pent->lng_coord));
		res_ent->Set(String::New("name"), String::New(pent->name));
		res_ent->Set(String::New("post_code"), Integer::New(pent->post_code));
		res_ent->Set(String::New("url"), String::New(pent->url));

		ret_result->Set(i, res_ent);
	}

	gettimeofday(&end_val, NULL);

	end_val.tv_sec -= start_val.tv_sec;
	end_val.tv_usec -= start_val.tv_usec;

	sec = end_val.tv_sec + (double)end_val.tv_usec / 1000000.0;

	printf("finish?  %lf\n", sec);

	Local<Object> obj = Object::New();
	obj->Set(String::New("result"), ret_result);
	obj->Set(String::New("time"), Number::New(sec));

	delete buffer;

	return scope.Close(obj);
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
		NODE_SET_METHOD(target, "set_position", ydb::set_position);
	}
	
	NODE_MODULE(ydb, init);
}
