#include "ParticleSystemScene.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;

Scene* ParticleSystemScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ParticleSystemScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

ParticleSystemScene::~ParticleSystemScene()
{
	if (_fireworkMove != nullptr) _fireworkMove->release();
	if (_fireworkCallback != nullptr) _fireworkCallback->release();
	if (_StarMove1 != nullptr) _StarMove1->release();
	if (_StarMove2 != nullptr) _StarMove2->release();
	if (_StarMove3 != nullptr) _StarMove3->release();
	if (_StarMove4 != nullptr) _StarMove4->release();
	if (_StarMove5 != nullptr) _StarMove5->release();
	if (_starMoveCallback != nullptr) _starMoveCallback->release();
	this->removeAllChildren();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("particle_add.plist");
}

// on "init" you need to initialize your instance
bool ParticleSystemScene::init()
{
	_visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto rootNode = CSLoader::createNode("MainScene.csb");
    addChild(rootNode);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("particle_add.plist");

	// For Emitter
	_EmitterSprite = CDraggableSprite::create();
	_EmitterSprite->setSpriteInfo("emittericon.png", Point(-125.0f + _visibleSize.width / 2.0f, _visibleSize.height / 2.0f));
	_EmitterSprite->setVisible(false);
	_bEmitterOn = false;
	this->addChild(_EmitterSprite, 5);

	auto emiterpos = (Sprite *)(rootNode->getChildByName("emitterpos"));
	Point loc = emiterpos->getPosition();
	emiterpos->setVisible(false);
	_emitterSwitchBtn = CSwitchButton::create();
	_emitterSwitchBtn->setButtonInfo("emitteroff.png", "emitteron.png", "emittertd.png", loc);
	this->addChild(_emitterSwitchBtn, 2);

	//firework btn
	_bFireworkOn = false;
	_fireworkLoc = (Sprite *)(rootNode->getChildByName("fireworkpos"));
	loc = _fireworkLoc->getPosition();
	_fireworkSwitchBtn = CSwitchButton::create();
	_fireworkSwitchBtn->setButtonInfo("firework_btn.png", "firework_btn_on.png", "firework_btn_click.png", loc);
	this->addChild(_fireworkSwitchBtn, 2);
	_fireworkLoc->setPosition(-125.0f + _visibleSize.width / 2.0f, -50.0f); //設定煙火位置
	_fireworkLoc->setVisible(false); //隱藏中心點圖案

	//Dofferent Emitter Buttons
	_bEmitter1On = false;
	auto emitter1pos = (Sprite *)(rootNode->getChildByName("emitter1pos"));
	loc = emitter1pos->getPosition();
	emitter1pos->setVisible(false);
	_emitter1SwitchBtn = CSwitchButton::create();
	_emitter1SwitchBtn->setButtonInfo("fire_btn.png", "fire_btn_on.png", "fire_btn_click.png", loc);
	this->addChild(_emitter1SwitchBtn, 2);

	_bEmitter2On = false;
	auto emitter2pos = (Sprite *)(rootNode->getChildByName("emitter2pos"));
	loc = emitter2pos->getPosition();
	emitter2pos->setVisible(false);
	_emitter2SwitchBtn = CSwitchButton::create();
	_emitter2SwitchBtn->setButtonInfo("rain_btn.png", "rain_btn_on.png", "rain_btn_click.png", loc);
	this->addChild(_emitter2SwitchBtn, 2);

	_bEmitter3On = false;
	_starMoveLoc = (Sprite *)(rootNode->getChildByName("emitter3pos"));
	loc = _starMoveLoc->getPosition();
	_emitter3SwitchBtn = CSwitchButton::create();
	_emitter3SwitchBtn->setButtonInfo("star_btn.png", "star_btn_on.png", "star_btn_click.png", loc);
	this->addChild(_emitter3SwitchBtn, 2);
	_starMoveLoc->setPosition(288.25f, 204.0f);
	_starMoveLoc->setVisible(false); //隱藏圖案

	//Particle Button
	_bParticleBtnPress = false;

	auto bubblepos = (Sprite *)(rootNode->getChildByName("bubble_btn"));
	loc = bubblepos->getPosition();
	bubblepos->setVisible(false);
	_bubbleSwitchBtn = CSwitchButton::create();
	_bubbleSwitchBtn->setButtonInfo("bubble.png", "bubble.png", "bubble.png", loc);
	this->addChild(_bubbleSwitchBtn, 2);

	auto cloudpos = (Sprite *)(rootNode->getChildByName("cloud_btn"));
	loc = cloudpos->getPosition();
	cloudpos->setVisible(false);
	_cloudSwitchBtn = CSwitchButton::create();
	_cloudSwitchBtn->setButtonInfo("cloud.png", "cloud.png", "cloud.png", loc);
	this->addChild(_cloudSwitchBtn, 2);

	auto cometpos = (Sprite *)(rootNode->getChildByName("comet_btn"));
	loc = cometpos->getPosition();
	cometpos->setVisible(false);
	_cometSwitchBtn = CSwitchButton::create();
	_cometSwitchBtn->setButtonInfo("comet.png", "comet.png", "comet.png", loc);
	this->addChild(_cometSwitchBtn, 2);

	auto flarepos = (Sprite *)(rootNode->getChildByName("flare_btn"));
	loc = flarepos->getPosition();
	flarepos->setVisible(false);
	_flareSwitchBtn = CSwitchButton::create();
	_flareSwitchBtn->setButtonInfo("flare.png", "flare.png", "flare.png", loc);
	this->addChild(_flareSwitchBtn, 2);

	auto sparkpos = (Sprite *)(rootNode->getChildByName("spark_btn"));
	loc = sparkpos->getPosition();
	sparkpos->setVisible(false);
	_sparkSwitchBtn = CSwitchButton::create();
	_sparkSwitchBtn->setButtonInfo("spark.png", "spark.png", "spark.png", loc);
	this->addChild(_sparkSwitchBtn, 2);


	// Particle Control System
	// 最好的方式是，以下的 Slider 根據這裡的設定值，顯示出正確的數值與位置
	_ParticleControl.init(*this);
	_ParticleControl._emitterPt = _EmitterSprite->getLoc();
	_ParticleControl._fDir = 90.0;
	_ParticleControl._iNumParticles = 100;
	_ParticleControl._iGenParticles = 0;
	_ParticleControl._fSpread = 180.0f;
	_ParticleControl._fVelocity = 2.5;	 // 分子的離開速度
	_ParticleControl._fLifeTime = 3.5f;	 // 分子的存活時間
	_ParticleControl._fOpacity = 255.0f;
	_ParticleControl._fSpin = 0;
	_ParticleControl._fGravity = 0;
	_ParticleControl._fElpasedTime = 0;
	_ParticleControl._iRed = 255;		//Colors
	_ParticleControl._iGreen = 255;
	_ParticleControl._iBlue = 255;
	_ParticleControl.setType(STAY_FOR_TWOSECONDS); // 分子運動的型態，預設為 0

	// 分子的可調整參數的捲動軸
	// Slider of Gravity
	auto *GravitySlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Gravity"));
	GravitySlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::GravityEvent, this));
	GravitySlider->setMaxPercent(100); 	// 將 0 到 100 對應到 -10 到 10 之間
	_GravityBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("GravityBMFont");

	// Slider of Spread
	auto *SpreadSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Spread"));
	SpreadSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::SpreadEvent, this));
	SpreadSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 180 之間
	_SpreadBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("SpreadBMFont");

	// Slider of Direction
	auto *DirectionSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Direction"));
	DirectionSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::DirectionEvent, this));
	DirectionSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 360 之間
	_DirectionBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("DirectionBMFont");

	// Slider of Spin
	auto *SpinSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Spin"));
	SpinSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::SpinEvent, this));
	SpinSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 360 之間
	_SpinBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("SpinBMFont");

	// Slider of Opacity
	auto *OpacitySlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Opacity"));
	OpacitySlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::OpacityEvent, this));
	OpacitySlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 255 之間
	_OpacityBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("OpacityBMFont");

	// Slider of Particles
	auto *ParticlesSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Particles"));
	ParticlesSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::ParticlesEvent, this));
	ParticlesSlider->setMaxPercent(100);  	// 將 0 到 100 對應到 0 到 200 之間
	_ParticlesBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("ParticlesBMFont");

	// Slider of Speed
	auto *SpeedSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Speed"));
	SpeedSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::SpeedEvent, this));
	SpeedSlider->setMaxPercent(100);   // 將 0 到 100 對應到 0 到 40 之間
	_SpeedBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("SpeedBMFont");

	// Slider of Lifetime
	auto *LifetimeSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Lifetime"));
	LifetimeSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::LifetimeEvent, this));
	LifetimeSlider->setMaxPercent(100); // 將 0 到 100 對應到 0 到 7 之間
	_LifetimeBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("LifetimeBMFont");

	// Slider of Red
	auto *RedSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Red"));
	RedSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::RedEvent, this));
	RedSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 255 之間
	_RedBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("RedBMFont");

	// Slider of Green
	auto *GreenSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Green"));
	GreenSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::GreenEvent, this));
	GreenSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 255 之間
	_GreenBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("GreenBMFont");

	// Slider of Blue
	auto *BlueSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Blue"));
	BlueSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::BlueEvent, this));
	BlueSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 255 之間
	_BlueBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("BlueBMFont");

	// Slider of WindForce
	auto *WindForceSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_WindForce"));
	WindForceSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::WindForceEvent, this));
	WindForceSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 10 之間
	_WindForceBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("WindForceBMFont");

	// Slider of WindDirection
	auto *WindDirectionSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_WindDirection"));
	WindDirectionSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::WindDirectionEvent, this));
	WindDirectionSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 360 之間
	_WindDirectionBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("WindDirectionBMFont");

	// Slider of Type
	auto *TypeSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Type"));
	TypeSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::TypeEvent, this));
	TypeSlider->setMaxPercent(100);
	_TypeBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("TypeBMFont");

	//------------------------------------------------------------------------------------------
	//firework
	_bFireworkMove = false;
	_fireworkMove = MoveBy::create(0.8f, Point(0, 470.0f)); //移動動畫
	_fireworkMove->retain();
	_fireworkCallback = CallFunc::create(this, callfunc_selector(ParticleSystemScene::FireworkFinished));
	_fireworkCallback->retain();

	//------------------------------------------------------------------------------------------
	//Emitter3(star track)
	_bStarAniFinished = false;
	_bStarAniMove = false;
	//start at Point(288.25f, 204.0f) //Point1
	_StarMove1 = MoveTo::create(1.5f, Point(629.2f, 463.0f));  //Point1 to 2
	_StarMove1->retain();
	_StarMove2 = MoveTo::create(1.5f, Point(207.0f, 463.0f));  //Point2 to 3
	_StarMove2->retain();
	_StarMove3 = MoveTo::create(1.5f, Point(547.0f, 204.0f));  //Point3 to 4
	_StarMove3->retain();
	_StarMove4 = MoveTo::create(1.5f, Point(418.0f, 624.0f));  //Point4 to 5
	_StarMove4->retain();
	_StarMove5 = MoveTo::create(1.5f, Point(288.25f, 204.0f)); //Point5 to 1
	_StarMove5->retain();
	_starMoveCallback = CallFunc::create(this, callfunc_selector(ParticleSystemScene::StarMoveFinished));
	_starMoveCallback->retain();

	//------------------------------------------------------------------------------------------
	_listener1 = EventListenerTouchOneByOne::create();	//創建一個一對一的事件聆聽器
	_listener1->onTouchBegan = CC_CALLBACK_2(ParticleSystemScene::onTouchBegan, this);		//加入觸碰開始事件
	_listener1->onTouchMoved = CC_CALLBACK_2(ParticleSystemScene::onTouchMoved, this);		//加入觸碰移動事件
	_listener1->onTouchEnded = CC_CALLBACK_2(ParticleSystemScene::onTouchEnded, this);		//加入觸碰離開事件

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//加入剛創建的事件聆聽器
	this->schedule(CC_SCHEDULE_SELECTOR(ParticleSystemScene::doStep));

    return true;
}

