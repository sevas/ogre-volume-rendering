/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

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
    void _createCube();
    void _createSlice();

    bool frameStarted(const Ogre::FrameEvent&);

protected:
    Volume mVolume;

    Ogre::TexturePtr mVolumeTexture;
    Ogre::MaterialPtr mVolumeMaterial;
    Ogre::ManualObject *mUnitCube;
    Ogre::SceneNode  *mCubeNode;

	//Ogre::ManualObject *mSlicePlane;
    Ogre::Entity *mSlicePlane;
    Ogre::SceneNode  *mSliceNode;

    MyVertexData mCube[8];
    MyCubeFace   mCubeFaces[6];

    Volume::Slice mSlice;
    Ogre::MaterialPtr mSliceMaterial;
    Ogre::Real mSlicer, mSlicerInc;
};
#endif // VolumeRenderingDemoApp_h__
