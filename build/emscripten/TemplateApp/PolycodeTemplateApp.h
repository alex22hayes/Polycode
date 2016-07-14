//
// Polycode template. Write your code here.
//

#include "Polycode.h"

using namespace Polycode;

class PolycodeTemplateApp : public EventHandler {
public:
    PolycodeTemplateApp(PolycodeView *view);
    ~PolycodeTemplateApp();
    
    void handleEvent(Event *event);
    
    bool Update();
    
private:
    
    Sound *bgSound;
    
    Sound *sound1;
    Sound *sound2;
    Sound *sound3;
    ScenePrimitive *test;
    
    Core *core;
};
