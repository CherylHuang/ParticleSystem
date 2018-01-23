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

	// ���l���i���㤣�P�Ѽƪ����
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


	// ��ܤ��P�����l���A�A�Φb Emitter �S���}�Ҫ����A
	cocos2d::ui::TextBMFont *_TypeBMValue;

	// For Emitter
	CDraggableSprite *_EmitterSprite;
	bool _bEmitterbtn; //�O�_�I�b���s�W
	bool _bEmitterOn;
	CSwitchButton *_emitterSwitchBtn;

	//Firework Button
	bool _bFireworkbtn; //�O�_�I�b���s�W
	bool _bFireworkOn;
	CSwitchButton *_fireworkSwitchBtn;
	//firework
	bool _bFireworkMove; //�Ϥ��O�_���ʤ�
	cocos2d::Sprite	  *_fireworkLoc;
	cocos2d::MoveBy   *_fireworkMove;
	cocos2d::CallFunc *_fireworkCallback;

	//Different Emitter Buttons
	bool _bEmitter1btn; //�O�_�I�b���s�W
	bool _bEmitter2btn; //�O�_�I�b���s�W
	bool _bEmitter3btn; //�O�_�I�b���s�W
	bool _bEmitter1On;
	bool _bEmitter2On;
	bool _bEmitter3On;
	CSwitchButton *_emitter1SwitchBtn; //fire
	CSwitchButton *_emitter2SwitchBtn;
	CSwitchButton *_emitter3SwitchBtn; //star track
	//Emitter3 star track
	bool _bStarAniFinished; //�ʵe����
	bool _bStarAniMove; //�P�P�O�_���ʤ�
	cocos2d::CallFunc *_starMoveCallback;
	cocos2d::Sprite	  *_starMoveLoc;
	cocos2d::MoveTo   *_StarMove1; //�q��1�I����
	cocos2d::MoveTo   *_StarMove2; //�q��2�I����
	cocos2d::MoveTo   *_StarMove3; //�q��3�I����
	cocos2d::MoveTo   *_StarMove4; //�q��4�I����
	cocos2d::MoveTo   *_StarMove5; //�q��5�I����
	
	//Particle Button
	bool _bParticleBtnPress; //���L���b�ɤl���s�W
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
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I�}�l�ƥ�
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I���ʨƥ�
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent); //Ĳ�I�����ƥ�

	void FireworkFinished(); // �Ϥ� �� CallBack �禡
	void StarMoveFinished(); // �P�P �� CallBack �禡

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
