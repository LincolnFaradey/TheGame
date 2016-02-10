#pragma once

#include "cocos2d.h"
#include "TheGameWebSocket.h"
#include "Spaceship.h"
#include <map>
#include <ui/CocosGUI.h>

USING_NS_CC;

class MainLevelScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;

    CREATE_FUNC(MainLevelScene);

    virtual void update(float delta) override;

    std::string createJSON(float x, float y);
    
    void buttonPressed(Ref *sender, ui::Button::TouchEventType touchEvent);

private:
    Spaceship *_spaceship;
    std::map<std::string, Spaceship *> _ships;
    TheGameWebSocket *_webSocket;
};




void setBackground(cocos2d::Layer *layer, const char *filename) {
    auto winSize = Director::getInstance()->getWinSize();
    auto bg = Sprite::create(filename);
    bg->setScaleX((winSize.width
                   / bg->getContentSize().width) * 1);
    bg->setScaleY((winSize.height
                   / bg->getContentSize().height) * 1);
    bg->setAnchorPoint(Vec2(0, 0));
    
    layer->addChild(bg, 0);
}