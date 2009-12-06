/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#include "precompiled.h"
#include "Volume.h"

#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

namespace fs = boost::filesystem;

unsigned short swapBytes(unsigned short u)
{
    U16_U8 source;
    U16_U8 dest;

    source.u16 = u;
    dest.u8[0] = source.u8[1];
    dest.u8[1] = source.u8[0];

    return dest.u16;
}
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
                U16_U8 val;
                val.u8[0] = file.get();
                val.u8[1] = file.get();
                mSlices[z][i] = val.u16;
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