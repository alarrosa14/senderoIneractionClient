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
}

InteractionBehaviour::~InteractionBehaviour(){
}

void InteractionBehaviour::customSetup (map<int,Pixel*>* pixels, vector<Pixel*>* pixelsFast) {
    
    this->connection = Channel::Create("localhost");

    connection->BindQueue("interactions_queue", "amq.direct", "");
    connection->BasicConsume("interactions_queue", "consumertag");
}

void InteractionBehaviour::update() {

    Envelope::ptr_t env;
    BasicMessage::ptr_t message;

    if (connection->BasicConsumeMessage("consumertag", env, 1))
    {
        message = env->Message();
        std::cout << "Message text: " << message->Body() << std::endl;
        std::string coords = message->Body();
        std::vector<std::string> coord = split(coords,',');

        double x = atof(coord[0].c_str());
        double y = atof(coord[1].c_str());
        double z = atof(coord[2].c_str());

        sphere.setRadius(10);
        sphere.setPosition(x*40, y*40, z*40);
    }

}

void InteractionBehaviour::draw() {
    sphere.draw();
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