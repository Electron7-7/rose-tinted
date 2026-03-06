#include "./player.hpp"
#include "gui/gui_globals.hpp"
#include <Nostalgia/application/application.hpp>
#include <Nostalgia/theatre/things/thinkers/3d/collider_3d.hpp>
#include <Nostalgia/theatre/things/thinkers/3d/mesh_instance_3d.hpp>
#include <Nostalgia/events/event.hpp>
#include <Nostalgia/managers/input_manager.hpp>
#include <Nostalgia/physics/engine.hpp>
#include <Nostalgia/settings/engine.hpp>
#include <Nostalgia/settings/world.hpp>
#include <Nostalgia/settings/player.hpp>
#include <Nostalgia/theatre/theatre.hpp>
#include <Nostalgia/theatre/thing_factory.hpp>

using namespace TheatreFile;

void RoseTintedPlayer3D::SetVariables(Farg<ThingData> data)
{
    NostalgiaPlayer3D::SetVariables(data);

    data.get_variable(mCockpitOffset, "CockpitOffset");
    data.get_variable(Settings::Player::EnableGravity, "EnableGravity", "Gravity", "Fall");
}

Shared<ThingData> RoseTintedPlayer3D::GetVariables() const
{
    Shared<ThingData> data{NostalgiaPlayer3D::GetVariables()};

    data->set_variable(mCockpitOffset, "CockpitOffset");
    data->set_variable(Settings::Player::EnableGravity, "EnableGravity");

    return data;
}

void RoseTintedPlayer3D::Ready()
{
    NostalgiaPlayer3D::Ready();

    bool _has_collider{false};
    for(ID child : Children())
    {
        if(ThingFactory::IsDerivedFrom(my_theatre()->TypeOf(child), ThingType::Collider3D))
            { _has_collider = true; mMainColliderID = child; }
    }
    if(!_has_collider)
    {
        TheatreFile::ThingData coll_dat{ThingType::Collider3D, "DefaultPlayerCollider"};
        coll_dat.set_variable(mLocalTransform.position, "Origin");
        coll_dat.set_variable(mLocalTransform.quaternion, "Quaternion");
        coll_dat.set_variable(mLocalTransform.scale, "Scale");
        coll_dat.set_variable(MotionType::Kinematic, "Motion");
        coll_dat.set_variable(ShapeType::Box, "Shape");
        my_theatre()->SetParent(mMainColliderID = my_theatre()->CreateThing(coll_dat), mUID);
        mLocalTransform.scale = glm::vec3{1.0f};
    }

    if(auto cockpit{my_theatre()->GetThing("Cockpit")};
        not cockpit->uid().invalid())
            { mCockPitID = cockpit->uid(); }

    GUI::CloseAll();
    GUI::SetInputFocus(false);
    MainWindow()->SetMouseMode(IWindow::MOUSE_MODE_DISABLED);
    mCaptureKeyboard = mCaptureMouse = true;
}

void RoseTintedPlayer3D::Tick()
{
    if(mCaptureMouse)
    {
        mLookWish = InputManager::MouseMotion() * Settings::Player::MouseSensitivity * Settings::Player::MouseSensitivityScale;
        SetRotationDegrees(RotationDegrees() - glm::vec3{mLookWish.y, mLookWish.x, 0.0f});
    }
    if(mCaptureKeyboard)
    {
        mMovementDirection.x = InputManager::IsActionDown("+right")   - InputManager::IsActionDown("+left");
        mMovementDirection.z = InputManager::IsActionDown("+forward") - InputManager::IsActionDown("+backward");
    }
    else
        { mMovementDirection.x = mMovementDirection.z = 0.0f; }

    auto collider{my_theatre()->GetThinker<Collider3D>(mMainColliderID)};

    glm::vec3 l_FrontBackVelocity{(Quaternion() * Settings::World::Front()) *
        mMovementDirection.z * Settings::Player::MovementSpeed};
    glm::vec3 l_LeftRightVelocity{(Quaternion() * Settings::World::Right()) *
        mMovementDirection.x * Settings::Player::MovementSpeed};
    glm::vec3 wish_velocity{l_FrontBackVelocity + l_LeftRightVelocity};

    for(int i{0}; i < 3; ++i)
    {
        if(wish_velocity[i] == mVelocity[i])
            { continue; }
        else if(wish_velocity[i] == 0.0f)
            { mVelocity[i] = 0.0f; }
        else
        {
            mVelocity[i] += wish_velocity[i] / Settings::Player::MovementAcceleration;
            if(glm::abs(mVelocity[i]) > glm::abs(wish_velocity[i]))
                { mVelocity[i] = wish_velocity[i]; }
        }
    }

    auto phys{PhysicsEngine::Instance()};
    auto& body_interface{phys->BodyInterface()};
    // auto jolt{phys->System()};

    body_interface.SetLinearVelocity(collider->id(), Math::Convert<JPH::Vec3>(mVelocity));
    body_interface.SetRotation(collider->id(), Math::Convert<JPH::Quat>(glm::normalize(mLocalTransform.quaternion)), JPH::EActivation::Activate);
    SetPosition(collider->Position());
    if(mCockPitID.invalid())
        { return; }
    auto cockpit{my_theatre()->GetThinker<Actor3D>(mCockPitID)};
    cockpit->SetPosition(mLocalTransform.position);
    if(not InputManager::IsKeyDown(Key::LeftAlt))
        { cockpit->SetQuaternion(mLocalTransform.quaternion); }
}

ID RoseTintedPlayer3D::GetMainColliderID() const
{ return mMainColliderID; }
