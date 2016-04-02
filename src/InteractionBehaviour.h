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
using namespace std;

typedef struct Color {
	int r, g, b;
	Color(int red, int green, int blue) : r(red), g(green), b(blue) {}
};

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
    
    Channel::ptr_t dataConnection;

    map<string, unsigned int> userColorIndex;

    vector<Color> colorPalette;
    unsigned int userColorIterator = 0;

    float radius;
};

#endif /* defined(__SenderoInteractionClient__InteractionBehaviour__) */
