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
	void addTemplate(ofRectangle rect,ofRectangle minRect, ofRectangle maxRect,float scaleX ,float scaleY);
	int getTemplateId(float width,float height);

	//Variables
	bool isLoaded;

	std::vector<Template>		templates;
	ofxXmlSettings				XML;

};

#endif