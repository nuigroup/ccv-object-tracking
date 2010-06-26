#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "ofMain.h"

class Template {
	
public:
	Template() 
	{
		id=185;    // <<<==== Have to change 
	}

	//The ID of the template
	int			id;

	//Rectangle Specifications
	float			height;
	float			width;
	float			minHeight;
	float			minWidth;
	float			maxHeight;
	float			maxWidth;

	bool matchWithTemplate(Template t);
};

#endif