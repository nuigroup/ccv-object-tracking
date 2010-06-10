/*
*  GUI.h
*  
*
*  Created on 2/2/09.
*  Copyright 2009 NUI Group. All rights reserved.
*
*/

#ifndef GUI_CONTROLS_H
#define GUI_CONTROLS_H

#include "../Modules/ofxSelectionTool.h"
#include "../Camera/CameraUtils.h"

ofxSelectionTool  *appPtr;
CameraUtils		cameraCount;

void ofxSelectionTool::setupControls()
{
	appPtr = this;

	//panel border color
	controls->mGlobals->mBorderColor.r = 0;
	controls->mGlobals->mBorderColor.g = 0;
	controls->mGlobals->mBorderColor.b = 0;
	controls->mGlobals->mBorderColor.a = .3;
	//panel color
	controls->mGlobals->mCoverColor.r = 1;
	controls->mGlobals->mCoverColor.g = 1;
	controls->mGlobals->mCoverColor.b = 1;
	controls->mGlobals->mCoverColor.a = .4;
	//control outline color
	controls->mGlobals->mFrameColor.r = 0;
	controls->mGlobals->mFrameColor.g = 0;
	controls->mGlobals->mFrameColor.b = 0;
	controls->mGlobals->mFrameColor.a = .3;
	//text color
	controls->mGlobals->mTextColor.r = 0;
	controls->mGlobals->mTextColor.g = 0;
	controls->mGlobals->mTextColor.b = 0;
	controls->mGlobals->mTextColor.a = 1;
	//button color
	controls->mGlobals->mButtonColor.r = 1;
	controls->mGlobals->mButtonColor.g = 0;
	controls->mGlobals->mButtonColor.b = 0;
	controls->mGlobals->mButtonColor.a = .8;
	//slider tip color
	controls->mGlobals->mHandleColor.r = 0;
	controls->mGlobals->mHandleColor.g = 0;
	controls->mGlobals->mHandleColor.b = 0;
	//slider color
	controls->mGlobals->mSliderColor.r = 1;
	controls->mGlobals->mSliderColor.g = 0;
	controls->mGlobals->mSliderColor.b = 0;
	controls->mGlobals->mSliderColor.a = .8;

	ofxGuiPanel* propPanel = controls->addPanel(appPtr->propertiesPanel, "Camera Properties", 535, 10, 12, OFXGUI_PANEL_SPACING);
	propPanel->addButton(appPtr->propertiesPanel_flipV, "Flip Vertical (j)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch, "");
	propPanel->addButton(appPtr->propertiesPanel_flipH, "Flip Horizontal (h)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch, "");
	propPanel->mObjWidth = 264;

	ofxGuiPanel* panel2 = controls->addPanel(appPtr->savePanel, "Settings", 535, 265, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
	panel2->addButton(appPtr->kParameter_SaveXml, "Save Settings (s)", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Trigger, "");
	panel2->mObjWidth = 264;

	//Source Image
	ofxGuiPanel* imgSetPanel = controls->addPanel(appPtr->imgSetPanel, "Image Settings", 41, 270, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
	imgSetPanel->addButton(appPtr->imgSetPanel_darkblobs, "Inverse", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch, "");
	imgSetPanel->addSlider(appPtr->imgSetPanel_threshold, "Image Threshold", 140, 13, 0.0f, 255.0f, filter->threshold, kofxGui_Display_Int, 0);
	imgSetPanel->addButton(appPtr->imgSetPanel_showBinary,"Show Binary Image",OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, kofxGui_Button_Off, kofxGui_Button_Switch, "");
	imgSetPanel->mObjHeight = 100;
	imgSetPanel->mObjWidth = 319;
	imgSetPanel->mObjects[0]->mObjX = 10;
	imgSetPanel->mObjects[0]->mObjY = 31;
	imgSetPanel->mObjects[1]->mObjX = 10;
	imgSetPanel->mObjects[1]->mObjY = 51;
	imgSetPanel->mObjects[2]->mObjX = 100;
	imgSetPanel->mObjects[2]->mObjY = 31;
	imgSetPanel->adjustToNewContent(100, 0);

	//Background Image
	ofxGuiPanel* bkPanel1 = controls->addPanel(appPtr->backgroundPanel, "Background", 535, 95, 10, 7);
	bkPanel1->addButton(backgroundPanel_remove, "Remove BG (b)", 10, 10, kofxGui_Button_Off, kofxGui_Button_Trigger, "");
	bkPanel1->mObjWidth = 127;
	bkPanel1->mObjHeight = 95;

	//Smooth Image
	ofxGuiPanel* sPanel = controls->addPanel(appPtr->smoothPanel, "Smooth", 535, 195, 10, 7);
	sPanel->addButton(smoothPanel_use, "", 12, 12, kofxGui_Button_Off, kofxGui_Button_Switch, "");
	sPanel->addSlider(smoothPanel_smooth, "Smooth", 110, 13, 0.0f, 15.0f, filter->smooth, kofxGui_Display_Int, 0);
	sPanel->mObjects[0]->mObjX = 105;
	sPanel->mObjects[0]->mObjY = 10;
	sPanel->mObjects[1]->mObjY = 30;
	sPanel->mObjWidth = 127;
	sPanel->mObjHeight = 65;

	//Selecting Area of the template
	ofxGuiPanel* aPanel = controls->addPanel(appPtr->areaPanel, "Area", 370, 195, 10, 7);
	aPanel->addSlider(areaPanel_minArea, "Min Area", 110, 13, 0.0f, 15.0f, appPtr->minArea, kofxGui_Display_Int, 0);
	aPanel->addSlider(areapanel_maxArea, "Max Area", 110, 13, 0.0f, 15.0f, appPtr->maxArea, kofxGui_Display_Int, 0);
	aPanel->mObjects[0]->mObjX = 105;
	aPanel->mObjects[0]->mObjY = 10;
	aPanel->mObjects[0]->mObjY = 30;
	aPanel->mObjWidth = 127;
	aPanel->mObjHeight = 65;

	//Highpass Image
	ofxGuiPanel* hpPanel = controls->addPanel(appPtr->highpassPanel, "Highpass", 672, 95, OFXGUI_PANEL_BORDER, 7);
	hpPanel->addButton(highpassPanel_use, "", 12, 12, kofxGui_Button_Off, kofxGui_Button_Switch, "");
	hpPanel->addSlider(highpassPanel_blur, "Blur", 110, 13, 0.0f, 200.0f, filter->highpassBlur, kofxGui_Display_Int, 0);
	hpPanel->addSlider(highpassPanel_noise, "Noise", 110, 13, 0.0f, 30.0f, filter->highpassNoise, kofxGui_Display_Int, 0);
	hpPanel->mObjects[0]->mObjX = 105;
	hpPanel->mObjects[0]->mObjY = 10;
	hpPanel->mObjects[1]->mObjY = 30;
	hpPanel->mObjects[2]->mObjY = 60;
	hpPanel->mObjWidth = 127;
	hpPanel->mObjHeight = 95;

	//Amplify Image
	ofxGuiPanel* ampPanel = controls->addPanel(appPtr->amplifyPanel, "Amplify", 672, 195, OFXGUI_PANEL_BORDER, 7);
	ampPanel->addButton(amplifyPanel_use, "", 12, 12, kofxGui_Button_Off, kofxGui_Button_Switch, "");
	ampPanel->addSlider(amplifyPanel_amp, "Amplify", 110, 13, 0.0f, 300.0f, filter->highpassAmp, kofxGui_Display_Int, 0);
	ampPanel->mObjects[0]->mObjX = 105;
	ampPanel->mObjects[0]->mObjY = 10;
	ampPanel->mObjects[1]->mObjY = 30;
	ampPanel->mObjWidth = 127;
	ampPanel->mObjHeight = 65;

	//do update while inactive

	controls->forceUpdate(false);
	controls->activate(true);

	/****************************
	* Set GUI values on startup
	****************************/


	controls->update(appPtr->propertiesPanel_flipV, kofxGui_Set_Bool, &appPtr->filter->bVerticalMirror, sizeof(bool));
	controls->update(appPtr->propertiesPanel_flipH, kofxGui_Set_Bool, &appPtr->filter->bHorizontalMirror, sizeof(bool));

	controls->update(appPtr->imgSetPanel_darkblobs, kofxGui_Set_Bool, &appPtr->filter->bTrackDark, sizeof(bool));
	controls->update(appPtr->imgSetPanel_showBinary, kofxGui_Set_Bool, &appPtr->filter->bShowBinaryImage, sizeof(bool));
	//Smooth
	controls->update(appPtr->smoothPanel_use, kofxGui_Set_Bool, &appPtr->filter->bSmooth, sizeof(bool));
	controls->update(appPtr->smoothPanel_smooth, kofxGui_Set_Bool, &appPtr->filter->smooth, sizeof(float));

	//Highpass
	controls->update(appPtr->highpassPanel_use, kofxGui_Set_Bool, &appPtr->filter->bHighpass, sizeof(bool));
	controls->update(appPtr->highpassPanel_blur, kofxGui_Set_Bool, &appPtr->filter->highpassBlur, sizeof(float));
	controls->update(appPtr->highpassPanel_noise, kofxGui_Set_Bool, &appPtr->filter->highpassNoise, sizeof(float));
	//Amplify
	controls->update(appPtr->amplifyPanel_use, kofxGui_Set_Bool, &appPtr->filter->bAmplify, sizeof(bool));
	controls->update(appPtr->amplifyPanel_amp, kofxGui_Set_Bool, &appPtr->filter->highpassAmp, sizeof(float));
	//Threshold
	controls->update(appPtr->imgSetPanel_threshold, kofxGui_Set_Bool, &appPtr->filter->threshold, sizeof(float));
}

void ofxSelectionTool ::handleGui(int parameterId, int task, void* data, int length)
{
	switch(parameterId)
	{
		//Source
		case propertiesPanel_flipH:
			if(length == sizeof(bool))
				filter->bHorizontalMirror = *(bool*)data;
			break;
		case propertiesPanel_flipV:
			if(length == sizeof(bool))
				filter->bVerticalMirror = *(bool*)data;
			break;
		case imgSetPanel_showBinary:
			if(length == sizeof(bool))
				filter->bShowBinaryImage = *(bool*)data;
			break;
		//Background
		case backgroundPanel_remove:
			if(length == sizeof(bool))
				filter->bLearnBakground = *(bool*)data;
			break;
		//Highpass
		case highpassPanel_use:
			if(length == sizeof(bool))
				filter->bHighpass = *(bool*)data;
			break;
		case highpassPanel_blur:
			if(length == sizeof(float))
				filter->highpassBlur = *(float*)data;
			break;
		case highpassPanel_noise:
			if(length == sizeof(float))
				filter->highpassNoise = *(float*)data;
			break;
		//Amplify
		case amplifyPanel_use:
			if(length == sizeof(bool))
				filter->bAmplify = *(bool*)data;
			break;
		case amplifyPanel_amp:
			if(length == sizeof(float))
				filter->highpassAmp = *(float*)data;
			break;
		case imgSetPanel_darkblobs:
			if(length == sizeof(bool))
				filter->bTrackDark = *(bool*)data;
			break;
		case imgSetPanel_threshold:
			if(length == sizeof(float))
				filter->threshold = *(float*)data;
			break;
		//smooth
		case smoothPanel_smooth:
			if(length == sizeof(float))
				filter->smooth = *(float*)data;
			break;
		case smoothPanel_use:
			if(length == sizeof(bool))
				filter->bSmooth = *(bool*)data;
			break;
		//Save Settings
		case kParameter_SaveXml:
			if(length == sizeof(bool))
			{
				if(*(bool*)data)
				{
					controls->saveToXml(OFXGUI_XML);
					saveSettings();
				}
			}
			break;

	}
}
#endif //GUI_CONTROLS_H