#include "CParticleSystem.h"

#define NUMBER_PARTICLES 1000 // �w�]�@�����o 1000 �� Particles

USING_NS_CC;

CParticleSystem::CParticleSystem()
{
	_fGravity = 0;
	_bEmitterOn = false;
}

CParticleSystem::~CParticleSystem()
{
	if (_iInUsed != 0) _InUsedList.clear(); // �����Ҧ��� NODE
	if (_iFree != 0) _FreeList.clear();
	delete[] _pParticles; // ����Ҧ����o�귽
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("particletexture.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("particle_add.plist");
}

void CParticleSystem::init(cocos2d::Layer &inlayer)
{
	_cParticleImage = "flare.png"; //���l�Ϥ�
	_iFree = NUMBER_PARTICLES;
	_iInUsed = 0;
	_bRoseExplode = false;
	_pParticles = new CParticle[NUMBER_PARTICLES]; // ���o�һݭn�� particle �Ŷ�
	 // Ū�J�x�s�h�i�Ϥ��� plist ��
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("particletexture.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("particle_add.plist");
	for (int i = 0; i < NUMBER_PARTICLES; i++) {
		_pParticles[i].setParticle(_cParticleImage, inlayer);
		_FreeList.push_front(&_pParticles[i]); 
	}
}

void CParticleSystem::doStep(float dt)
{
	CParticle *get, *get2;
	list <CParticle *>::iterator it, it2;
	//-----------------------------------------------------------------------------------
	//DEFAULT
	if (_bEmitterOn) { // �ھ� Emitter �]�w�������ѼơA���ͬ۹��������l
		// ���p��b�֥[
		int n = (int)(_fElpasedTime * _iNumParticles); // ��ثe�������Ӳ��ͪ����l�Ӽ�
		if (n > _iGenParticles) {  // ���ͪ����l�ӼƤ����A���ͨ� n �Ӥ��l
			for (int i = 0; i < n - _iGenParticles; i++) {
				// �ھ� Emitter �������ѼơA�]�w�Ҳ��ͤ��l���Ѽ�
				if (_iFree != 0) {
					get = _FreeList.front();
					get->setRed(_iRed);				//�]�w�T��
					get->setGreen(_iGreen);
					get->setBlue(_iBlue);
					get->setBehavior(EMITTER_DEFAULT);
					get->setVelocity(_fVelocity);
					get->setOpacity(_fOpacity);
					get->setSpin(_fSpin);
					get->setLifetime(_fLifeTime);
					get->setGravity(_fGravity);
					get->setPosition(_emitterPt);
					get->setSize(0.125f);
					get->setParticleImage(_cParticleImage); //�]�w�ɤl�Ϯ�
					// �ھ� _fSpread �P _vDir ���ͤ�V
					float t = (rand() % 1001) / 1000.0f; // ���ͤ��� 0 �� 1 ������
					t = _fSpread - t * _fSpread * 2; //  ���ͪ����סA�ন����
					t = ( _fDir + t )* M_PI / 180.0f;
					Vec2 vdir(cosf(t), sinf(t)); // �N���l����V�q�����ন���V�q (cos,sin)
					get->setDirection(vdir);
					get->setWindForce(_fWindForce);
					get->setWindDirNum(_fWindDir);
					_FreeList.pop_front();
					_InUsedList.push_front(get);
					_iFree--; _iInUsed++;
				}
			}
			_iGenParticles = n; // �ثe�w�g���� n �Ӥ��l
			
		}
		if (_fElpasedTime >= 1.0f) {
			_fElpasedTime -= 1.0f;
			if (_iGenParticles >= _iNumParticles) _iGenParticles -= _iNumParticles;
			else _iGenParticles = 0;
		}
		_fElpasedTime += dt;
	}
	//----------------------------------------------------------------------------------
	// ���K
	if (_bEmitter1On) {
		int n = (int)(_fElpasedTime * _iNumParticles); // ��ثe�������Ӳ��ͪ����l�Ӽ�
		if (n > _iGenParticles) {  // ���ͪ����l�ӼƤ����A���ͨ� n �Ӥ��l
			for (int i = 0; i < n - _iGenParticles; i++) {
				if (_iFree != 0) {
					get2 = _FreeList.front();
					get2->setBehavior(EMITTER_FIRE);
					get2->setRotation(0);
					get2->setGravity(_fGravity);
					get2->setWindForce(_fWindForce);
					get2->setTag(0);
															// �ھ� _fSpread �P _vDir ���ͤ�V
					float t = (rand() % 1001) / 1000.0f; // ���ͤ��� 0 �� 1 ������
					//t = _fSpread - t * _fSpread * 2; //  ���ͪ����סA�ন����
					t = (90.0f + t)* M_PI / 180.0f;
					Vec2 vdir(cosf(t), sinf(t)); // �N���l����V�q�����ন���V�q (cos,sin)
					get2->setDirection(vdir);
					get2->setWindDirNum(_fWindDir);
					_FreeList.pop_front();
					_InUsedList.push_front(get2);
					_iFree--; _iInUsed++;
				}
			}
			_iGenParticles = n; // �ثe�w�g���� n �Ӥ��l

		}
		if (_fElpasedTime >= 1.0f) {
			_fElpasedTime -= 1.0f;
			if (_iGenParticles >= _iNumParticles) _iGenParticles -= _iNumParticles;
			else _iGenParticles = 0;
		}
		_fElpasedTime += dt;
	}
	//----------------------------------------------------------------------------------
	// �B
	if (_bEmitter2On) {
		int n = (int)(_fElpasedTime * _iNumParticles); // ��ثe�������Ӳ��ͪ����l�Ӽ�
		if (n > _iGenParticles) {  // ���ͪ����l�ӼƤ����A���ͨ� n �Ӥ��l
			for (int i = 0; i < n - _iGenParticles; i++) {
				if (_iFree != 0) {
					get = _FreeList.front();
					get->setBehavior(EMITTER_RAIN);
					get->setRotation(0);
					get->setGravity(-10);
					get->setWindForce(_fWindForce);
					get->setTag(1);

					float t = (rand() % 1001) / 1000.0f; // ���ͤ��� 0 �� 1 ������
					t = (270.0f + t)* M_PI / 180.0f;
					Vec2 vdir(cosf(t), sinf(t)); // �N���l����V�q�����ন���V�q (cos,sin)
					get->setDirection(vdir);
					get->setWindDirNum(_fWindDir);
					_FreeList.pop_front();
					_InUsedList.push_front(get);
					_iFree--; _iInUsed++;
				}
			}
			_iGenParticles = n; // �ثe�w�g���� n �Ӥ��l

		}
		if (_fElpasedTime >= 1.0f) {
			_fElpasedTime -= 1.0f;
			if (_iGenParticles >= _iNumParticles) _iGenParticles -= _iNumParticles;
			else _iGenParticles = 0;
		}
		_fElpasedTime += dt;
	}
	//----------------------------------------------------------------------------------
	// �P�P
	if (_bEmitter3On && _bStarAniMove) {
		int n = (int)(_fElpasedTime * _iNumParticles); // ��ثe�������Ӳ��ͪ����l�Ӽ�
		if (n > _iGenParticles) {  // ���ͪ����l�ӼƤ����A���ͨ� n �Ӥ��l
			for (int i = 0; i < n - _iGenParticles; i++) {
				if (_iFree != 0) {
					get = _FreeList.front();
					get->setStar(_bStarAniFinished);
					get->setBehavior(EMITTER_DRAWSTAR);
					get->setOpacity(_fOpacity);
					get->setGravity(0);
					get->setPosition(_starmovePt);
					get->setParticleImage(_cParticleImage); //�]�w�ɤl�Ϯ�
					_FreeList.pop_front();
					_InUsedList.push_front(get);
					_iFree--; _iInUsed++;
				}
			}
			_iGenParticles = n; // �ثe�w�g���� n �Ӥ��l

		}
		if (_fElpasedTime >= 1.0f) {
			_fElpasedTime -= 1.0f;
			if (_iGenParticles >= _iNumParticles) _iGenParticles -= _iNumParticles;
			else _iGenParticles = 0;
		}
		_fElpasedTime += dt;
	}
	//-----------------------------------------------------------------------------------
	// �Ϥ��s�}�ҡA�B�|�����z���I(�o�g���A)
	if (_bFireworkOn && _bFireworkMove) {
		int n = (int)(_fElpasedTime * _iNumParticles); // ��ثe�������Ӳ��ͪ����l�Ӽ�
		if (n > _iGenParticles) {  // ���ͪ����l�ӼƤ����A���ͨ� n �Ӥ��l
			for (int i = 0; i < n - _iGenParticles; i++) {
				// �ھ� Emitter �������ѼơA�]�w�Ҳ��ͤ��l���Ѽ�
				if (_iFree != 0) {
					get = _FreeList.front();
					get->setBehavior(FIREWORK_TAIL);
					get->setVelocity(2.0f);
					get->setOpacity(255);
					get->setRotation(_fSpin);
					get->setLifetime(0.5f);
					get->setGravity(-5);
					get->setPosition(_fireworkPt);
					get->setParticleImage("comet.png"); //�]�w�ɤl�Ϯ�
															// �ھ� _fSpread �P _vDir ���ͤ�V
					float t = 0;
					t = (270.0f + t)* M_PI / 180.0f;
					Vec2 vdir(cosf(t), sinf(t));
					get->setDirection(vdir);
					_FreeList.pop_front();
					_InUsedList.push_front(get);
					_iFree--; _iInUsed++;
				}
			}
			_iGenParticles = n; // �ثe�w�g���� n �Ӥ��l

		}
		if (_fElpasedTime >= 1.0f) {
			_fElpasedTime -= 1.0f;
			if (_iGenParticles >= _iNumParticles) _iGenParticles -= _iNumParticles;
			else _iGenParticles = 0;
		}
		_fElpasedTime += dt;
	}

	//-----------------------------------------------------------------------------------
	//if (_bEmitter1On && _bEmitter2On && _iInUsed != 0) { // ���K & �B�P�ɶ}��
	//	for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
	//		if ((*it)->getTag() == 1) { // ���l���B
	//			for (it2 = _InUsedList.begin(); it2 != _InUsedList.end(); ) {
	//				if ((*it2)->getTag() == 0) { // ���l����
	//					if ( ((*it)->_rect).intersectsRect( (*it2)->_rect ) ) {
	//						if ((*it2)->getParticleImg() == "fireParticle.png")
	//							(*it2)->setParticleImage("cloud.png");
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//-----------------------------------------------------------------------------------
	if (_iInUsed != 0) { // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end(); ) {
			if ((*it)->doStep(dt)) { // ���l�ͩR�g���w�g��F
				_FreeList.push_front((*it)); // �N�ثe�o�@�Ӹ`�I�����e��^ _FreeList
				it = _InUsedList.erase(it); // �����ثe�o�@��, 
				_iFree++; _iInUsed--;
			}
			else it++;
		}
	}
	
	//===================================================================================
	if (_bRoseExplode) {		//�ĤG�h��
		_fCount += dt;
		if (_fCount >= 0.3f) {	//����X�{
			_fCount -= 0.3f;
			rose2Explotion();
			_bRoseExplode = false;
		}
	}
}

int CParticleSystem::getInUsedNumber()
{
	return _iInUsed;
}

void CParticleSystem::setGravity(float fGravity)
{
	// �]�w�ثe particle �O�b inused �� gravity
	_fGravity = fGravity;
	list <CParticle *>::iterator it;
	if (_iInUsed != 0) { // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setGravity(_fGravity);
		}
	}
}

