#include "./test_things.hpp"
#include "app/rose_tinted.hpp"
#include "common/game_state.hpp"
#include <Nostalgia/physics/engine.hpp>
#include <Nostalgia/theatre/theatre.hpp>

void CollisionTester3D::SetVariables(Farg<TheatreFile::ThingData> data)
{
    auto outData{data};
    outData.set_variable(MotionType::Dynamic, "Motion");
    Super::SetVariables(outData);

    data.get_variable(mGravity, "Gravity", "GravityFactor");
    data.get_variable(mMovementSpeed, "Speed");
    if(data.get_variable(mMovementDirection, "Direction") == OK)
        { mMovementDirection = glm::normalize(mMovementDirection); }
}

Shared<TheatreFile::ThingData> CollisionTester3D::GetVariables() const
{
    auto data{Super::GetVariables()};

    data->set_variable(mGravity, "Gravity");
    data->set_variable(glm::normalize(mMovementDirection), "Direction");
    data->set_variable(mMovementSpeed, "Speed");

    return data;
}

void CollisionTester3D::Ready()
{
    Super::Ready();

    PhysicsEngine::Instance()->BodyInterface().SetGravityFactor(mBodyID, mGravity);
    PhysicsEngine::Instance()->BodyInterface().AddForce(mBodyID,
        Math::Convert<JPH::Vec3>(mMovementDirection * mMovementSpeed));
}

void CollisionTester3D::Update()
{
    Super::Update();
}

void CollisionTester3D::Tick()
{
    Super::Tick();
}

void CollisionTester3D::OnContactAdded(ID inOtherColliderID,
    Farg<JPH::Body> inBody1,
    Farg<JPH::Body> inBody2,
    Farg<JPH::ContactManifold> manifold,
    JPH::ContactSettings& ioSettings)
{
    Super::OnContactAdded(inOtherColliderID, inBody1, inBody2, manifold, ioSettings);
    if(GameState::PlayerColliderID == inOtherColliderID)
        { RoseTinted::TriggerGameOver(); }
}
