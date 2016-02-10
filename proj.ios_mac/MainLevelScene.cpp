#include "MainLevelScene.h"
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"



using namespace std::chrono;

const std::string remoteServerAddr = "ws://188.226.135.225:8080";
const std::string localServerAddr = "ws://127.0.0.1:8080";

Scene* MainLevelScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    auto layer = MainLevelScene::create();
//    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_SHAPE);

    scene->addChild(layer);
    return scene;
}

bool MainLevelScene::init()
{
    if ( !LayerColor::initWithColor(cocos2d::Color4B::WHITE) )
    {
        return false;
    }
    setBackground(this, "terrain.jpg");
    
    _webSocket = TheGameWebSocket::create(localServerAddr);
    _webSocket->connect();

    auto winSize = Director::getInstance()->getWinSize();
    
    auto time = system_clock::to_time_t(system_clock::now());
    std::string name = std::string(std::ctime(&time));
    
    _spaceship = Spaceship::create(name, "redfighter.png");
    _spaceship->setPosition(winSize.width / 2, winSize.height / 2);

    auto button = ui::Button::create("fire_button.png");
    button->setPosition(Vec2(150.f, 150.f));
    button->setTouchEnabled(true);
    button->addTouchEventListener(CC_CALLBACK_2(MainLevelScene::buttonPressed, this));

    this->addChild(button, 100);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [&](Touch *touch, Event *event) -> bool {
        auto dest = touch->getLocation();
        std::string json = createJSON(dest.x, dest.y);
        std::cout << "JSON: " << json << std::endl;
        _webSocket->send(json);
        _spaceship->moveTo(dest);

        return true;
    };
    

    _webSocket->receive = [&](std::string &json) {
        rapidjson::Document d;
        d.Parse(json.c_str());
        if (d.HasMember("method")) {
            std::string method = d["method"].GetString();
            if (method == "destroy") {
                auto it = _ships.find(d["name"].GetString());
                
                if (it == _ships.end())
                    return;
                
                auto theShip = it->second;
                theShip->removeFromParent();
                _ships.erase(it);
            }
            return;
        }
        std::string userName = d["name"].GetString();
        float x = (float)d["x"].GetDouble();
        float y = (float)d["y"].GetDouble();
        
        auto it = _ships.find(userName);
        if (it != _ships.end()) {
            std::cout << "Moved ship name " << it->second->name();
            it->second->moveTo(Vec2(x, y));
        }else {
            auto sp = Spaceship::create(userName, "alien.png");
            sp->setPosition(x, y);
            _ships.insert(std::pair<std::string, Spaceship *>(userName, sp));
            this->addChild(sp);
        }
    };


    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _spaceship);
    this->addChild(_spaceship);
    return true;
}

void MainLevelScene::buttonPressed(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchEvent) {
    switch (touchEvent) {
        case ui::Widget::TouchEventType::BEGAN:
            log("Fire!");
            break;
        default:
            break;
    }
}


std::string MainLevelScene::createJSON(float x, float y) {
    rapidjson::Document document;
    document.SetObject();

    auto &allocator = document.GetAllocator();

    document.AddMember("name",
                       rapidjson::Value(_spaceship->name().c_str(), allocator).Move(),
                       allocator);
    document.AddMember("x",
                       rapidjson::Value(x).Move(),
                       allocator);
    document.AddMember("y",
                       rapidjson::Value(y).Move(),
                       allocator);

    rapidjson::StringBuffer stringBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(stringBuffer);
    document.Accept(writer);

    return std::string(stringBuffer.GetString());
}


void MainLevelScene::update(float delta) {

}

