#ifndef DRAGONBONES_NODE_H
#define DRAGONBONES_NODE_H

#include "../framework/DragonBonesHeaders.h"
#include "CCArmatureDisplay.h"
#include "DragonBonesDataCache.h"
#include "cocos2d.h"
USING_NS_CC;

DRAGONBONES_NAMESPACE_BEGIN

class DragonBonesNode : public Node
{
public:
	static DragonBonesNode* create(const std::string& dragonBonesDataFilePath, const std::string& textureAtlasDataFilePath, const std::string& dragonBonesName = "", float scale = 1.0f);
	static DragonBonesNode* create(CCArmatureDisplay* armatureDisplay);
	void setFlippedX(bool isFlippedX);
	bool getFlippedX() { return _isFlippedX; }
	Sprite* getSlotDisplay(const std::string& slotName);
	CC_SYNTHESIZE_READONLY(DragonBonesData*, _dragonBonesData, DragonBonesData);
	CC_SYNTHESIZE_READONLY(CCArmatureDisplay*, _armatureDisplay, ArmatureDisplay);
	CC_SYNTHESIZE_READONLY(Armature*, _armature, Armature);
	bool changeArmature(const std::string& armatureName);
	bool playAnimation(const std::string& animationName , int playTimes = -1);
	bool stopAnimation(const std::string& animationName);
	void changeSlot(const std::string& dragonBonesName, const std::string& armatureName, const std::string& desSlotName, const std::string& displayName, const std::string& srcSlotName, int displayIndex = -1);
	DragonBonesNode* clone();
    void replaceTexture(const std::string& slotName, const std::string&replaceFilePath, int displayIndex = -1);
protected:
	bool initDragonBonesData(const std::string& dragonBonesDataFilePath, const std::string& textureAtlasDataFilePath, const std::string& dragonBonesName, float scale);
	void initRenderer();
	bool initRenderer(CCArmatureDisplay* armatureDisplay);
protected:
	DragonBonesNode();
	~DragonBonesNode(){};
	DragonBonesDataCache* _dragonBonesDataCache;
	bool _isFlippedX;
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_NODE_H
