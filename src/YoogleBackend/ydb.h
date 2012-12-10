#ifndef _YDB_H_
#define _YDB_H_

class ydb {
public:
	ydb() {}
	~ydb() {}

	int create_index(const char *datapath);

private:

	double cal_distance(double x1, double y1, double x2, double y2);



};


#endif
