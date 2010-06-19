#ifndef TEMPLATEUTILS_H
#define TEMPLATEUTILS_H

#include "Template.h"

class TemplateUtils	{
public:
	TemplateUtils()
	{
	}

	~TemplateUtils()
	{
	}
	//Functions 

	int loadTemplateXml();
	int saveTemplateXml();
	int matchTemplate();
	void addTemplate();
	int reset();

	std::vector<Template>		templates;
	ofxXmlSettings				XML;

};


#endif