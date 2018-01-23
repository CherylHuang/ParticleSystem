#include <cmath>
#include "CParticle.h"


// 根據重力與時間計算 delta time 之間的位移差量 再轉成螢幕的位移
// 假設 2.5 秒讓一個分子從螢幕最上方移動到最下方, 也就是移動 720 PIXEL
// 720 PIXEL = 0.5*9.8*2.5*2.5 m => 1M = 23.5102 PIXEL
// 因為Y 軸往下為負, 所以重力要加上負號, 代表方向

#define FALLING_TIME 2.5f
#define MAX_HEIGHT 720.0f
#define PIXEL_PERM (2.0f*MAX_HEIGHT/(9.8f*FALLING_TIME*FALLING_TIME))
#define GRAVITY_Y(t,dt,g) ((g)*(t+0.5f*(dt)))  //已經經過 t 秒後，再經過 dt 時間內落下距離

#define LIFE_NOISE(f) ((f)*(1.0f-(rand()%2001/1000.0f)))
//#define INTENSITY(f)  ( (f) >= 255 ? 255 : (f) )

inline int INTENSITY(float f) {
	if (f >= 255) return(255);
	else if (f <= 0) return(0);
	else return((int)f);
}

USING_NS_CC;

CParticle::CParticle()
{
	_fGravity = 0;
}

