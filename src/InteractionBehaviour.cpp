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

    userColors = std::map<std::string, std::vector<int> >();
}

InteractionBehaviour::~InteractionBehaviour(){
}

void InteractionBehaviour::customSetup (map<int,Pixel*>* pixels, vector<Pixel*>* pixelsFast) {
    this->connection = Channel::Create("localhost");

    connection->BindQueue("dragging_queue", "amq.direct", "");
    connection->BasicConsume("dragging_queue", "consumertag");
}

void InteractionBehaviour::update() {

    Envelope::ptr_t env;
    BasicMessage::ptr_t message;

    for(int i = 0; i < pixelsFast->size(); i++){
        Pixel* px = (*pixelsFast)[i];
        // px->blendRGBA(0,0,0,255,1);
        px->fadeToBlack(0.9);
    }

    for(int i = 0; i < 5; i++){

        if (connection->BasicConsumeMessage("consumertag", env, 0)){

            message = env->Message();

            std::string web_client_id;
        	int r, g, b;
            if (message->HeaderTableIsSet() && message->HeaderTable().count("web_client_id") > 0) {
            	web_client_id = message->HeaderTable().at("web_client_id").GetString();
            	std::cout << "user: " << message->HeaderTable().at("web_client_id").GetString() << endl;

            	if (userColors.count(web_client_id) > 0) {
            		r = userColors.at(web_client_id)[0];
            		g = userColors.at(web_client_id)[1];
					b = userColors.at(web_client_id)[2];
            	} else {
            		std::vector<int> rgb = std::vector<int>();
            		rgb.push_back(rand() % 256);
            		rgb.push_back(rand() % 256);
            		rgb.push_back(rand() % 256);
            		userColors[web_client_id] = rgb;
            	}
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

        }else{

            // if (i == 0){
            //     for(int i = 0; i < pixelsFast->size(); i++){
            //         Pixel* px = (*pixelsFast)[i];
            //         px->fadeToBlack(0.9);
            //     }
            // }
            break;
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