void ParticleSystemScene::doStep(float dt)
{
	_ParticleControl.doStep(dt);

	int inUsed = _ParticleControl.getInUsedNumber(); //取得目前使用分子數

	//firework
	if (_bFireworkOn) { 
		_ParticleControl._fireworkPt = _fireworkLoc->getPosition(); //傳入煙火中心點位置
		_ParticleControl._bFireworkMove = _bFireworkMove; //傳入煙火是否發射中
	}
	if (_bFireworkOn && !_bFireworkMove && inUsed == 0) { //煙火結束，重新開始
		_fireworkLoc->setPosition(-125.0f + _visibleSize.width / 2.0f, -50.0f); //設定煙火位置
		_ParticleControl._fSpin = 0;
		_fireworkLoc->runAction(Sequence::create(_fireworkMove, _fireworkCallback, NULL)); //執行動畫
		_bFireworkMove = true;
	}
	//-----------------------------------------------------------------------------

	//star
	if (_bEmitter3On) {
		_ParticleControl._starmovePt = _starMoveLoc->getPosition(); //傳入星星位置
		_ParticleControl._bStarAniMove = _bStarAniMove;
	}
	if (_bEmitter3On && inUsed == 0) { //重設 "路徑未完成" 狀態
		_bStarAniFinished = false;
		_ParticleControl._bStarAniFinished = _bStarAniFinished;
		_ParticleControl.setStar(_bStarAniFinished);
	}
	if (!_bEmitter3On && inUsed != 0) { //尚有粒子未消失
		_bStarAniFinished = true;
		_ParticleControl._bStarAniFinished = _bStarAniFinished;
		_ParticleControl.setStar(_bStarAniFinished);
	}
	if (_bEmitter3On && !_bStarAniFinished && !_bStarAniMove && inUsed == 0) { //STAR按鈕開啟，動畫進行中，粒子數0
		_starMoveLoc->setPosition(288.25f, 204.0f); //設定畫星星起點位置 
		_starMoveLoc->runAction(Sequence::create(_StarMove1, _StarMove2, _StarMove3, _StarMove4, _StarMove5, _starMoveCallback, NULL)); //執行路徑動畫
		_bStarAniMove = true;
	}
}

