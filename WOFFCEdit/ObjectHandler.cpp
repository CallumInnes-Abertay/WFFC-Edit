#include "ObjectHandler.h"

#include "InputCommands.h"


void ObjectHandler::Initialise(std::vector<DisplayObject>* startingObjects,
                               const std::shared_ptr<DX::DeviceResources>& device_resources)
{
	m_allDisplayObjects = startingObjects;
	m_device_resource = device_resources;


	for (int i = 0; i < m_allDisplayObjects->size(); ++i)
	{
		(*m_allDisplayObjects)[i].m_ID = i;
	}
}

void ObjectHandler::Update(const InputCommands& input_commands)
{
	if (!m_selectedObjects.empty())
	{
		for (int i = 0; i < m_allDisplayObjects->size(); ++i)
		{
			if (std::find(m_selectedObjects.begin(), m_selectedObjects.end(), i) != m_selectedObjects.end())
			{
				if (input_commands.upArrowDown)
				{
					(*m_allDisplayObjects)[i].m_position.x += 0.1f;
				}

				if (input_commands.downArrowDown)
				{
					(*m_allDisplayObjects)[i].m_position.x -= 0.1f;
				}

				if (input_commands.leftArrowDown)
				{
					(*m_allDisplayObjects)[i].m_position.z -= 0.1f;
				}

				if (input_commands.rightArrowDown)
				{
					(*m_allDisplayObjects)[i].m_position.z += 0.1f;
				}
			}
		}
	}
}


void ObjectHandler::MultiTextureChange()
{
	//We didn't select anything so don't bother and return early.
	//if (m_selectedObjects.empty()) return;

	for (int i = 0; i < m_allDisplayObjects->size(); ++i)
	{
		if (std::find(m_selectedObjects.begin(), m_selectedObjects.end(), i) != m_selectedObjects.end())
		{
			DirectX::CreateDDSTextureFromFile(m_device_resource->GetD3DDevice(), L"database/data/error.dds",
			                                  nullptr, &(*m_allDisplayObjects)[i].m_texture_diffuse);
			(*m_allDisplayObjects)[i].m_model->UpdateEffects([&](DirectX::IEffect* effect)
			{
				const auto basic_effect = dynamic_cast<DirectX::BasicEffect*>(effect);
				if (basic_effect)
				{
					basic_effect->SetTexture((*m_allDisplayObjects)[i].m_texture_diffuse);
				}

				//(*m_allDisplayObjects)[i].m_position.y += 1;
			});
		}
		else if (std::find(m_selectedObjects.begin(), m_selectedObjects.end(), i) == m_selectedObjects.end())
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
}

void ObjectHandler::RemoveTextureChange(int idToRemove = -1)
{
	for (int i = 0; i < m_allDisplayObjects->size(); ++i)
	{
		if (idToRemove == i)
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
}

void ObjectHandler::RemoveAllTextureChanges()
{
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

DisplayObject ObjectHandler::GetDisplayObject()
{
	for (int i = 0; i < m_allDisplayObjects->size(); ++i)
	{
		if (m_selectedObjects.empty()) return DisplayObject();
		if (i == (m_selectedObjects.back()))
		{
			return (*m_allDisplayObjects)[i];
		}
	}
	return DisplayObject();
}

void ObjectHandler::SetDisplayObject(const DisplayObject& newObjectParams)
{
	for (int i = 0; i < m_allDisplayObjects->size(); ++i)
	{
		if (i == newObjectParams.m_ID)
		{
			(*m_allDisplayObjects)[i].m_position = newObjectParams.m_position;
			(*m_allDisplayObjects)[i].m_scale = newObjectParams.m_scale;
			(*m_allDisplayObjects)[i].m_orientation = newObjectParams.m_orientation;
			return;
		}
	}
}

void ObjectHandler::RollBackChanges()
{
	if (m_objectHistory.empty()) return;
	const DisplayObject oldObject = m_objectHistory.top();
	m_objectHistory.pop();

	SetDisplayObject(oldObject);
}

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

		//(*m_allDisplayObjects)[i].m_position.y += 1;
	});
	objectToSpawn.m_texture_diffuse = (*m_allDisplayObjects)[0].m_texture_diffuse;


	objectToSpawn.m_position = Vector3(2, 2, 4);
	objectToSpawn.m_scale = Vector3(1, 1, 1);
	objectToSpawn.m_orientation = Vector3(0, 0, 0);


	m_allDisplayObjects->push_back(objectToSpawn);
	for (int i = 0; i < m_allDisplayObjects->size(); i++)
	{
		(*m_allDisplayObjects)[i].m_ID = i;
	}
}

void ObjectHandler::SpawnObject(DisplayObject objectToSpawn)
{
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
	objectToSpawn.m_texture_diffuse = (*m_allDisplayObjects)[0].m_texture_diffuse;


	objectToSpawn.m_position = Vector3(2, 2, 4);
	objectToSpawn.m_scale = Vector3(1, 1, 1);
	objectToSpawn.m_orientation = Vector3(0, 0, 0);


	m_allDisplayObjects->push_back(objectToSpawn);
	for (int i = 0; i < m_allDisplayObjects->size(); i++)
	{
		(*m_allDisplayObjects)[i].m_ID = i;
	}
}

void ObjectHandler::DeleteObjects()
{
	for (int i = 0; i < m_selectedObjects.size(); i++)
	{
		int objectId = m_selectedObjects[i];
		auto obj = std::find_if(m_allDisplayObjects->begin(), m_allDisplayObjects->end(),
		                        [objectId](const DisplayObject& obj) { return obj.m_ID == objectId; });

		if (obj != m_allDisplayObjects->end()) m_allDisplayObjects->erase(obj);
	}

	// After deletion, reassign IDs
	for (int i = 0; i < m_allDisplayObjects->size(); i++)
	{
		(*m_allDisplayObjects)[i].m_ID = i;
	}
	m_selectedObjects.clear();
}

void ObjectHandler::Copy()
{
	m_objectsToCopy.clear();
	//Add all selected objects to a seperate vector to be later lasted
	for (int i = 0; i < m_allDisplayObjects->size(); i++)
	{
		if ((std::find(m_selectedObjects.begin(), m_selectedObjects.end(), i) != m_selectedObjects.end()))
		{
			m_objectsToCopy.push_back((*m_allDisplayObjects)[i]);
		}
	}
}

void ObjectHandler::Paste()
{
	for (int i = 0; i < m_objectsToCopy.size(); i++)
	{
		DisplayObject object;

		object.m_ID = m_allDisplayObjects->size();

		object.m_model = m_objectsToCopy[i].m_model;

		//apply new texture to models effect
		object.m_model->UpdateEffects(
			[&](DirectX::IEffect* effect) //This uses a Lambda function,  if you dont understand it: Look it up.
			{
				auto lights = dynamic_cast<DirectX::BasicEffect*>(effect);
				if (lights)
				{
					lights->SetTexture(m_objectsToCopy[i].m_texture_diffuse);
				}
			});
		Vector3 pos(m_objectsToCopy[i].m_position.x, m_objectsToCopy[i].m_position.y + 2,
		            m_objectsToCopy[i].m_position.z);

		object.m_position = pos;
		object.m_scale = m_objectsToCopy[i].m_scale;
		object.m_orientation = m_objectsToCopy[i].m_orientation;

		m_allDisplayObjects->push_back(object);
	}
}

bool ObjectHandler::m_isInstanceMade = false;
