#include "ObjectHandler.h"

#include "DisplayObject.h"

ObjectHandler::ObjectHandler()
{
	selectedId = -1;
}

ObjectHandler::ObjectHandler(const std::shared_ptr<DX::DeviceResources>& device_resources):
	allDisplayObjects(nullptr)
{
	selectedId = -1;
	m_device_resource = device_resources;
}

void ObjectHandler::Initialise(std::vector<DisplayObject>* startingObjects)
{
	selectedId = -1;
	allDisplayObjects = startingObjects;
	//defaultSize = (*startingObjects)[0].m_scale;
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
			DirectX::CreateDDSTextureFromFile(m_device_resource->GetD3DDevice(), L"database/data/placeholder.dds", nullptr,
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
			DirectX::CreateDDSTextureFromFile(m_device_resource->GetD3DDevice(), L"database/data/placeholder.dds", nullptr,
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
