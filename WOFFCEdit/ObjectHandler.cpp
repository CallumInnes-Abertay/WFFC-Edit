#include "ObjectHandler.h"

#include "InputCommands.h"


void ObjectHandler::Initialise(std::vector<DisplayObject>* startingObjects,
                               const std::shared_ptr<DX::DeviceResources>& device_resources)
{
	allDisplayObjects = startingObjects;
	m_device_resource = device_resources;


	for (int i = 0; i < allDisplayObjects->size(); ++i)
	{
		(*allDisplayObjects)[i].m_ID = i;
	}
}

void ObjectHandler::Update(const InputCommands& input_commands)
{
	if (!selectedObjects.empty())
	{
		for (int i = 0; i < allDisplayObjects->size(); ++i)
		{
			if (std::find(selectedObjects.begin(), selectedObjects.end(), i) != selectedObjects.end())
			{
				if (input_commands.forward)
				{
					(*allDisplayObjects)[i].m_position.x += 0.1f;
				}

				if (input_commands.back)
				{
					(*allDisplayObjects)[i].m_position.x -= 0.1f;
				}
			}
		}
	}
}


void ObjectHandler::MultiTextureChange()
{
	//We didn't select anything so don't bother and return early.
	//if (selectedObjects.empty()) return;

	for (int i = 0; i < allDisplayObjects->size(); ++i)
	{
		if (std::find(selectedObjects.begin(), selectedObjects.end(), i) != selectedObjects.end())
		{
			DirectX::CreateDDSTextureFromFile(m_device_resource->GetD3DDevice(), L"database/data/error.dds",
			                                  nullptr, &(*allDisplayObjects)[i].m_texture_diffuse);
			(*allDisplayObjects)[i].m_model->UpdateEffects([&](DirectX::IEffect* effect)
			{
				const auto basic_effect = dynamic_cast<DirectX::BasicEffect*>(effect);
				if (basic_effect)
				{
					basic_effect->SetTexture((*allDisplayObjects)[i].m_texture_diffuse);
				}

				//(*allDisplayObjects)[i].m_position.y += 1;
			});
		}
		else if (std::find(selectedObjects.begin(), selectedObjects.end(), i) == selectedObjects.end())
		{
			DirectX::CreateDDSTextureFromFile(m_device_resource->GetD3DDevice(), L"database/data/placeholder.dds",
			                                  nullptr,
			                                  &(*allDisplayObjects)[i].m_texture_diffuse);
			(*allDisplayObjects)[i].m_model->UpdateEffects([&](DirectX::IEffect* effect)
			{
				const auto basic_effect = dynamic_cast<DirectX::BasicEffect*>(effect);
				if (basic_effect)
				{
					basic_effect->SetTexture((*allDisplayObjects)[i].m_texture_diffuse);
				}
			});
		}
	}
}

void ObjectHandler::RemoveTextureChange(int idToRemove = -1)
{
	for (int i = 0; i < allDisplayObjects->size(); ++i)
	{
		if (idToRemove == i)
		{
			DirectX::CreateDDSTextureFromFile(m_device_resource->GetD3DDevice(), L"database/data/placeholder.dds",
			                                  nullptr,
			                                  &(*allDisplayObjects)[i].m_texture_diffuse);
			(*allDisplayObjects)[i].m_model->UpdateEffects([&](DirectX::IEffect* effect)
			{
				const auto basic_effect = dynamic_cast<DirectX::BasicEffect*>(effect);
				if (basic_effect)
				{
					basic_effect->SetTexture((*allDisplayObjects)[i].m_texture_diffuse);
				}
			});
		}
	}
}

void ObjectHandler::RemoveAllTextureChanges()
{
	for (int i = 0; i < allDisplayObjects->size(); ++i)
	{
		DirectX::CreateDDSTextureFromFile(m_device_resource->GetD3DDevice(), L"database/data/placeholder.dds",
		                                  nullptr,
		                                  &(*allDisplayObjects)[i].m_texture_diffuse);
		(*allDisplayObjects)[i].m_model->UpdateEffects([&](DirectX::IEffect* effect)
		{
			const auto basic_effect = dynamic_cast<DirectX::BasicEffect*>(effect);
			if (basic_effect)
			{
				basic_effect->SetTexture((*allDisplayObjects)[i].m_texture_diffuse);
			}
		});
	}
}

DisplayObject ObjectHandler::GetDisplayObject()
{
	for (int i = 0; i < allDisplayObjects->size(); ++i)
	{
		if (selectedObjects.empty()) return DisplayObject();
		if (i == (selectedObjects.back()))
		{
			return (*allDisplayObjects)[i];
		}
	}
	return DisplayObject();
}

void ObjectHandler::SetDisplayObject(const DisplayObject& newObjectParams)
{
	for (int i = 0; i < allDisplayObjects->size(); ++i)
	{
		if (i == newObjectParams.m_ID)
		{
			(*allDisplayObjects)[i].m_position = newObjectParams.m_position;
			(*allDisplayObjects)[i].m_scale = newObjectParams.m_scale;
			(*allDisplayObjects)[i].m_orientation = newObjectParams.m_orientation;
			return;
		}
	}
}

void ObjectHandler::RollBackChanges()
{
	if (objectHistory.empty()) return;
	const DisplayObject oldObject = objectHistory.top();
	objectHistory.pop();

	SetDisplayObject(oldObject);
}

void ObjectHandler::SpawnObject()
{
	DisplayObject objectToSpawn;
	objectToSpawn.m_model = (*allDisplayObjects)[0].m_model;
	objectToSpawn.m_texture_diffuse = (*allDisplayObjects)[0].m_texture_diffuse;


	objectToSpawn.m_position = Vector3(2, 2, 4);
	objectToSpawn.m_scale = Vector3(1, 1, 1);
	objectToSpawn.m_orientation = Vector3(0, 0, 0);



	allDisplayObjects->push_back(objectToSpawn);
	for (int i = 0; i < allDisplayObjects->size(); i++)
	{
		(*allDisplayObjects)[i].m_ID = i;
	}
}

void ObjectHandler::DeleteObjects()
{
	for (int i = 0; i < selectedObjects.size(); i++)
	{
		int objectId = selectedObjects[i];
		auto obj = std::find_if(allDisplayObjects->begin(), allDisplayObjects->end(),
		                        [objectId](const DisplayObject& obj) { return obj.m_ID == objectId; });

		if (obj != allDisplayObjects->end()) allDisplayObjects->erase(obj);
	}

	// After deletion, reassign IDs
	for (int i = 0; i < allDisplayObjects->size(); i++)
	{
		(*allDisplayObjects)[i].m_ID = i;
	}
	selectedObjects.clear();

}

bool ObjectHandler::isInstanceMade = false;
