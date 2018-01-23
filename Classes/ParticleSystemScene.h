#ifndef __PARTICLESYSTEM_SCENE_H__
#define __PARTICLESYSTEM_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "cparticlesystem.h"
#include "Common/CSwitchButton.h"
#include  "Common/CDraggableSprite.h"


class ParticleSystemScene : public cocos2d::Layer
{
private:
	cocos2d::Size _visibleSize;

	// 分子的可提整不同參數的顯示
	cocos2d::ui::TextBMFont *_GravityBMValue;
	cocos2d::ui::TextBMFont *_SpreadBMValue;
	cocos2d::ui::TextBMFont *_DirectionBMValue;
	cocos2d::ui::TextBMFont *_SpinBMValue;
	cocos2d::ui::TextBMFont *_OpacityBMValue;
	cocos2d::ui::TextBMFont *_ParticlesBMValue;
	cocos2d::ui::TextBMFont *_SpeedBMValue;
	cocos2d::ui::TextBMFont *_LifetimeBMValue;
	cocos2d::ui::TextBMFont *_RedBMValue;
	cocos2d::ui::TextBMFont *_GreenBMValue;
	cocos2d::ui::TextBMFont *_BlueBMValue;
	cocos2d::ui::TextBMFont *_WindForceBMValue;
	cocos2d::ui::TextBMFont *_WindDirectionBMValue;


	// 選擇不同的分子型態，用在 Emitter 沒有開啟的狀態
	cocos2d::ui::TextBMFont *_TypeBMValue;

	// For Emitter
	CDraggableSprite *_EmitterSprite;
	bool _bEmitterbtn; //是否點在按鈕上
	bool _bEmitterOn;
	CSwitchButton *_emitterSwitchBtn;

	//Firework Button
	bool _bFireworkbtn; //是否點在按鈕上
	bool _bFireworkOn;
	CSwitchButton *_fireworkSwitchBtn;
	//firework
	bool _bFireworkMove; //煙火是否移動中
	cocos2d::Sprite	  *_fireworkLoc;
	cocos2d::MoveBy   *_fireworkMove;
	cocos2d::CallFunc *_fireworkCallback;

	//Different Emitter Buttons
	bool _bEmitter1btn; //是否點在按鈕上
	bool _bEmitter2btn; //是否點在按鈕上
	bool _bEmitter3btn; //是否點在按鈕上
	bool _bEmitter1On;
	bool _bEmitter2On;
	bool _bEmitter3On;
	CSwitchButton *_emitter1SwitchBtn; //fire
	CSwitchButton *_emitter2SwitchBtn;
	CSwitchButton *_emitter3SwitchBtn; //star track
	//Emitter3 star track
	bool _bStarAniFinished; //動畫結束
	bool _bStarAniMove; //星星是否移動中
	cocos2d::CallFunc *_starMoveCallback;
	cocos2d::Sprite	  *_starMoveLoc;
	cocos2d::MoveTo   *_StarMove1; //從第1點移動
	cocos2d::MoveTo   *_StarMove2; //從第2點移動
	cocos2d::MoveTo   *_StarMove3; //從第3點移動
	cocos2d::MoveTo   *_StarMove4; //從第4點移動
	cocos2d::MoveTo   *_StarMove5; //從第5點移動
	
	//Particle Button
	bool _bParticleBtnPress; //有無按在粒子按鈕上
	CSwitchButton *_bubbleSwitchBtn;
	CSwitchButton *_cloudSwitchBtn;
	CSwitchButton *_cometSwitchBtn;
	CSwitchButton *_flareSwitchBtn;
	CSwitchButton *_sparkSwitchBtn;

public:

	CParticleSystem _ParticleControl;

	~ParticleSystemScene();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

	void doStep(float dt);

	cocos2d::EventListenerTouchOneByOne *_listener1;
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰開始事件
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰移動事件
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //觸碰結束事件

	void FireworkFinished(); // 煙火 的 CallBack 函式
	void StarMoveFinished(); // 星星 的 CallBack 函式

	void GravityEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
	void SpreadEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
	void DirectionEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
	void SpinEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
	void OpacityEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
	void ParticlesEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
	void SpeedEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
	void LifetimeEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
	void RedEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
	void GreenEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
	void BlueEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
	void WindForceEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
	void WindDirectionEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
	void TypeEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);

    // implement the "static create()" method manually
    CREATE_FUNC(ParticleSystemScene);
};

#endif // __MAIN_SCENE_H__
