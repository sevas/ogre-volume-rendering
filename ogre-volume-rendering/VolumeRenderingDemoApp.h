#ifndef VolumeRenderingDemoApp_h__
#define VolumeRenderingDemoApp_h__

#include "OgreApplication.h"

#include "Volume.h"

class VolumeRenderingDemoApp : public OgreApplication
{
protected:
	typedef struct{
		Ogre::Vector3 Position;
		Ogre::Vector3 TexCoord;
	} MyVertexData;
	typedef struct{
		unsigned short V1, V2, V3, V4;
		Ogre::Vector3 Normal;
	}MyCubeFace;

public:
	VolumeRenderingDemoApp(void);
	virtual ~VolumeRenderingDemoApp(void);

protected:
	void createScene();
	void _loadVolume();
	void _create3DTextureFromVolume();
	void _loadSlice( const std::string &_directory
					,const std::string& _basename);
	void _createTextureFromSlice();
	void _createCube();
	void _createSlice();

	bool frameStarted(const Ogre::FrameEvent&);

protected:
	Volume mVolume;

	Ogre::TexturePtr mVolumeTexture;
	Ogre::MaterialPtr mVolumeMaterial;
	Ogre::ManualObject *mUnitCube;
	Ogre::SceneNode  *mCubeNode;

	Ogre::ManualObject *mSlicePlane;
	Ogre::SceneNode  *mSliceNode;

	MyVertexData mCube[8];
	MyCubeFace   mCubeFaces[6];

	Volume::Slice mSlice;
	Ogre::TexturePtr mSliceTexture;
	Ogre::MaterialPtr mSliceMaterial;
	Ogre::GpuProgramParametersSharedPtr mVolumeSlicerParams;

	Ogre::Real mSlicer, mSlicerInc;
};
#endif // VolumeRenderingDemoApp_h__
