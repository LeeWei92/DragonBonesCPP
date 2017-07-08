#include "DragonBonesDataCache.h"
#include "CCTextureData.h"

DRAGONBONES_NAMESPACE_BEGIN

DragonBonesDataCache *DragonBonesDataCache::_sharedDragonBonesDataCache = nullptr;

DragonBonesDataCache::DragonBonesDataCache()
{
	_dragonBonesDataMap.clear();
	_textureAtlasDataMap.clear();
}

DragonBonesDataCache::~DragonBonesDataCache()
{
	_dragonBonesDataMap.clear();
	_textureAtlasDataMap.clear();
}

DragonBonesDataCache* DragonBonesDataCache::getInstance()
{
	if (!_sharedDragonBonesDataCache)
	{
		_sharedDragonBonesDataCache = new (std::nothrow) DragonBonesDataCache();
	}
	return _sharedDragonBonesDataCache;
}

void DragonBonesDataCache::destroyInstance()
{
	CC_SAFE_DELETE(_sharedDragonBonesDataCache);
}

void DragonBonesDataCache::loadDragonBones(const std::string& skeFilePath, const std::string& texFilePath, const std::string& dragonBonesName, float scale /* = 0.f */)
{
	loadDragonBonesData(skeFilePath, dragonBonesName);
	loadTextureAtlasData(texFilePath, dragonBonesName, scale);
}

DragonBonesData* DragonBonesDataCache::loadDragonBonesData(const std::string& filePath, const std::string& dragonBonesName)
{
	if (_dragonBonesDataMap.find(filePath) == _dragonBonesDataMap.end())
	{
		_dragonBonesDataMap[filePath] = _factory.loadDragonBonesData(filePath, dragonBonesName);
	}
	return _dragonBonesDataMap[filePath];
}

TextureAtlasData* DragonBonesDataCache::loadTextureAtlasData(const std::string& filePath, const std::string& dragonBonesName, float scale)
{
	if (_textureAtlasDataMap.find(filePath) == _textureAtlasDataMap.end())
	{
		_textureAtlasDataMap[filePath] = _factory.loadTextureAtlasData(filePath, dragonBonesName);
	}
	return _textureAtlasDataMap[filePath];
}

CCArmatureDisplay * DragonBonesDataCache::buildArmatureDisplay(const std::string& armatureName, const std::string& dragonBonesName, const std::string& skinName)
{
	return _factory.buildArmatureDisplay(armatureName, dragonBonesName, skinName);
}

void DragonBonesDataCache::replaceSlotDisplay(const std::string& dragonBonesName, const std::string& armatureName, const std::string& slotName, const std::string& displayName, Slot* slot, int displayIndex)
{
	_factory.replaceSlotDisplay(dragonBonesName, armatureName, slotName, displayName, *slot, displayIndex);
}

void DragonBonesDataCache::clear()
{
	_factory.clear(false);
	_textureAtlasDataMap.clear();
	_dragonBonesDataMap.clear();
}

DRAGONBONES_NAMESPACE_END
