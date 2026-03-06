#include "./test_things.hpp"
#include <Nostalgia/physics/engine.hpp>
#include <Nostalgia/theatre/theatre.hpp>

void CollisionTester3D::SetVariables(Farg<TheatreFile::ThingData> data)
{
    auto outData{data};
    outData.set_variable(MotionType::Dynamic, "Motion");
    Collider3D::SetVariables(outData);


    data.get_variable(mGravity, "Gravity", "GravityFactor");
    data.get_variable(mMovementSpeed, "Speed");
    if(data.get_variable(mMovementDirection, "Direction") == OK)
        { mMovementDirection = glm::normalize(mMovementDirection); }
}

Shared<TheatreFile::ThingData> CollisionTester3D::GetVariables() const
{
    auto data{Collider3D::GetVariables()};

    data->set_variable(mGravity, "Gravity");
    data->set_variable(glm::normalize(mMovementDirection), "Direction");
    data->set_variable(mMovementSpeed, "Speed");

    return data;
}

void CollisionTester3D::Ready()
{
    Collider3D::Ready();

    PhysicsEngine::Inst()->BodyInterface().SetGravityFactor(mBodyID, mGravity);
    PhysicsEngine::Inst()->BodyInterface().AddForce(mBodyID,
        Math::Convert<JPH::Vec3>(mMovementDirection * mMovementSpeed));
}

void CollisionTester3D::Update()
{}

void CollisionTester3D::Tick()
{
    if(mBodyID.IsInvalid())
        { return; }

    auto phys_engine{PhysicsEngine::Instance()};
    auto& body_interface{phys_engine->BodyInterface()};

    // Avoid updating global transforms every tick by only updating rotation and position when they don't match
    // with the Jolt physics body's rotation and position.
    //
    // This is already done by `Collider3D::Tick` in Nostalgia v0.1.7, but this app uses v0.1.6, currently. Thanks
    // to not calling `Collider3D::Tick`, however, I can just implement it here and keep on truckin.
    if(auto new_quat{Math::Convert<glm::quat>(body_interface.GetRotation(mBodyID))};
        new_quat != mLocalTransform.quaternion)
            { Actor3D::SetQuaternion(new_quat); }
    if(auto new_pos{Math::Convert<glm::vec3>(body_interface.GetCenterOfMassPosition(mBodyID))};
        new_pos != mLocalTransform.position)
            { Actor3D::SetPosition(new_pos); }
}
