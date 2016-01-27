#include "MainLevelScene.h"
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

USING_NS_CC;

Scene*MainLevelScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    auto layer = MainLevelScene::create();
//    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_SHAPE);

    scene->addChild(layer);

    return scene;
}

using std::chrono::system_clock;
std::string createName() {
    auto time = system_clock::to_time_t(system_clock::now());
    std::string name = std::string(std::ctime(&time));
    name.erase(std::remove(name.begin(), name.end(), '\n'), name.end());
    return name;
}

bool MainLevelScene::init()
{
    if ( !LayerColor::initWithColor(cocos2d::Color4B::WHITE) )
    {
        return false;
    }
    _webSocket = TheGameWebSocket::create("ws://188.226.135.225:8080");
    _webSocket->connect();

    setBackground("terrain.jpg");
    auto winSize = Director::getInstance()->getWinSize();
    std::string name = createName();
    _spaceship = Spaceship::create(name, "redfighter.png");
    _spaceship->setPosition(winSize.width / 2, winSize.height / 2);

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
        std::string userName = d["name"].GetString();
        float x = (float)d["x"].GetDouble();
        float y = (float)d["y"].GetDouble();

        std::cout << "User: " << userName << std::endl
        << "x: " << x << " y: " << y << std::endl;
//        auto sp = Spaceship::create(userName, "alien.png");
        auto sp = _ships.at(userName);
        if (sp != nullptr) {
            std::cout << "Moved ship name " << sp->name();
            sp->moveTo(Vec2(x, y));
        }else {
            sp = Spaceship::create(userName, "alien.png");
            sp->setPosition(x, y);
            _ships.insert(std::pair(userName, sp));
            this->addChild(sp);
        }

//        auto it = _spaceships.find(sp);
//        if (it == _spaceships.end()) {
//            size_t setSize = _spaceships.size();
//            cocos2d::log("Size %d", (int) setSize);
//            _spaceships.insert(sp);
//            sp->setPosition(x, y);
//            this->addChild(sp);
//        }else {
//            std::cout << "Moved ship name " << (*it)->name();
//            (*it)->moveTo(Vec2(x, y));
//        }
    };


    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _spaceship);
    this->addChild(_spaceship);
    return true;
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

void MainLevelScene::setBackground(const char *filename) {
    auto winSize = Director::getInstance()->getWinSize();
    auto bg = Sprite::create(filename);
    bg->setScaleX((winSize.width / bg->getContentSize().width) * 1);
    bg->setScaleY((winSize.height / bg->getContentSize().height) * 1);
    bg->setAnchorPoint(Vec2(0, 0));

    this->addChild(bg, 0);
}

