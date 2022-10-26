#pragma once

#include <string>
#include <memory>

#include "NY_Object3D.h"
#include "Colider.h"


class GameObject
{






private:

	std::unique_ptr<Object3d>		object3d;

	std::shared_ptr<BaseCollider>	collider;




};