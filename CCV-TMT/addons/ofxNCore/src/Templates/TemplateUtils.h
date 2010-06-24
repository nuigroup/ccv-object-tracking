#ifndef TEMPLATEUTILS_H
#define TEMPLATEUTILS_H

#include "Template.h"
#include "ofMain.h"
#include "ofxXmlSettings.h"

#define MAX_NUM_TEMPLATES 20

class TemplateUtils	{

public:

	TemplateUtils()
	{
		isLoaded = false;
	}

	~TemplateUtils()
	{
	}

	//Functions 
	bool loadTemplateXml();
	void saveTemplateXml();
	int matchTemplate();
	void addTemplate(ofRectangle rect,ofRectangle minRect, ofRectangle maxRect);

	//Variables
	bool isLoaded;

	std::vector<Template>		templates;
	ofxXmlSettings				XML;

};

#endif