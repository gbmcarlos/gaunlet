#include "PhysicsWorld.h"

namespace engine {

    PhysicsWorld::PhysicsWorld(glm::vec2 gravity) {

        m_physicsWorld = new b2World({gravity.x, gravity.y});
        m_physicsWorld->SetAutoClearForces(false);
        m_physicsWorld->SetAllowSleeping(true);

    }

    PhysicsWorld::~PhysicsWorld() {

        if (m_physicsWorld) {
            destroyPhysics();
        }

    }

    void PhysicsWorld::createRigidBody(RigidBodyComponent& rigidBody, TransformComponent transform) {

        b2BodyDef bodyDefinition;
        bodyDefinition.type = convertRigidBodyType(rigidBody.m_type);
        bodyDefinition.position.Set(transform.m_translation.x, transform.m_translation.y);
        bodyDefinition.fixedRotation = rigidBody.m_fixedRotation;
        bodyDefinition.angle = glm::radians(transform.m_rotation.z);
        bodyDefinition.allowSleep = true;

        b2Body* body = m_physicsWorld->CreateBody(&bodyDefinition);
        rigidBody.m_runtimeBody = body;

    }

    void PhysicsWorld::createBoxFixture(b2Body* body, BoxColliderComponent& boxCollider, TransformComponent transform) {

        b2PolygonShape shape;
        shape.SetAsBox(
            (boxCollider.m_size.x * transform.m_scale.x) + boxCollider.m_padding.x,
            (boxCollider.m_size.y * transform.m_scale.y) + boxCollider.m_padding.y
        );

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = boxCollider.m_density;
        fixtureDef.friction = boxCollider.m_friction;
        fixtureDef.restitution = boxCollider.m_restitution;
        fixtureDef.restitutionThreshold = boxCollider.m_restitutionThreshold;

        auto fixture = body->CreateFixture(&fixtureDef);
        boxCollider.m_runtimeFixture = fixture;

    }

    void PhysicsWorld::createCircleFixture(b2Body* body, CircleColliderComponent& circleCollider, TransformComponent transform) {

        b2CircleShape shape;

        float scale = std::max(transform.m_scale.x, transform.m_scale.x);
        shape.m_p.Set(0.0f, 0.0f); // Position the fixture relative to the center of the body' position
        shape.m_radius = (circleCollider.m_radius * scale) + circleCollider.m_padding;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = circleCollider.m_density;
        fixtureDef.friction = circleCollider.m_friction;
        fixtureDef.restitution = circleCollider.m_restitution;
        fixtureDef.restitutionThreshold = circleCollider.m_restitutionThreshold;

        auto fixture = body->CreateFixture(&fixtureDef);
        circleCollider.m_runtimeFixture = fixture;

    }

    void PhysicsWorld::simulatePhysics(TimeStep timeStep) {

        // This is how much time is unprocessed
        m_fixedTimeStepAccumulator += timeStep.getSeconds();

        // How many steps can we take, with a fixed timestemp
        const int nSteps = static_cast<int> (std::floor (m_fixedTimeStepAccumulator / FIXED_TIMESTEP));

        // Subtract the time that we are going to process
        if (nSteps > 0) {
            m_fixedTimeStepAccumulator -= nSteps * FIXED_TIMESTEP;
        }

        // Avoid having too many sub-steps
        const int nStepsClamped = std::min(nSteps, 5);

        for (int i = 0; i < nStepsClamped; ++ i){
            m_physicsWorld->Step(FIXED_TIMESTEP, 10, 6);
        }

        m_physicsWorld->ClearForces();

    }

    void PhysicsWorld::updateBody(RigidBodyComponent& rigidBody, TransformComponent& transform) {

        auto* body = (b2Body*) rigidBody.m_runtimeBody;
        const auto& position = body->GetPosition();
        transform.m_translation.x = position.x;
        transform.m_translation.y = position.y;
        transform.m_rotation.z = glm::degrees(body->GetAngle());

    }

    void PhysicsWorld::destroyPhysics() {

        m_physicsWorld->ClearForces();
        delete m_physicsWorld;

    }

}