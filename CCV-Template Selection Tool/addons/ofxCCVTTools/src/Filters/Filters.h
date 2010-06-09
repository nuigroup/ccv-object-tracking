/*
*  Filters.h
*  
*
*  Created on 2/2/09.
*  Copyright 2009 NUI Group. All rights reserved.
*
*/

#ifndef FILTERS_H_
#define FILTERS_H_

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "CPUImageFilter.h"

#define CAMERA_EXPOSURE_TIME  2200.0f

class Filters {
  
	public:

	  Filters(){

		camWidth = 320;
		camHeight = 240;

		//filter values
		threshold = 120;
		smooth = 0;
		highpassBlur = 0;
		highpassNoise = 0;
		highpassAmp = 0;
	  }

    int camWidth;
    int camHeight;

    //filter values
	int threshold;
    int smooth;
    int highpassBlur;
    int highpassNoise;
    int	highpassAmp;

    float fLearnRate;// rate to learn background

    //filter bools
	bool bLearnBakground;
	bool bVerticalMirror;
	bool bHorizontalMirror;
	bool bSmooth;
	bool bHighpass;
	bool bAmplify;
    bool bThreshold;
	bool bTrackDark;
	bool bShowBinaryImage;

    //CPU
    ofxCvGrayscaleImage grayImg;
    ofxCvGrayscaleImage grayBg;
    ofxCvGrayscaleImage grayDiff;
	ofxCvShortImage		floatBgImg;

    virtual void allocate( int w, int h ) = 0;
    virtual void applyCPUFilters(CPUImageFilter& img) = 0;
    virtual void draw() = 0;
};


#endif
