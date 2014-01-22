#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

HelloWorld::HelloWorld(){
    _tileMap = NULL;
    _background = NULL;
    _player = NULL;
    _meta = NULL;
    _foreground = NULL;
    
    _numCollected = 0;
    _hud = NULL;
}

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    
    /*Hud*/
    HelloWorldHud *hud = HelloWorldHud::create();
    scene->addChild(hud);
    layer->setHud(hud);
    
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    /*add tilemap*/

    bool bRet = false;
    do{
        CC_BREAK_IF(!CCLayer::init());
        
        this->setTileMap(CCTMXTiledMap::create("firsttest_cjy.tmx"));
        this->setBackground(_tileMap->layerNamed("Background"));
        
        this->setMeta(_tileMap->layerNamed("Meta"));
        _meta->setVisible(false);
        
        this->setForeground(_tileMap->layerNamed("Foreground"));
        
        this->addChild(_tileMap,-1);
        bRet = true;
    
    }while (0);
    
    /*add objects*/
    
    CCTMXObjectGroup *objects = _tileMap->objectGroupNamed("Objects");
    CCAssert(objects != NULL, "Objects' object group not found");
    CCDictionary *spawnPoint = objects->objectNamed("SpawnPoint");
    CCAssert(spawnPoint != NULL, "SpawnPoint object not found");
    
    int x = spawnPoint->valueForKey("x")->intValue();
    int y = spawnPoint->valueForKey("y")->intValue();
    this->setPlayer(CCSprite::create("Player.png"));
    _player->setPosition(ccp(x, y));
    this->addChild(_player);
    this->setViewpointCenter(_player->getPosition());
    
    /*add move*/
    this->setTouchEnabled(true);
    
    /*add music*/
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("pickup.wav");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("hit.wav");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("move.wav");
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("TileMap.wav");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("TileMap.wav",true);
    
    
    
    return bRet;
}

void HelloWorld::setViewpointCenter(cocos2d::CCPoint position){

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    int x = MAX(position.x, winSize.width / 2);
    int y = MAX(position.y, winSize.height / 2);
    
    x = MIN(x, (_tileMap->getMapSize().width * _tileMap->getTileSize().width - winSize.width / 2));
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height - winSize.width / 2));
    
    CCPoint actualPotision = ccp(x,y);
    CCPoint centerOfView = ccp(winSize.width / 2,winSize.height / 2);
    CCPoint viewPoint = ccpSub(centerOfView, actualPotision);
    
    this->setPosition(viewPoint);

}

void HelloWorld::registerWithTouchDispatcher(void){
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
}

bool HelloWorld::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

    return true;
}

void HelloWorld::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
    CCPoint touchLocation = this->convertTouchToNodeSpace(pTouch);
    CCPoint playerPos = _player->getPosition();
    CCPoint diff = ccpSub(touchLocation, playerPos);
    
    if (abs(diff.x) > abs(diff.y)) {
        if (diff.x > 0) {
            playerPos.x += _tileMap->getTileSize().width;
            
        }else{
            
            playerPos.x -= _tileMap->getTileSize().width;
        }
    }else{
    
        if (diff.y > 0) {
            playerPos.y += _tileMap->getTileSize().height;
        }else{
        
            playerPos.y -= _tileMap->getTileSize().height;
        }
    }
    
    if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
        playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
        playerPos.x >= 0 &&
        playerPos.y >= 0) {
        setPlayerPosition(playerPos);
    }

    setViewpointCenter(_player->getPosition());

}

void HelloWorld::setPlayerPosition(cocos2d::CCPoint position){
    
    CCPoint tileCoord = this->tileCoordForPosition(position);
    int tileGid = _meta->tileGIDAt(tileCoord);
    if (tileGid) {
        CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
        if (properties) {
            /*walktable*/
            const CCString *collision = properties->valueForKey("Collidable");
            if (collision && collision->compare("true") == 0) {
                /*music*/
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
                return;
            }
            
            /*eatable*/
            const CCString *collectable = properties->valueForKey("Collectable");
            if (collectable && collectable->compare("true") == 0) {
                _meta->removeTileAt(tileCoord);
                _foreground->removeTileAt(tileCoord);
                
                /*score*/
                _numCollected++;
                _hud->numCollectedChanged(_numCollected);
                
                /*music*/
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pickup.wav");
                
            }
        }
    }
    
    
    _player->setPosition(position);
}

CCPoint HelloWorld::tileCoordForPosition(cocos2d::CCPoint position){

    int x = position.x / _tileMap->getTileSize().width;
    int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - position.y) / _tileMap->getTileSize().height;
    
    return ccp(x,y);
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

bool HelloWorldHud::init(){

    bool bRet = false;
    do{
    
        CC_BREAK_IF(!CCLayer::init());
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        lable = CCLabelTTF::create("0","Verdana-Bold",18.0,CCSizeMake(50, 20),kCCTextAlignmentRight);
        lable->setColor(ccc3(0, 0, 0));
        int margin = 10;
        lable->setPosition(ccp(winSize.width - (lable->getContentSize().width / 2) - margin,
                               lable->getContentSize().height / 2 + margin));
        this->addChild(lable);
        bRet = true;
    }while (0);
    
    return bRet;
}

void HelloWorldHud::numCollectedChanged(int numCollected){

    lable->setString(CCString::createWithFormat("%d",numCollected)->getCString());
}














































