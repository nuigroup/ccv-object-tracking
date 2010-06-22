#include "TemplateUtils.h"

//Adds template data to the vector
void TemplateUtils::addTemplate(ofRectangle rect,ofRectangle minRect, ofRectangle maxRect) 
{
	Template temp = Template();
	temp.width = rect.width;
	temp.height = rect.height;
	temp.maxWidth = maxRect.width;
	temp.maxHeight = maxRect.height;
	temp.minWidth = minRect.width;
	temp.minHeight = minRect.height;

	templates.push_back(temp);
	printf("Template added. Number of templates = %d\n",templates.size());
}

//Loads the XML file from templates.xml file. This returns false if the file does not exist
bool TemplateUtils::loadTemplateXml()
{
	if( XML.loadFile("templates.xml") )
	{
		int numTags=XML.getNumTags("TEMPLATE");
		if( numTags > 0 )
		{
			int total = MIN(numTags,MAX_NUM_TEMPLATES);
			;
			for(int i=0; i < total; i++)
			{
				float width =(float) XML.getValue("TEMPLATE:WIDTH",0);
				float height =(float)XML.getValue("TEMPLATE:HEIGHT",0);
				float minWidth =(float)  XML.getValue("TEMPLATE:MINWIDTH",0);
				float minHeight = (float) XML.getValue("TEMPLATE:MINHEIGHT",0);
				float maxWidth = (float) XML.getValue("TEMPLATE:MAXWIDTH",0);
				float maxHeight =(float) XML.getValue("TEMPLATE:MAXHEIGHT",0);

				if(width !=0 && height != 0 && minWidth != 0 && minHeight != 0 && maxWidth != 0 && maxHeight != 0)
				{//Only then add a template

					Template temp = Template();
					temp.width = width;
					temp.height = height;
					temp.maxWidth = maxWidth;
					temp.maxHeight = maxHeight;
					temp.minWidth = minWidth;
					temp.minHeight = minHeight;

					templates.push_back(temp);
				}
			}
			isLoaded = true;// <- This is inside the if statement so that if the file exists but no xml data in it,
							//this will still be false
		}
		return true;
	}
	else
	{
		printf("Templates.xml could not be loaded. Make sure it is there in data folder\n");
		return false;
	}
}

//Saves into XML file from the template vector.
void TemplateUtils::saveTemplateXml()
{
	XML.clear();

	for(int i = 0 ; i < templates.size() ; i++ )
	{
		int tagNum=XML.addTag("TEMPLATE");

		XML.setValue("TEMPLATE:WIDTH",templates[i].width,tagNum);
		XML.setValue("TEMPLATE:HEIGHT",templates[i].height,tagNum);
		XML.setValue("TEMPLATE:MINWIDTH",templates[i].minWidth,tagNum);
		XML.setValue("TEMPLATE:MINHEIGHT",templates[i].minHeight,tagNum);
		XML.setValue("TEMPLATE:MAXWIDTH",templates[i].maxWidth,tagNum);
		XML.setValue("TEMPLATE:MAXHEIGHT",templates[i].maxHeight,tagNum);
		
		XML.popTag();	
	}
	XML.saveFile("templates.xml");
	printf("Template is saved\n");
}