bool ParticleSystemScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//觸碰開始事件
{
	Point touchLoc = pTouch->getLocation();
	int inUsed = _ParticleControl.getInUsedNumber(); //取得目前使用分子數
	if (!_bEmitterOn && !_bEmitter1On && !_bEmitter2On && !_bEmitter3On) { //其他emitter關閉時煙火才可開啟
		if (_fireworkSwitchBtn->touchesBegan(touchLoc)) {
			_bFireworkbtn = true; //點在煙火按鈕上
			if (!_bFireworkOn) {
				_ParticleControl._fSpin = 0;
				_fireworkLoc->runAction(Sequence::create(_fireworkMove, _fireworkCallback, NULL)); //執行動畫
				_bFireworkMove = true;
			}
			else {
				_fireworkLoc->setPosition(-125.0f + _visibleSize.width / 2.0f, -50.0f); //設定煙火位置
				_bFireworkMove = false;
			}
		}
		else _bFireworkbtn = false;
	}
	if (!_bEmitterOn && !_bFireworkOn /*&& !_bEmitter2On*/ && !_bEmitter3On) { //其他按鈕關閉時Emitter1(fire)才可開啟
		if (_emitter1SwitchBtn->touchesBegan(touchLoc)) _bEmitter1btn = true;
		else _bEmitter1btn = false;
	}
	if (!_bEmitterOn && !_bFireworkOn /*&& !_bEmitter1On*/ && !_bEmitter3On) { //其他按鈕關閉時Emitter2(rain)才可開啟
		if (_emitter2SwitchBtn->touchesBegan(touchLoc)) _bEmitter2btn = true;
		else _bEmitter2btn = false;
	}
	if (!_bEmitterOn && !_bFireworkOn && !_bEmitter1On && !_bEmitter2On) { //其他按鈕關閉時Emitter3(star)才可開啟
		if (_emitter3SwitchBtn->touchesBegan(touchLoc)) _bEmitter3btn = true;
		else _bEmitter3btn = false;
	}
	if (!_bFireworkOn && !_bEmitter1On && !_bEmitter2On && !_bEmitter3On) { //其他按鈕關閉時emitter才可開啟
		if (_emitterSwitchBtn->touchesBegan(touchLoc)) _bEmitterbtn = true; //點在emitter按鈕上
		else _bEmitterbtn = false;
	}

	//Particle Button
	if (_bubbleSwitchBtn->touchesBegan(touchLoc)) _bParticleBtnPress = true;
	if (_cloudSwitchBtn->touchesBegan(touchLoc)) _bParticleBtnPress = true;
	if (_cometSwitchBtn->touchesBegan(touchLoc)) _bParticleBtnPress = true;
	if (_flareSwitchBtn->touchesBegan(touchLoc)) _bParticleBtnPress = true;
	if (_sparkSwitchBtn->touchesBegan(touchLoc)) _bParticleBtnPress = true;

	//顯示 Emitter 時，可拖曳該圖式
	if( _bEmitterOn ) _EmitterSprite->touchesBegan(touchLoc);

	// 沒有顯示 Emitter，而且沒有按在 Emitter 切換按鈕上，才讓 touch 可以點選顯示分子 && 沒有按在Particle紐上 && 沒有煙火
	if ( !_bEmitterbtn && !_bEmitterOn && !_bParticleBtnPress && !_bFireworkbtn && !_bFireworkOn && 
		 !_bEmitter1btn && !_bEmitter1On && !_bEmitter2btn && !_bEmitter2On && !_bEmitter3btn && !_bEmitter3On)
		_ParticleControl.onTouchesBegan(touchLoc);
	return true;
}

