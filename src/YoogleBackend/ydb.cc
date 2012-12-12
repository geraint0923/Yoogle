#include <node.h>
#include <v8.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include "ydb.h"


using namespace std;
using namespace node;
using namespace v8;

ydb* ydb::instance = NULL;

Handle<Value> ydb::insert_record(const Arguments &args) {
	ydb *db = get_instance();

	if(db) {
		struct entry ent;
		char *buffer;
		ent.id = args[0]->NumberValue();
		ent.date = args[2]->NumberValue();
		ent.lat_coord = args[3]->NumberValue();
		ent.lng_coord = args[4]->NumberValue();
		ent.post_code = args[6]->NumberValue();

		buffer = *String::Utf8Value(args[1]);
		ent.addr = new char[strlen(buffer)+1];
		strcpy(ent.addr, buffer);

		buffer = *String::Utf8Value(args[5]);
		ent.name = new char[strlen(buffer)+1];
		strcpy(ent.name, buffer);

		buffer = *String::Utf8Value(args[7]);
		ent.url = new char[strlen(buffer)+1];
		strcpy(ent.url, buffer);

		/*
		if(args[1]->IsString()) {
			printf("is number!\n");
		}
		printf("DB not null, 0x%08x addr:%s\n", db, ent.addr);
		*/

		db->process_entry(ent);

		db->entry_list.push_back(ent);
		
		//printf("%d\n", db->entry_list.size());
	}

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

void ydb::process_entry(struct entry &ent) {
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
	}
	
	NODE_MODULE(ydb, init);
}