bool CParticle::doStep(float dt)
{
	float cost, sint;
	switch (_iType) {
	case STAY_FOR_TWOSECONDS:
		if (!_bVisible && _fElapsedTime >= _fDelayTime ) {
			_fElapsedTime = _fElapsedTime - _fDelayTime; // 重新開始計時
			_bVisible = true;
			_Particle->setVisible(_bVisible);
			_Particle->setColor(_color);
			_Particle->setPosition(_Pos);
		}
		else if (_fElapsedTime> _fLifeTime) {
			_bVisible = false;
			_Particle->setVisible(_bVisible);
			return true; // 分子生命週期已經結束
		}
		else {
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_Particle->setScale(1 + sint * 2);
			_Particle->setOpacity(_fOpacity * cost);
			_Particle->setColor(Color3B(INTENSITY((_color.r + sint * 64)*(1 + sint)), 
				INTENSITY((_color.g - cost * 32)*(1 + sint)), INTENSITY((_color.b - sint * 64)*(1 + sint))));
			float tt = GRAVITY_Y(_fElapsedTime, dt, _fGravity);
			_Pos.y += (_Direction.y  * _fVelocity + tt) * dt * PIXEL_PERM;
			_Particle->setPosition(_Pos);
			_fdegree += dt *_fSpin; // 計算 dt 時間內轉動的角度量，並累加到目前的角度中
			_Particle->setRotation(_fdegree);
		}
		break;
	case RANDOMS_FALLING:
		if (!_bVisible && _fElapsedTime >= _fDelayTime) {
			_fElapsedTime = _fElapsedTime - _fDelayTime; // 重新開始計時
			_bVisible = true;
			_Particle->setVisible(_bVisible);
			_Particle->setColor(_color);
			_Particle->setPosition(_Pos);
		}
		else if (_fElapsedTime > _fLifeTime) {
			_bVisible = false;
			_Particle->setVisible(_bVisible);
			return true; // 分子生命週期已經結束
		}
		else {
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_Particle->setScale(1 + sint * 1.25f);
			_Particle->setOpacity(_fOpacity * cost);
			_Particle->setColor(_color);
			float wind = (rand() % 1001) / 1000.0f; // 產生介於 0 到 1 間的數
			wind = (_fWindDir + wind)* M_PI / 180.0f;
			_WindDir = Point(cosf(wind), sinf(wind));

			float fwind = GRAVITY_Y(_fElapsedTime, dt, _fWindForce);
			_Pos.x += (_Direction.x * _fVelocity + fwind * _WindDir.x) * dt * PIXEL_PERM;
			float tt = GRAVITY_Y(_fElapsedTime, dt, _fGravity);
			_Pos.y += (_Direction.y * _fVelocity + tt + fwind * _WindDir.y)* dt * PIXEL_PERM;
			_Particle->setPosition(_Pos);
			_fdegree += dt *_fSpin; // 計算 dt 時間內轉動的角度量，並累加到目前的角度中
			_Particle->setRotation(_fdegree);
		}
		break;
	case FREE_FLY:
		if (!_bVisible && _fElapsedTime >= _fDelayTime) {
			_fElapsedTime = _fElapsedTime - _fDelayTime; // 重新開始計時
			_bVisible = true;
			_Particle->setVisible(_bVisible);
			_Particle->setColor(_color);
			_Particle->setPosition(_Pos);
		}
		else if (_fElapsedTime > _fLifeTime) {
			_bVisible = false;
			_Particle->setVisible(_bVisible);
			return true; // 分子生命週期已經結束
		}
		else {
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_Particle->setScale(1 + sint * 2);
			_Particle->setOpacity(_fOpacity * cost);
			_Particle->setColor(_color);
			float wind = (rand() % 1001) / 1000.0f; // 產生介於 0 到 1 間的數
			wind = (_fWindDir + wind)* M_PI / 180.0f;
			_WindDir = Point(cosf(wind), sinf(wind));

			float fwind = GRAVITY_Y(_fElapsedTime, dt, _fWindForce);
			_Pos.x += (_Direction.x * _fVelocity + fwind * _WindDir.x) * dt * PIXEL_PERM;
			float tt = GRAVITY_Y(_fElapsedTime, dt, _fGravity);
			_Pos.y += (_Direction.y * _fVelocity + tt + fwind * _WindDir.y)* dt * PIXEL_PERM;
			_Particle->setPosition(_Pos);
			_fdegree += dt *_fSpin; // 計算 dt 時間內轉動的角度量，並累加到目前的角度中
			_Particle->setRotation(_fdegree);
		}
		break;
	case EXPLOSION:
		if (!_bVisible && _fElapsedTime >= _fDelayTime) {
			_fElapsedTime = _fElapsedTime - _fDelayTime; // 重新開始計時
			_bVisible = true;
			_Particle->setVisible(_bVisible);
			_Particle->setColor(_color);
			_Particle->setPosition(_Pos);
		}
		else if (_fElapsedTime > _fLifeTime) {
			_bVisible = false;
			_Particle->setVisible(_bVisible);
			return true; // 分子生命週期已經結束
		}
		else {
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_Particle->setScale(1.25 + sint*2.0);
			_Particle->setOpacity(_fOpacity * cost);
			_Particle->setColor(Color3B(INTENSITY(_color.r*(1 + sint)), INTENSITY(_color.g*(1 + sint)), INTENSITY(_color.b*(1 + sint))));
			float wind = (rand() % 1001) / 1000.0f; // 產生介於 0 到 1 間的數
			wind = (_fWindDir + wind)* M_PI / 180.0f;
			_WindDir = Point(cosf(wind), sinf(wind));

			float fwind = GRAVITY_Y(_fElapsedTime, dt, _fWindForce);
			_Pos.x += (_Direction.x * _fVelocity + fwind * _WindDir.x) * dt * PIXEL_PERM;
			float tt = GRAVITY_Y(_fElapsedTime, dt, _fGravity);
			_Pos.y += (_Direction.y * _fVelocity + tt + fwind * _WindDir.y)* dt * PIXEL_PERM;
			_Particle->setPosition(_Pos);
			_fdegree += dt *_fSpin; // 計算 dt 時間內轉動的角度量，並累加到目前的角度中
			_Particle->setRotation(_fdegree);
		}
		break;
	case HEARTSHAPE:
	case BUTTERFLYSHAPE:
	case ROSESHAPE:
	case ROSESHAPE_2:
		if (!_bVisible && _fElapsedTime >= _fDelayTime) {
			_fElapsedTime = _fElapsedTime - _fDelayTime; // 重新開始計時
			_bVisible = true;
			_Particle->setVisible(_bVisible);
			_Particle->setColor(_color);
			_Particle->setPosition(_Pos);
		}
		else if (_fElapsedTime > _fLifeTime) {
			_bVisible = false;
			_Particle->setVisible(_bVisible);
			return true; // 分子生命週期已經結束
		}
		else {
			// 根據心型公式，計算每一個分子的終止位置
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_Particle->setScale(1.25 + (1 - cost)*2.0);
			_Particle->setOpacity(_fOpacity * cost);
			_Particle->setColor(_color);
			float wind = (rand() % 1001) / 1000.0f; // 產生介於 0 到 1 間的數
			wind = (_fWindDir + wind)* M_PI / 180.0f;
			_WindDir = Point(cosf(wind), sinf(wind));

			float fwind = GRAVITY_Y(_fElapsedTime, dt, _fWindForce);
			_Pos.x += (_Direction.x * _fVelocity + fwind * _WindDir.x) * dt * PIXEL_PERM;
			float tt = GRAVITY_Y(_fElapsedTime, dt, _fGravity);
			_Pos.y += (_Direction.y * _fVelocity + tt + fwind * _WindDir.y)* dt * PIXEL_PERM;
			_Particle->setPosition(_Pos);
			_fdegree += dt *_fSpin; // 計算 dt 時間內轉動的角度量，並累加到目前的角度中
			_Particle->setRotation(_fdegree);
		}
		break;
	case SPIRALSHAPE:
		if (!_bVisible && _fElapsedTime >= _fDelayTime) {
			_fElapsedTime = _fElapsedTime - _fDelayTime; // 重新開始計時
			_bVisible = true;
			_Particle->setVisible(_bVisible);
			_Particle->setColor(_color);
			_Particle->setPosition(_Pos);
		}
		else if (_fElapsedTime > _fLifeTime) {
			_bVisible = false;
			_Particle->setVisible(_bVisible);
			return true; // 分子生命週期已經結束
		}
		else {
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_Particle->setScale(1.25 + (1 - cost)*2.0);
			_Particle->setOpacity(_fOpacity * cost);
			_Particle->setColor(_color);

			_ftheta += dt; //累積旋轉
			_Pos.x -= _ftouchX; //平移到原點
			_Pos.y -= _ftouchY;
			_Pos.x = cosf(_ftheta) * _Pos.x + sinf(_ftheta) * _Pos.y; //旋轉
			_Pos.y = -sinf(_ftheta) * _Pos.x + cosf(_ftheta) * _Pos.y;
			_Pos.x += _ftouchX; //平移
			_Pos.y += _ftouchY;
			_Particle->setPosition(_Pos);

			_fdegree += dt *_fSpin; // 計算 dt 時間內轉動的角度量，並累加到目前的角度中
			_Particle->setRotation(_fdegree);
		}
		break;
	case CRACKER:
		if (!_bVisible && _fElapsedTime >= _fDelayTime) {
			_fElapsedTime = _fElapsedTime - _fDelayTime; // 重新開始計時
			_bVisible = true;
			_Particle->setVisible(_bVisible);
			_Particle->setColor(_color);
			_Particle->setPosition(_Pos);
		}
		else if (_fElapsedTime < _fLifeTime && _fElapsedTime > (_fLifeTime - 0.3f * _fLifeTime)) {
			_Particle->setSpriteFrame(_cParticleImage);
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_Particle->setScale(1.0);
			_Particle->setOpacity(_fOpacity);
			//_Particle->setColor(Color3B(INTENSITY(_color.r*(1 + sint)), INTENSITY(_color.g*(1 + sint)), INTENSITY(_color.b*(1 + sint))));
			
			float wind = (rand() % 1001) / 1000.0f; // 產生介於 0 到 1 間的數
			wind = (_fWindDir + wind)* M_PI / 180.0f;
			_WindDir = Point(cosf(wind), sinf(wind));

			float fwind = GRAVITY_Y(_fElapsedTime, dt, _fWindForce);
			_Pos.x += (fwind * _WindDir.x) * dt * PIXEL_PERM;
			float tt = GRAVITY_Y(_fElapsedTime, dt, _fGravity);
			_Pos.y += (tt + fwind * _WindDir.y)* dt * PIXEL_PERM;
			_Particle->setPosition(_Pos);
			_fdegree += dt *_fSpin; // 計算 dt 時間內轉動的角度量，並累加到目前的角度中
			_Particle->setRotation(_fdegree);
		}
		else if (_fElapsedTime > _fLifeTime) {
			float fwind = GRAVITY_Y(_fElapsedTime, dt, _fWindForce + _iCrackerRand); //隨機風大小
			_Pos.x += (fwind * _WindDir.x) * dt * PIXEL_PERM;
			float tt = GRAVITY_Y(_fElapsedTime, dt, _fGravity);
			_Pos.y += (tt + fwind * _WindDir.y)* dt * PIXEL_PERM;
			_Particle->setPosition(_Pos);
			_fdegree += dt *_fSpin; // 計算 dt 時間內轉動的角度量，並累加到目前的角度中
			_Particle->setRotation(_fdegree);
			if (tt == 0 && fwind == 0) {
				_fCrackerCount += dt;
				if (_fCrackerCount > 1.0f) {
					_fCrackerCount -= 1.0f; //重設
					_bVisible = false;
					_Particle->setVisible(_bVisible);
					return true; // 分子生命週期已經結束
				}
			}
			if (_Pos.x < 0 || _Pos.x > 1280.0f || _Pos.y < 0 || _Pos.y > 720.0f) {
				_bVisible = false;
				_Particle->setVisible(_bVisible);
				return true; // 分子生命週期已經結束
			}
		}
		else {
			_Particle->setSpriteFrame("cornParticle.png");
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_Particle->setScale(0.5);
			_Particle->setOpacity(_fOpacity);
			//_Particle->setColor(Color3B(INTENSITY(_color.r*(1 + sint)), INTENSITY(_color.g*(1 + sint)), INTENSITY(_color.b*(1 + sint))));
			
			float wind = (rand() % 1001) / 1000.0f; // 產生介於 0 到 1 間的數
			wind = (_fWindDir + wind)* M_PI / 180.0f;
			_WindDir = Point(cosf(wind), sinf(wind));

			float fwind = GRAVITY_Y(_fElapsedTime, dt, _fWindForce);
			_Pos.x += (_Direction.x * _fVelocity * cost + fwind * _WindDir.x) * dt * PIXEL_PERM;
			float tt = GRAVITY_Y(_fElapsedTime, dt, _fGravity);
			_Pos.y += (_Direction.y * _fVelocity * cost + tt + fwind * _WindDir.y)* dt * PIXEL_PERM;
			_Particle->setPosition(_Pos);
			_fdegree += dt *_fSpin; // 計算 dt 時間內轉動的角度量，並累加到目前的角度中
			_Particle->setRotation(_fdegree);
		}
		break;
	case EMITTER_DEFAULT:
		if (!_bVisible && _fElapsedTime >= _fDelayTime) {
			_fElapsedTime = _fElapsedTime - _fDelayTime; // 重新開始計時
			_bVisible = true;
			_Particle->setVisible(_bVisible);
			_Particle->setColor(_color);
			_Particle->setPosition(_Pos);
		}
		else if (_fElapsedTime > _fLifeTime) {
			_bVisible = false;
			_Particle->setVisible(_bVisible);
			return true; // 分子生命週期已經結束
		}
		else {
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_Particle->setScale(_fSize + sint * 1.5f);
			_Particle->setOpacity(_fOpacity * cost);
			_Particle->setColor(Color3B(INTENSITY(_color.r*(1 + sint)), INTENSITY(_color.g*(1 + sint)), INTENSITY(_color.b*(1 + sint))));

			float wind = (rand() % 1001) / 1000.0f; // 產生介於 0 到 1 間的數
			wind = (_fWindDir + wind)* M_PI / 180.0f;
			_WindDir = Point(cosf(wind), sinf(wind));

			float fwind = GRAVITY_Y(_fElapsedTime, dt, _fWindForce);
			_Pos.x += (_Direction.x * _fVelocity + fwind * _WindDir.x) * dt * PIXEL_PERM;
			float tt = GRAVITY_Y(_fElapsedTime, dt, _fGravity);
			_Pos.y += (_Direction.y * _fVelocity + tt + fwind * _WindDir.y)* dt * PIXEL_PERM;
			_Particle->setPosition(_Pos);
			_fdegree += dt *_fSpin; // 計算 dt 時間內轉動的角度量，並累加到目前的角度中
			_Particle->setRotation(_fdegree);
		}
		break;
	case EMITTER_FIRE:
		if (!_bVisible && _fElapsedTime >= _fDelayTime) {
			_fElapsedTime = _fElapsedTime - _fDelayTime; // 重新開始計時
			_bVisible = true;
			_Particle->setVisible(_bVisible);
			_Particle->setColor(_color);
			_Particle->setPosition(_Pos);
			_Particle->setRotation(_fdegree); //BUG修正 : 一開始角度偏離
			_Particle->setSpriteFrame(_cParticleImage);
		}
		else if (_fLifeTime > 1.0f && _fElapsedTime < _fLifeTime && _fElapsedTime > (_fLifeTime - _fCloudTime)) {
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_fSize -= dt*1.2f;
			_Particle->setScale(_fSize);
			_Particle->setOpacity(_fOpacity * cost);
			_Particle->setColor(Color3B(INTENSITY(_color.r*(1 + sint)), INTENSITY(_color.g*(1 + sint)), INTENSITY(_color.b*(1 + sint))));
			
			float wind = (rand() % 1001) / 1000.0f; // 產生介於 0 到 1 間的數
			wind = (_fWindDir + wind)* M_PI / 180.0f;
			_WindDir = Point(cosf(wind), sinf(wind));
			
			float fwind = GRAVITY_Y(_fElapsedTime, dt, _fWindForce);
			_Pos.x += (_Direction.x * _fVelocity + fwind * _WindDir.x) * dt * PIXEL_PERM;
			float tt = GRAVITY_Y(_fElapsedTime, dt, _fGravity);
			_Pos.y += (_Direction.y * _fVelocity + tt + fwind * _WindDir.y)* dt * PIXEL_PERM;
			_Particle->setPosition(_Pos);
			_fdegree += dt *_fSpin; // 計算 dt 時間內轉動的角度量，並累加到目前的角度中
			_Particle->setRotation(_fdegree);
			if(_cParticleImage == "fireParticle.png")_Particle->setSpriteFrame("cloud.png"); //替換煙霧圖片
		}
		else if (_fElapsedTime > _fLifeTime) {
			_bVisible = false;
			_Particle->setVisible(_bVisible);
			return true; // 分子生命週期已經結束
		}
		else {
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_fSize -= dt*1.2f;
			_Particle->setScale(_fSize);
			_Particle->setOpacity(_fOpacity * cost);
			_Particle->setColor(Color3B(INTENSITY(_color.r*(1 + sint)), INTENSITY(_color.g*(1 + sint)), INTENSITY(_color.b*(1 + sint))));
			
			float wind = (rand() % 1001) / 1000.0f; // 產生介於 0 到 1 間的數
			wind = (_fWindDir + wind)* M_PI / 180.0f;
			_WindDir = Point(cosf(wind), sinf(wind));
			
			float fwind = GRAVITY_Y(_fElapsedTime, dt, _fWindForce);
			_Pos.x += (_Direction.x * _fVelocity + fwind * _WindDir.x) * dt * PIXEL_PERM;
			float tt = GRAVITY_Y(_fElapsedTime, dt, _fGravity);
			_Pos.y += (_Direction.y * _fVelocity + tt + fwind * _WindDir.y)* dt * PIXEL_PERM;
			_Particle->setPosition(_Pos);
			_fdegree += dt *_fSpin; // 計算 dt 時間內轉動的角度量，並累加到目前的角度中
			_Particle->setRotation(_fdegree);
			_Particle->setSpriteFrame(_cParticleImage);
		}
		break;
	case EMITTER_RAIN:
		if (!_bVisible && _fElapsedTime >= _fDelayTime) {
			_fElapsedTime = _fElapsedTime - _fDelayTime; // 重新開始計時
			_bVisible = true;
			_Particle->setVisible(_bVisible);
			_Particle->setColor(_color);
			_Particle->setPosition(_Pos);
			_Particle->setSpriteFrame(_cParticleImage);
		}
		else if (_fElapsedTime > _fLifeTime) {
			_Particle->setSpriteFrame("rippleParticle.png"); //漣漪
			if (_fRippleCount > _fRippleLifeTime) {
				_fRippleCount = 0; //漣漪 重新計時
				_bVisible = false;
				_Particle->setVisible(_bVisible);
				return true; // 分子生命週期已經結束
			}
			else {
				_Particle->setRotation(0);
				sint = sinf(M_PI*_fRippleCount / _fRippleLifeTime);
				cost = cosf(M_PI_2*_fRippleCount / _fRippleLifeTime);
				_Particle->setScale(_fSize + (1 - sint) * 0.4f);
				_Particle->setOpacity(_fRippleOpacity * cost * 0.5f);
				_Particle->setColor(Color3B(INTENSITY(_color.r*(1 + sint)), INTENSITY(_color.g*(1 + sint)), INTENSITY(_color.b*(1 + sint))));
			}
		}
		else {
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_Particle->setScale(_fSize);
			_Particle->setOpacity(_fOpacity * cost);
			_Particle->setColor(Color3B(INTENSITY(_color.r*(1 + sint)), INTENSITY(_color.g*(1 + sint)), INTENSITY(_color.b*(1 + sint))));

			float wind = (rand() % 1001) / 1000.0f; // 產生介於 0 到 1 間的數
			wind = (_fWindDir + wind)* M_PI / 180.0f;
			_WindDir = Point(cosf(wind), sinf(wind));
			float fwind = GRAVITY_Y(_fElapsedTime, dt, _fWindForce);

			if (_fWindDir <= 180.0f) _fRainAngle = (_fWindDir / 9.0f * 2.0f) - 20.0f; //產生風力角
			else _fRainAngle = (-(_fWindDir - 360) / 9.0f * 2.0f) - 20.0f;

			_Pos.x += (_Direction.x * _fVelocity + fwind * _WindDir.x) * dt * PIXEL_PERM;
			float tt = GRAVITY_Y(_fElapsedTime, dt, _fGravity);
			_Pos.y += (_Direction.y * _fVelocity + tt + fwind * _WindDir.y)* dt * PIXEL_PERM;
			_Particle->setPosition(_Pos);
			_fdegree = _fRainAngle * (_fWindForce / 10.0f);	// 根據風力大小產生角度
			_Particle->setRotation(_fdegree);
		}
		break;
	case EMITTER_DRAWSTAR:
		if (_bStarAniFinished) {
			if (_fOpacity <= 0) {
				_bVisible = false;
				_Particle->setVisible(_bVisible);
				return true; // 分子生命週期已經結束
			}
			else {
				_fOpacity -= _fElapsedTime; //透明度
				_Particle->setOpacity(_fOpacity);
			}
		}
		else {
			if (!_bVisible && _fElapsedTime >= _fDelayTime) {
				_fElapsedTime = _fElapsedTime - _fDelayTime; // 重新開始計時
				_bVisible = true;
				_Particle->setVisible(_bVisible);
				_Particle->setColor(_color);
				_Particle->setPosition(_Pos);
			}
			else {
				sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
				cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
				_Particle->setScale(1 + sint * 1.25f);
				_Particle->setOpacity(_fOpacity);
				if(_fOpacity > 100.0f) _fOpacity -= _fElapsedTime; //透明度
				_Particle->setColor(_color);
				_Particle->setPosition(_Pos);
				_fdegree += dt *_fSpin; // 計算 dt 時間內轉動的角度量，並累加到目前的角度中
				_Particle->setRotation(_fdegree);
			}
		}

		break;
	case FIREWORK_TAIL:
		if (!_bVisible && _fElapsedTime >= _fDelayTime) {
			_fElapsedTime = _fElapsedTime - _fDelayTime; // 重新開始計時
			_bVisible = true;
			_Particle->setVisible(_bVisible);
			_Particle->setColor(_color);
			_Particle->setPosition(_Pos);
			_Particle->setRotation(_fdegree); //BUG修正 : 一開始角度偏離
		}
		else if (_fElapsedTime > _fLifeTime) {
			_bVisible = false;
			_Particle->setVisible(_bVisible);
			return true; // 分子生命週期已經結束
		}
		else {
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_Particle->setScale(_fSize + sint * 1.5f );
			_Particle->setOpacity(_fOpacity * cost);
			_Particle->setColor(Color3B(INTENSITY(_color.r*(1 + sint)), INTENSITY(_color.g*(1 + sint)), INTENSITY(_color.b*(1 + sint))));
			_Pos.x += _Direction.x * _fVelocity * dt * PIXEL_PERM;
			float tt = GRAVITY_Y(_fElapsedTime, dt, _fGravity);
			_Pos.y += (_Direction.y * _fVelocity + tt)* dt * PIXEL_PERM;
			_Particle->setPosition(_Pos);
			_fdegree += dt *_fSpin; // 計算 dt 時間內轉動的角度量，並累加到目前的角度中
			_Particle->setRotation(_fdegree);
		}
		break;
	case FIREWORK_EXPLOSION:
		if (!_bVisible && _fElapsedTime >= _fDelayTime) {
			_fElapsedTime = _fElapsedTime - _fDelayTime; // 重新開始計時
			_bVisible = true;
			_Particle->setVisible(_bVisible);
			_Particle->setColor(_color);
			_Particle->setPosition(_Pos);
		}
		else if (_fElapsedTime > _fLifeTime) {
			_bVisible = false;
			_Particle->setVisible(_bVisible);
			return true; // 分子生命週期已經結束
		}
		else {
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_Particle->setScale(1.25 + (1 - cost)*2.0);
			_Particle->setOpacity(_fOpacity * sint);
			_Particle->setColor(_color);
			_Pos.x += _Direction.x * cost * _fVelocity * dt * PIXEL_PERM;
			float tt = GRAVITY_Y(_fElapsedTime, dt, _fGravity);
			_Pos.y += (_Direction.y * cost * _fVelocity + tt)* dt * PIXEL_PERM;
			_Particle->setPosition(_Pos.x, _Pos.y + 50.0f); //提高高度至粒子頂端
			_fdegree += dt *_fSpin; // 計算 dt 時間內轉動的角度量，並累加到目前的角度中
			_Particle->setRotation(_fdegree);
		}
		break;
	}

	// 累加時間
	_fElapsedTime += dt;
	_fRippleCount += dt;

	_rect = _Particle->getBoundingBox(); //碰撞
	return false;
}