void  ParticleSystemScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰移動事件
{
	Point touchLoc = pTouch->getLocation();

	if (!_bEmitterOn && !_bEmitter1On && !_bEmitter2On && !_bEmitter3On) { //其他emitter關閉時煙火才可開啟
		if (_fireworkSwitchBtn->touchesMoved(touchLoc)) _bFireworkbtn = true; //點在煙火按鈕上
		else _bFireworkbtn = false;
	}
	if (!_bEmitterOn && !_bFireworkOn /*&& !_bEmitter2On*/ && !_bEmitter3On) { //其他按鈕關閉時Emitter1(fire)才可開啟
		if (_emitter1SwitchBtn->touchesMoved(touchLoc)) _bEmitter1btn = true;
		else _bEmitter1btn = false;
	}
	if (!_bEmitterOn && !_bFireworkOn /*&& !_bEmitter1On*/ && !_bEmitter3On) { //其他按鈕關閉時Emitter2(rain)才可開啟
		if (_emitter2SwitchBtn->touchesMoved(touchLoc))_bEmitter2btn = true;
		else _bEmitter2btn = false;
	}
	if (!_bEmitterOn && !_bFireworkOn && !_bEmitter1On && !_bEmitter2On) { //其他按鈕關閉時Emitter3(star)才可開啟
		if (_emitter3SwitchBtn->touchesMoved(touchLoc)) _bEmitter3btn = true;
		else _bEmitter3btn = false;
	}
	if (!_bFireworkOn && !_bEmitter1On && !_bEmitter2On && !_bEmitter3On) { //其他按鈕關閉時emitter才可開啟
		if (_emitterSwitchBtn->touchesMoved(touchLoc)) _bEmitterbtn = true; //點在emitter按鈕上
		else _bEmitterbtn = false;
	}

	//Particle Button
	_bubbleSwitchBtn->touchesMoved(touchLoc);
	_cloudSwitchBtn->touchesMoved(touchLoc);
	_cometSwitchBtn->touchesMoved(touchLoc);
	_flareSwitchBtn->touchesMoved(touchLoc);
	_sparkSwitchBtn->touchesMoved(touchLoc);

	if (_bEmitterOn) {
		if (_EmitterSprite->touchesMoved(touchLoc)) // 移動並更新 Emitter 的位置
			_ParticleControl._emitterPt = _EmitterSprite->getLoc();
	}
	// 沒有顯示 Emitter，而且沒有按在 Emitter 切換按鈕上，才讓 touch 可以點選顯示分子
	if ( !_bEmitterbtn && !_bEmitterOn && !_bFireworkbtn && !_bFireworkOn &&
		 !_bEmitter1btn && !_bEmitter1On && !_bEmitter2btn && !_bEmitter2On && !_bEmitter3btn && !_bEmitter3On)
		_ParticleControl.onTouchesMoved(touchLoc);
}

