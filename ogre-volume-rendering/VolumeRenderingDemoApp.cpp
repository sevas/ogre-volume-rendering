#include "precompiled.h"
#include "VolumeRenderingDemoApp.h"

#include <boost/format.hpp>


typedef union
{
    Ogre::uint32 u32;
    Ogre::uint8  u8[4];

}U32_U8;

const int R=0;
const int G=1;
const int B=2;
const int A=3;

//-----------------------------------------------------------------------------
VolumeRenderingDemoApp::VolumeRenderingDemoApp(void)
	:OgreApplication("Ogre Volume Rendering")
	,mSlicer(0.0f)
	,mSlicerInc(1.0f)
{

}
//-----------------------------------------------------------------------------
VolumeRenderingDemoApp::~VolumeRenderingDemoApp(void)
{
}
//-----------------------------------------------------------------------------
void VolumeRenderingDemoApp::createScene()
{
	_createGrid(500);
	_createLight();
	_loadVolume();
	_create3DTextureFromVolume();
	_createCube();

	//_loadSlice("../volumes/MRbrain2", "MRbrain");
	_createSlice();
	//_createTextureFromSlice();
}
//-----------------------------------------------------------------------------
void VolumeRenderingDemoApp::_loadVolume()
{
	mVolume.loadSlices("../volumes/MRbrain2", 109);
}
//-----------------------------------------------------------------------------
void VolumeRenderingDemoApp::_create3DTextureFromVolume()
{	using namespace Ogre;
	// Create the texture
	mVolumeTexture = TextureManager::getSingleton().createManual(
		"MRbrain Volume", 
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		TEX_TYPE_3D,      
		256, 256,         
		109,		      
		0,                
	    PF_BYTE_RGBA,	
		TU_DEFAULT);		
		

	HardwarePixelBufferSharedPtr buffer = mVolumeTexture->getBuffer();

	buffer->lock(HardwareBuffer::HBL_NORMAL);
	const Ogre::PixelBox &pb = buffer->getCurrentLock();

	Ogre::uint32 *pbptr = static_cast<Ogre::uint32*>(pb.data);

	for(size_t z = pb.front ; z<pb.back ; z++)
	{
		for (size_t y=pb.top ; y<pb.bottom; y++)
		{
			for(size_t x=pb.left ; x<pb.right; x++)
			{
                U32_U8 val;
                val.u8[R] = 0;
                val.u8[G] = 0;
                val.u8[B] = 0;
                val.u8[A] = mVolume.getVoxelByteValue(x, y, z);
                pbptr[x] = val.u32;
			}
			pbptr += pb.rowPitch;
		}
		pbptr += pb.getSliceSkip();
	}


	buffer->unlock();
   
    mVolumeMaterial = MaterialManager::getSingleton().getByName("VolumeRender/Raycasting");
	mVolumeMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("MRbrain Volume");


}
//-----------------------------------------------------------------------------
void VolumeRenderingDemoApp::_createCube()
{
	
	// vertices

	//back
	Real z=0.0f;
	Real offset=0.0f;

	mCube[0].Position=Vector3(0.0f,	0.0f,	z);
	mCube[0].TexCoord=Vector3(0.0f+offset,	1.0f-offset,	z+offset);

	mCube[1].Position=Vector3(0.0f,	1.0f,	z);
	mCube[1].TexCoord=Vector3(0.0f+offset,	0.0f+offset,	z+offset);
	
	mCube[2].Position=Vector3(1.0f,	1.0f,	z);
	mCube[2].TexCoord=Vector3(1.0f-offset,	0.0f+offset,	z+offset);

	mCube[3].Position=Vector3(1.0f,	0.0f,	z);
	mCube[3].TexCoord=Vector3(1.0f-offset,	1.0f-offset,	z+offset);

	//front
	z = 1.0f;
	mCube[4].Position=Vector3(0.0f,	0.0f,	z);
	mCube[4].TexCoord=Vector3(0.0f+offset, 1.0f-offset, z-offset);

	mCube[5].Position=Vector3(0.0f,	1.0f,	z);
	mCube[5].TexCoord=Vector3(0.0f+offset,	0.0f+offset,	z-offset);

	mCube[6].Position=Vector3(1.0f,	1.0f,	z);
	mCube[6].TexCoord=Vector3(1.0f-offset,	0.0f+offset,	z-offset);

	mCube[7].Position=Vector3(1.0f,	0.0f,	z);
	mCube[7].TexCoord=Vector3(1.0f-offset,	1.0f-offset,	z-offset);

	// faces
	unsigned i=0;
	// back
	mCubeFaces[i].Normal = Vector3::NEGATIVE_UNIT_Z;
	mCubeFaces[i].V1 = 0;
	mCubeFaces[i].V2 = 1;
	mCubeFaces[i].V3 = 2;
	mCubeFaces[i].V4 = 3;

	// right
	i++;
	mCubeFaces[i].Normal = Vector3::UNIT_X;
	mCubeFaces[i].V1 = 7;
	mCubeFaces[i].V2 = 3;
	mCubeFaces[i].V3 = 2;
	mCubeFaces[i].V4 = 6;
	
	//front
	i++;
	mCubeFaces[i].Normal = Vector3::UNIT_Z;
	mCubeFaces[i].V1 = 4;
	mCubeFaces[i].V2 = 7;
	mCubeFaces[i].V3 = 6;
	mCubeFaces[i].V4 = 5;

	//left
	i++;
	mCubeFaces[i].Normal = Vector3::NEGATIVE_UNIT_X;
	mCubeFaces[i].V1 = 4;
	mCubeFaces[i].V2 = 5;
	mCubeFaces[i].V3 = 1;
	mCubeFaces[i].V4 = 0;

	// top
	i++;
	mCubeFaces[i].Normal = Vector3::UNIT_Y;
	mCubeFaces[i].V1 = 5;
	mCubeFaces[i].V2 = 6;
	mCubeFaces[i].V3 = 2;
	mCubeFaces[i].V4 = 1;

	// bottom
	i++;
	mCubeFaces[i].Normal = Vector3::NEGATIVE_UNIT_Y;
	mCubeFaces[i].V1 = 4;
	mCubeFaces[i].V2 = 0;
	mCubeFaces[i].V3 = 3;
	mCubeFaces[i].V4 = 7;

	// build the mesh
	unsigned idx=0;
	mUnitCube = mSceneMgr->createManualObject("Unit Cube");
	mUnitCube->begin("", RenderOperation::OT_TRIANGLE_LIST);

	BOOST_FOREACH(MyCubeFace face, mCubeFaces)
	{
		Vector3 n = face.Normal;
		mUnitCube->position(mCube[face.V1].Position);
		mUnitCube->textureCoord(mCube[face.V1].TexCoord);
		mUnitCube->normal(n);
		mUnitCube->index(idx++);
		mUnitCube->position(mCube[face.V2].Position);
		mUnitCube->textureCoord(mCube[face.V2].TexCoord);
		mUnitCube->normal(n);
		mUnitCube->index(idx++);
		mUnitCube->position(mCube[face.V4].Position);
		mUnitCube->textureCoord(mCube[face.V4].TexCoord);
		mUnitCube->normal(n);
		mUnitCube->index(idx++);

		mUnitCube->position(mCube[face.V4].Position);
		mUnitCube->textureCoord(mCube[face.V4].TexCoord);
		mUnitCube->normal(n);
		mUnitCube->index(idx++);
		mUnitCube->position(mCube[face.V2].Position);
		mUnitCube->textureCoord(mCube[face.V2].TexCoord);
		mUnitCube->normal(n);
		mUnitCube->index(idx++);
		mUnitCube->position(mCube[face.V3].Position);
		mUnitCube->textureCoord(mCube[face.V3].TexCoord);
		mUnitCube->normal(n);
		mUnitCube->index(idx++);
	}

	mUnitCube->end();

	mUnitCube->setMaterialName(0, "VolumeRender/Raycasting");
	mCubeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mCubeNode->attachObject(mUnitCube);
	mCubeNode->scale(100,100,100);
	//mCubeNode->translate(-50,0,-50);
}
//-----------------------------------------------------------------------------
void VolumeRenderingDemoApp::_createSlice()
{	using namespace Ogre;

	unsigned idx=0;    
    Ogre::ManualObject *obj = mSceneMgr->createManualObject("Slice");

	obj->begin("VolumeRender/SlicerCustom", RenderOperation::OT_TRIANGLE_STRIP);

	obj->position(0,0,0);
	obj->normal(Vector3::UNIT_Z);
	obj->textureCoord(0.0f, 1.0f, 0.0f);
	obj->index(idx++);

	obj->position(1,0,0);
	obj->normal(Vector3::UNIT_Z);
	obj->textureCoord(1.0f, 1.0f, 0.0f);
	obj->index(idx++);

	obj->position(0,1,0);
	obj->normal(Vector3::UNIT_Z);
	obj->textureCoord(0.0f, 0.0f, 0.0f);
	obj->index(idx++);

	obj->position(1,1,0);
	obj->normal(Vector3::UNIT_Z);
	obj->textureCoord(1.0f, 0.0f, 0.0f);
	obj->index(idx++);

	obj->end();

	MaterialPtr volumeSlicer = MaterialManager::getSingleton().getByName("VolumeRender/SlicerCustom");
	//bind texture unit to material
	volumeSlicer->getTechnique(0)->getPass(0)->createTextureUnitState("MRbrain Volume");


    Ogre::MeshPtr mesh = obj->convertToMesh("Slice.mesh");
    mSceneMgr->destroyManualObject(obj);

    mSlicePlane = mSceneMgr->createEntity("Slicer", "Slice.mesh");
    mSlicePlane->setMaterialName("VolumeRender/SlicerCustom");
    mSlicePlane->getSubEntity(0)->setCustomParameter(1, Ogre::Vector4());

	
	mSliceNode =  mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mSliceNode->attachObject(mSlicePlane);
	mSliceNode->scale(100,100,1);
	mSliceNode->translate(-120, 0, 1);
}

//-----------------------------------------------------------------------------
bool VolumeRenderingDemoApp::frameStarted(const Ogre::FrameEvent &evt)
{	using namespace Ogre;

	Real inc = mSlicerInc * evt.timeSinceLastFrame;
	
    if(mSlicer+inc<1.0f && mSlicer+inc>0.0f)
        mSlicer+=inc;
    else
		mSlicerInc = -mSlicerInc;
	

    mSlicePlane->getSubEntity(0)->setCustomParameter(1, Ogre::Vector4(mSlicer, 0, 0, 0));
	mSliceNode->setPosition(-120, 0, mSlicer*100);

	return OgreApplication::frameStarted(evt);
}
//-----------------------------------------------------------------------------