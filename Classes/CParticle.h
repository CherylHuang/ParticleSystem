#ifndef __CPARTICLE_H__
#define __CPARTICLE_H__

#define STAY_FOR_TWOSECONDS 0	// 在手指滑過的地方產生 particle
#define RANDOMS_FALLING		1	// 在手指滑過的地方往下產生 particle
#define FREE_FLY 2				// 在手指滑過的地方任意向四方產生 particle
#define EXPLOSION 3				// 噴射出圓形的煙火
#define HEARTSHAPE 4			// 噴射出心型的煙火
#define BUTTERFLYSHAPE 5		// 噴射出蝴蝶外型的煙火
#define ROSESHAPE 6				// 第一層花
#define ROSESHAPE_2 66			// 第二層花
#define SPIRALSHAPE 7			// 螺旋圖案
#define CRACKER 8				// 拉炮

#define FIREWORK_TAIL 98		//煙火發射
#define FIREWORK_EXPLOSION 99	//煙火爆炸
#define EMITTER_DEFAULT 100		//一般連續噴射particle
#define EMITTER_FIRE 101		//火
#define EMITTER_RAIN 102		//雨
#define EMITTER_DRAWSTAR 103	//畫出星星

#include "cocos2d.h"

class CParticle
{
private:
	cocos2d::Sprite *_Particle;		// 分子本體
	cocos2d::Point  _OldPos;		// 分子前一個位置
	cocos2d::Point  _Pos;			// 分子目前的位置
	cocos2d::Point  _Direction;		// 分子目前的運動方向，單位向量
	cocos2d::Point  _WindDir;		// 風力方向

	float _fVelocity;	// 分子的速度
	float _fLifeTime;	// 生命週期
	float _fIntensity;	// 分子能量的最大值，通常用於改變分子的顏色
	float _fOpacity;	// 目前分子的透明度
	float _fSpin;		// 分子的旋轉量
	float _fdegree;		//-----旋轉角
	float _fSize;		// 分子的大小
	float _fGravity;	// 分子的引力方向
	float _fWindForce;	// 風力大小
	float _fWindDir;	// 風力方向
	cocos2d::Color3B _color;	// 分子的顏色
	char  *_cParticleImage;	//分子圖片
	int	  _iRed;
	int	  _iGreen;
	int	  _iBlue;

	int _iType;		// 行為模式
	bool _bVisible; // 顯示與否

	int	_iTag; //標籤

	// 時間
	float _fElapsedTime; // 分子從顯示到目前為止的時間
	float _fDelayTime;	 // 分子顯示前的延遲時間

	//spiral
	float	_ftouchX;			// 記錄觸碰點
	float	_ftouchY;
	float	_ftheta;			//轉動角度
	//cracker
	float	_fCrackerCount;		//沒有重力&風力時 記時
	int		_iCrackerRand;		//隨機(風)

	//fire
	int		_randNum;			//隨機數
	float	_fCloudTime;		//煙霧時間
	//rain
	float	_fRainAngle;		//雨的受風角度
	float	_fRippleCount;		//漣漪計時
	float	_fRippleLifeTime;	//漣漪生命週期
	float	_fRippleOpacity;	//漣漪透明度
	//star
	bool	_bStarAniFinished;	//動畫結束
	bool	_bEmitter3On;		//星星按鈕按下

	bool	_bEmitter1On;		//火焰按鈕按下
	bool	_bEmitter2On;		//雨按鈕按下
	
public:
	CParticle();
	void setParticle(const char *pngName, cocos2d::Layer &inlayer);
	void setParticleImage(const char *pngName);
	char* getParticleImg();

	bool doStep(float dt);
	void setPosition(const cocos2d::Point &inPos);
	//void setColor(cocos2d::Color3B &color) { _color = color;}
	void setBehavior(int iType); // 設定分子產生的起始行為模式
	void setVisible();
	void setGravity(const float fGravity);
	void setWindForce(const float fWindForce);
	void setWindDirNum(const float fWindDir) { _fWindDir = fWindDir; }
	void setSpin(const float fSpin);
	void setRotation(const float fRo);
	void setOpacity(const float fOpacity) { _fOpacity = fOpacity; }
	void setVelocity(const float v) { _fVelocity = v; }
	void setLifetime(const float lt);
	void setDirection(const cocos2d::Point pt) { _Direction = pt; }
	void setSize(float s) { _fSize = s; _Particle->setScale(_fSize); }

	//標籤
	void setTag(const int itag);
	int getTag();
	cocos2d::Rect _rect;

	//set Color
	void setRed(const int iRed);
	void setGreen(const int iGreen);
	void setBlue(const int iBlue);

	//Emitter3
	void setStar(const bool bStar);
	void setEmitter3Btn(const bool bBtn);

	void setEmitter1Btn(const bool bBtn);
	void setEmitter2Btn(const bool bBtn);
};

#endif