void  ParticleSystemScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰結束事件 
{
	Point touchLoc = pTouch->getLocation();

	//Particle Button
	_bParticleBtnPress = false;
	if (_bubbleSwitchBtn->touchesEnded(touchLoc)) _ParticleControl._cParticleImage = "bubble.png";
	if (_cloudSwitchBtn->touchesEnded(touchLoc)) _ParticleControl._cParticleImage = "cloud.png";
	if (_cometSwitchBtn->touchesEnded(touchLoc)) _ParticleControl._cParticleImage = "comet.png";
	if (_flareSwitchBtn->touchesEnded(touchLoc)) _ParticleControl._cParticleImage = "flare.png";
	if (_sparkSwitchBtn->touchesEnded(touchLoc)) _ParticleControl._cParticleImage = "spark.png";

	if (_bEmitterOn) {
		if( _EmitterSprite->touchesEnded(touchLoc) ) 
			_ParticleControl._emitterPt = _EmitterSprite->getLoc();
	}
	// 點在 Emitter 切換的圖示上，進行必要的狀態改變
	if (_emitterSwitchBtn->touchesEnded(touchLoc))
	{
		_bEmitterOn = _emitterSwitchBtn->getStatus();
		if ( _bEmitterOn ) { // 顯示 Emitter 圖示
			_EmitterSprite->setVisible(true);			
		}
		else { // 關閉 Emitter 圖示
			_EmitterSprite->setVisible(false);
		}
		_ParticleControl.setEmitter(_bEmitterOn); // 更新控制系統中的 Emitter 狀態
	}

	//firework btn
	if (_fireworkSwitchBtn->touchesEnded(touchLoc))
	{
		_bFireworkOn = _fireworkSwitchBtn->getStatus();
		_ParticleControl.setFirework(_bFireworkOn); // 更新控制系統中的煙火狀態
	}

	//Different Emitter Buttons
	if (_emitter1SwitchBtn->touchesEnded(touchLoc))
	{
		_bEmitter1On = _emitter1SwitchBtn->getStatus();
		_ParticleControl._iNumParticles = 70;
		_ParticleControl.setEmitter1(_bEmitter1On);
	}
	if (_emitter2SwitchBtn->touchesEnded(touchLoc))
	{
		_bEmitter2On = _emitter2SwitchBtn->getStatus();
		_ParticleControl.setEmitter2(_bEmitter2On);
	}
	if (_emitter3SwitchBtn->touchesEnded(touchLoc))
	{
		_bEmitter3On = _emitter3SwitchBtn->getStatus();
		_ParticleControl._iNumParticles = 20;
		_ParticleControl.setEmitter3(_bEmitter3On);
		if (_bEmitter3On) {
			_starMoveLoc->runAction(Sequence::create(_StarMove1, _StarMove2, _StarMove3, _StarMove4, _StarMove5, _starMoveCallback, NULL)); //執行路徑動畫
			_bStarAniMove = true;
		}
		else {
			_starMoveLoc->stopAllActions();
			_starMoveLoc->setPosition(288.25f, 204.0f); //設定畫星星起點位置
			_bStarAniMove = false;
		}
	}
	
}

