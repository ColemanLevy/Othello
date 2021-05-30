//A large header file containting multiple class and the manager that controls all the
//objects in the game and their activity
#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

//Creation of the Component and Entity classes
class Component;
class Entity;

using ComponentID = std::size_t;

//A small function that creates the ID numbers for the components based on the total number of component types
inline ComponentID getComponentTypeID()
{
	static ComponentID lastID = 0;
	return lastID++;
}

//A small function that returns the ID of the component called upon
template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	static ComponentID typeID = getComponentTypeID();
	return typeID;
}

//Gives the max possible components an entity may have
constexpr std::size_t maxComponents = 6;
//Controls the number of components on an entity
using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

//The component class, which allows for each component to have overrides of the init, update, 
//and draw functions
class Component
{
public:
	//The entity which uses the component assigned to it
	Entity* entity;

	//The function overrides
	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	//The deconstructor for the component
	virtual ~Component() {}
};

//The class for all entities(aka game objects) in the game
class Entity
{
private:
	//A boolean value which, once turned to false, removes the entity from the program
	bool active = true;
	//Gives unique pointers to each component added to the entity
	std::vector<std::unique_ptr<Component>> components;

	//The array that holds each component attached to the entity and the bitset that
	//limits the number of possible components
	ComponentArray componentArray;
	ComponentBitSet componentBitSet;

public:
	//Allows each component's update function to run through the game's update function
	void update()
	{
		for (auto& c : components) c->update();
	}

	//Allows each component's draw function to run through the game's draw function
	void draw()
	{
		for (auto& c : components) c->draw();
	}

	//A small function that returns if the object is active used by the manager
	bool isActive() { return active; }
	//A small function that changes active to false to set it to be removed by the manager
	void destroy() { active = false; }

	//A small function that returns if the entity has a given component
	template <typename T> bool hasComponent() const
	{
		return componentBitSet[getComponentTypeID<T>];
	}

	//This function adds a specified component to an entity
	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		//Creates a new instance of the specified component
		T* c(new T(std::forward<TArgs>(mArgs)...));
		//Sets the entity that the component will be attached to
		c->entity = this;
		//Give the new instance a unique pointer
		std::unique_ptr<Component> uPtr{ c };
		//Shifts the new component to the end of the componentArray
		components.emplace_back(std::move(uPtr));

		//Sets the component's ID into the entity's array
		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		//Initializes the component on the entity
		c->init();
		//Returns the new component's address for the entity to use
		return *c;
	}

	//A small function that simply returns an entity's specified component if it has one
	template<typename T> T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};

//The class that largely handles the creation, rendering, updating, and deletion of every entity
class Manager
{
private:
	//A list of unique pointers representing every entity in the game 
	std::vector<std::unique_ptr<Entity>> entities;

public:
	//Updates every entity each frame
	void update()
	{
		for (auto& e : entities) e->update();
	}
	//Draws every entity each frame
	void draw()
	{
		for (auto& e : entities) e->draw();
	}
	//Every frame, checks each entity to see if they are active and removes the entity if it is not
	void refresh()
	{
		entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<Entity> &mEntity)
		{
			return !mEntity->isActive();
		}),
			std::end(entities));
	}
	//The function that creates a new entity whenever an entity is added through code
	Entity& addEntity()
	{
		//Creates a new empty entity
		Entity* e = new Entity();
		//Give the entity a new unique pointer
		std::unique_ptr<Entity> uPtr{ e };
		//Adds the entity to the list of entities managed by the manager
		entities.emplace_back(std::move(uPtr));
		//Returns the new entity's address for the game to use
		return *e;
	}
};