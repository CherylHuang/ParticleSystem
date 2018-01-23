#ifndef __CPARTICLE_SYSTEM_H__
#define __CPARTICLE_SYSTEM_H__

#include "cocos2d.h"
#include "CParticle.h"
#include <list>

using namespace std;


class CParticleSystem
{
private:
	CParticle* _pParticles;
	list<CParticle*> _FreeList;
	list<CParticle*> _InUsedList;
	int _iFree;					// 可用的  Particle 個數
	int _iInUsed;				// 正在使用的 Particle 個數
	int _iType;					// 目前控制的分子運動模式是哪一種型態

	bool _bEmitterOn;			// Emitter 是否顯示
	bool _bFireworkOn;			//是否開啟煙火
	bool _bEmitter1On;
	bool _bEmitter2On;
	bool _bEmitter3On;

	//Type 6 rose
	cocos2d::Point _touchPt;	// 點擊位置
	bool _bRoseExplode;			//第一朵花是否爆炸
	float _fCount;				//計時
	void rose2Explotion();		//花爆炸設定

	//Type 8 cracker
	cocos2d::Point _explodePt;	// 拉炮爆炸位置

public:
	// Emitter 設定的相關參數
	cocos2d::Point _emitterPt;	// Emitter 位置
	cocos2d::Point _fireworkPt; // firework 位置
	cocos2d::Point _starmovePt;	// 星星移動位置 
	float	_fWindDir;			//風力方向(角度)
	float   _fWindForce;		//風力大小
	float   _fDir;				// Emitter 的噴射方向，0 到 360度，逆時針方向為正
	int		_iNumParticles;		// 每秒鐘產生的分子個數
	int     _iGenParticles;		// 到目前為止所產生的分子個數(以每秒為單位)
	float	_fOpacity;			// 0~255分子透明度
	float	_fSpread;			// 0 到 180 度，預設為 180 
	float   _fVelocity;			// 分子的離開速度
	float   _fLifeTime;			// 分子的存活時間
	float	_fSpin;				// 分子的旋轉量 degree/sec
	float   _fGravity;
	float   _fElpasedTime;		// 經過多少時間，以秒為單位，作為成生分子的計算依據
	char	*_cParticleImage;   //分子圖片

	//Color
	int		_iRed;
	int		_iGreen;
	int		_iBlue;

	//firework
	bool _bFireworkMove;
	void fireworkExplotion(); //煙火爆炸設定

	//Emitter3
	bool _bStarAniFinished;	//星星動畫結束
	bool _bStarAniMove;

	CParticleSystem();
	~CParticleSystem();
	void init(cocos2d::Layer &inlayer);
	void doStep(float dt);
	void setEmitter(bool bEm);
	void setFirework(bool bFirework); //煙火
	void setEmitter1(bool bEm); //Other emitters
	void setEmitter2(bool bEm);
	void setEmitter3(bool bEm);
	void setType(int type) { _iType = type; }

 // void onTouchesEnded(const cocos2d::CCPoint &touchPoint);
    void onTouchesBegan(const cocos2d::CCPoint &touchPoint);
    void onTouchesMoved(const cocos2d::CCPoint &touchPoint);

	void setGravity(float fGravity);
	void setWindForce(float fWindForce);
	void setWindDirection(float fWindDir);
	void setRed(int iRed);
	void setGreen(int iGreen);
	void setBlue(int iBlue);

	void setStar(bool bAniFinished);

	int getInUsedNumber();
};

#endif