void CParticle::setBehavior(int iType)
{
	float t;
	_iType = iType;
	switch (_iType) {
	case STAY_FOR_TWOSECONDS:
		_fVelocity = 0;
		_fLifeTime = 2.5f + LIFE_NOISE(0.15f);
		_fIntensity = 1;
		_fOpacity = 255;
		_fSpin = 0;
		_fSize = 1;
		_color = Color3B(64 + rand() % 128, 64 + rand() % 128, 64 + rand() % 128);
		_fElapsedTime = 0;
		_fDelayTime = 0;
		_fGravity = 0;
		_Particle->setOpacity(_fOpacity);
		_Particle->setScale(_fSize);
		break;
	case RANDOMS_FALLING:
		_fVelocity = 5.0f + rand() % 10 / 10.0f; // 單位 M/Sec
		_Direction.x = 0;
		_Direction.y = -1;
		_fLifeTime = 3.0f + LIFE_NOISE(0.15f);
		_fIntensity = 1;
		_fOpacity = 255;
		_fSpin = 0;
		_fSize = 1;
		_color = Color3B(128 + rand() % 128, 128 + rand() % 128, 128 + rand() % 128);
		_fElapsedTime = 0;
		_fDelayTime = 0;
		_fGravity = 0;
		break;
	case FREE_FLY:
		_fVelocity = 5.0f + rand() % 10 / 10.0f; // 單位 M/Sec
		t = 2.0f * M_PI * (rand() % 1000) / 1000.0f;
		_Direction.x = cosf(t);
		_Direction.y = sinf(t);
		_fLifeTime = 3.0f + LIFE_NOISE(0.15f);
		_fIntensity = 1;
		_fOpacity = 255;
		_fSpin = 0;
		_fSize = 1;
		_color = Color3B(128 + rand() % 128, 128 + rand() % 128, 128 + rand() % 128);
		_fElapsedTime = 0;
		_fDelayTime = 0;
		_fGravity = 0;
		break;
	case EXPLOSION:
		_fVelocity = 15.0f + rand() % 10 / 10.0f;
		t = 2.0f * M_PI * (rand() % 1000) / 1000.0f;
		_Direction.x = cosf(t);
		_Direction.y = sinf(t);
		_fLifeTime = 1.5f + LIFE_NOISE(0.15f);
		_fIntensity = 1;
		_fOpacity = 255;
		_fSpin = 0;
		_fSize = 1;
		_color = Color3B(64 + rand() % 128, 64 + rand() % 128,64 + rand() % 128);
		_fElapsedTime = 0;
		_fDelayTime = rand() % 100 / 1000.0f;
		_fGravity = 0;
		break;
	case HEARTSHAPE:
		_fVelocity = 1.0f;
		t = 2.0f * M_PI * (rand() % 1000) / 1000.0f;
		float sint, cost, sin2t, cos2t, cos3t, cos4t, sin12t; //---------定義----------
		sint = sinf(t);  cost = cosf(t); cos2t = cosf(2*t); cos3t = cosf(3 * t); cos4t = cosf(4 * t);
		sin12t = sin(t/12.0f);
		_Direction.x = 16*sint*sint*sint;
		_Direction.y = 13*cost - 5*cos2t - 2*cos3t - cos4t;
		_fLifeTime = 1.5f + LIFE_NOISE(0.15f);
		_fIntensity = 1;
		_fOpacity = 255;
		_fSpin = 0;
		_fSize = 1;
		_color = Color3B(128 + rand() % 128, 128 + rand() % 128, 128 + rand() % 128);
		_fElapsedTime = 0;
		_fDelayTime = rand() % 100 / 1000.0f;
		_fGravity = 0;
		break;
	case BUTTERFLYSHAPE:
		_fVelocity = 6.5f;
		t = 2.0f * M_PI * (rand() % 1000) / 1000.0f;
		sint = sinf(t);  cost = cosf(t); cos4t = cosf(4 * t); sin12t = sin(t / 12.0f);
		_Direction.x = sint*(expf(cost) - 2 * cos4t - powf(sin12t, 5));
		_Direction.y = cost*(expf(cost) - 2 * cos4t - powf(sin12t, 5));
		_fLifeTime = 1.5f + LIFE_NOISE(0.15f);
		_fIntensity = 1;
		_fOpacity = 255;
		_fSpin = 0;
		_fSize = 1;
		_color = Color3B(128 + rand() % 128, 128 + rand() % 128, 128 + rand() % 128);
		_fElapsedTime = 0;
		_fDelayTime = rand() % 100 / 1000.0f;
		_fGravity = 0;
		break;
	case ROSESHAPE:
		_fVelocity = 1.0f;
		t = 2.0f * M_PI * (rand() % 1000) / 1000.0f;
		//float sin4t;
		//sint = sinf(t);  cost = cosf(t); sin4t = sinf(4 * t);
		sint = sinf(t);  cost = cosf(t); sin2t = sinf(2 * t);
		//_Direction.x = 15 * cost * sin4t;
		//_Direction.y = 15 * sint * sin4t;
		_Direction.x = 15 * cost * sin2t;
		_Direction.y = 15 * sint * sin2t;
		_fLifeTime = 1.5f + LIFE_NOISE(0.15f);
		_fIntensity = 1;
		_fOpacity = 255;
		_fSpin = 0;
		_fSize = 1;
		_color = Color3B(128 + rand() % 128, 128 + rand() % 128, 128 + rand() % 128);
		_fElapsedTime = 0;
		_fDelayTime = rand() % 100 / 1000.0f;
		_fGravity = 0;
		break;
	case ROSESHAPE_2:
		_fVelocity = 1.0f;
		t = 2.0f * M_PI * (rand() % 1000) / 1000.0f;
		sint = sinf(t);  cost = cosf(t); cos2t = cosf(2 * t);
		_Direction.x = 15 * cost * cos2t;
		_Direction.y = 15 * sint * cos2t;
		_fLifeTime = 1.5f + LIFE_NOISE(0.15f);
		_fIntensity = 1;
		_fOpacity = 255;
		_fSpin = 0;
		_fSize = 1;
		_color = Color3B(128 + rand() % 128, 128 + rand() % 128, 128 + rand() % 128);
		_fElapsedTime = 0;
		_fDelayTime = rand() % 100 / 1000.0f;
		_fGravity = 0;
		break;
	case SPIRALSHAPE:
		_fVelocity = 5.0f + rand() % 10 / 10.0f;
		t = 2.0f * M_PI * (rand() % 1000) / 1000.0f;
		_Direction.x = cosf(t) * t;
		_Direction.y = sinf(t) * t;
		_fLifeTime = 1.5f + LIFE_NOISE(0.15f);
		_fIntensity = 1;
		_fOpacity = 255;
		_fSpin = 0;
		_fSize = 1;
		_color = Color3B(64 + rand() % 128, 64 + rand() % 128, 64 + rand() % 128);
		_fElapsedTime = 0;
		_fDelayTime = rand() % 100 / 1000.0f;
		_fGravity = 0;

		_ftouchX = _Pos.x; //記錄觸碰點
		_ftouchY = _Pos.y;
		_Pos.x += _Direction.x * 2 * PIXEL_PERM; //原圖形座標
		_Pos.y += _Direction.y * 2 * PIXEL_PERM;
		_ftheta = 0; //累積旋轉角
		break;
	case CRACKER:
		_fVelocity = 30.0f + rand() % 100 / 100.0f;
		if(_Pos.x > 640.0f) t = 2.0f * M_PI * (333 + rand() % 85) / 1000.0f; //限制角度範圍
		else t = 2.0f * M_PI * (83 + rand() % 85) / 1000.0f;
		_Direction.x = cosf(t);
		_Direction.y = sinf(t);
		_fLifeTime = 1.0f + LIFE_NOISE(0.5f);
		_fIntensity = 1;
		_fOpacity = 255;
		if (rand() % 2)_fSpin = (30 + rand() % 181) * (-1);
		else _fSpin = (30 + rand() % 181);
		_fSize = 1;
		_color = Color3B(255, 255, 255);
		_fElapsedTime = 0;
		_fDelayTime = rand() % 100 / 1000.0f;
		_fGravity = 0;
		if (rand() % 3 == 0)_cParticleImage = "popcorn1Particle.png";
		else if (rand() % 3 == 1)_cParticleImage = "popcorn2Particle.png";
		else _cParticleImage = "popcorn3Particle.png";
		_iCrackerRand = 0.5f + (rand() % 41) / 10.0f;
		break;
	case EMITTER_DEFAULT:
		_fIntensity = 1;
		_fOpacity = 255;
		_fSize = 1;

		int Rrand, Grand, Brand; //隨機顏色
		if (_iRed <= 200) Rrand = rand() % 55;
		else Rrand = rand() % (256 - _iRed);
		if (_iGreen <= 200) Grand = rand() % 55;
		else Grand = rand() % (256 - _iGreen);
		if (_iBlue <= 200) Brand = rand() % 55;
		else Brand = rand() % (256 - _iBlue);

		_color = Color3B(_iRed + Rrand, _iGreen + Grand, _iBlue + Brand);
		_fElapsedTime = 0;
		_fDelayTime = 0;
		_Particle->setScale(_fSize);
		break;
	case EMITTER_FIRE:
		_fVelocity = 13.0f + (rand() % 10 / 10.0f);
		_fIntensity = 1;
		_fOpacity = 255;
		_fSpin = 0;
		_fElapsedTime = 0;
		_fDelayTime = 0;
		_fGravity = 10.0f;
		_Particle->setOpacity(_fOpacity);
		_Particle->setScale(_fSize);
		_randNum = rand() % 6;
		if (_randNum == 0) {
			_Pos = Point(430.0f + rand() % 121, 230.0f + rand() % 3);
			_cParticleImage = "flare.png";
			_color = Color3B(255, 255, 50 + rand() % 151);
			_fSize = 1.0f;
			_fLifeTime = 0.8f + rand() % 10 / 10.0f;
		}
		else {
			_Pos = Point(450.0f + rand() % 81, 230.0f + rand() % 3);
			_cParticleImage = "fireParticle.png";
			_color = Color3B(255, 255, 255);
			_fSize = 2.5f;
			_fLifeTime = 0.5f + rand() % 10 / 10.0f;
		}
		if (_bEmitter2On) _fCloudTime = 0.8f;
		else _fCloudTime = 0.4f;
		break;
	case EMITTER_RAIN:
		_fVelocity = 40.0f;
		_fIntensity = 1;
		_fOpacity = 255;
		_fSize = 1.0f + (rand() % 6 / 10.0f);
		_fSpin = 0;
		_fElapsedTime = 0;
		_fRippleCount = 0;
		_fDelayTime = 0;
		_Pos = Point(rand() % 1281, 750.0f + rand() % 11 + (rand() % 10 / 10.f));
		_cParticleImage = "rainParticle.png";
		_color = Color3B(255, 255, 255);
		_fLifeTime = 0.4f + (rand() % 3 / 10.0f) + (rand() % 10 / 100.0f);
		_fRippleLifeTime = 1.0f;
		_randNum = rand() % 10;
		if (_randNum == 0) _fRippleOpacity = 255;
		else  _fRippleOpacity = 0;
		break;
	case EMITTER_DRAWSTAR:
		_fVelocity = 5.0f + rand() % 10 / 10.0f; // 單位 M/Sec
		_Direction.x = 0;
		_Direction.y = -1;
		_fLifeTime = 10.0f + LIFE_NOISE(0.15f);
		_fIntensity = 1;
		_fOpacity = 255;
		_fSpin = 0;
		_fSize = 1;
		_color = Color3B(128 + rand() % 128, 128 + rand() % 128, 128 + rand() % 128);
		_fElapsedTime = 0;
		_fDelayTime = 0;
		_fGravity = 0;
		break;
	case FIREWORK_TAIL:
		_fVelocity = 2.0f;
		_fLifeTime = 1.0f;
		_fIntensity = 1;
		_fOpacity = 255;
		_fSpin = 0;
		_fSize = 1;
		_color = Color3B(255, 255, 255);
		_fElapsedTime = 0;
		_fDelayTime = 0;
		_fGravity = -10.0f;
		_Particle->setOpacity(_fOpacity);
		_Particle->setScale(_fSize);
		break;
	case FIREWORK_EXPLOSION:
		_fVelocity = 15.0f + rand() % 10 / 10.0f;
		t = 2.0f * M_PI * (rand() % 1000) / 1000.0f;
		_Direction.x = cosf(t);
		_Direction.y = sinf(t);
		_fLifeTime = LIFE_NOISE(1.5f);
		_fIntensity = 1;
		_fOpacity = 255;
		_fSpin = (rand() % 180) * (0 - rand() % 2); //隨機方向旋轉
		_fSize = 1;
		_color = Color3B(64 + rand() % 128, 64 + rand() % 128, 64 + rand() % 128);
		_fElapsedTime = 0;
		_fDelayTime = rand() % 100 / 1000.0f;
		_fGravity = -10;
		break;
	}
}