void ParticleSystemScene::GravityEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		float fGravity = (-50.0f + percent) / 5.0f;
		_GravityBMValue->setString(StringUtils::format("%2.1f", fGravity));
		_ParticleControl.setGravity(fGravity);
	}
}

void ParticleSystemScene::SpreadEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fSpread = percent*1.8f; // 0 到 180 之間
		_SpreadBMValue->setString(StringUtils::format("%2.1f", fSpread));
		_ParticleControl._fSpread = fSpread;
	}
}

void ParticleSystemScene::DirectionEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fDir = percent*3.6f; // 0 到 360 之間
		_DirectionBMValue->setString(StringUtils::format("%2.1f", fDir));
		_ParticleControl._fDir = fDir;
	}
}

void ParticleSystemScene::SpinEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type) {
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent(); // 捲動鈕目前的位置 0 ~ 100
		float fSpin = percent*3.6f;		// 0 到 360 之間
		_SpinBMValue->setString(StringUtils::format("%3.0f", fSpin));
		_ParticleControl._fSpin = fSpin;
	}
}

void ParticleSystemScene::OpacityEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type) {
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent(); // 捲動鈕目前的位置 0 ~ 100
		float fOpacity = percent*2.55f;		// 將 0 到 100 對應到 0 到 255 之間
		_OpacityBMValue->setString(StringUtils::format("%3.0f", fOpacity));
		_ParticleControl._fOpacity = fOpacity;
	}
}

