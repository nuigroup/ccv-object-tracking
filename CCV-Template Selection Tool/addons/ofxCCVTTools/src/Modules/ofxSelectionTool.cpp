#include "ofxSelectionTool.h"
#include "../Controls/gui.h"

/******************************************************************************
* The setup function is run once to perform initializations in the application
*****************************************************************************/
void ofxSelectionTool::_setup(ofEventArgs &e)
{	
	//set the title
	ofSetWindowTitle(" Template Selection Tool ");
	
	//create filter
	if(filter == NULL)	filter = new ProcessFilters();
	
	//Load Settings from config.xml file
	loadXMLSettings();

	//Setup Window Properties
	ofSetWindowShape(winWidth,winHeight);
	//ofSetVerticalSync(false);	            //Set vertical sync to false for better performance?
	
	//printf("Application Loaded...\n?");

	//load camera/video
	initDevice();
	//set framerate
	ofSetFrameRate(camRate * 1.3);			//This will be based on camera fps in the future

	/*****************************************************************************************************
	* Allocate images (needed for drawing/processing images)
	******************************************************************************************************/
	processedImg.allocate(camWidth, camHeight); //main Image that'll be processed.
	processedImg.setUseTexture(false);			//We don't need to draw this so don't create a texture
	sourceImg.allocate(camWidth, camHeight);    //Source Image
	sourceImg.setUseTexture(false);				//We don't need to draw this so don't create a texture
	/******************************************************************************************************/
	printf("Cameras Loaded...\n");

	//Fonts - Is there a way to dynamically change font size?
	verdana.loadFont("verdana.ttf", 8, true, true);	   //Font used for small images

	//Static Images
	background.loadImage("images/background.jpg");
	
	//GUI Controls
	controls = ofxGui::Instance(this);
	setupControls();
	
	printf("Controls Loaded...\n");


	//Allocate Filters
	filter->allocate( camWidth, camHeight );
	//For the initial background subtraction
	filter->bLearnBakground=true;

	/*****************************************************************************************************
	* Startup Modes
	******************************************************************************************************/
	
	#ifdef TARGET_WIN32
	    //get rid of the console window
  //      FreeConsole();
	#endif

	printf("Template Selection Tool is setup!\n\n");
}

/****************************************************************
*	Load/Save config.xml file Settings
****************************************************************/
void ofxSelectionTool::loadXMLSettings()
{	
	if (!XML.loadFile("config.xml"))  printf("XML Settings loading Failed, Make sure config.xml is in data folder");
	//--------------------------------------------------------------
	//  START BINDING XML TO VARS
	//--------------------------------------------------------------
	winWidth					= XML.getValue("CONFIG:WINDOW:WIDTH", 850);
	winHeight					= XML.getValue("CONFIG:WINDOW:HEIGHT", 450);
	deviceID					= XML.getValue("CONFIG:CAMERA_0:DEVICE", 0);
	camWidth					= XML.getValue("CONFIG:CAMERA_0:WIDTH", 320);
	camHeight					= XML.getValue("CONFIG:CAMERA_0:HEIGHT", 240);
	camRate						= XML.getValue("CONFIG:CAMERA_0:FRAMERATE", 0);
	filter->bVerticalMirror		= XML.getValue("CONFIG:BOOLEAN:VMIRROR",0);
	filter->bHorizontalMirror	= XML.getValue("CONFIG:BOOLEAN:HMIRROR",0);
	filter->bShowBinaryImage	= XML.getValue("CONFIG:BOOLEAN:SHOWBINARY",0);
	//Filters
	filter->bTrackDark			= XML.getValue("CONFIG:BOOLEAN:TRACKDARK", 0);
	filter->bHighpass			= XML.getValue("CONFIG:BOOLEAN:HIGHPASS",1);
	filter->bAmplify			= XML.getValue("CONFIG:BOOLEAN:AMPLIFY", 1);
	filter->bSmooth				= XML.getValue("CONFIG:BOOLEAN:SMOOTH", 1);
	//Filter Settings
	filter->threshold			= XML.getValue("CONFIG:INT:THRESHOLD",0);
	filter->highpassBlur		= XML.getValue("CONFIG:INT:HIGHPASSBLUR",0);
	filter->highpassNoise		= XML.getValue("CONFIG:INT:HIGHPASSNOISE",0);
	filter->highpassAmp			= XML.getValue("CONFIG:INT:HIGHPASSAMP",0);
	filter->smooth				= XML.getValue("CONFIG:INT:SMOOTH",0);
	//Area Setings
	minArea						= XML.getValue("CONFIG:INT:MINAREA",10);
	maxArea						= XML.getValue("CONFIG:INT:MAXAREA",10);
	//--------------------------------------------------------------
	//  END XML SETUP
}

