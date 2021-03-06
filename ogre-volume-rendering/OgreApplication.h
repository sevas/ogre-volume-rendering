/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#ifndef __OgreApplication_h__
#define __OgreApplication_h__

#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>

#include "TextRenderer.h"

using namespace Ogre;

class OgreApplication   : public FrameListener
                        , public WindowEventListener 
                        , public OIS::MouseListener
                        , public OIS::KeyListener
{
public:
   OgreApplication(const String&);
   virtual ~OgreApplication();

   virtual void go();

protected:
   virtual bool initialise();
   virtual bool initOgreCore();

   virtual void createSceneManager();
   virtual void createCamera();
   virtual void createViewports();
   virtual void createResourceListener();
   virtual void createFrameListener();
   virtual void createInputSystem();

   virtual void addResourceLocations();
   virtual void initResources();

   virtual void createScene() = 0; // I am pure virtual, override me!
   virtual void destroyScene();

   // WindowEventListener
    virtual void windowResized(RenderWindow* rw);
    virtual void windowClosed(RenderWindow* rw);

   // FrameListener overrides 
   virtual bool frameStarted(const FrameEvent& evt); 
   virtual bool frameEnded(const FrameEvent& evt); 

   // OIS crap
   virtual bool keyPressed( const OIS::KeyEvent &e );
   virtual bool keyReleased( const OIS::KeyEvent &e );

   virtual bool mouseMoved( const OIS::MouseEvent &e );
   virtual bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
   virtual bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );


protected:
    void _createGrid(int);
    void _createLight();
    void _createDebugOverlay();
    void _updateDebugOverlay();

    String mWindowTitle;
    Root *mRoot;
    Camera* mCamera;
    SceneManager* mSceneMgr;
    RenderWindow* mWindow;

    SceneNode *mCameraNode, *mCameraTargetNode, *mCameraBaseNode;
    Real mRotateSpeed;

    OIS::InputManager *mInputManager;
    OIS::Keyboard *mKeyboard;
    OIS::Mouse *mMouse;

    BillboardSet *mBBset;
    Billboard *mLightFlare;
    Light *mLight;
    SceneNode *mLightNode;

    TextRenderer *mDebugText;
};
 
#endif // __OgreApplication_h__