//
//  InteractionBehaviour.cpp
//  SenderoInteractionClient
//
//  Created by Christian Bouvier on 9/16/15.
//
//

#include "InteractionBehaviour.h"

#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>     /* atof */
#include <vector>

static const Color rgbColors[] = {
	Color(238,64,53),
	Color(243,119,54),
	Color(253,244,152),
	Color(123,192,67),
	Color(3,146,207)
};

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

InteractionBehaviour::InteractionBehaviour(){
    radius = 30;
    userColorIterator = 0;
    userColorIndex = map<string, unsigned int>();
    colorPalette = vector<Color>(rgbColors, rgbColors + sizeof(rgbColors) / sizeof(Color) );
}

InteractionBehaviour::~InteractionBehaviour(){
}

void InteractionBehaviour::customSetup (map<int,Pixel*>* pixels, vector<Pixel*>* pixelsFast) {
    dataConnection = Channel::Create("localhost");
    dataConnection->BindQueue("dragging_queue", "amq.direct");
    dataConnection->BasicConsume("dragging_queue");

    controlConnection = Channel::Create("localhost");
    controlConnection ->BindQueue("control_queue", "amq.direct");
    controlConnection ->BasicConsume("control_queue");
}

void InteractionBehaviour::update() {

    Envelope::ptr_t env;
    BasicMessage::ptr_t message;
	std::string web_client_id;

    Envelope::ptr_t controlEnv;
    if (controlConnection->BasicConsumeMessage(controlEnv, 0)) {
    	message = controlEnv->Message();
    	std::string command = message->Body();
    	if (command == "disconnected" && message->HeaderTableIsSet() && message->HeaderTable().count("web_client_id") > 0) {
			web_client_id = message->HeaderTable().at("web_client_id").GetString();
			cout << "Client disconnnected: " << web_client_id << endl;
			if (userColorIndex.count(web_client_id) > 0)
				userColorIndex.erase(web_client_id);
    	}
    }

    for(int i = 0; i < pixelsFast->size(); i++){
        Pixel* px = (*pixelsFast)[i];
        // px->blendRGBA(0,0,0,255,1);
        px->fadeToBlack(0.95);
    }

	int i;
    for (i = 0; i < 10 && dataConnection->BasicConsumeMessage(env, 0); i++) {

		message = env->Message();

		int r, g, b;
		if (message->HeaderTableIsSet() && message->HeaderTable().count("web_client_id") > 0) {
			web_client_id = message->HeaderTable().at("web_client_id").GetString();
			std::cout << "user: " << message->HeaderTable().at("web_client_id").GetString() << endl;

			if (userColorIndex.count(web_client_id) == 0) {
				userColorIndex[web_client_id] = userColorIterator;
				userColorIterator = (userColorIterator + 1) % colorPalette.size();
			}

			r = colorPalette.at(userColorIndex[web_client_id]).r;
			g = colorPalette.at(userColorIndex[web_client_id]).g;
			b = colorPalette.at(userColorIndex[web_client_id]).b;
		}

		std::cout << "Message: " << message->Body() << endl;
		std::string coords = message->Body();
		std::vector<std::string> coord = split(coords,',');

		float x = atof(coord[0].c_str());
		float y = atof(coord[1].c_str());
		float z = atof(coord[2].c_str());
//            int r = atoi(coord[3].c_str());
//            int g = atoi(coord[4].c_str());
//            int b = atoi(coord[5].c_str());

		ofVec3f touchPosition(x, y, z);

		for(int i = 0; i < pixelsFast->size(); i++){
			Pixel* px = (*pixelsFast)[i];
			ofVec3f pxPosition = px->getPosition();

			float dist = touchPosition.distance(pxPosition);

//                px->blendRGBA(255,255,255,255,1);

			if (dist < this->radius){
				float normalizedDist = 1 - dist/this->radius;
				px->blendRGBA(r,g,b,255,ofLerp(0.1,1,normalizedDist));
			}
		}
    }
}

void InteractionBehaviour::draw() {
}

void InteractionBehaviour::keyPressed(int key){
    //sample -> paint every pixel with red at key pressed and blend with black at update
    vector<Pixel*>::iterator it = this->pixelsFast->begin();
    
    while (it!=this->pixelsFast->end()){
        Pixel* px = *it;
        px->blendRGBA(255, 0, 255, 255, 1.0f);
        it++;
    }
    //end of sample
    
}

void InteractionBehaviour::exit() {
    SpecificBehaviour::exit();
}