void CParticle::setParticle(const char *pngName, cocos2d::Layer &inlayer)
{
	_Particle = Sprite::createWithSpriteFrameName(pngName);
	_Particle->setPosition(Point(rand() % 1024, rand() % 768));
	_Particle->setOpacity(_fOpacity);
	_Particle->setColor(Color3B::WHITE);
	_bVisible = false;
	_Particle->setVisible(false);
	_iType = 0;
	//BlendFunc blendfunc = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};  
	BlendFunc blendfunc = { GL_SRC_ALPHA, GL_ONE };
	_Particle->setBlendFunc(blendfunc);
	inlayer.addChild(_Particle, 1);
}

void CParticle::setParticleImage(const char *pngName)
{
	_Particle->setSpriteFrame(pngName);
}

void CParticle::setVisible()
{
	_bVisible = true;
	_Particle->setVisible(_bVisible);
}

void CParticle::setPosition(const cocos2d::Point &inPos) {
	_Pos = inPos;
};

void CParticle::setGravity(const float fGravity)
{
	_fGravity = fGravity;
}

void CParticle::setWindForce(const float fWindForce)
{
	_fWindForce = fWindForce;
}

void CParticle::setSpin(const float fSpin)
{
	_fSpin = fSpin;
}

void CParticle::setLifetime(const float lt) {
	_fLifeTime = lt + LIFE_NOISE(0.15f);;
}

void CParticle::setRed(const int iRed)
{
	_iRed = iRed;
}

void CParticle::setGreen(const int iGreen)
{
	_iGreen = iGreen;
}

void CParticle::setBlue(const int iBlue)
{
	_iBlue = iBlue;
}

void CParticle::setRotation(const float fRo)
{
	_fdegree = fRo;
}

void CParticle::setStar(const bool bStar)
{
	_bStarAniFinished = bStar;
}

void CParticle::setEmitter1Btn(const bool bBtn)
{
	_bEmitter1On = bBtn;
}

void CParticle::setEmitter2Btn(const bool bBtn)
{
	_bEmitter2On = bBtn;
}

void CParticle::setEmitter3Btn(const bool bBtn)
{
	_bEmitter3On = bBtn;
}

void CParticle::setTag(int itag)
{
	_iTag = itag;
}

int CParticle::getTag()
{
	return(_iTag);
}

char* CParticle::getParticleImg()
{
	return(_cParticleImage);
}