#include "DragonBonesNode.h"
#include "CCTextureData.h"

DRAGONBONES_NAMESPACE_BEGIN

DragonBonesNode::DragonBonesNode()
: _armature(nullptr)
, _armatureDisplay(nullptr)
, _dragonBonesData(nullptr)
, _isFlippedX(false)
{
	_dragonBonesDataCache = DragonBonesDataCache::getInstance();
}

DragonBonesNode* DragonBonesNode::create(const std::string& dragonBonesDataFilePath, const std::string& textureAtlasDataFilePath, const std::string& dragonBonesName, float scale /* = 1.0f */)
{
	auto pNode = new (std::nothrow) DragonBonesNode();
	if (pNode && pNode->initDragonBonesData(dragonBonesDataFilePath, textureAtlasDataFilePath, dragonBonesName, scale))
	{
		pNode->autorelease();
		return pNode;
	}
	CC_SAFE_DELETE(pNode);
	return nullptr;
}

DragonBonesNode* DragonBonesNode::create(CCArmatureDisplay* armatureDisplay)
{
	auto pNode = new (std::nothrow) DragonBonesNode();
	if (pNode && pNode->initRenderer(armatureDisplay))
	{
		pNode->autorelease();
		return pNode;
	}
	CC_SAFE_DELETE(pNode);
	return nullptr;
}

bool DragonBonesNode::initRenderer(CCArmatureDisplay* armatureDisplay)
{
	if (!Node::init())
	{
		return false;
	}
	if (armatureDisplay)
	{
		_armatureDisplay = armatureDisplay;
		_armature = _armatureDisplay->getArmature();
		auto &armatureData = _armature->getArmatureData();
		setContentSize(Size(armatureData.aabb.width, armatureData.aabb.height));
		// Add Armature Display.
		addChild(_armatureDisplay);

		auto animationNames = _armatureDisplay->getAnimation()->getAnimationNames();
		if (!animationNames.empty())
		{
			_armatureDisplay->getAnimation()->play(animationNames[0]);
		}
	}

	return true;
}

bool DragonBonesNode::initDragonBonesData(const std::string& dragonBonesDataFilePath, const std::string& textureAtlasDataFilePath, const std::string& dragonBonesName, float scale)
{
	if (!Node::init())
	{
		return false;
	}

	_dragonBonesData = _dragonBonesDataCache->loadDragonBonesData(dragonBonesDataFilePath, dragonBonesName);
	_dragonBonesDataCache->loadTextureAtlasData(textureAtlasDataFilePath, dragonBonesName, scale);

	const auto& armatureNames = _dragonBonesData->getArmatureNames();
	if (armatureNames.empty())
	{
		return false;
	}

	// Remove prev Armature.
	if (_armature)
	{
		_armature->dispose();
		this->removeChild(_armatureDisplay);

		// b.
		// dragonBones::WorldClock::clock.remove(_armature);
	}

	const auto& armatureName = armatureNames[0];

	// a. Build Armature Display. (buildArmatureDisplay will advanceTime animation by Armature Display)
	auto armatureDisplay = _dragonBonesDataCache->buildArmatureDisplay(armatureName, dragonBonesName);

	return initRenderer(armatureDisplay);
}

bool DragonBonesNode::playAnimation(const std::string& animationName , int playTimes /* = -1 */)
{
	if (!_armatureDisplay) return false;
	auto animationNames = _armatureDisplay->getAnimation()->getAnimationNames();
	if (animationNames.empty())
	{
		return false;
	}
	
	auto it = std::find(animationNames.begin(), animationNames.end(), animationName);
	if (it != animationNames.end())
	{
		// Play Animation.
		_armatureDisplay->getAnimation()->play(animationName, playTimes);

		return true;
	}
	return false;
}

bool DragonBonesNode::stopAnimation(const std::string& animationName)
{
	if (!_armatureDisplay) return false;
	_armatureDisplay->getAnimation()->stop(animationName);
	return true;
}

bool DragonBonesNode::changeArmature(const std::string& armatureName)
{
	const auto& armatureNames = _dragonBonesData->getArmatureNames();
	if (armatureNames.empty())
	{
		return false;
	}
	auto it = std::find(armatureNames.begin(), armatureNames.end(), armatureName);
	if (it != armatureNames.end())
	{
		// Remove prev Armature.
		if (_armature)
		{
			_armature->dispose();
			this->removeChild(_armatureDisplay);

			// b.
			// dragonBones::WorldClock::clock.remove(_armature);
		}

		// a. Build Armature Display. (buildArmatureDisplay will advanceTime animation by Armature Display)
		_armatureDisplay = _dragonBonesDataCache->buildArmatureDisplay(*it, _dragonBonesData->name);
		_armature = _armatureDisplay->getArmature();
		_armatureDisplay->setAnchorPoint(Vec2(0, 0));
		setContentSize(_armatureDisplay->getContentSize());
		// Add Armature Display.
		addChild(_armatureDisplay);

		return true;
	}
	return false;
}

void DragonBonesNode::replaceTexture(const std::string& slotName, const std::string&fileName, int displayIndex)
{
}

void DragonBonesNode::replaceSlot(const std::string& dragonBonesName, const std::string& armatureName, const std::string& desSlotName, const std::string& displayName, const std::string& srcSlotName, int displayIndex)
{
	_dragonBonesDataCache->replaceSlotDisplay(dragonBonesName, armatureName, desSlotName, displayName, (_armature->getSlot(srcSlotName)), displayIndex);
}

void DragonBonesNode::setFlippedX(bool isFlippedX)
{
	if (_isFlippedX != isFlippedX)
	{
		int dir = isFlippedX ? -1 : 1;
		_armatureDisplay->setScaleX(dir *_armatureDisplay->getScaleX());
		_isFlippedX = isFlippedX;
	}
}

DragonBonesNode* DragonBonesNode::clone()
{
	const auto& armatureNames = _armature->_armatureData->parent->getArmatureNames();

	const auto& armatureName = armatureNames[0];
	auto armatureDisplay = _dragonBonesDataCache->buildArmatureDisplay(armatureName, _dragonBonesData->name);
	armatureDisplay->getArmature()->_replaceAnimationData = _armature->_replaceAnimationData;
	armatureDisplay->getArmature()->_replaceSlots = _armature->_replaceSlots;
	_dragonBonesDataCache->replaceSlot(*armatureDisplay->getArmature(), _armature->_replaceSlots);
	for (auto slot : _armature->getSlots())
	{
		auto newSlot = armatureDisplay->getArmature()->getSlot(slot->name);
		newSlot->_replacedDisplayDataSet = slot->_replacedDisplayDataSet;
		newSlot->_updateMeshData(false);
		newSlot->invalidUpdate();
	}
	auto pNode = DragonBonesNode::create(armatureDisplay);
	pNode->setPosition(getPosition());
	pNode->setAnchorPoint(getAnchorPoint());
	pNode->setScale(getScale());
	pNode->setFlippedX(getFlippedX());
	return pNode;
}

Sprite* DragonBonesNode::getSlotDisplay(const std::string& slotName)
{
	auto slot = _armature->getSlot(slotName);
	if (slot)
	{
		return (Sprite*)(slot->getDisplay());
	}
	return nullptr;
}

DRAGONBONES_NAMESPACE_END
