#pragma once
#ifndef __COLLISION_MANAGER__
#define __COLLISION_MANAGER__

// core libraries
#include <iostream>
#include <SDL_rect.h>

#include "GameObject.h"
#include <glm/gtx/norm.hpp>
#include "Agent.h"
#include "SoundManager.h"

class CollisionManager
{
public:
	static int SquaredDistance(glm::vec2 p1, glm::vec2 p2);
	static bool SquaredRadiusCheck(GameObject* object1, GameObject* object2);

	static bool AABBCheck(GameObject* object1, GameObject* object2);
	static bool AABBCheck(SDL_FRect* rect, GameObject* object);

	static bool LineLineCheck(glm::vec2 line1_start, glm::vec2 line1_end, glm::vec2 line2_start, glm::vec2 line2_end);

	static bool LineRectCheck(glm::vec2 line_start, glm::vec2 line_end, glm::vec2 rect_start, float rect_width, float rect_height);
	static bool LineRectEdgeCheck(glm::vec2 line_start, glm::vec2 rect_start, float rect_width, float rect_height);

	static int MinSquaredDistanceLineLine(glm::vec2 line1_start, glm::vec2 line1_end, glm::vec2 line2_start, glm::vec2 line2_end);
	static bool LineAABBCheck(Agent* object1, GameObject* object2);

	static int CircleAABBSquaredDistance(glm::vec2 circle_centre, int circle_radius, glm::vec2 box_start, int box_width, int box_height);
	static bool CircleAABBCheck(GameObject* object1, GameObject* object2);

	static bool PointRectCheck(glm::vec2 point, glm::vec2 rect_start, float rect_width, float rect_height);

	static bool LOSCheck(Agent* agent, glm::vec2 end_point, const std::vector<DisplayObject*>& objects, DisplayObject* target);

	static void RotateAABB(GameObject* object1, float angle);


	static void CircleCollisionPush(GameObject* object1, GameObject* object2, glm::vec2& mtv, bool push_from_object1 = false);
private:
	CollisionManager();
	~CollisionManager();
};

#endif /* defined (__COLLISION_MANAGER__) */