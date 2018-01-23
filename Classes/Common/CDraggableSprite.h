#ifndef __CDRAGGABLE_SPRITE_H__
#define __CDRAGGABLE_SPRITE_H__

// ��ª��}���]�w�A������ touch �ƥ�
// �i�H�]�w�w�]�O�}�٬O���A�N�O��ܬ۹������ϥ�

#include "cocos2d.h"

USING_NS_CC;

class CDraggableSprite :public Node
{ 
private:
	cocos2d::Sprite *_SpriteBody;
	float _fscale;
	cocos2d::Point _tdPreLoc;
	cocos2d::Point _tdLoc;
	bool _bTouched;
	bool _bVisible;

	cocos2d::Size  _SpriteSize;
	cocos2d::Point _SpriteLoc;
	cocos2d::Rect  _SpriteRect;
public:
	CDraggableSprite();
	static CDraggableSprite* create(); // �إߪ��󪺹���
	void setSpriteInfo(const char *Img, const cocos2d::Point locPt);
	void setScale(float fscale);
	void setVisible(bool bvis);
	const Point getLoc();

	bool touchesBegan(cocos2d::Point inPos);
	bool touchesMoved(cocos2d::Point inPos);
	bool touchesEnded(cocos2d::Point inPos);
};


#endif