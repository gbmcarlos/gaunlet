#pragma once

#include "scene/entity/Entity.h"

namespace engine::Scene {

    class NativeScript {

    public:
        virtual ~NativeScript() {}

        // These are utility functions that the native script can call
        template<typename T>
        bool hasComponent() {
            return m_entity.hasComponent<T>();
        }

        template<typename T>
        T& getComponent() {
            return m_entity.getComponent<T>();
        }

    protected:

        // These are the methods to be overridden by the user's native script class
        virtual void onCreate() {}
        virtual void onDelete() {}
        virtual void onUpdate(Core::TimeStep timeStep) {}

    private:
        Entity m_entity;

    friend class Scene;

    };

    struct NativeScriptComponent {

        // The script instance
        NativeScript* m_nativeScriptInstance = nullptr;

        // A pointer to a function that will be used to instantiate the native script. That function returns a pointer to the instance
        NativeScript* (*m_instantiateScriptFunction)() = nullptr;

        // A pointer to a function that will be used to destroy the native script. That function receives a pointer to this component
        void (*m_destroyScriptFunction)(NativeScriptComponent*) = nullptr;

        template<typename T>
        NativeScriptComponent& bind() {

            // Create a lambda that will create a new instance of the native script
            m_instantiateScriptFunction = []() {
                return static_cast<NativeScript*>(new T());
            };

            // Create a lambda that will destroy the native script instance
            m_destroyScriptFunction = [](NativeScriptComponent* nativeScriptComponent) {
                delete nativeScriptComponent->m_nativeScriptInstance;
                nativeScriptComponent->m_nativeScriptInstance = nullptr;
            };

            return *this;

        }

        inline NativeScript* getScriptInstance() {return m_nativeScriptInstance;}

    };

}