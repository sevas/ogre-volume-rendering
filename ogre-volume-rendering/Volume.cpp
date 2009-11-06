#include "precompiled.h"
#include "Volume.h"

#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

namespace fs = boost::filesystem;

//-----------------------------------------------------------------------------
Volume::Volume(void)
	:mSliceHeight(256)
	,mSliceWidth(256)
{
}
//-----------------------------------------------------------------------------
Volume::~Volume(void)
{
}
//-----------------------------------------------------------------------------
void Volume::loadSlices(const std::string &_directory, unsigned _nSlices)
{
	mNSlices = _nSlices;
	mSlices.resize(_nSlices);
	boost::format fmt("%s/%s.%d");
	
	for (unsigned z=0; z<_nSlices ; ++z)
	{	
		mSlices[z].resize(mSliceHeight * mSliceWidth);
		std::ifstream file;
		
		fmt % _directory % "MRbrain" % (z+1);
		std::string filename = fmt.str();
		file.open(filename.c_str(), std::ios_base::binary);
		if(file)
		{
			for(unsigned i=0 ; i<mSliceWidth*mSliceHeight ; ++i)
			{
				
				unsigned short val;
				val = (unsigned short) file.get();
				mSlices[z][i] = val;
			}
			file.close();
		}

	}
}
//-----------------------------------------------------------------------------
unsigned short Volume::getVoxelValue(unsigned _x, unsigned _y, unsigned _z)
{
	if (_x < mSliceWidth && _y < mSliceHeight && _z < mNSlices)
	{
		return mSlices[_z][_y*mSliceWidth + _x];
	}
	else
		return -1;
}
//-----------------------------------------------------------------------------