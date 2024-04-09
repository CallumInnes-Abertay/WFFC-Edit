#include "ObjectHandler.h"

#include "DisplayObject.h"

ObjectHandler::ObjectHandler()
{
	selectedId = -1;
}

ObjectHandler::ObjectHandler(std::vector<DisplayObject>* startingObjects)
{
	selectedId = -1;
	allObjects = startingObjects;
}

void ObjectHandler::Initialise(std::vector<DisplayObject>* startingObjects)
{
	selectedId = -1;
	allObjects = startingObjects;
	//defaultSize = (*startingObjects)[0].m_scale;
}

void ObjectHandler::TextureChange()
{
	if (selectedId == -1) return;

	for (int i = 0; i < allObjects->size(); ++i)
	{
		if ((*allObjects)[i].m_ID == selectedId)
		{
			(*allObjects)[i].m_wireframe = true;
		}
		{
			(*allObjects)[i].m_wireframe = false;

		}
	}
}
