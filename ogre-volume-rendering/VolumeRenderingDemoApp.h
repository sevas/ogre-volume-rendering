#ifndef VolumeRenderingDemoApp_h__
#define VolumeRenderingDemoApp_h__

#include "OgreApplication.h"

class VolumeRenderingDemoApp : public OgreApplication
{
public:
	VolumeRenderingDemoApp(void);
	virtual ~VolumeRenderingDemoApp(void);

	void createScene() { _createGrid(500);};
};
#endif // VolumeRenderingDemoApp_h__
