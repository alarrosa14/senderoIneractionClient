//
//  InteractionBehaviour.h
//  SenderoInteractionClient
//
//  Created by Christian Bouvier on 9/16/15.
//
//

#ifndef __SenderoInteractionClient__InteractionBehaviour__
#define __SenderoInteractionClient__InteractionBehaviour__

#include <iostream>
#include "SpecificBehaviour.h"
#include <SimpleAmqpClient/SimpleAmqpClient.h>

using namespace AmqpClient;

class InteractionBehaviour : public SpecificBehaviour {
public:
    InteractionBehaviour();
    ~InteractionBehaviour();
    
private:
    void customSetup (map<int,Pixel*>* iPixels, vector<Pixel*>* iPixelsFast);
    void update();
    void draw();
    void keyPressed(int key);
    void exit();
    
    Channel::ptr_t connection;
    std::map<std::string, std::vector<int> > userColors;
    float radius;
};

#endif /* defined(__SenderoInteractionClient__InteractionBehaviour__) */
