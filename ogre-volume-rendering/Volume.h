
#ifndef Volume_h__
#define Volume_h__

#include <vector>

class Volume
{
public:
	typedef std::vector<unsigned short> Slice;
	typedef std::vector<Slice>			Slices;
public:
	Volume(void);
	virtual ~Volume(void);

	void loadSlices(const std::string&, unsigned);
	unsigned short getVoxelValue(unsigned, unsigned, unsigned);

protected:
	Slices mSlices;
	unsigned mHeight, mWidth;
	unsigned mSliceHeight, mSliceWidth;
	unsigned mNSlices;
};
#endif // Volume_h__