void CParticleSystem::setWindForce(float fWindForce)
{
	_fWindForce = fWindForce;
	list <CParticle *>::iterator it;
	if (_iInUsed != 0) { // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setWindForce(_fWindForce);
		}
	}
}

void CParticleSystem::setWindDirection(float fWindDir)
{
	_fWindDir = fWindDir;
	list <CParticle *>::iterator it;
	if (_iInUsed != 0) { // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setWindDirNum(_fWindDir);
		}
	}
}

void CParticleSystem::setRed(int iRed)
{
	_iRed = iRed;
	list <CParticle *>::iterator it;
	if (_iInUsed != 0) { // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setRed(_iRed);
		}
	}
}

void CParticleSystem::setGreen(int iGreen)
{
	_iGreen = iGreen;
	list <CParticle *>::iterator it;
	if (_iInUsed != 0) { // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setGreen(_iGreen);
		}
	}
}

void CParticleSystem::setBlue(int iBlue)
{
	_iBlue = iBlue;
	list <CParticle *>::iterator it;
	if (_iInUsed != 0) { // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setBlue(_iBlue);
		}
	}
}

void CParticleSystem::setStar(bool bAniFinished) //�P�P�ʵe
{
	_bStarAniFinished = bAniFinished;
	list <CParticle *>::iterator it;
	if (_iInUsed != 0) { // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setStar(_bStarAniFinished);
		}
	}
}

