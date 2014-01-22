#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

/*score*/
class HelloWorldHud : public cocos2d::CCLayer{

public:
    virtual bool init();
    CREATE_FUNC(HelloWorldHud);
    void numCollectedChanged(int numCollected);
    cocos2d::CCLabelTTF *lable;
};


class HelloWorld : public cocos2d::CCLayer
{


public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
    
    
    /*cjy*/
    CC_SYNTHESIZE_RETAIN(cocos2d::CCTMXTiledMap*, _tileMap, TileMap);
    CC_SYNTHESIZE_RETAIN(cocos2d::CCTMXLayer*, _background, Background);
    CC_SYNTHESIZE_RETAIN(cocos2d::CCSprite*, _player, Player);
    CC_SYNTHESIZE_RETAIN(cocos2d::CCTMXLayer*, _meta, Meta);
    CC_SYNTHESIZE_RETAIN(cocos2d::CCTMXLayer*, _foreground, Foreground);
    CC_SYNTHESIZE(int, _numCollected, NumCollected);
    CC_SYNTHESIZE_RETAIN(HelloWorldHud*, _hud, Hud);
    
    
    HelloWorld();
    void setViewpointCenter(cocos2d::CCPoint position);
    
    void registerWithTouchDispatcher(void);
    
    bool ccTouchBegan(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent);
    
  
    
    void setPlayerPosition(cocos2d::CCPoint position);
    cocos2d::CCPoint tileCoordForPosition(cocos2d::CCPoint position);
};

#endif // __HELLOWORLD_SCENE_H__

