void ParticleSystemScene::ParticlesEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type) {
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent(); // 捲動鈕目前的位置 0 ~ 100
		int iParticles = percent*2;		// 0 到 200
		_ParticlesBMValue->setString(StringUtils::format("%3d", iParticles));
		_ParticleControl._iNumParticles = iParticles;
	}
}

void ParticleSystemScene::SpeedEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type) {
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent(); // 捲動鈕目前的位置 0 ~ 100									
		float fSpeed = percent / 2.5f; // 將 0 到 100 對應到 0 到 40 之間
		_SpeedBMValue->setString(StringUtils::format("%2.1f", fSpeed));
		_ParticleControl._fVelocity = fSpeed;
	}
}

void ParticleSystemScene::LifetimeEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type) {
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent(); // 捲動鈕目前的位置 0 ~ 100									
		float fLifetime = percent*0.07f; // 將 0 到 100 對應到 0 到 7 之間
		_LifetimeBMValue->setString(StringUtils::format("%1.1f", fLifetime));
		_ParticleControl._fLifeTime = fLifetime;
	}
}

void ParticleSystemScene::RedEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type) {
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent(); // 捲動鈕目前的位置 0 ~ 100									
		int iRed = percent*2.55f; // 將 0 到 100 對應到 0 到 255 之間
		_RedBMValue->setString(StringUtils::format("%3d", iRed));
		_ParticleControl.setRed(iRed);
	}
}

void ParticleSystemScene::GreenEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type) {
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent(); // 捲動鈕目前的位置 0 ~ 100									
		int iGreen = percent*2.55f; // 將 0 到 100 對應到 0 到 255 之間
		_GreenBMValue->setString(StringUtils::format("%3d", iGreen));
		_ParticleControl.setGreen(iGreen);
	}
}

void ParticleSystemScene::BlueEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type) {
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent(); // 捲動鈕目前的位置 0 ~ 100									
		int iBlue = percent*2.55f; // 將 0 到 100 對應到 0 到 255 之間
		_BlueBMValue->setString(StringUtils::format("%3d", iBlue));
		_ParticleControl.setBlue(iBlue);
	}
}

void ParticleSystemScene::WindForceEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		float fWindForce = percent / 10.0f;
		_WindForceBMValue->setString(StringUtils::format("%2.1f", fWindForce));
		_ParticleControl.setWindForce(fWindForce);
	}
}

void ParticleSystemScene::WindDirectionEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fWindDir = percent*3.6f; // 0 到 360 之間
		_WindDirectionBMValue->setString(StringUtils::format("%2.1f", fWindDir));
		_ParticleControl.setWindDirection(fWindDir);
	}
}

void ParticleSystemScene::TypeEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		int iType = percent * 0.08f; // 0 到 8 之間
		_TypeBMValue->setString(StringUtils::format("%2d", iType));
		_ParticleControl.setType(iType);
	}
}

//----------------------------------------------------------------------------------------

void ParticleSystemScene::FireworkFinished()
{
	_bFireworkMove = false;
	if(_bFireworkOn) _ParticleControl.fireworkExplotion(); //成功到爆炸點，設定粒子爆炸資訊
	else _fireworkLoc->setPosition(-125.0f + _visibleSize.width / 2.0f, -50.0f); //未爆炸前關閉按鈕，重設煙火位置
}

void ParticleSystemScene::StarMoveFinished()
{
	_bStarAniFinished = true;
	_ParticleControl._bStarAniFinished = _bStarAniFinished; //星星路徑結束
	_ParticleControl.setStar(_bStarAniFinished);
	_bStarAniMove = false;
}