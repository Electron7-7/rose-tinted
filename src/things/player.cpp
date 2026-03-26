#include "./player.hpp"
#include "gui/gui_globals.hpp"
#include "common/game_state.hpp"
#include <Nostalgia/application/application.hpp>
#include <Nostalgia/things/thinkers/3d/collider_3d.hpp>
#include <Nostalgia/things/thinkers/3d/mesh_instance_3d.hpp>
#include <Nostalgia/events/event.hpp>
#include <Nostalgia/managers/input_manager.hpp>
#include <Nostalgia/physics/engine.hpp>
#include <Nostalgia/settings/engine.hpp>
#include <Nostalgia/settings/world.hpp>
#include <Nostalgia/settings/player.hpp>
#include <Nostalgia/theatre/theatre.hpp>

using namespace TheatreFile;

float RoseTintedPlayer3D::m_sMaxSpeed{10.0f};
float RoseTintedPlayer3D::m_sVelocityMultiplier{5.0f};
float RoseTintedPlayer3D::m_sAngularFriction{0.5f};
float RoseTintedPlayer3D::m_sLinearFriction{0.1f};

void RoseTintedPlayer3D::SetVariables(Farg<ThingData> data)
{
    Super::SetVariables(data);

    data.get_variable(mCockpitOffset, "CockpitOffset");
    data.get_variable(Settings::Player::EnableGravity, "EnableGravity", "Gravity", "Fall");
}

Shared<ThingData> RoseTintedPlayer3D::GetVariables() const
{
    Shared<ThingData> data{Super::GetVariables()};

    data->set_variable(mCockpitOffset, "CockpitOffset");
    data->set_variable(Settings::Player::EnableGravity, "EnableGravity");

    return data;
}

void RoseTintedPlayer3D::Ready()
{
    Super::Ready();

    bool _has_collider{false};
    for(ID child : Children())
    {
        if(Theatre::Current()->DerivedFrom(child, ThingType::Collider3D))
            { _has_collider = true; mMainColliderID = child; }
    }
    if(!_has_collider)
    {
        TheatreFile::ThingData coll_dat{ThingType::Collider3D, "DefaultPlayerCollider"};
        coll_dat.set_variable(mLocalTransform.position, "Origin");
        coll_dat.set_variable(mLocalTransform.quaternion, "Quaternion");
        coll_dat.set_variable(mLocalTransform.scale, "Scale");
        coll_dat.set_variable(MotionType::Dynamic, "Motion");
        coll_dat.set_variable(ShapeType::Box, "Shape");
        mMainColliderID = Theatre::Current()->CreateThing(coll_dat);
        mLocalTransform.scale = glm::vec3{1.0f};
        PhysicsEngine::Instance()->BodyInterface().SetGravityFactor(Theatre::Current()
            ->GetThinker<Collider3D>(mMainColliderID)->id(), 0);
    }

    if(auto cockpit{Theatre::Current()->GetThing("Cockpit")};
        not cockpit->uid().invalid())
            { mCockPitID = cockpit->uid(); }

    GUI::CloseAll();
    GUI::SetInputFocus(false);
    MainWindow()->SetMouseMode(IWindow::MOUSE_MODE_DISABLED);
    mCaptureKeyboard = mCaptureMouse = true;
    GameState::PlayerColliderID = mMainColliderID;
}

void RoseTintedPlayer3D::Shutdown()
{
    GameState::PlayerColliderID = ID::Invalid;
    MainWindow()->SetMouseMode(IWindow::MOUSE_MODE_VISIBLE);
    GUI::SetInputFocus(true);
    GUI::OpenAll();
}

void RoseTintedPlayer3D::Tick()
{
    if(mCaptureKeyboard)
    {
        mThrust = InputManager::IsActionDown("forward") -
            InputManager::IsActionDown("backward");
        mYaw = InputManager::IsActionDown("turn_left") -
            InputManager::IsActionDown("turn_right");
        mPitch = InputManager::IsActionDown("tilt_up") -
            InputManager::IsActionDown("tilt_down");
        mRoll = InputManager::IsActionDown("roll_left") -
            InputManager::IsActionDown("roll_right");
    }
    else
        { mYaw = mPitch = mRoll = mThrust = 0.0f; }

    auto collider{Theatre::Current()->GetThinker<Collider3D>(mMainColliderID)};
    auto phys{PhysicsEngine::Instance()};
    auto& body_interface{phys->BodyInterface()};
    FAUTO quaternion{Quaternion()};

    glm::vec3 torque{ quaternion * glm::vec3{mPitch, mYaw, mRoll} * m_sVelocityMultiplier };
    glm::vec3 thrust{ quaternion * Settings::World::Front() * mThrust };

    JPH::Vec3 angular{ body_interface.GetAngularVelocity(collider->id()) };
    JPH::Vec3 linear{ body_interface.GetLinearVelocity(collider->id()) };
    JPH::Vec3 linear_air_resistance{JPH::Vec3::sZero()};
    JPH::Vec3 angular_air_resistance{JPH::Vec3::sZero()};

    for(short i{0}; i < 3; ++i)
    {
        if(std::abs(angular[i]) > 0.0f)
            { angular_air_resistance.SetComponent(i, (-angular[i] * 0.1f) * m_sAngularFriction); }
        if(std::abs(linear[i]) > 0.0f)
            { linear_air_resistance.SetComponent(i, (-linear[i] * 0.1f) * m_sLinearFriction); }
    }

    body_interface.AddTorque(collider->id(), Math::Convert<JPH::Vec3>(torque));
    body_interface.AddLinearAndAngularVelocity(collider->id(),
        Math::Convert<JPH::Vec3>(thrust) + linear_air_resistance,
        angular_air_resistance);
    body_interface.SetMaxLinearVelocity(collider->id(), m_sMaxSpeed);

    SetPosition(collider->Position());
    if(not InputManager::IsKeyDown(Key::LeftAlt))
        { SetQuaternion(collider->Quaternion()); }
    else if(mCaptureMouse)
    {
        mLookWish = InputManager::MouseMotion() * Settings::Player::MouseSensitivity * Settings::Player::MouseSensitivityScale;
        SetRotationDegrees(RotationDegrees() - glm::vec3{mLookWish.y, mLookWish.x, 0.0f});
    }
    if(not mCockPitID.invalid())
    {
        auto cockpit{Theatre::Current()->GetThinker<Actor3D>(mCockPitID)};
        cockpit->SetPosition(collider->Position());
        cockpit->SetQuaternion(collider->Quaternion());
    }
}

ID RoseTintedPlayer3D::GetMainColliderID() const
{ return mMainColliderID; }
