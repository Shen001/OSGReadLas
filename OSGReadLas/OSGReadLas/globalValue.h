#pragma once 
#include <osg/Switch> 

extern osg::ref_ptr<osg::Switch> root_one = new osg::Switch;
extern osg::ref_ptr<osg::Switch> root_all = new osg::Switch;

class global_val
{
public:
	global_val(void);
	~global_val(void);
};