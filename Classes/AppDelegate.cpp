#include "AppDelegate.h"
#include "MainLevelScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

typedef struct tagResource
{
    cocos2d::Size size;
    char directory[100];
}Resource;

std::vector<std::string> searchPath;
static Resource designResource =  { cocos2d::Size(768, 1024),  "ipad"};
static Resource smallResource  =  { cocos2d::Size(640, 960),   "iphone"};
static Resource mediumResource =  { cocos2d::Size(768, 1024),  "ipad"};
static Resource galax10Resource =  { cocos2d::Size(800, 1280),  "gt_10p"};
static Resource fullHDResource  =  { cocos2d::Size(1080, 1920), "fullhd"};
static Resource largeResource  =  { cocos2d::Size(1536, 2048), "ipadhd"};

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("The Game");

        glview->setFrameSize(designResource.size.height, designResource.size.width);

        director->setOpenGLView(glview);
    }
    glview->setDesignResolutionSize(smallResource.size.height,
                                    smallResource.size.width,
                                    ResolutionPolicy::NO_BORDER);
    director->setDisplayStats(true);
    auto scene = MainLevelScene::createScene();
    director->runWithScene(scene);
//    director->setAnimationInterval(1/60.f);

    cocos2d::Size frameSize = glview->getFrameSize();
    Resource realResource;
// if the frame's height is larger than the height of medium size.
    if (frameSize.height > fullHDResource.size.height) {
        realResource = largeResource;
    } else if (frameSize.height > galax10Resource.size.height) {
        realResource = fullHDResource;
    } else if (frameSize.height > mediumResource.size.height) {
        realResource = galax10Resource;
    } else if (frameSize.height > smallResource.size.height) {
        realResource = mediumResource;
    } else {
        realResource = smallResource;
    }
    director->setContentScaleFactor(MIN(realResource.size.height/designResource.size.height, realResource.size.width/designResource.size.width));
    searchPath.push_back(realResource.directory);
    auto fileUtils = FileUtils::getInstance();
    fileUtils->setSearchPaths(searchPath);
    return true;
}

void AppDelegate::applicationDidEnterBackground() {
}

void AppDelegate::applicationWillEnterForeground() {
}