void ofxSelectionTool::saveSettings()
{
	XML.setValue("CONFIG:WINDOW:WIDTH", winWidth);
	XML.setValue("CONFIG:WINDOW:HEIGHT", winHeight);
	XML.setValue("CONFIG:CAMERA_0:DEVICE", deviceID);
	XML.setValue("CONFIG:CAMERA_0:WIDTH", camWidth);
	XML.setValue("CONFIG:CAMERA_0:HEIGHT", camHeight);
	XML.setValue("CONFIG:CAMERA_0:FRAMERATE", camRate);
	XML.setValue("CONFIG:BOOLEAN:VMIRROR", filter->bVerticalMirror);
	XML.setValue("CONFIG:BOOLEAN:HMIRROR", filter->bHorizontalMirror);
	XML.setValue("CONFIG:BOOLEAN:SHOWBINARY", filter->bShowBinaryImage);
	XML.setValue("CONFIG:BOOLEAN:TRACKDARK", filter->bTrackDark);
	XML.setValue("CONFIG:BOOLEAN:HIGHPASS", filter->bHighpass);
	XML.setValue("CONFIG:BOOLEAN:AMPLIFY", filter->bAmplify);
	XML.setValue("CONFIG:BOOLEAN:SMOOTH", filter->bSmooth);
	XML.setValue("CONFIG:INT:THRESHOLD", filter->threshold);
	XML.setValue("CONFIG:INT:HIGHPASSBLUR", filter->highpassBlur);
	XML.setValue("CONFIG:INT:HIGHPASSNOISE", filter->highpassNoise);
	XML.setValue("CONFIG:INT:HIGHPASSAMP", filter->highpassAmp);
	XML.setValue("CONFIG:INT:SMOOTH", filter->smooth);
	XML.setValue("CONFIG:INT:MINAREA",minArea);
	XML.setValue("CONFIG:INT:MAXAREA",maxArea);
	XML.saveFile("config.xml");
}

