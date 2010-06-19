#ifndef TEMPLATEUTILS_H
#define TEMPLATEUTILS_H

class TemplateUtils	{
public:
	void TemplateUtils()
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