void CParticleSystem::setEmitter(bool bEm)
{
	_bEmitterOn = bEm;
}

void CParticleSystem::setFirework(bool bFirework)
{
	_bFireworkOn = bFirework;
}

void CParticleSystem::setEmitter1(bool bEm)
{
	_bEmitter1On = bEm;
	list <CParticle *>::iterator it;
	if (_iInUsed != 0) { // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setEmitter1Btn(_bEmitter1On);
		}
	}
}
void CParticleSystem::setEmitter2(bool bEm)
{
	_bEmitter2On = bEm;
	list <CParticle *>::iterator it;
	if (_iInUsed != 0) { // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setEmitter2Btn(_bEmitter2On);
		}
	}
}
void CParticleSystem::setEmitter3(bool bEm)
{
	_bEmitter3On = bEm;
	list <CParticle *>::iterator it;
	if (_iInUsed != 0) { // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setEmitter3Btn(_bEmitter3On);
		}
	}
}

void CParticleSystem::fireworkExplotion()
{
	CParticle *get;
	if (_iFree > 500) {
		for (int i = 0; i < 500; i++) {
			get = _FreeList.front();
			get->setBehavior(FIREWORK_EXPLOSION);
			get->setGravity(-10);
			get->setParticleImage("spark.png"); //�]�w�ɤl�Ϯ�
			get->setPosition(_fireworkPt); //�]�w�Ϥ��z����m
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
	}
	else return;// �S�����l, �ҥH�N������
}

void CParticleSystem::rose2Explotion()
{
	CParticle *get;
	if (_iFree > 130) {
		for (int i = 0; i < 130; i++) {
			get = _FreeList.front();
			get->setBehavior(ROSESHAPE_2);
			get->setPosition(_touchPt); //�Prose1�P�@��m
			get->setGravity(_fGravity);
			get->setWindForce(_fWindForce);
			get->setWindDirNum(_fWindDir);
			get->setParticleImage(_cParticleImage); //�]�w�ɤl�Ϯ�
			get->setSpin(_fSpin);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
	}
	else return;// �S�����l, �ҥH�N������
}

void CParticleSystem::onTouchesBegan(const cocos2d::CCPoint &touchPoint)
{
	CParticle *get;
	switch (_iType)
	{
	case STAY_FOR_TWOSECONDS:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(STAY_FOR_TWOSECONDS);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			get->setWindForce(_fWindForce);
			get->setParticleImage(_cParticleImage); //�]�w�ɤl�Ϯ� 
			get->setSpin(_fSpin);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case RANDOMS_FALLING :
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(RANDOMS_FALLING);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			get->setWindForce(_fWindForce);
			get->setWindDirNum(_fWindDir);
			get->setParticleImage(_cParticleImage); //�]�w�ɤl�Ϯ�
			get->setSpin(_fSpin);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case FREE_FLY:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(FREE_FLY);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			get->setWindForce(_fWindForce);
			get->setWindDirNum(_fWindDir);
			get->setParticleImage(_cParticleImage); //�]�w�ɤl�Ϯ�
			get->setSpin(_fSpin);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case EXPLOSION:
		// �q _FreeList ���o 99 �Ӥ��l����� _InUsed
		if (_iFree > 100) {
			for (int i = 0; i < 100; i++) {
				get = _FreeList.front();
				get->setBehavior(EXPLOSION);
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				get->setWindForce(_fWindForce);
				get->setWindDirNum(_fWindDir);
				get->setParticleImage(_cParticleImage); //�]�w�ɤl�Ϯ�
				get->setSpin(_fSpin);
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case HEARTSHAPE:
		if (_iFree > 150) {
			for (int i = 0; i < 150; i++) {
				get = _FreeList.front();
				get->setBehavior(HEARTSHAPE);
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				get->setWindForce(_fWindForce);
				get->setWindDirNum(_fWindDir);
				get->setParticleImage(_cParticleImage); //�]�w�ɤl�Ϯ�
				get->setSpin(_fSpin);
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case BUTTERFLYSHAPE:
		if (_iFree > 150) {
			for (int i = 0; i < 150; i++) {
				get = _FreeList.front();
				get->setBehavior(BUTTERFLYSHAPE);
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				get->setWindForce(_fWindForce);
				get->setWindDirNum(_fWindDir);
				get->setParticleImage(_cParticleImage); //�]�w�ɤl�Ϯ�
				get->setSpin(_fSpin);
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case ROSESHAPE:
		_bRoseExplode = true; //ROSE1�z��

		if (_iFree > 130) {
			for (int i = 0; i < 130; i++) {
				get = _FreeList.front();
				get->setBehavior(ROSESHAPE);
				_touchPt = touchPoint;
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				get->setWindForce(_fWindForce);
				get->setWindDirNum(_fWindDir);
				get->setParticleImage(_cParticleImage); //�]�w�ɤl�Ϯ�
				get->setSpin(_fSpin);
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case SPIRALSHAPE:
		if (_iFree > 300) {
			for (int i = 0; i < 300; i++) {
				get = _FreeList.front();
				get->setPosition(touchPoint);
				get->setBehavior(SPIRALSHAPE);
				get->setGravity(_fGravity);
				get->setWindForce(_fWindForce);
				get->setWindDirNum(_fWindDir);
				get->setParticleImage(_cParticleImage); //�]�w�ɤl�Ϯ�
				get->setSpin(_fSpin);
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case CRACKER:
		if (touchPoint.x > 640.0f) _explodePt = Point(916.4f, 60.6f);
		else _explodePt = Point(115.0f, 60.6f);

		if (_iFree > 30) {
			for (int i = 0; i < 30; i++) {
				get = _FreeList.front();
				get->setPosition(_explodePt);
				get->setBehavior(CRACKER);
				get->setGravity(_fGravity);
				get->setWindForce(_fWindForce);
				get->setWindDirNum(_fWindDir);
				get->setParticleImage(_cParticleImage); //�]�w�ɤl�Ϯ�
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// �S�����l, �ҥH�N������
		break;
	}
}

void CParticleSystem::onTouchesMoved(const cocos2d::CCPoint &touchPoint)
{
	CParticle *get;
	switch (_iType)
	{
	case STAY_FOR_TWOSECONDS:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(STAY_FOR_TWOSECONDS);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			get->setWindForce(_fWindForce);
			get->setWindDirNum(_fWindDir);
			get->setParticleImage(_cParticleImage); //�]�w�ɤl�Ϯ�
			get->setSpin(_fSpin);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case RANDOMS_FALLING:
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(RANDOMS_FALLING);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			get->setWindForce(_fWindForce);
			get->setWindDirNum(_fWindDir);
			get->setParticleImage(_cParticleImage); //�]�w�ɤl�Ϯ�
			get->setSpin(_fSpin);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case FREE_FLY:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(FREE_FLY);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			get->setWindForce(_fWindForce);
			get->setWindDirNum(_fWindDir);
			get->setParticleImage(_cParticleImage); //�]�w�ɤl�Ϯ�
			get->setSpin(_fSpin);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	}
}