#pragma once
#include "DisplayChunk.h"

using namespace DirectX::SimpleMath;

#include <vector>
class DisplayObject;

class ObjectHandler
{
public:
	ObjectHandler();
	ObjectHandler(std::vector<DisplayObject>* startingObjects);

	void Initialise(std::vector<DisplayObject>* startingObjects);

	int selectedId;
	std::vector<int>* selectedObjects;
	std::vector<DisplayObject>* allObjects;

	void TextureChange();

protected:

private:
	Vector3 defaultSize;

};
