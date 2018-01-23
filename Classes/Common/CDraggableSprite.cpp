#include "CDraggableSprite.h"

USING_NS_CC;

CDraggableSprite* CDraggableSprite::create()
{
	CDraggableSprite *lightobj = new (std::nothrow) CDraggableSprite();
	if (lightobj)
	{
		lightobj->autorelease();
		return lightobj;
	}
	CC_SAFE_DELETE(lightobj);
	return nullptr;
}

CDraggableSprite::CDraggableSprite()
{
	_SpriteBody = nullptr;
}
const Point CDraggableSprite::getLoc()
{
	return(_SpriteLoc);
}


void CDraggableSprite::setSpriteInfo(const char *Img, const cocos2d::Point locPt)
{
	_bVisible = false; // �w�]�������
	_SpriteBody = (Sprite *)Sprite::createWithSpriteFrameName(Img);
	_SpriteLoc = locPt;
	_SpriteBody->setPosition(_SpriteLoc);
	_SpriteBody->setVisible(_bVisible);
	this->addChild(_SpriteBody, 5);  // �[�J�ثe�� Layer �� 1: Z �b���h���A�V�j�N��b�V�W�h
	_fscale = 1.0f;

	// ���o�j�p
	_SpriteSize = _SpriteBody->getContentSize();
	// �]�w�P�_�ϰ�
	_SpriteRect.size = _SpriteSize;
	_SpriteRect.origin.x = _SpriteLoc.x - _SpriteSize.width*0.5f;
	_SpriteRect.origin.y = _SpriteLoc.y - _SpriteSize.height*0.5f;
	_bTouched = false;
	_fscale = 1.0f;
}

void CDraggableSprite::setScale(float fscale)
{
	_fscale = fscale;
	_SpriteBody->setScale(_fscale);
}

bool CDraggableSprite::touchesBegan(cocos2d::Point inPos)
{
	if (!_bTouched && _bVisible && _SpriteRect.containsPoint(inPos) ) {
		_tdPreLoc = inPos; // ���b�ϥܤW
		_bTouched = true;
		return (true);
	}
	return (false);
}

bool CDraggableSprite::touchesMoved(cocos2d::Point inPos)
{
	if ( _bTouched &&  _bVisible ) {
		// �H�۫��U����m����
		_tdLoc = inPos; // �ثe����m
		Point diff = _tdLoc - _tdPreLoc;
		_SpriteLoc = _SpriteLoc + diff;
		_SpriteBody->setPosition(_SpriteLoc);
		_tdPreLoc = _tdLoc;
		// ���s�p��P�_�d��
		_SpriteRect.size = _SpriteSize;
		_SpriteRect.origin.x = _SpriteLoc.x - _SpriteSize.width*0.5f;
		_SpriteRect.origin.y = _SpriteLoc.y - _SpriteSize.height*0.5f;
		return (true);
	}
	return (false);
}
bool CDraggableSprite::touchesEnded(cocos2d::Point inPos)
{
	if (_bTouched && _bVisible && _SpriteRect.containsPoint(inPos)) {
		// �H�۫��U����m����
		_tdLoc = inPos; // �ثe����m
		Point diff = _tdLoc - _tdPreLoc;
		_SpriteLoc = _SpriteLoc + diff;
		_SpriteBody->setPosition(_SpriteLoc);
		_tdPreLoc = _tdLoc;
		// ���s�p��P�_�d��
		_SpriteRect.size = _SpriteSize;
		_SpriteRect.origin.x = _SpriteLoc.x - _SpriteSize.width*0.5f;
		_SpriteRect.origin.y = _SpriteLoc.y - _SpriteSize.height*0.5f;

		_bTouched = false;
		return (true);
	}
	return (false);
}

void CDraggableSprite::setVisible(bool bvis)
{
	_bVisible = bvis;
	_SpriteBody->setVisible(_bVisible);
}