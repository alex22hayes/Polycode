//
// Polycode template. Write your code here.
//

#include "Polycode.h"
#include "polycode/modules/ui/PolycodeUI.h"
#include "polycode/modules/physics3D/Polycode3DPhysics.h"

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
    Scene *scene;

    Core *core;
    Vector3 rot;
    Number running;
    SceneLabel* fps;

    Scene* game;
    ScenePrimitive* box;
    UIButton* btn;
};
