#pragma once
#include "DisplayChunk.h"
#include <vector>
#include <memory>

using namespace DirectX::SimpleMath;

class DisplayObject;
struct InputCommands;

class ObjectHandler
{
public:
	ObjectHandler();
	ObjectHandler(const std::shared_ptr<DX::DeviceResources>& device_resources);

	void Initialise(std::vector<DisplayObject>* startingObjects);
	void Update(const InputCommands& input_commands);

	void TextureChange();
	void MultiTextureChange();
	void RemoveTextureChange(int idToRemove);

	int selectedId;
	std::vector<int> selectedObjects;
	bool wasPreviousSelectionModeSingle = false;

protected:

private:
	std::vector<DisplayObject>* allDisplayObjects;
	Vector3 defaultSize;

	std::shared_ptr<DX::DeviceResources> m_device_resource;
};
