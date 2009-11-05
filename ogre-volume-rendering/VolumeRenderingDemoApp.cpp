#include "precompiled.h"
#include "VolumeRenderingDemoApp.h"

//-----------------------------------------------------------------------------
VolumeRenderingDemoApp::VolumeRenderingDemoApp(void)
	:OgreApplication("Ogre Volume Rendering")
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
	_createSlice();
}
//-----------------------------------------------------------------------------
void VolumeRenderingDemoApp::_loadVolume()
{
	mVolume.loadSlices("../volumes/MRbrain", 109);
}
//-----------------------------------------------------------------------------
void VolumeRenderingDemoApp::_create3DTextureFromVolume()
{	using namespace Ogre;
	// Create the texture
	mVolumeTexture = TextureManager::getSingleton().createManual(
		"MRbrain Volume", // name
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		TEX_TYPE_3D,      // type
		256, 256,         // width & height
		109,		      // # slices
		0,                // number of mipmaps
	    PF_SHORT_L,			 // pixel format
		TU_DEFAULT);		
		

	HardwarePixelBufferSharedPtr buffer = mVolumeTexture->getBuffer();

	buffer->lock(HardwareBuffer::HBL_NORMAL);
	const Ogre::PixelBox &pb = buffer->getCurrentLock();

	Ogre::uint8 *pbptr = static_cast<Ogre::uint8*>(pb.data);
	size_t x, y, z;

	for(z=pb.front; z<pb.back; z++) 
	{
		for(y=pb.top; y<pb.bottom; y++)
		{
			for(x=pb.left; x<pb.right; x++)
			{
				Ogre::uint8 val = mVolume.getVoxelValue(x, y, z);
				*((Ogre::uint8*)&pbptr[x]) = val;


				//if(mVolume.getVoxelValue(x, y, z) != 0)
				//{
				//	float foo = 2;
				//}

				//if(z==pb.front || z==(pb.back-1) || y==pb.top|| y==(pb.bottom-1) ||
				//	x==pb.left || x==(pb.right-1))
				//{
				//	// On border, must be zero
				//	pbptr[x] = 0;
				//} 
				//else
				//{
				//	//Density = (curr[x][y][z].dens*elapsedt + last[x][y][z].dens*(totalt-elapsedt))/totalt;

				//	((Ogre::uint16*)&pbptr[x])[0] = static_cast<Ogre::uint16>(mVolume.getVoxelValue(x, y, z)); // R
				//}   
			}
			pbptr += pb.rowPitch;
		}
		pbptr += pb.getSliceSkip();
	}

	buffer->unlock();


	// Create a material using the texture
	mVolumeMaterial = MaterialManager::getSingleton().create(
						"Volume", // name
						ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	mVolumeMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("MRbrain Volume");
	//mVolumeMaterial->getTechnique(0)->getPass(0)->setSceneBlending(SBT_TRANSPARENT_ALPHA);

}
//-----------------------------------------------------------------------------
void VolumeRenderingDemoApp::_createCube()
{
	float z = 1.0f;
	// vertices
	mCube[0].Position=Vector3(0.0f,	0.0f,	z);
	mCube[0].TexCoord=Vector3(0.0f,	0.0f,	z);

	mCube[1].Position=Vector3(0.0f,	1.0f,	z);
	mCube[1].TexCoord=Vector3(0.0f,	1.0f,	z);
	
	mCube[2].Position=Vector3(1.0f,	1.0f,	z);
	mCube[2].TexCoord=Vector3(1.0f,	1.0f,	z);

	mCube[3].Position=Vector3(1.0f,	0.0f,	z);
	mCube[3].TexCoord=Vector3(1.0f,	0.0f,	z);

	z = 1.0f;
	mCube[4].Position=Vector3(0.0f,	0.0f,	z);
	mCube[4].TexCoord=Vector3(0.0f,	0.0f,	z);

	mCube[5].Position=Vector3(0.0f,	1.0f,	z);
	mCube[5].TexCoord=Vector3(0.0f,	1.0f,	z);

	mCube[6].Position=Vector3(1.0f,	1.0f,	z);
	mCube[6].TexCoord=Vector3(1.0f,	1.0f,	z);

	mCube[7].Position=Vector3(1.0f,	0.0f,	z);
	mCube[7].TexCoord=Vector3(1.0f,	0.0f,	z);

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

	mUnitCube->setMaterialName(0, "Volume");
	mCubeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mCubeNode->attachObject(mUnitCube);
	mCubeNode->scale(50,50,50);
	mCubeNode->translate(100,0,0);
}
//-----------------------------------------------------------------------------
void VolumeRenderingDemoApp::_createSlice()
{	using namespace Ogre;

	unsigned idx=0;
	float z=50;
	mSlicePlane = mSceneMgr->createManualObject("Slice");

	mSlicePlane->begin("VolumeRender/Slicer", RenderOperation::OT_TRIANGLE_STRIP);

	mSlicePlane->position(0,0,0);
	mSlicePlane->normal(Vector3::UNIT_Z);
	mSlicePlane->textureCoord(0.0f, 0.0f, z);
	mSlicePlane->index(idx++);

	mSlicePlane->position(1,0,0);
	mSlicePlane->normal(Vector3::UNIT_Z);
	mSlicePlane->textureCoord(1.0f, 0.0f, z);
	mSlicePlane->index(idx++);

	mSlicePlane->position(0,1,0);
	mSlicePlane->normal(Vector3::UNIT_Z);
	mSlicePlane->textureCoord(0.0f, 1.0f, z);
	mSlicePlane->index(idx++);

	mSlicePlane->position(1,1,0);
	mSlicePlane->normal(Vector3::UNIT_Z);
	mSlicePlane->textureCoord(1.0f, 1.0f, z);
	mSlicePlane->index(idx++);

	mSlicePlane->end();

	MaterialPtr volumeSlicer = MaterialManager::getSingleton().getByName("VolumeRender/Slicer");
	volumeSlicer->getTechnique(0)->getPass(0)->createTextureUnitState("MRbrain Volume");
	
	mSlicePlane->getSection(0)->setCustomParameter(0, 0.5f);

	mSliceNode =  mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mSliceNode->attachObject(mSlicePlane);
	mSliceNode->scale(50,50,50);
	mSliceNode->translate(-25, 0, 1);
}

//-----------------------------------------------------------------------------
