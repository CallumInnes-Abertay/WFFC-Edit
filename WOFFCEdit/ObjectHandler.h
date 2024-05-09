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
		m_isInstanceMade = true;
		return instance;
	}

	ObjectHandler(const ObjectHandler&) = delete; // Disable copy constructor
	void operator=(const ObjectHandler&) = delete; // Disable assignment operator

	void Initialise(std::vector<DisplayObject>* startingObjects,
	                const std::shared_ptr<DX::DeviceResources>& device_resources);
	void Update(const InputCommands& input_commands);
	static bool IsInstanceMade() { return m_isInstanceMade; }

	void MultiTextureChange();
	void RemoveTextureChange(int idToRemove);
	void RemoveAllTextureChanges();

	DisplayObject GetLastSelectedDisplayObject();
	DisplayObject GetObject(int id);
	void SetDisplayObject(const DisplayObject& newObjectParams);
	void UndoChanges();
	void SpawnObject();
	void SpawnObject(DisplayObject objectToSpawn);


	void DeleteObjects();
	void Copy();
	void Paste();


	bool m_isEditing = false;
	std::vector<int> m_selectedObjects;
	std::stack<DisplayObject> m_objectHistory;

protected:
	ObjectHandler();

private:
	std::vector<DisplayObject>* m_allDisplayObjects;

	//Seperate vector to selected objects so users can still select without worrying about messing with their copied objects.
	std::vector<DisplayObject> m_objectsToCopy;
	std::shared_ptr<DX::DeviceResources> m_device_resource;
	static bool m_isInstanceMade;
};

// Implementations

inline ObjectHandler::ObjectHandler()
{
} // Private default constructor
