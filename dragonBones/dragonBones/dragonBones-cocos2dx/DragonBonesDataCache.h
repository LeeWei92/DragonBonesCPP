#ifndef DRAGONBONES_DATA_CACHE_H
#define DRAGONBONES_DATA_CACHE_H

#include "../framework/DragonBonesHeaders.h"
#include "CCFactory.h"
#include "CCArmatureDisplay.h"
#include "cocos2d.h"
USING_NS_CC;

DRAGONBONES_NAMESPACE_BEGIN

class DragonBonesDataCache : public Ref
{
public:
	static DragonBonesDataCache* getInstance();
	static void destroyInstance();

	void loadDragonBones(const std::string& skeFilePath, const std::string& texFilePath, const std::string& dragonBonesName, float scale = 0.f);
	DragonBonesData* loadDragonBonesData(const std::string& filePath, const std::string& dragonBonesName);
	TextureAtlasData* loadTextureAtlasData(const std::string& filePath, const std::string& dragonBonesName, float scale = 0.f);
	CCArmatureDisplay* buildArmatureDisplay(const std::string& armatureName, const std::string& dragonBonesName, const std::string& skinName = "");
	void replaceSlot(Armature& armature, std::map<std::string, Slot*> _replaceSlots);
	void replaceSlotDisplay(const std::string& dragonBonesName, const std::string& armatureName, const std::string& slotName, const std::string& displayName, Slot* slot, int displayIndex = -1);
	void clear();
protected:
	DragonBonesDataCache();
	~DragonBonesDataCache();
	CCFactory _factory;
	std::map<std::string, DragonBonesData*> _dragonBonesDataMap;
	std::map<std::string, TextureAtlasData*> _textureAtlasDataMap;
	static DragonBonesDataCache* _sharedDragonBonesDataCache;
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_DATA_CACHE_H
