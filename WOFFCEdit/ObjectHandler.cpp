#include "ObjectHandler.h"

#include "InputCommands.h"


void ObjectHandler::Initialise(std::vector<DisplayObject>* startingObjects,
                               const std::shared_ptr<DX::DeviceResources>& device_resources)
{
	m_allDisplayObjects = startingObjects;
	m_device_resource = device_resources;

	// Make sure each object has it's own ID
	for (int i = 0; i < m_allDisplayObjects->size(); ++i)
	{
		(*m_allDisplayObjects)[i].m_ID = i;
	}
}

//Update for object handler, primarily used for object manipulation
void ObjectHandler::Update(const InputCommands& input_commands)
{
	// If there is selected objects
	if (!m_selectedObjects.empty())
	{
		// Then loop through all objects, and if selected then move them accordingly to the arrow keys
		for (int i = 0; i < m_allDisplayObjects->size(); ++i)
		{
			if (std::find(m_selectedObjects.begin(), m_selectedObjects.end(), i) != m_selectedObjects.end())
			{
				//Up
				if (input_commands.upArrowDown)
				{
					(*m_allDisplayObjects)[i].m_position.x += 0.1f;
				}

				//Down
				if (input_commands.downArrowDown)
				{
					(*m_allDisplayObjects)[i].m_position.x -= 0.1f;
				}

				//Left
				if (input_commands.leftArrowDown)
				{
					(*m_allDisplayObjects)[i].m_position.z -= 0.1f;
				}

				//Right
				if (input_commands.rightArrowDown)
				{
					(*m_allDisplayObjects)[i].m_position.z += 0.1f;
				}
			}
		}
	}
}

// Change texture for selected objects or revert to default texture
void ObjectHandler::MultiTextureChange()
{
	//Loop through all objects checking if they should be selected or not
	for (int i = 0; i < m_allDisplayObjects->size(); ++i)
	{
		//If selected
		if (std::find(m_selectedObjects.begin(), m_selectedObjects.end(), i) != m_selectedObjects.end())
		{
			//Then set the "error" texture to act as a indicator
			DirectX::CreateDDSTextureFromFile(m_device_resource->GetD3DDevice(), L"database/data/error.dds",
			                                  nullptr, &(*m_allDisplayObjects)[i].m_texture_diffuse);
			(*m_allDisplayObjects)[i].m_model->UpdateEffects([&](DirectX::IEffect* effect)
			{
				const auto basic_effect = dynamic_cast<DirectX::BasicEffect*>(effect);
				if (basic_effect)
				{
					basic_effect->SetTexture((*m_allDisplayObjects)[i].m_texture_diffuse);
				}

			});
		}
		//If not selected
		else if (std::find(m_selectedObjects.begin(), m_selectedObjects.end(), i) == m_selectedObjects.end())
		{
			//Then set the "error" texture to act as a default texture, showing it's not selected
			DirectX::CreateDDSTextureFromFile(m_device_resource->GetD3DDevice(), L"database/data/placeholder.dds",
			                                  nullptr,
			                                  &(*m_allDisplayObjects)[i].m_texture_diffuse);
			(*m_allDisplayObjects)[i].m_model->UpdateEffects([&](DirectX::IEffect* effect)
			{
				const auto basic_effect = dynamic_cast<DirectX::BasicEffect*>(effect);
				if (basic_effect)
				{
					basic_effect->SetTexture((*m_allDisplayObjects)[i].m_texture_diffuse);
				}
			});
		}
	}
}
// Remove texture changes for a specific object
void ObjectHandler::RemoveTextureChange(const int idToRemove)
{
	//Loop through all objects
	for (int i = 0; i < m_allDisplayObjects->size(); ++i)
	{
		//If the id matches
		if (idToRemove == i)
		{
			//Then change the texture back to the "default" placeholder
			DirectX::CreateDDSTextureFromFile(m_device_resource->GetD3DDevice(), L"database/data/placeholder.dds",
			                                  nullptr,
			                                  &(*m_allDisplayObjects)[i].m_texture_diffuse);
			(*m_allDisplayObjects)[i].m_model->UpdateEffects([&](DirectX::IEffect* effect)
			{
				const auto basic_effect = dynamic_cast<DirectX::BasicEffect*>(effect);
				if (basic_effect)
				{
					basic_effect->SetTexture((*m_allDisplayObjects)[i].m_texture_diffuse);
				}
			});
		}
	}
}

