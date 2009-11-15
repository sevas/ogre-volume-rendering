#include "precompiled.h"
#include "VolumeRenderingDemoApp.h"

#include <boost/format.hpp>


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

	_loadSlice("../volumes/MRbrain2", "MRbrain");
	_createSlice();
	_createTextureFromSlice();
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

	Ogre::uint16 *pbptr = static_cast<Ogre::uint16*>(pb.data);
	//size_t x, y, z;

	for(size_t z = pb.front ; z<pb.back ; z++)
	{
		for (size_t y=pb.top ; y<pb.bottom; y++)
		{
			for(size_t x=pb.left ; x<pb.right; x++)
			{
				Ogre::uint16 val = mVolume.getVoxelValue(x, y, z);
				pbptr[x]= val;

			}
			pbptr += pb.rowPitch;
		}
		pbptr += pb.getSliceSkip();
	}


	buffer->unlock();


	// Create a material using the texture
	mVolumeMaterial = MaterialManager::getSingleton().create(
						"VolumeMaterial", // name
						ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	mVolumeMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("MRbrain Volume");
	mVolumeMaterial->getTechnique(0)->getPass(0)->setSceneBlending(SBT_TRANSPARENT_ALPHA);

}
//-----------------------------------------------------------------------------
void VolumeRenderingDemoApp::_loadSlice( const std::string &_directory
										,const std::string& _basename)
{
	mSlice.resize(256*256);
	std::ifstream file;
	boost::format fmt("%s/%s.%d");
	fmt % _directory % "MRbrain" % 50;
	std::string filename = fmt.str();
	file.open(filename.c_str(), std::ios_base::binary);
	if(file)
	{
		for(unsigned i=0 ; i<256*256; ++i)
		{

			//unsigned short val;
			U16_U8 val;
			val.u8[0] = file.get();
			val.u8[1] = file.get();
			mSlice[i] = val.u16;
		}
		file.close();
	}
}
//-----------------------------------------------------------------------------
void VolumeRenderingDemoApp::_createTextureFromSlice()
{
	using namespace Ogre;
	// Create the texture
	mSliceTexture = TextureManager::getSingleton().createManual(
		"MRbrain Slice", // name
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		TEX_TYPE_2D,      // type
		256, 256,         // width & height
		0,                // number of mipmaps
		PF_SHORT_L,			 // pixel format
		HardwareBuffer::HBU_STATIC_WRITE_ONLY );		

	
	HardwarePixelBufferSharedPtr buffer = mSliceTexture->getBuffer();

	buffer->lock(HardwareBuffer::HBL_NORMAL);
	const Ogre::PixelBox &pb = buffer->getCurrentLock();

	Ogre::uint16 *pbptr = static_cast<Ogre::uint16*>(pb.data);

	for (size_t y=pb.top ; y<pb.bottom; y++)
	{
		for(size_t x=pb.left ; x<pb.right; x++)
		{
			Ogre::uint16 val = mSlice[y*256 + x];
			pbptr[x]= val;

		}
		pbptr += pb.rowPitch;
	}



	buffer->unlock();


	// Create a material using the texture
	mSliceMaterial = MaterialManager::getSingleton().create(
		"SliceMaterial", // name
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	mSliceMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("MRbrain Slice");
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

	mUnitCube->setMaterialName(0, "VolumeMaterial");
	mCubeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mCubeNode->attachObject(mUnitCube);
	mCubeNode->scale(100,100,100);
	mCubeNode->translate(-50,0,-50);
}
//-----------------------------------------------------------------------------
void VolumeRenderingDemoApp::_createSlice()
{	using namespace Ogre;

	unsigned idx=0;
	float z=0.3f;
	mSlicePlane = mSceneMgr->createManualObject("Slice");

	mSlicePlane->begin("VolumeRender/Slicer", RenderOperation::OT_TRIANGLE_STRIP);

	mSlicePlane->position(0,0,0);
	mSlicePlane->normal(Vector3::UNIT_Z);
	mSlicePlane->textureCoord(0.0f, 1.0f, 0.0f);
	mSlicePlane->index(idx++);

	mSlicePlane->position(1,0,0);
	mSlicePlane->normal(Vector3::UNIT_Z);
	mSlicePlane->textureCoord(1.0f, 1.0f, 0.0f);
	mSlicePlane->index(idx++);

	mSlicePlane->position(0,1,0);
	mSlicePlane->normal(Vector3::UNIT_Z);
	mSlicePlane->textureCoord(0.0f, 0.0f, 0.0f);
	mSlicePlane->index(idx++);

	mSlicePlane->position(1,1,0);
	mSlicePlane->normal(Vector3::UNIT_Z);
	mSlicePlane->textureCoord(1.0f, 0.0f, 0.0f);
	mSlicePlane->index(idx++);

	mSlicePlane->end();

	//mSlicePlane->setMaterialName(0, "VolumeMaterial");

	MaterialPtr volumeSlicer = MaterialManager::getSingleton().getByName("VolumeRender/Slicer");
	//bind texture unit to material
	volumeSlicer->getTechnique(0)->getPass(0)->createTextureUnitState("MRbrain Volume");
	
	mSlicePlane->setMaterialName(0, "VolumeRender/Slicer");
	

	// keep a handle to shader parameters
	Technique* pTechnique =	 volumeSlicer->getTechnique(0);
	if ( pTechnique ) 
	{
		// assuming single pass material
		Pass* pPass = pTechnique->getPass( 0 );
		
		if ( pPass && pPass->hasVertexProgram() ) 
		{
			// Retrieve the shader parameters 
			GpuProgramParametersSharedPtr pParams = pPass->getVertexProgramParameters();

			if ( ! pParams.isNull() )
			{
				if ( pParams->_findNamedConstantDefinition( "slice" ) )
				{
					mVolumeSlicerParams = pParams;
				}	
			}
		}
	}

	
	mSliceNode =  mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mSliceNode->attachObject(mSlicePlane);
	mSliceNode->scale(100,100,10);
	mSliceNode->translate(-250, 0, 1);
}

//-----------------------------------------------------------------------------
bool VolumeRenderingDemoApp::frameStarted(const Ogre::FrameEvent &evt)
{	using namespace Ogre;

	Real inc = mSlicerInc * evt.timeSinceLastFrame;
	
	if(mSlicer+inc>1.0f || mSlicer+inc<0.0f)
		mSlicerInc = -mSlicerInc;

	mSlicer+=inc;
	//mSlicePlane->getSection(0)->setCustomParameter(0, Vector4(mSlicer, 0,0,0));

	// update uniform
	if (!mVolumeSlicerParams.isNull())
		mVolumeSlicerParams->setNamedConstant( "slice", mSlicer );



	mSliceNode->setPosition(-250, 0, mSlicer*100);

	return OgreApplication::frameStarted(evt);
}