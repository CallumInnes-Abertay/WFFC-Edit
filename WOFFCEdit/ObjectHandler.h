#pragma once
#include "DisplayChunk.h"
#include <vector>
#include <memory>

using namespace DirectX::SimpleMath;

class DisplayObject;

class ObjectHandler
{
public:
	ObjectHandler();
	ObjectHandler(const std::shared_ptr<DX::DeviceResources>& device_resources);

	void Initialise(std::vector<DisplayObject>* startingObjects);

	int selectedId;
	std::vector<int> selectedObjects;

	void TextureChange();
	void MultiTextureChange();

protected:

private:
	std::vector<DisplayObject>* allDisplayObjects;
	Vector3 defaultSize;

	std::shared_ptr<DX::DeviceResources> m_device_resource;
};
