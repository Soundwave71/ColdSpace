//
// Created by Leonardo on 6/27/2017.
//

#ifndef COLDSPACE_ENTITYMANAGER_H
#define COLDSPACE_ENTITYMANAGER_H

#include "C_Position.h"
#include "C_SpriteSheet.h"
/*#include "C_State.h"
#include "C_Movable.h"
#include "C_Controller.h"
#include "C_Collidable.h"*/
#include "Bitmask.h"
#include "TextureManager.h"
#include "EnumHasher.h"
#include <unordered_map>
#include <vector>
#include <functional>
#include "EntityEvents.h" //ADDED

using EntityId = unsigned int;

using ComponentContainer = std::vector<C_Base*>;
using EntityData = std::pair<Bitmask,ComponentContainer>;
using EntityContainer = std::unordered_map<EntityId,EntityData>;
using ComponentFactory = std::unordered_map<Component,std::function<C_Base*(void)>, EnumClassHash>;

class SystemManager;
class Entity_Manager{
public:
    Entity_Manager(SystemManager* l_sysMgr, TextureManager* l_textureMgr);
    ~Entity_Manager();

    int AddEntity(const Bitmask& l_mask);
    int AddEntity(const std::string& l_entityFile);
    bool RemoveEntity(const EntityId& l_id);

    bool AddComponent(const EntityId& l_entity, const Component& l_component);

    template<class T>
    T* GetComponent(const EntityId& l_entity, const Component& l_component)
    {
        auto itr = m_entities.find(l_entity);
        if (itr == m_entities.end()){ return nullptr; }
        // Found the entity.
        if (!itr->second.first.GetBit((unsigned int)l_component)){ return nullptr; }
        // Component exists.
        auto& container = itr->second.second;
        auto component = std::find_if(container.begin(), container.end(),
                                      [&l_component](C_Base* c){ return c->GetType() == l_component; }); //lambda function to find the right type
        return (component != container.end() ? dynamic_cast<T*>(*component) : nullptr);
    }

    bool RemoveComponent(const EntityId& l_entity, const Component& l_component);
    bool HasComponent(const EntityId& l_entity, const Component& l_component);

    void Purge();
private:
    template<class T>
    void AddComponentType(const Component& l_id){
        m_cFactory[l_id] = []()->C_Base* { return new T(); };
    }

    // Data members
    unsigned int m_idCounter;
    EntityContainer m_entities;
    ComponentFactory m_cFactory;

    SystemManager* m_systems;
    TextureManager* m_textureManager;
};

#endif //COLDSPACE_ENTITYMANAGER_H
