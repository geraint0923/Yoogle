#ifndef _YDB_H_
#define _YDB_H_

#include <node.h>
#include <v8.h>


using namespace v8;
using namespace node;


class ydb : ObjectWrap {
public:
	ydb() {}
	~ydb() {}

	int create_index(const char *datapath);

	void helloworld();

	static Handle<Value> New(const Arguments &args);

	static void Initialize(Handle<Object> target);

	static Handle<Value> get(const Arguments &args);

private:

	double cal_distance(double x1, double y1, double x2, double y2);



};


#endif
