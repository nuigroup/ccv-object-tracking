/*
*  ProcessFilters.h
*  
*
*  Created on 2/2/09.
*  Copyright 2009 NUI Group. All rights reserved.
*
*/

#ifndef PROCESS_FILTERS_H_
#define PROCESS_FILTERS_H_

#include "Filters.h"

class ProcessFilters : public Filters {

  public:

    void allocate( int w, int h ) {

        camWidth = w;
        camHeight = h;
		//initialize learnrate
        //CPU Setup
        grayImg.allocate(camWidth, camHeight);		//Gray Image
        grayBg.allocate(camWidth, camHeight);		//Background Image
        grayDiff.allocate(camWidth, camHeight);		//Difference Image between Background and Source
        floatBgImg.allocate(camWidth, camHeight);	//ofxShortImage used for simple dynamic background subtraction
    }

/****************************************************************
 *	CPU Filters
 ****************************************************************/
     void applyCPUFilters(CPUImageFilter& img){

        //Set Mirroring Horizontal/Vertical
        if(bVerticalMirror || bHorizontalMirror) img.mirror(bVerticalMirror, bHorizontalMirror);

        grayImg = img; //for drawing

        //Capture full background
        if (bLearnBakground == true){
            floatBgImg = img;
			//grayBg = floatBgImg;  // not yet implemented
			cvConvertScale( floatBgImg.getCvImage(), grayBg.getCvImage(), 255.0f/65535.0f, 0 );       
			grayBg.flagImageChanged();
            bLearnBakground = false;
        }

		//Background Subtraction
        //img.absDiff(grayBg, img);
		if(bTrackDark)
			cvSub(grayBg.getCvImage(), img.getCvImage(), img.getCvImage());
		else
			cvSub(img.getCvImage(), grayBg.getCvImage(), img.getCvImage());

		img.flagImageChanged();
    
		
		if(bSmooth){//Smooth
            img.blur((smooth * 2) + 1); //needs to be an odd number
        }

        if(bHighpass){//HighPass
            img.highpass(highpassBlur, highpassNoise);
        }

        if(bAmplify){//Amplify
            img.amplify(img, highpassAmp);
        }

        img.threshold(threshold); //Threshold
		//img.adaptiveThreshold(threshold, -3);

        grayDiff = img; //for drawing
    }

/****************************************************************
 *	Draw Filter Images
 ****************************************************************/
    void draw()
    {
		if(!bShowBinaryImage)
		{
        grayImg.draw(40, 30, 320, 240);
		}
		else
		{
        grayDiff.draw(40, 30, 320, 240);
		}
    }

};
#endif
