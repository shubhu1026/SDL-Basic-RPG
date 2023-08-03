#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID GetNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T> inline ComponentID GetComponentTypeID() noexcept
{
	static ComponentID typeID = GetNewComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;

using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
	Entity* entity;

	virtual void Init() {}
	virtual void Update() {}
	virtual void Draw() {}

	virtual ~Component() {}
};

class Entity
{
private:
	Manager& m_manager;
	bool m_active = true;
	std::vector<std::unique_ptr<Component>> m_components;

	ComponentArray m_componentArray;
	ComponentBitSet m_componentBitset;
	GroupBitSet m_groupBitset;

public:
	Entity(Manager& manager) : m_manager(manager) {}

	void Update()
	{
		for (auto& c : m_components) c->Update();
	}

	void Draw()
	{
		for (auto& c : m_components) c->Draw();
	}

	bool IsActive() { return m_active; }
	void Destroy() { m_active = false; }

	bool HasGroup(Group group)
	{
		return m_groupBitset[group];
	}

	void AddGroup(Group group);

	void DelGroup(Group group)
	{
		m_groupBitset[group] = false;
	}
		
	template <typename T> bool HasComponent() const
	{
		return m_componentBitset[GetComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs>
	T& AddComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		m_components.emplace_back(std::move(uPtr));

		m_componentArray[GetComponentTypeID<T>()] = c;
		m_componentBitset[GetComponentTypeID<T>()] = true;

		c->Init();
		return *c;
	}

	template<typename T> T& GetComponent() const
	{
		auto ptr(m_componentArray[GetComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};

class Manager
{
private:
	std::vector<std::unique_ptr<Entity>> m_entities;
	std::array<std::vector<Entity*>, maxGroups> m_groupedEntites;

public:
	void Update()
	{
		for (auto& e : m_entities) e->Update();
	}

	void Draw()
	{
		for (auto& e : m_entities) e->Draw();
	}

	void Refresh()
	{
		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v(m_groupedEntites[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v),
					[i](Entity* entity)
					{
						return !entity->IsActive() || !entity->HasGroup(i);
					}
				),
				std::end(v));
		}

		m_entities.erase(std::remove_if(std::begin(m_entities), std::end(m_entities),
			[](const std::unique_ptr<Entity>& entity)
			{
				return !entity->IsActive();
			}), 
			std::end(m_entities));
	}

	void AddToGroup(Entity* entity, Group group)
	{
		m_groupedEntites[group].emplace_back(entity);
	}

	std::vector<Entity*>& GetGroup(Group group)
	{
		return m_groupedEntites[group];
	}

	Entity& AddEntity()
	{
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		m_entities.emplace_back(std::move(uPtr));

		return *e;
	}
};

