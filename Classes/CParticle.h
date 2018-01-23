#ifndef __CPARTICLE_H__
#define __CPARTICLE_H__

#define STAY_FOR_TWOSECONDS 0	// �b����ƹL���a�貣�� particle
#define RANDOMS_FALLING		1	// �b����ƹL���a�詹�U���� particle
#define FREE_FLY 2				// �b����ƹL���a����N�V�|�貣�� particle
#define EXPLOSION 3				// �Q�g�X��Ϊ��Ϥ�
#define HEARTSHAPE 4			// �Q�g�X�߫����Ϥ�
#define BUTTERFLYSHAPE 5		// �Q�g�X�����~�����Ϥ�
#define ROSESHAPE 6				// �Ĥ@�h��
#define ROSESHAPE_2 66			// �ĤG�h��
#define SPIRALSHAPE 7			// ���۹Ϯ�
#define CRACKER 8				// �Ԭ�

#define FIREWORK_TAIL 98		//�Ϥ��o�g
#define FIREWORK_EXPLOSION 99	//�Ϥ��z��
#define EMITTER_DEFAULT 100		//�@��s��Q�gparticle
#define EMITTER_FIRE 101		//��
#define EMITTER_RAIN 102		//�B
#define EMITTER_DRAWSTAR 103	//�e�X�P�P

#include "cocos2d.h"

class CParticle
{
private:
	cocos2d::Sprite *_Particle;		// ���l����
	cocos2d::Point  _OldPos;		// ���l�e�@�Ӧ�m
	cocos2d::Point  _Pos;			// ���l�ثe����m
	cocos2d::Point  _Direction;		// ���l�ثe���B�ʤ�V�A���V�q
	cocos2d::Point  _WindDir;		// ���O��V

	float _fVelocity;	// ���l���t��
	float _fLifeTime;	// �ͩR�g��
	float _fIntensity;	// ���l��q���̤j�ȡA�q�`�Ω���ܤ��l���C��
	float _fOpacity;	// �ثe���l���z����
	float _fSpin;		// ���l������q
	float _fdegree;		//-----���ਤ
	float _fSize;		// ���l���j�p
	float _fGravity;	// ���l���ޤO��V
	float _fWindForce;	// ���O�j�p
	float _fWindDir;	// ���O��V
	cocos2d::Color3B _color;	// ���l���C��
	char  *_cParticleImage;	//���l�Ϥ�
	int	  _iRed;
	int	  _iGreen;
	int	  _iBlue;

	int _iType;		// �欰�Ҧ�
	bool _bVisible; // ��ܻP�_

	int	_iTag; //����

	// �ɶ�
	float _fElapsedTime; // ���l�q��ܨ�ثe����ɶ�
	float _fDelayTime;	 // ���l��ܫe������ɶ�

	//spiral
	float	_ftouchX;			// �O��Ĳ�I�I
	float	_ftouchY;
	float	_ftheta;			//��ʨ���
	//cracker
	float	_fCrackerCount;		//�S�����O&���O�� �O��
	int		_iCrackerRand;		//�H��(��)

	//fire
	int		_randNum;			//�H����
	float	_fCloudTime;		//�����ɶ�
	//rain
	float	_fRainAngle;		//�B����������
	float	_fRippleCount;		//�����p��
	float	_fRippleLifeTime;	//�����ͩR�g��
	float	_fRippleOpacity;	//�����z����
	//star
	bool	_bStarAniFinished;	//�ʵe����
	bool	_bEmitter3On;		//�P�P���s���U

	bool	_bEmitter1On;		//���K���s���U
	bool	_bEmitter2On;		//�B���s���U
	
public:
	CParticle();
	void setParticle(const char *pngName, cocos2d::Layer &inlayer);
	void setParticleImage(const char *pngName);
	char* getParticleImg();

	bool doStep(float dt);
	void setPosition(const cocos2d::Point &inPos);
	//void setColor(cocos2d::Color3B &color) { _color = color;}
	void setBehavior(int iType); // �]�w���l���ͪ��_�l�欰�Ҧ�
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

	//����
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