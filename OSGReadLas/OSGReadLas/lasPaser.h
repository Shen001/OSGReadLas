// lasreader.h: interface for the lasreader class. 
// 
////////////////////////////////////////////////////////////////////// 
#ifndef LAS_READER_H 
#define LAS_READER_H 

#include "lasdefinitions.h" 

#include <stdio.h> 

class laspointReader
{
public:
	virtual bool read_point(LASpoint* point, double* gps_time = 0) = 0;
	virtual ~laspointReader() {};
};

class LASpointReader0raw : public laspointReader
{
public:
	inline bool read_point(LASpoint* point, double* gps_time = 0)
	{
		return (fread(point, sizeof(LASpoint), 1, file) == 1);
	};
	LASpointReader0raw(FILE* file)
	{
		this->file = file;
	};
	~LASpointReader0raw() {};
private:
	FILE* file;
};

class LASpointReader1raw : public laspointReader
{
public:
	inline bool read_point(LASpoint* point, double* gps_time = 0)
	{
		fread(point, sizeof(LASpoint), 1, file);
		return (fread(gps_time, sizeof(double), 1, file) == 1);
	};
	LASpointReader1raw(FILE* file)
	{
		this->file = file;
	};
	~LASpointReader1raw() {};
private:
	FILE* file;
};

class lasreader
{
public:
	LASheader header;
	LASpoint point;
	double gps_time;

	int npoints;
	int p_count;

	bool open(FILE* file, bool skip_to_point_data = true, bool only_skip_variable_header = false);
	bool open_skipheader();

	bool read_point();
	bool read_point(float* coordinates);
	bool read_point(double* coordinates);
	bool read_point(LASpoint* Ppoint, double* gps_time = 0);
	bool read_point(LASpt* Ppoint);
	void close();

	lasreader();
	virtual ~lasreader();

private:
	FILE* file;
	laspointReader* pointReader;
};

#endif 