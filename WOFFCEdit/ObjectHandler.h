#pragma once
#include "DisplayChunk.h"
#include <vector>
#include <memory>
#include <stack>
#include "DisplayObject.h"

using namespace DirectX::SimpleMath;

struct InputCommands;

class ObjectHandler
{
public:
	static ObjectHandler& Instance()
	{
		static ObjectHandler instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		isInstanceMade = true;
		return instance;
	}

	ObjectHandler(const ObjectHandler&) = delete; // Disable copy constructor
	void operator=(const ObjectHandler&) = delete; // Disable assignment operator

	void Initialise(std::vector<DisplayObject>* startingObjects,
	                const std::shared_ptr<DX::DeviceResources>& device_resources);
	void Update(const InputCommands& input_commands);
	static bool IsInstanceMade() { return isInstanceMade; }

	void TextureChange();
	void MultiTextureChange();
	void RemoveTextureChange(int idToRemove);

	DisplayObject GetDisplayObject();
	void SetDisplayObject(const DisplayObject& newObjectParams);
	void RollBackChanges();
	void SpawnObject();


	bool isEditing = false;
	int selectedId;
	std::vector<int> selectedObjects;
	std::stack<DisplayObject> objectHistory;

protected:
	ObjectHandler();

private:
	std::vector<DisplayObject>* allDisplayObjects;
	std::shared_ptr<DX::DeviceResources> m_device_resource;
	static bool isInstanceMade;
};

// Implementations

inline ObjectHandler::ObjectHandler()
{
} // Private default constructor
