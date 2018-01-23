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
	int _iFree;					// �i�Ϊ�  Particle �Ӽ�
	int _iInUsed;				// ���b�ϥΪ� Particle �Ӽ�
	int _iType;					// �ثe������l�B�ʼҦ��O���@�ث��A

	bool _bEmitterOn;			// Emitter �O�_���
	bool _bFireworkOn;			//�O�_�}�ҷϤ�
	bool _bEmitter1On;
	bool _bEmitter2On;
	bool _bEmitter3On;

	//Type 6 rose
	cocos2d::Point _touchPt;	// �I����m
	bool _bRoseExplode;			//�Ĥ@����O�_�z��
	float _fCount;				//�p��
	void rose2Explotion();		//���z���]�w

	//Type 8 cracker
	cocos2d::Point _explodePt;	// �Ԭ��z����m

public:
	// Emitter �]�w�������Ѽ�
	cocos2d::Point _emitterPt;	// Emitter ��m
	cocos2d::Point _fireworkPt; // firework ��m
	cocos2d::Point _starmovePt;	// �P�P���ʦ�m 
	float	_fWindDir;			//���O��V(����)
	float   _fWindForce;		//���O�j�p
	float   _fDir;				// Emitter ���Q�g��V�A0 �� 360�סA�f�ɰw��V����
	int		_iNumParticles;		// �C�������ͪ����l�Ӽ�
	int     _iGenParticles;		// ��ثe����Ҳ��ͪ����l�Ӽ�(�H�C�����)
	float	_fOpacity;			// 0~255���l�z����
	float	_fSpread;			// 0 �� 180 �סA�w�]�� 180 
	float   _fVelocity;			// ���l�����}�t��
	float   _fLifeTime;			// ���l���s���ɶ�
	float	_fSpin;				// ���l������q degree/sec
	float   _fGravity;
	float   _fElpasedTime;		// �g�L�h�֮ɶ��A�H�����A�@�����ͤ��l���p��̾�
	char	*_cParticleImage;   //���l�Ϥ�

	//Color
	int		_iRed;
	int		_iGreen;
	int		_iBlue;

	//firework
	bool _bFireworkMove;
	void fireworkExplotion(); //�Ϥ��z���]�w

	//Emitter3
	bool _bStarAniFinished;	//�P�P�ʵe����
	bool _bStarAniMove;

	CParticleSystem();
	~CParticleSystem();
	void init(cocos2d::Layer &inlayer);
	void doStep(float dt);
	void setEmitter(bool bEm);
	void setFirework(bool bFirework); //�Ϥ�
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