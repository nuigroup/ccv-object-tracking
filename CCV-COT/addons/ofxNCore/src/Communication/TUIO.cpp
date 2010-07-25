/*
*  TUIO.h
*  
*
*  Created on 2/2/09.
*  Copyright 2009 NUI Group. All rights reserved.
*
*/

#include "TUIO.h"

TUIO::TUIO() {
}

TUIO::~TUIO() {

	// this could be useful for whenever we get rid of an object
}

void TUIO::setup(const char* host, int port, int flashport) {

	localHost = host;
	TUIOPort = port;
	TUIOFlashPort = flashport;
	frameseq = 0;

	//FOR TCP
	bIsConnected = m_tcpServer.setup(TUIOFlashPort);
	//FOR OSC
	TUIOSocket.setup(localHost, TUIOPort);
}

void TUIO::sendTUIO(std::map<int, Blob> * blobs)
{
	frameseq += 1;

	//if sending OSC (not TCP)
	if(bOSCMode){
		ofxOscBundle b;
		ofxOscMessage alive;
		// Sends alive message - saying 'Hey, there's no alive blobs'
		alive.setAddress("/tuio/2Dcur");
		alive.addStringArg("alive");

		// Send fseq message
		ofxOscMessage fseq;
		fseq.setAddress( "/tuio/2Dcur" );
		fseq.addStringArg( "fseq" );
		fseq.addIntArg(frameseq);

		if(blobs->size() == 0)
		{
			b.addMessage( alive );		// add message to bundle
			b.addMessage( fseq );		// add message to bundle
			TUIOSocket.sendBundle( b ); // send bundle
		}
		else // actually send the blobs
		{
			map<int, Blob>::iterator blob;
			for(blob = blobs->begin(); blob != blobs->end(); blob++)
			{
				// omit point (0,0) since this means that we are outside of the range
				if(blob->second.centroid.x == 0 && blob->second.centroid.y == 0)
					continue;

				//Set Message
				ofxOscMessage set;
				set.setAddress( "/tuio/2Dcur" );
				set.addStringArg("set");
				set.addIntArg(blob->second.id);				// id
				set.addFloatArg(blob->second.centroid.x);	// x
				set.addFloatArg(blob->second.centroid.y);	// y
				set.addFloatArg(blob->second.D.x);			// dX
				set.addFloatArg(blob->second.D.y);			// dY
				set.addFloatArg(blob->second.maccel);		// m
				if(bHeightWidth)
				{
					set.addFloatArg(blob->second.boundingRect.width);	// wd
					set.addFloatArg(blob->second.boundingRect.height);	// ht
				}
				b.addMessage( set );							// add message to bundle
				alive.addIntArg(blob->second.id);				// add blob to list of ALL active IDs
			}

			b.addMessage( alive );		//add message to bundle
			b.addMessage( fseq );		//add message to bundle
			TUIOSocket.sendBundle( b ); //send bundle
		}
	}else if(bTCPMode) //else, if TCP (flash) mode
	{
		if(blobs->size() == 0){

						m_tcpServer.sendToAll("<OSCPACKET ADDRESS=\"127.0.0.1\" PORT=\""+ofToString(TUIOPort)+"\" TIME=\""+ofToString(ofGetElapsedTimef())+"\">" +
							 "<MESSAGE NAME=\"/tuio/2Dcur\">"+
							 "<ARGUMENT TYPE=\"s\" VALUE=\"alive\"/>"+
							 "</MESSAGE>"+
							 "<MESSAGE NAME=\"/tuio/2Dcur\">"+
							 "<ARGUMENT TYPE=\"s\" VALUE=\"fseq\"/>"+
							 "<ARGUMENT TYPE=\"i\" VALUE=\""+ofToString(frameseq)+"\"/>" +
							 "</MESSAGE>"+
							 "</OSCPACKET>");
		}
		else
		{
			string setBlobsMsg;

			map<int, Blob>::iterator this_blob;
			for(this_blob = blobs->begin(); this_blob != blobs->end(); this_blob++)
			{
				//if sending height and width
				if(bHeightWidth){
					setBlobsMsg += "<MESSAGE NAME=\"/tuio/2Dcur\"><ARGUMENT TYPE=\"s\" VALUE=\"set\"/><ARGUMENT TYPE=\"i\" VALUE=\""+ofToString(this_blob->second.id)+"\"/>"+
					"<ARGUMENT TYPE=\"f\" VALUE=\""+ofToString(this_blob->second.centroid.x)+"\"/>"+
					"<ARGUMENT TYPE=\"f\" VALUE=\""+ofToString(this_blob->second.centroid.y)+"\"/>"+
					"<ARGUMENT TYPE=\"f\" VALUE=\""+ofToString(this_blob->second.D.x)+"\"/>"+
					"<ARGUMENT TYPE=\"f\" VALUE=\""+ofToString(this_blob->second.D.y)+"\"/>"+
					"<ARGUMENT TYPE=\"f\" VALUE=\""+ofToString(this_blob->second.maccel)+"\"/>"+
					"<ARGUMENT TYPE=\"f\" VALUE=\""+ofToString(this_blob->second.boundingRect.width)+"\"/>"+
					"<ARGUMENT TYPE=\"f\" VALUE=\""+ofToString(this_blob->second.boundingRect.height)+"\"/>"+
					"</MESSAGE>";
				}
				else{
					setBlobsMsg += "<MESSAGE NAME=\"/tuio/2Dcur\"><ARGUMENT TYPE=\"s\" VALUE=\"set\"/><ARGUMENT TYPE=\"i\" VALUE=\""+ofToString(this_blob->second.id)+"\"/>"+
					"<ARGUMENT TYPE=\"f\" VALUE=\""+ofToString(this_blob->second.centroid.x)+"\"/>"+
					"<ARGUMENT TYPE=\"f\" VALUE=\""+ofToString(this_blob->second.centroid.y)+"\"/>"+
					"<ARGUMENT TYPE=\"f\" VALUE=\""+ofToString(this_blob->second.D.x)+"\"/>"+
					"<ARGUMENT TYPE=\"f\" VALUE=\""+ofToString(this_blob->second.D.y)+"\"/>"+
					"<ARGUMENT TYPE=\"f\" VALUE=\""+ofToString(this_blob->second.maccel)+"\"/>"+
					"</MESSAGE>";
				}
			}

			string aliveBeginMsg = "<MESSAGE NAME=\"/tuio/2Dcur\"><ARGUMENT TYPE=\"s\" VALUE=\"alive\"/>";

			string aliveBlobsMsg;
			std::map<int, Blob>::iterator this_blobID;
			for(this_blobID = blobs->begin(); this_blobID != blobs->end(); this_blobID++)
			{
				aliveBlobsMsg += "<ARGUMENT TYPE=\"i\" VALUE=\""+ofToString(this_blobID->second.id)+"\"/>";
			}

			string aliveEndMsg = "</MESSAGE>";

			string fseq = "<MESSAGE NAME=\"/tuio/2Dcur\"><ARGUMENT TYPE=\"s\" VALUE=\"fseq\"/><ARGUMENT TYPE=\"i\" VALUE=\""+ofToString(frameseq)+"\"/></MESSAGE>";

			m_tcpServer.sendToAll("<OSCPACKET ADDRESS=\"127.0.0.1\" PORT=\""+ofToString(TUIOPort)+"\" TIME=\""+ofToString(ofGetElapsedTimef())+"\">" +
									setBlobsMsg + aliveBeginMsg + aliveBlobsMsg + aliveEndMsg + fseq + "</OSCPACKET>");
		}
	}
}

void TUIO::sendObjTUIO(std::map<int, Blob> * blobs)
{
}
void TUIO::sendFidTUIO(std::list <ofxFiducial> * fiducialsList)
{
}
