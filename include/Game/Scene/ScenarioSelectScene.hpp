#include "Game/Scene/Scene.h"

class ScenarioSelectScene : public Scene {
public:
    ScenarioSelectScene();

    virtual ~ScenarioSelectScene();
    virtual void init();
    virtual void start();
    virtual void update();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void calcViewAndEntry();
};