/************************************************
*				Init Device
************************************************/
//Init Device (camera/video)
void ofxSelectionTool::initDevice()
{
		//check if a firefly, ps3 camera, or other is plugged in
		#ifdef TARGET_WIN32
			/****PS3 - PS3 camera only****/
		    if(ofxPS3::getDeviceCount() > 0 && PS3 == NULL)
			{
				PS3 = new ofxPS3();
				PS3->listDevices();
				PS3->initPS3(camWidth, camHeight, camRate);
				camWidth = PS3->getCamWidth();
			    camHeight = PS3->getCamHeight();
				printf("Camera Mode\nAsked for %i by %i - actual size is %i by %i \n\n", camWidth, camHeight, PS3->getCamWidth(), PS3->getCamHeight());
				return;
			}
			/****ffmv - firefly camera only****/
			else if(ofxffmv::getDeviceCount() > 0 && ffmv == NULL)
			{
			   ffmv = new ofxffmv();
			   ffmv->listDevices();
			   ffmv->initFFMV(camWidth,camHeight);
			   printf("Camera Mode\nAsked for %i by %i - actual size is %i by %i \n\n", camWidth, camHeight, ffmv->getCamWidth(), ffmv->getCamHeight());
			   camWidth = ffmv->getCamWidth();
			   camHeight = ffmv->getCamHeight();
			   return;
			}
			else if( vidGrabber == NULL ) 
			{
				vidGrabber = new ofVideoGrabber();
				vidGrabber->listDevices();
				vidGrabber->setVerbose(true);
				vidGrabber->initGrabber(camWidth,camHeight);
				printf("Camera Mode\nAsked for %i by %i - actual size is %i by %i \n\n", camWidth, camHeight, vidGrabber->width, vidGrabber->height);
				camWidth = vidGrabber->width;
				camHeight = vidGrabber->height;
				return;
			}
			else if( dsvl == NULL) 
			{
				dsvl = new ofxDSVL();
				dsvl->initDSVL();
				printf("Camera Mode\nAsked for %i by %i - actual size is %i by %i \n\n", camWidth, camHeight, dsvl->getCamWidth(), dsvl->getCamHeight());
				camWidth = dsvl->getCamWidth();
				camHeight = dsvl->getCamHeight();
				return;
			}
		#else 
			if( vidGrabber == NULL ) 
			{
				vidGrabber = new ofVideoGrabber();
				vidGrabber->listDevices();
				vidGrabber->setVerbose(true);
				vidGrabber->initGrabber(camWidth,camHeight);
				printf("Camera Mode\nAsked for %i by %i - actual size is %i by %i \n\n", camWidth, camHeight, vidGrabber->width, vidGrabber->height);
				camWidth = vidGrabber->width;
				camHeight = vidGrabber->height;
				return;
			}
		#endif
}

/******************************************************************************
* The update function runs continuously. Use it to update states and variables
*****************************************************************************/
void ofxSelectionTool::_update(ofEventArgs &e)
{
	printf("\nMaxArea=%d",maxArea);
	bNewFrame = false;

		#ifdef TARGET_WIN32
			if(PS3!=NULL)//ps3 camera
			{
				bNewFrame = PS3->isFrameNew();
			}
			else if(ffmv!=NULL)
			{
				ffmv->grabFrame();
				bNewFrame = true;
			}
			else if(vidGrabber !=NULL)
			{
				vidGrabber->grabFrame();
				bNewFrame = vidGrabber->isFrameNew();
			}
			else if(dsvl !=NULL)
			{
				bNewFrame = dsvl->isFrameNew();
			}
		#else
			vidGrabber->grabFrame();
			bNewFrame = vidGrabber->isFrameNew();
		#endif

	//if no new frame, return
	if(!bNewFrame)
	{
		return;
	}
	else//else process camera frame
	{
		ofBackground(0, 0, 0);

		// Calculate FPS of Camera
		frames++;
		float time = ofGetElapsedTimeMillis();
		if (time > (lastFPSlog + 1000))
		{
			fps = frames;
			frames = 0;
			lastFPSlog = time;
		}//End calculation

			float beforeTime = ofGetElapsedTimeMillis();

			grabFrameToCPU();
			filter->applyCPUFilters( processedImg );

		//get DSP time
		differenceTime = ofGetElapsedTimeMillis() - beforeTime;

	}
}


/************************************************
*				Input Device Stuff
************************************************/
//get pixels from camera
void ofxSelectionTool::getPixels()
{
#ifdef TARGET_WIN32
	if(PS3!=NULL)
	{
		//already grayscale
		processedImg.setFromPixels(PS3->getPixels(), camWidth, camHeight);
	}
	else if(ffmv != NULL)
	{
		processedImg.setFromPixels(ffmv->fcImage[ffmv->getDeviceID()].pData, camWidth, camHeight);
	}
	else if(vidGrabber != NULL ) 
	{
		sourceImg.setFromPixels(vidGrabber->getPixels(), camWidth, camHeight);
		//convert to grayscale
		processedImg = sourceImg;
	}
	else if(dsvl!=NULL)
	{
		if(dsvl->getNumByes() != 1){ //if not grayscale
			sourceImg.setFromPixels(dsvl->getPixels(), camWidth, camHeight);
			//convert to grayscale
			processedImg = sourceImg;
		}
		else
		{	//if grayscale
			processedImg.setFromPixels(dsvl->getPixels(), camWidth, camHeight);
		}
	}
#endif	
}