// Remove texture changes for all objects
void ObjectHandler::RemoveAllTextureChanges()
{
	//Loop through all objects, changing them back to placeholder.dds
	for (int i = 0; i < m_allDisplayObjects->size(); ++i)
	{
		DirectX::CreateDDSTextureFromFile(m_device_resource->GetD3DDevice(), L"database/data/placeholder.dds",
		                                  nullptr,
		                                  &(*m_allDisplayObjects)[i].m_texture_diffuse);
		(*m_allDisplayObjects)[i].m_model->UpdateEffects([&](DirectX::IEffect* effect)
		{
			const auto basic_effect = dynamic_cast<DirectX::BasicEffect*>(effect);
			if (basic_effect)
			{
				basic_effect->SetTexture((*m_allDisplayObjects)[i].m_texture_diffuse);
			}
		});
	}
}

//Get the last selected object
DisplayObject ObjectHandler::GetLastSelectedDisplayObject()
{
	//Loop through all objects
	for (int i = 0; i < m_allDisplayObjects->size(); ++i)
	{
		if (m_selectedObjects.empty()) return {};
		//If the object is the last one the user has selected then return that one
		if (i == (m_selectedObjects.back()))
		{
			return (*m_allDisplayObjects)[i];
		}
	}
	//Else return null
	return {};
}


//Get a specific object by its ID.
DisplayObject ObjectHandler::GetObject(int id)
{
	for (int i = 0; i < m_allDisplayObjects->size(); ++i)
	{
		if (i == id)
		{
			return (*m_allDisplayObjects)[i];
		}
	}
	return {};
}

//Set a display object with new parameters to override it with
void ObjectHandler::SetDisplayObject(const DisplayObject& newObjectParams)
{
	//Loop through all objects
	for (int i = 0; i < m_allDisplayObjects->size(); ++i)
	{
		//If it matches to the one to change
		if (i == newObjectParams.m_ID)
		{
			//Change position,scale,rotation
			(*m_allDisplayObjects)[i].m_position = newObjectParams.m_position;
			(*m_allDisplayObjects)[i].m_scale = newObjectParams.m_scale;
			(*m_allDisplayObjects)[i].m_orientation = newObjectParams.m_orientation;
			//No point continuing to iterate if object changed
			return;
		}
	}
}

// Allow undoing changes and return an object to it's previous state.
void ObjectHandler::UndoChanges()
{
	//If theres nothing to undo, just return early.
	if (m_objectHistory.empty()) return;

	//If there is get the top most object 
	const DisplayObject oldObject = m_objectHistory.top();
	m_objectHistory.pop();

	//And then update it's specific object
	SetDisplayObject(oldObject);
}

// Spawn a new display object with default parameters (you would usually run the overloaded method with parameters)
void ObjectHandler::SpawnObject()
{
	DisplayObject objectToSpawn;
	objectToSpawn.m_model = (*m_allDisplayObjects)[0].m_model;
	DirectX::CreateDDSTextureFromFile(m_device_resource->GetD3DDevice(), L"database/data/placeholder.dds",
	                                  nullptr, &objectToSpawn.m_texture_diffuse);
	objectToSpawn.m_model->UpdateEffects([&](DirectX::IEffect* effect)
	{
		const auto basic_effect = dynamic_cast<DirectX::BasicEffect*>(effect);
		if (basic_effect)
		{
			basic_effect->SetTexture(objectToSpawn.m_texture_diffuse);
		}

	});
	objectToSpawn.m_texture_diffuse = (*m_allDisplayObjects)[0].m_texture_diffuse;


	//Default values purely just for showcase
	objectToSpawn.m_position = Vector3(2, 2, 4);
	objectToSpawn.m_scale = Vector3(1, 1, 1);
	objectToSpawn.m_orientation = Vector3(0, 0, 0);

	//Add to vector of all objects to be rendered
	m_allDisplayObjects->push_back(objectToSpawn);
	for (int i = 0; i < m_allDisplayObjects->size(); i++)
	{
		(*m_allDisplayObjects)[i].m_ID = i;
	}
}

