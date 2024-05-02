#include "ObjectHandler.h"

#include "InputCommands.h"


void ObjectHandler::Initialise(std::vector<DisplayObject>* startingObjects,
                               const std::shared_ptr<DX::DeviceResources>& device_resources)
{
	selectedId = -1;
	allDisplayObjects = startingObjects;
	m_device_resource = device_resources;


	for (int i = 0; i < allDisplayObjects->size(); ++i)
	{
		(*allDisplayObjects)[i].m_ID = i;
	}
}

void ObjectHandler::Update(const InputCommands& input_commands)
{
	if (selectedObjects.empty() && selectedId != -1)
	{
		for (int i = 0; i < allDisplayObjects->size(); ++i)
		{
			if (i == selectedId)
			{
				if (input_commands.forward)
				{
					(*allDisplayObjects)[selectedId].m_position.x += 0.1f;
				}

				if (input_commands.back)
				{
					(*allDisplayObjects)[selectedId].m_position.x -= 0.1f;
				}
			}
		}
	}
	else if (!selectedObjects.empty())
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

void ObjectHandler::TextureChange()
{
	//We didn't select anything so don't bother and return early.
	if (selectedId == -1) return;

	// 
	for (int i = 0; i < allDisplayObjects->size(); ++i)
	{
		if (i == selectedId)
		{
			DirectX::CreateDDSTextureFromFile(m_device_resource->GetD3DDevice(), L"database/data/error.dds",
			                                  nullptr, &(*allDisplayObjects)[selectedId].m_texture_diffuse);
			(*allDisplayObjects)[selectedId].m_model->UpdateEffects([&](DirectX::IEffect* effect)
			{
				const auto basic_effect = dynamic_cast<DirectX::BasicEffect*>(effect);
				if (basic_effect)
				{
					basic_effect->SetTexture((*allDisplayObjects)[selectedId].m_texture_diffuse);
				}

				//(*allDisplayObjects)[selectedId].m_position.y += 1;
			});
		}
		else if (i != selectedId)
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
		else if (std::find(selectedObjects.begin(), selectedObjects.end(), i) != selectedObjects.end())
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

void ObjectHandler::RemoveTextureChange(int idToRemove)
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

DisplayObject ObjectHandler::GetDisplayObject()
{
	for (int i = 0; i < allDisplayObjects->size(); ++i)
	{
		if (i == selectedId)
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
	objectToSpawn = (*allDisplayObjects)[0];
	
	objectToSpawn.m_position = Vector3(2, 2, 4);
	objectToSpawn.m_scale = Vector3(1, 1, 1);
	objectToSpawn.m_orientation = Vector3(0, 0, 0);


	objectToSpawn.m_ID = allDisplayObjects->size() + 1;
	
	allDisplayObjects->push_back(objectToSpawn);
}

bool ObjectHandler::isInstanceMade = false;
