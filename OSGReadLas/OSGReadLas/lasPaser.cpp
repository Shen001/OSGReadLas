// lasreader.cpp: implementation of the lasreader class.   
//   
//////////////////////////////////////////////////////////////////////   

//#include "stdafx.h"   
#include "lasPaser.h"   

//#ifdef _DEBUG   
//#undef THIS_FILE   
//static char THIS_FILE[]=__FILE__;   
//#define new DEBUG_NEW   
//#endif   

#ifdef _WIN32   
#include <fcntl.h>   
#include <io.h>   
#endif   

#include <stdlib.h>   
#include <string.h>   
//////////////////////////////////////////////////////////////////////   
// Construction/Destruction   
//////////////////////////////////////////////////////////////////////   

lasreader::lasreader()
{
	file = 0;
	gps_time = 0.0;

	npoints = -1;
	p_count = -1;

	pointReader = 0;
}

lasreader::~lasreader()
{

}

void lasreader::close()
{
	file = 0;
	gps_time = 0.0;
	header.Init();
	npoints = -1;
	p_count = -1;
	point.Init();
	if (pointReader)
	{
		delete pointReader;
		pointReader = 0;
	}
}

bool lasreader::open(FILE* file, bool skip_to_point_data, bool only_skip_variable_header)
{
	if (file == 0)
	{
		fprintf(stderr, "ERROR: file pointer is zero\n");
		return false;
	}

	this->file = file;

	// read header variable after variable (to avoid alignment issues)   

	if (fread(&(header.file_signature), sizeof(char), 4, file) != 4)
	{
		//fprintf(stderr,"ERROR: reading header.file_signature\n");   
		return false;
	}
	if (fread(&(header.file_source_id), sizeof(unsigned short), 1, file) != 1)
	{
		//  fprintf(stderr,"ERROR: reading header.file_source_id\n");   
		return false;
	}
	if (fread(&(header.reserved), sizeof(unsigned short), 1, file) != 1)
	{
		//fprintf(stderr,"ERROR: reading header.reserved\n");   
		return false;
	}
	if (fread(&(header.project_ID_GUID_data_1), sizeof(unsigned int), 1, file) != 1)
	{
		//fprintf(stderr,"ERROR: reading header.project_ID_GUID_data_1\n");   
		return false;
	}
	if (fread(&(header.project_ID_GUID_data_2), sizeof(unsigned short), 1, file) != 1)
	{
		//fprintf(stderr,"ERROR: reading header.project_ID_GUID_data_2\n");   
		return false;
	}
	if (fread(&(header.project_ID_GUID_data_3), sizeof(unsigned short), 1, file) != 1)
	{
		//fprintf(stderr,"ERROR: reading header.project_ID_GUID_data_3\n");   
		return false;
	}
	if (fread(&(header.project_ID_GUID_data_4), sizeof(char), 8, file) != 8)
	{
		//fprintf(stderr,"ERROR: reading header.project_ID_GUID_data_4\n");   
		return false;
	}
	if (fread(&(header.version_major), sizeof(char), 1, file) != 1)
	{
		//fprintf(stderr,"ERROR: reading header.version_major\n");   
		return false;
	}
	if (fread(&(header.version_minor), sizeof(char), 1, file) != 1)
	{
		//fprintf(stderr,"ERROR: reading header.version_minor\n");   
		return false;
	}
	if (fread(&(header.system_identifier), sizeof(char), 32, file) != 32)
	{
		//fprintf(stderr,"ERROR: reading header.system_identifier\n");   
		return false;
	}
	if (fread(&(header.generating_software), sizeof(char), 32, file) != 32)
	{
		//fprintf(stderr,"ERROR: reading header.generating_software\n");   
		return false;
	}
	if (fread(&(header.file_creation_day), sizeof(unsigned short), 1, file) != 1)
	{
		//fprintf(stderr,"ERROR: reading header.file_creation_day\n");   
		return false;
	}
	if (fread(&(header.file_creation_year), sizeof(unsigned short), 1, file) != 1)
	{
		//fprintf(stderr,"ERROR: reading header.file_creation_year\n");   
		return false;
	}
	if (fread(&(header.header_size), sizeof(unsigned short), 1, file) != 1)
	{
		//fprintf(stderr,"ERROR: reading header.header_size\n");   
		return false;
	}
	if (fread(&(header.offset_to_point_data), sizeof(unsigned int), 1, file) != 1)
	{
		//fprintf(stderr,"ERROR: reading header.offset_to_point_data\n");   
		return false;
	}
	if (fread(&(header.number_of_variable_length_records), sizeof(unsigned int), 1, file) != 1)
	{
		//fprintf(stderr,"ERROR: reading header.number_of_variable_length_records\n");   
		return false;
	}
	if (fread(&(header.point_data_format), sizeof(unsigned char), 1, file) != 1)
	{
		//fprintf(stderr,"ERROR: reading header.point_data_format\n");   
		return false;
	}
	if (fread(&(header.point_data_record_length), sizeof(unsigned short), 1, file) != 1)
	{
		//fprintf(stderr,"ERROR: reading header.point_data_record_length\n");   
		return false;
	}
	if (fread(&(header.number_of_point_records), sizeof(unsigned int), 1, file) != 1)
	{
		//fprintf(stderr,"ERROR: reading header.number_of_point_records\n");   
		return false;
	}
	if (fread(&(header.number_of_points_by_return), sizeof(unsigned int), 5, file) != 5)
	{
		//  fprintf(stderr,"ERROR: reading header.number_of_points_by_return\n");   
		return false;
	}
	if (fread(&(header.x_scale_factor), sizeof(double), 1, file) != 1)
	{
		//fprintf(stderr,"ERROR: reading header.x_scale_factor\n");   
		return false;
	}
	if (fread(&(header.y_scale_factor), sizeof(double), 1, file) != 1)
	{
		//fprintf(stderr,"ERROR: reading header.y_scale_factor\n");   
		return false;
	}
	if (fread(&(header.z_scale_factor), sizeof(double), 1, file) != 1)
	{
		//fprintf(stderr,"ERROR: reading header.z_scale_factor\n");   
		return false;
	}
	if (fread(&(header.x_offset), sizeof(double), 1, file) != 1)
	{
		//fprintf(stderr,"ERROR: reading header.x_offset\n");   
		return false;
	}
	if (fread(&(header.y_offset), sizeof(double), 1, file) != 1)
	{
		//  fprintf(stderr,"ERROR: reading header.y_offset\n");   
		return false;
	}
	if (fread(&(header.z_offset), sizeof(double), 1, file) != 1)
	{
		//  fprintf(stderr,"ERROR: reading header.z_offset\n");   
		return false;
	}
	if (fread(&(header.max_x), sizeof(double), 1, file) != 1)
	{
		//  fprintf(stderr,"ERROR: reading header.max_x\n");   
		return false;
	}
	if (fread(&(header.min_x), sizeof(double), 1, file) != 1)
	{
		//  fprintf(stderr,"ERROR: reading header.min_x\n");   
		return false;
	}
	if (fread(&(header.max_y), sizeof(double), 1, file) != 1)
	{
		//  fprintf(stderr,"ERROR: reading header.max_y\n");   
		return false;
	}
	if (fread(&(header.min_y), sizeof(double), 1, file) != 1)
	{
		//  fprintf(stderr,"ERROR: reading header.min_y\n");   
		return false;
	}
	if (fread(&(header.max_z), sizeof(double), 1, file) != 1)
	{
		//  fprintf(stderr,"ERROR: reading header.max_z\n");   
		return false;
	}
	if (fread(&(header.min_z), sizeof(double), 1, file) != 1)
	{
		//  fprintf(stderr,"ERROR: reading header.min_z\n");   
		return false;
	}

	// check header contents   

	if (strncmp(header.file_signature, "LASF", 4) != 0)
	{
		//fprintf(stderr,"ERROR: wrong file signature '%s'\n", header.file_signature);   
		return false;
	}
	if ((header.version_major != 1) || ((header.version_minor != 0) && (header.version_minor != 1)))
	{
		//fprintf(stderr,"WARNING: unknown version %d.%d (should be 1.0 or 1.1)\n", header.version_major, header.version_minor);   
	}
	if (header.header_size != 227)
	{
		//fprintf(stderr,"WARNING: header size is %d but should be 227\n", header.header_size);   
		//::AfxMessageBox("header size should be 227!");   
	}
	if (header.offset_to_point_data < header.header_size)
	{
		//fprintf(stderr,"ERROR: offset to point data %d is smaller than header size %d\n", header.offset_to_point_data, header.header_size);   
		return false;
	}
	if (header.number_of_point_records <= 0)
	{
		//fprintf(stderr,"WARNING: number of point records is %d\n", header.number_of_point_records);   
	}
	if ((header.point_data_format & 127) == 0)
	{
		if (header.point_data_record_length != 20)
		{
			//fprintf(stderr,"WARNING: wrong point data record length of %d instead of 20 for format 0\n", header.point_data_record_length);   
		}
	}
	else if ((header.point_data_format & 127) == 1)
	{
		if (header.point_data_record_length != 28)
		{
			//fprintf(stderr,"WARNING: wrong point data record length of %d instead of 28 for format 1\n", header.point_data_record_length);   
		}
	}
	else
	{
		//fprintf(stderr,"WARNING: unknown point data format %d\n", header.point_data_format);   
	}
	if (header.x_scale_factor == 0 || header.y_scale_factor == 0 || header.z_scale_factor == 0)
	{
		//fprintf(stderr,"WARNING: some scale factors are zero %g %g %g. those are set to 0.01.\n", header.x_scale_factor, header.y_scale_factor, header.z_scale_factor);   
		if (header.x_scale_factor == 0) header.x_scale_factor = 0.01;
		if (header.y_scale_factor == 0) header.y_scale_factor = 0.01;
		if (header.z_scale_factor == 0) header.z_scale_factor = 0.01;
	}
	if (header.max_x < header.min_x || header.max_y < header.min_y || header.max_z < header.min_z)
	{
		//fprintf(stderr,"WARNING: invalid bounding box [ %g %g %g / %g %g %g ]\n", header.min_x, header.min_y, header.min_z, header.max_x, header.max_y, header.max_z);   
	}

	// create the right point reader in dependance on compression and point data format   

	if (header.point_data_format & 128)
	{
		//::AfxMessageBox("1");   
	}
	else
	{
		if (header.point_data_format)
		{
			pointReader = new LASpointReader1raw(file);
		}
		else
		{
			pointReader = new LASpointReader0raw(file);
		}
	}

	// should we move the file pointer to where the point data starts   

	if (skip_to_point_data)
	{
		for (int i = 227; i < (int)header.offset_to_point_data; i++)
		{
			fgetc(file);
		}
	}
	else if (only_skip_variable_header)
	{
		LASvariable_header variable_header;
		LASvariable_header_geo_keys variable_header_geo_keys;
		LASvariable_header_key_entry variable_header_key_entry;

		// read some of the other stuff (just for the exercise)   

		for (int i = 0; i < (int)header.number_of_variable_length_records; i++)
		{
			if (fread(&variable_header, sizeof(LASvariable_header), 1, file) != 1)
			{
				fprintf(stderr, "ERROR: reading variable_header %d of %d\n", i, header.number_of_variable_length_records);
				return false;
			}
			if (strcmp(variable_header.user_id, "LASF_Projection") == 0 && variable_header.record_id == 34735)
			{
				if (fread(&variable_header_geo_keys, sizeof(LASvariable_header_geo_keys), 1, file) != 1)
				{
					fprintf(stderr, "ERROR: reading geo keys of variable_header %d of %d\n", i, header.number_of_variable_length_records);
					return false;
				}
				for (int j = 0; j < variable_header_geo_keys.number_of_keys; j++)
				{
					if (fread(&variable_header_key_entry, sizeof(LASvariable_header_key_entry), 1, file) != 1)
					{
						fprintf(stderr, "ERROR: reading key entry %d of %d of variable_header %d of %d\n", j, variable_header_geo_keys.number_of_keys, i, header.number_of_variable_length_records);
						return false;
					}
				}
			}
			else
			{
				for (int j = 0; j < variable_header.record_length_after_header; j++)
				{
					fgetc(file);
				}
			}
		}
	}

	gps_time = 0.0;
	npoints = header.number_of_point_records;
	p_count = 0;

	return true;
}

