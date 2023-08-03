#include "ECS.h"

void Entity::AddGroup(Group group)
{
	m_groupBitset[group] = true;
	m_manager.AddToGroup(this, group);
}