// Overloaded Spawn object, to spawn with certain parameters
void ObjectHandler::SpawnObject(DisplayObject objectToSpawn)
{

	//Set model and texture 
	objectToSpawn.m_model = (*m_allDisplayObjects)[0].m_model;
	DirectX::CreateDDSTextureFromFile(m_device_resource->GetD3DDevice(), L"database/data/placeholder.dds",
	                                  nullptr, &objectToSpawn.m_texture_diffuse);
	objectToSpawn.m_model->UpdateEffects([&](DirectX::IEffect* effect)
	{
		const auto basic_effect = dynamic_cast<DirectX::BasicEffect*>(effect);
		if (basic_effect)
		{
			basic_effect->SetTexture(objectToSpawn.m_texture_diffuse);
		}

		//(*m_allDisplayObjects)[i].m_position.y += 1;
	});
	//Add it to the list of all objects to be rendered successfully.
	m_allDisplayObjects->push_back(objectToSpawn);
	//Account for the new object and update all ids.
	for (int i = 0; i < m_allDisplayObjects->size(); i++)
	{
		(*m_allDisplayObjects)[i].m_ID = i;
	}
}

// Multi delete objects
void ObjectHandler::DeleteObjects()
{
	//Loop through all objects
	for (int i = 0; i < m_selectedObjects.size(); i++)
	{
		int objectId = m_selectedObjects[i];

		// Since display objects can't be directly compared use a lambda to check if the id of one object matches up with the selected object bound for deletion
		auto obj = std::find_if(m_allDisplayObjects->begin(), m_allDisplayObjects->end(),
		                        [objectId](const DisplayObject& currentObj) { return currentObj.m_ID == objectId; });

		//If it does, then erase that from the list of display objects
		if (obj != m_allDisplayObjects->end()) m_allDisplayObjects->erase(obj);
	}

	// After deletion, reassign IDs
	for (int i = 0; i < m_allDisplayObjects->size(); i++)
	{
		(*m_allDisplayObjects)[i].m_ID = i;
	}
	//Then clear objects so the user cant delete already deleted objects (or if the ids have been swapped, delete objects they're no longer selecting.
	m_selectedObjects.clear();
}

void ObjectHandler::Copy()
{
	// First clear the vector so the copied vector doesn't account for more than the user wanted to copy.
	m_objectsToCopy.clear();
	//Add all selected objects to a separate vector to be later pasted.
	for (int i = 0; i < m_allDisplayObjects->size(); i++)
	{
		// If one of the display objects is the same as the selected object, then add that to a vector of objects to be later copied.
		if ((std::find(m_selectedObjects.begin(), m_selectedObjects.end(), i) != m_selectedObjects.end()))
		{
			m_objectsToCopy.push_back((*m_allDisplayObjects)[i]);
		}
	}
}
// Paste the copied objects
void ObjectHandler::Paste()
{
	if (m_objectsToCopy.empty()) return;
	// Loop through all objects that are made to be copied
	for (int i = 0; i < m_objectsToCopy.size(); i++)
	{
		DisplayObject objectToPaste;
		//Creating a new object with these parameters
		objectToPaste = m_objectsToCopy[i];
		////Setting their id temporarily to the size of the vector
		//objectToPaste.m_ID = m_allDisplayObjects->size();

		//// And copying the vector
		//objectToPaste.m_model = m_objectsToCopy[i].m_model;

		////apply new texture to models effect
		//objectToPaste.m_model->UpdateEffects(
		//	[&](DirectX::IEffect* effect)
		//	{
		//		auto lights = dynamic_cast<DirectX::BasicEffect*>(effect);
		//		if (lights)
		//		{
		//			lights->SetTexture(m_objectsToCopy[i].m_texture_diffuse);
		//		}
		//	});

		//Set up the position (but above the original by a bit to highlight they've been pasted)
		Vector3 pos(m_objectsToCopy[i].m_position.x, m_objectsToCopy[i].m_position.y + 2,
		            m_objectsToCopy[i].m_position.z);

		//Sets the new object to the 
		objectToPaste.m_position = pos;
		//objectToPaste.m_scale = m_objectsToCopy[i].m_scale;
		//objectToPaste.m_orientation = m_objectsToCopy[i].m_orientation;

		//Add that to the list of all objects.
		m_allDisplayObjects->push_back(objectToPaste);


	}

	//Reset all IDs
	for (int i = 0; i < m_allDisplayObjects->size(); ++i)
	{
		(*m_allDisplayObjects)[i].m_ID = i;
	}
}

void ObjectHandler::SelectAll()
{
	m_selectedObjects.clear();
	for (int i = 0; i < m_allDisplayObjects->size(); ++i)
	{
		m_selectedObjects.push_back(i);
	}
	MultiTextureChange();
}

bool ObjectHandler::m_isInstanceMade = false;