bool lasreader::open_skipheader()
{
	fseek(file, header.offset_to_point_data, SEEK_SET);
	gps_time = 0.0;
	npoints = header.number_of_point_records;
	p_count = 0;
	return true;
}

bool lasreader::read_point()
{
	if (p_count < npoints)
	{
		if (pointReader->read_point(&point, &gps_time) == false)
		{
			//fprintf(stderr,"WARNING: end-of-file after %d of %d points\n", p_count, npoints);   
			return false;
		}
		p_count++;
		return true;
	}
	return false;
}

bool lasreader::read_point(float* coordinates)
{
	if (read_point())
	{
		coordinates[0] = (float)(point.x*header.x_scale_factor + header.x_offset);
		coordinates[1] = (float)(point.y*header.y_scale_factor + header.y_offset);
		coordinates[2] = (float)(point.z*header.z_scale_factor + header.z_offset);
		return true;
	}
	return false;
}

bool lasreader::read_point(double* coordinates)
{
	if (read_point())
	{
		coordinates[0] = point.x*header.x_scale_factor + header.x_offset;
		coordinates[1] = point.y*header.y_scale_factor + header.y_offset;
		coordinates[2] = point.z*header.z_scale_factor + header.z_offset;
		return true;
	}
	return false;
}

bool lasreader::read_point(LASpt* Ppoint)
{
	if (read_point())
	{
		Ppoint->x = point.x*header.x_scale_factor + header.x_offset;
		Ppoint->y = point.y*header.y_scale_factor + header.y_offset;
		Ppoint->z = point.z*header.z_scale_factor + header.z_offset;
		Ppoint->intensity = point.intensity;
		Ppoint->return_number = point.return_number;

		return true;
	}
	return false;
}