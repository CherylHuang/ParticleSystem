#ifndef __CDRAGGABLE_SPRITE_H__
#define __CDRAGGABLE_SPRITE_H__

// 單純的開關設定，不提供 touch 事件
// 可以設定預設是開還是關，就是顯示相對應的圖示

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
	static CDraggableSprite* create(); // 建立物件的實體
	void setSpriteInfo(const char *Img, const cocos2d::Point locPt);
	void setScale(float fscale);
	void setVisible(bool bvis);
	const Point getLoc();

	bool touchesBegan(cocos2d::Point inPos);
	bool touchesMoved(cocos2d::Point inPos);
	bool touchesEnded(cocos2d::Point inPos);
};


#endif