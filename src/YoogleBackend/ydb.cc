#include <node.h>
#include <v8.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "ydb.h"


using namespace std;
using namespace node;
using namespace v8;


Handle<Value> ydb::New(const Arguments &args) {
	HandleScope scope;
	
	ydb *db = new ydb();
	db->Wrap(args.This());

	return scope.Close(args.This());
}

void ydb::Initialize(Handle<Object> target) {
	HandleScope scope;

	Local<FunctionTemplate> t = FunctionTemplate::New(New);
	t->InstanceTemplate()->SetInternalFieldCount(1);

	NODE_SET_PROTOTYPE_METHOD(t, "get", ydb::get);
	target->Set(String::NewSymbol("ydb"), t->GetFunction());
}

Handle<Value> ydb::get(const Arguments &args) {
	HandleScope scope;
	
	static int count = 0;

	ydb *db = ObjectWrap::Unwrap<ydb>(args.This());
	Local<Object> result = Object::New();

	result->Set(v8::String::New("ret"), v8::Integer::New(count));
	
	++count;

	return scope.Close(result);
}


extern "C" void init(Handle<Object> target) {
	HandleScope scope;
	ydb::Initialize(target);
}
