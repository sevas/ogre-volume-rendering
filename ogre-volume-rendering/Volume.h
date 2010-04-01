/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#ifndef Volume_h__
#define Volume_h__

#include <vector>

typedef union {
    unsigned short u16;
    unsigned char  u8[2];
} U16_U8;

unsigned short swapBytes(unsigned short);
//-----------------------------------------------------------------------------
class Volume
{
public:
    typedef std::vector<unsigned short> Slice;
    typedef std::vector<Slice>          Slices;
public:
    Volume(void);
    virtual ~Volume(void);

    void loadSlices(const std::string&, unsigned);
    unsigned short getVoxelValue(unsigned, unsigned, unsigned);
    unsigned char Volume::getVoxelByteValue(unsigned _x, unsigned _y, unsigned _z);

protected:
    Slices mSlices;
    unsigned mHeight, mWidth;
    unsigned mSliceHeight, mSliceWidth;
    unsigned mNSlices;
};
#endif // Volume_h__
