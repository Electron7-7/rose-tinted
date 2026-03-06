#include "./imgui_debugger.hpp"
#include "things/player.hpp"
#include "things/test_things.hpp"
#include "thirdparty/DearImGui/imgui.h"
#include <Nostalgia/events/event.hpp>
#include <Nostalgia/application/application.hpp>
#include <Nostalgia/managers/manager.hpp>
#include <Nostalgia/managers/theatre_manager.hpp>
#include <Nostalgia/theatre/theatre.hpp>
#include <Nostalgia/theatre/things/thinkers/3d/collider_3d.hpp>

bool ImGuiDebugger::m_sOpen{false};

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
    if(Manager::GetTheatreState() == ManagerEnums::IN_LEVEL)
    {
        auto theatre{g_pTheatreManager->CurrentTheatre()};
        auto player_collider{theatre->GetThinker<CollisionTester3D>(
            theatre->GetThinker<RoseTintedPlayer3D>(UID::a_Player)->GetMainColliderID())};
    }
    ImGui::End();
}
