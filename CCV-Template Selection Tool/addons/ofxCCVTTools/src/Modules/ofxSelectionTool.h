#ifndef _ofxNCoreVision_H
#define _ofxNCoreVision_H

//Main
#include "ofMain.h"
//Addons
#ifdef TARGET_WIN32
    #include "ofxffmv.h"
    #include "ofxPS3.h"
	#include "ofxDSVL.h"
#endif
#include "ofxOpenCv.h"
#include "ofxDirList.h"
#include "ofxVectorMath.h"
#include "ofxXmlSettings.h"

// Our Addon
#include "ofxCCVTTools.h"

// height and width of the source/tracked draw window
#define MAIN_WINDOW_HEIGHT 240.0f
#define MAIN_WINDOW_WIDTH  320.0f

class ofxSelectionTool : public ofxGuiListener
{
	// ofxGUI setup stuff
	enum
	{
		propertiesPanel,
		propertiesPanel_flipV,
		propertiesPanel_flipH,

		backgroundPanel,
		backgroundPanel_remove,

		smoothPanel,
		smoothPanel_use,
		smoothPanel_smooth,

		amplifyPanel,
		amplifyPanel_use,
		amplifyPanel_amp,

		highpassPanel,
		highpassPanel_use,
		highpassPanel_blur,
		highpassPanel_noise,

		imgSetPanel,
		imgSetPanel_darkblobs,
		imgSetPanel_threshold,
		imgSetPanel_showBinary,

		savePanel,
		kParameter_SaveXml,
		kParameter_File,

		areaPanel,
		areaPanel_minArea,
		areaPanel_maxArea,
	};

public:
	ofxSelectionTool()
	{
		ofAddListener(ofEvents.mousePressed, this, &ofxSelectionTool::_mousePressed);
		ofAddListener(ofEvents.mouseDragged, this, &ofxSelectionTool::_mouseDragged);
		ofAddListener(ofEvents.mouseReleased, this, &ofxSelectionTool::_mouseReleased);
		ofAddListener(ofEvents.keyPressed, this, &ofxSelectionTool::_keyPressed);
		ofAddListener(ofEvents.keyReleased, this, &ofxSelectionTool::_keyReleased);
		ofAddListener(ofEvents.setup, this, &ofxSelectionTool::_setup);
		ofAddListener(ofEvents.update, this, &ofxSelectionTool::_update);
		ofAddListener(ofEvents.draw, this, &ofxSelectionTool::_draw);
		ofAddListener(ofEvents.exit, this, &ofxSelectionTool::_exit);

		#ifdef TARGET_WIN32
            PS3  = NULL;
			ffmv = NULL;
			dsvl = NULL;
		#endif

		vidGrabber = NULL;
		//initialize filter
		filter = NULL;
		//fps and dsp calculation
		frames		= 0;
		fps			= 0;
		lastFPSlog	= 0;
		differenceTime = 0;
		//camera
		camRate = 30;
		camWidth = 320;
		camHeight = 240;

		minArea=10;
		maxArea=10;
	}

	~ofxSelectionTool()
	{
		// AlexP
		// C++ guarantees that operator delete checks its argument for null-ness
		delete filter;		filter = NULL;
		delete vidGrabber;	vidGrabber = NULL;
		#ifdef TARGET_WIN32
		delete PS3;		PS3 = NULL;
		delete ffmv; 	ffmv = NULL;
		delete dsvl;	dsvl = NULL;
		#endif
	}

	/****************************************************************
	 *						Public functions
	 ****************************************************************/
	//Basic Events called every frame
    void _setup(ofEventArgs &e);
    void _update(ofEventArgs &e);
	void _draw(ofEventArgs &e);
    void _exit(ofEventArgs &e);
    //Mouse Events
    void _mousePressed(ofMouseEventArgs &e);
    void _mouseDragged(ofMouseEventArgs &e);
    void _mouseReleased(ofMouseEventArgs &e);
    //Key Events
    void _keyPressed(ofKeyEventArgs &e);
    void _keyReleased(ofKeyEventArgs &e);

	//GUI
	void setupControls();
	void		handleGui(int parameterId, int task, void* data, int length);
	ofxGui*		controls;

	//image processing stuff
	void initDevice();
	void getPixels();
	void grabFrameToCPU();
	void grabFrameToGPU(GLuint target);

	//drawing
	void drawFullMode();

	//Load/save settings
	void loadXMLSettings();
	void saveSettings();


	/***************************************************************
	 *					Video Capture Devices
	 ***************************************************************/
    #ifdef TARGET_WIN32
        ofxffmv*            ffmv; //for firefly mv
        ofxPS3*				PS3;  //for ps3
		ofxDSVL*			dsvl;
	#endif
	ofVideoGrabber*		vidGrabber;

	/****************************************************************
	 *            Variables in config.xml Settings file
	 *****************************************************************/
    int					deviceID;
	int 				frameseq;
//	int 				threshold;
	int 				camRate;
	int 				camWidth;
	int 				camHeight;
	int					winWidth;
	int					winHeight;

	bool				bNewFrame;


	//Object Tracking Variables
	int					minArea;
	int					maxArea;

	/****************************************************
	 *End config.xml variables
	 *****************************************************/
	//FPS variables
	int 				frames;
	int  				fps;
	float				lastFPSlog;
	int					differenceTime;

	//Fonts
	ofTrueTypeFont		verdana;
	ofTrueTypeFont      sidebarTXT;

	//Images
	ofImage				background;

    /****************************************************************
	 *						Private Stuff
	 ****************************************************************/
	string videoFileName;

	//Image filters
	Filters*			filter;
	CPUImageFilter      processedImg;
	ofxCvColorImage		sourceImg;

	//XML Settings Vars
	ofxXmlSettings		XML;

	//Logging
	char				dateStr [9];
	char				timeStr [9];
	time_t				rawtime;
	struct tm *			timeinfo;
	char				fileName [80];
	FILE *				stream ;
};
#endif