#ifndef __CSWITCHBUTTON_H__
#define __CSWITCHBUTTON_H__

#include "cocos2d.h"

USING_NS_CC;

class CSwitchButton :public Node
{ 
private:
	cocos2d::Sprite *_SwitchOffPic;
	cocos2d::Sprite *_SwitchTDPic;
	cocos2d::Sprite *_SwitchOnPic;

	cocos2d::Size  _BtnSize;
	cocos2d::Point _BtnLoc;
	cocos2d::Rect  _BtnRect;

	float _fScale;	// �Ϥ��Y�񪺤j�p
	bool _bTouched;	// �O�_�Q���U
	bool _bSwitchOn;// ���s�O�_�A�}�Ҫ��A
	bool _bVisible; // �O�_���

public:
	CSwitchButton();
	static CSwitchButton* create(); // �إߪ��󪺹���
	void setButtonInfo(const char *offImg, const char *onImg, const char *tdImg, const cocos2d::Point locPt);
	bool touchesBegan(cocos2d::Point inPos);
	bool touchesMoved(cocos2d::Point inPos); 
	bool touchesEnded(cocos2d::Point inPos); 
	void setVisible(bool bVis);
	void setScale(float scale);
	bool getStatus();
	void setColor(const Color3B& color);

};


#endif