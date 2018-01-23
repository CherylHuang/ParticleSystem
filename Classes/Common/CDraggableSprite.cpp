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
	_bVisible = false; // 預設為不顯示
	_SpriteBody = (Sprite *)Sprite::createWithSpriteFrameName(Img);
	_SpriteLoc = locPt;
	_SpriteBody->setPosition(_SpriteLoc);
	_SpriteBody->setVisible(_bVisible);
	this->addChild(_SpriteBody, 5);  // 加入目前的 Layer 中 1: Z 軸的層次，越大代表在越上層
	_fscale = 1.0f;

	// 取得大小
	_SpriteSize = _SpriteBody->getContentSize();
	// 設定判斷區域
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
		_tdPreLoc = inPos; // 按在圖示上
		_bTouched = true;
		return (true);
	}
	return (false);
}

bool CDraggableSprite::touchesMoved(cocos2d::Point inPos)
{
	if ( _bTouched &&  _bVisible ) {
		// 隨著按下的位置移動
		_tdLoc = inPos; // 目前的位置
		Point diff = _tdLoc - _tdPreLoc;
		_SpriteLoc = _SpriteLoc + diff;
		_SpriteBody->setPosition(_SpriteLoc);
		_tdPreLoc = _tdLoc;
		// 重新計算判斷範圍
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
		// 隨著按下的位置移動
		_tdLoc = inPos; // 目前的位置
		Point diff = _tdLoc - _tdPreLoc;
		_SpriteLoc = _SpriteLoc + diff;
		_SpriteBody->setPosition(_SpriteLoc);
		_tdPreLoc = _tdLoc;
		// 重新計算判斷範圍
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