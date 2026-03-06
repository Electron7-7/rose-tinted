#include "./imgui_main_menu.hpp"
#include "./gui_globals.hpp"
#include "thirdparty/DearImGui/imgui.h"
#include "thirdparty/DearImGui/imgui_stdlib.h"
#include <Nostalgia/events/event.hpp>
#include <Nostalgia/application/application.hpp>
#include <Nostalgia/managers/manager.hpp>
#include <Nostalgia/managers/theatre_manager.hpp>

bool ImGuiMainMenu::m_sOpen{true};

void ImGuiMainMenu::SetOpen(bool inIsOpen)
{ m_sOpen = inIsOpen; }

void ImGuiMainMenu::Input(InputEvent* inEvent)
{
    if(inEvent->IsInputAction() and inEvent->IsActive("toggle_main_menu"))
        { GUI::SetInputFocus(m_sOpen = !m_sOpen); }
    else if(inEvent->IsJustPressed(Key::F5) and g_pTheatreManager->LoadNewTheatre(mTheatrePath))
    {
        GUI::SetInputFocus(m_sOpen = false);
        mLastTheatrePath = mTheatrePath;
    }
    else if(inEvent->IsJustPressed(Key::F6))
    {
        g_pTheatreManager->ShutdownTheatre();
        GUI::SetInputFocus(m_sOpen = true);
    }
    else if(inEvent->IsInputAction() and inEvent->IsActive("toggle_fullscreen"))
    {
        MainWindow()->SetWindowMode((MainWindow()->IsFullscreen())
            ? IWindow::WINDOW_MODE_WINDOWED
            : IWindow::WINDOW_MODE_FULLSCREEN);
    }
}

void ImGuiMainMenu::Update()
{
    if(!m_sOpen)
        { return; }
    static auto main_window_flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse};
    if(!ImGui::Begin("MainMenu", nullptr, main_window_flags))
        { ImGui::End(); return; }
    ImGui::InputText("Theatre File Path", &mTheatrePath);
    ImGui::SameLine();
    if(ImGui::Button("Load Theatre") and g_pTheatreManager->LoadNewTheatre(mTheatrePath))
    {
        GUI::SetInputFocus(m_sOpen = false);
        mLastTheatrePath = mTheatrePath;
    }
    ImGui::BeginDisabled(Manager::GetTheatreState() != ManagerEnums::IN_LEVEL);
    if(ImGui::Button("Exit Theatre"))
    {
        g_pTheatreManager->ShutdownTheatre();
        GUI::SetInputFocus(m_sOpen = true);
    }
    ImGui::EndDisabled();
    ImGui::End();
}
