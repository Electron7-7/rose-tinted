#include "./imgui_debugger.hpp"
#include "things/player.hpp"
#include "thirdparty/DearImGui/imgui.h"
#include <Nostalgia/events/event.hpp>
#include <Nostalgia/application/application.hpp>
#include <Nostalgia/managers/manager.hpp>
#include <Nostalgia/physics/engine.hpp>
#include <Nostalgia/theatre/theatre.hpp>
#include <Nostalgia/things/thinkers/3d/collider_3d.hpp>

bool ImGuiDebugger::m_sOpen{true};

void ImGuiDebugger::SetOpen(bool inIsOpen)
{ m_sOpen = inIsOpen; }

void ImGuiDebugger::Input(InputEvent* inEvent)
{
    if(inEvent->IsJustPressed(Key::D) and inEvent->IsModifierActive(Key::Mod_Control))
        { m_sOpen = !m_sOpen; }
}

void ImGuiDebugger::Update()
{
    if(not m_sOpen)
        { return; }
    static auto debugger_flags{ImGuiWindowFlags_MenuBar};
    ImGui::SetNextWindowSize({200, 80}, ImGuiCond_FirstUseEver);
    if(!ImGui::Begin("Debug Window", nullptr, debugger_flags))
        { ImGui::End(); return; }
    ImGui::InputFloat("Player Max Speed", &RoseTintedPlayer3D::m_sMaxSpeed);
    ImGui::InputFloat("Player Velocity Multiplier", &RoseTintedPlayer3D::m_sVelocityMultiplier);
    if(Manager::GetTheatreState() == ManagerEnums::IN_LEVEL)
    {
        auto theatre{Theatre::Current()};
        auto player{theatre->GetThinker<RoseTintedPlayer3D>(UID::o_Player)};
        auto pos1{theatre->GetThinker<Actor3D>(player->mLight1ID)->GlobalPosition()};
        auto pos2{theatre->GetThinker<Actor3D>(player->mLight2ID)->GlobalPosition()};
        ImGui::Text("Light 1 Position: [%f, %f, %f]", pos1[0], pos1[1], pos1[2]);
        ImGui::Text("Light 2 Position: [%f, %f, %f]", pos2[0], pos2[1], pos2[2]);
        ImGui::Text("Thrust: %f", player->mThrust);
        ImGui::Text("Yaw:    %f", player->mYaw);
        ImGui::Text("Pitch:  %f", player->mPitch);
        ImGui::Text("Roll:   %f", player->mRoll);
        FAUTO body_int{PhysicsEngine::Instance()->BodyInterface()};
        auto collider{theatre->GetThinker<Collider3D>(player->GetMainColliderID())};
        auto angular{body_int.GetAngularVelocity(collider->id())};
        auto linear{body_int.GetLinearVelocity(collider->id())};
        ImGui::Text("Angular Velocity: [%f, %f, %f]", angular[0], angular[1], angular[2]);
        ImGui::Text("Linear Velocity: [%f, %f, %f]", linear[0], linear[1], linear[2]);
        ImGui::DragFloat("Angular Friction", &player->m_sAngularFriction);
        ImGui::DragFloat("Linear Friction", &player->m_sLinearFriction);
    }
    ImGui::End();
}