//Grab frame from CPU
void ofxSelectionTool::grabFrameToCPU()
{
	//Set sourceImg as new camera/video frame
	    #ifdef TARGET_WIN32
			getPixels();
 		#else
            sourceImg.setFromPixels(vidGrabber->getPixels(), camWidth, camHeight);
 			//convert to grayscale
 			processedImg = sourceImg;
 		#endif
}

/******************************************************************************
* The draw function paints the textures onto the screen. It runs after update.
*****************************************************************************/
void ofxSelectionTool::_draw(ofEventArgs &e)
{
		drawFullMode();
		//draw gui controls
		controls->draw();
}

void ofxSelectionTool::drawFullMode()
{
	ofSetColor(0xFFFFFF);
	//Draw Background Image
	background.draw(0, 0);
	filter->draw();
	//Display Application information in bottom right
	string str = "Calculation Time: ";
	str+= ofToString(differenceTime, 0)+" ms \n\n";

		string str2 = "Camera Resolution: ";
        str2+= ofToString(camWidth, 0) + " x " + ofToString(camHeight, 0)  + "\n";
		string str4 = "Camera Framerate: ";
		str4+= ofToString(fps, 0)+" FPS \n";
		ofSetColor(0x000000);
		verdana.drawString(str + str2 + str4, 545, 350);
}
/*****************************************************************************
* KEY EVENTS
*****************************************************************************/
void ofxSelectionTool::_keyPressed(ofKeyEventArgs &e)
{
		switch (e.key)
		{
		case 'a':
			filter->threshold++;
			controls->update(appPtr->imgSetPanel_threshold, kofxGui_Set_Int, &appPtr->filter->threshold, sizeof(int));
			break;
		case 'z':
			filter->threshold--;
			controls->update(appPtr->imgSetPanel_threshold, kofxGui_Set_Int, &appPtr->filter->threshold, sizeof(int));
			break;
		case 'h':
			filter->bHorizontalMirror ? filter->bHorizontalMirror = false : filter->bHorizontalMirror = true;
			controls->update(appPtr->propertiesPanel_flipH, kofxGui_Set_Bool, &appPtr->filter->bHorizontalMirror, sizeof(bool));
			break;
		case 'j':
			filter->bVerticalMirror ? filter->bVerticalMirror = false : filter->bVerticalMirror = true;
			controls->update(appPtr->propertiesPanel_flipV, kofxGui_Set_Bool, &appPtr->filter->bVerticalMirror, sizeof(bool));
			break;
	}
}

void ofxSelectionTool::_keyReleased(ofKeyEventArgs &e)
{
}

/*****************************************************************************
*	MOUSE EVENTS
*****************************************************************************/
void ofxSelectionTool::_mouseDragged(ofMouseEventArgs &e)
{
		controls->mouseDragged(e.x, e.y, e.button); //guilistener
}

void ofxSelectionTool::_mousePressed(ofMouseEventArgs &e)
{
		controls->mousePressed(e.x, e.y, e.button); //guilistener
}

void ofxSelectionTool::_mouseReleased(ofMouseEventArgs &e)
{
		controls->mouseReleased(e.x, e.y, 0); //guilistener
}

/*****************************************************************************
* ON EXIT
*****************************************************************************/
void ofxSelectionTool::_exit(ofEventArgs &e)
{

	// -------------------------------- SAVE STATE ON EXIT
	saveSettings();
	// AlexP
	// C++ guarantees that operator delete checks its argument for null-ness
    #ifdef TARGET_WIN32
		delete PS3;		PS3 = NULL;
		delete ffmv; 	ffmv = NULL;
		delete dsvl;	dsvl = NULL;
	#endif
	delete filter;		filter = NULL;
	delete vidGrabber;	vidGrabber = NULL;
	printf("Template Selection Tool has exited!\n");
}