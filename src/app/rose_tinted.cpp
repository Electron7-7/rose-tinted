#include "./rose_tinted.hpp"
#include "gui/gui_globals.hpp"
#include "gui/imgui_implementor.hpp"
#include "gui/imgui_main_menu.hpp"
#include "gui/imgui_debugger.hpp"
#include "things/player.hpp"
#include "things/test_things.hpp"
#include <Nostalgia/application/window.hpp>
#include <Nostalgia/events/event_queue.hpp>
#include <Nostalgia/events/action.hpp>
#include <Nostalgia/managers/physics_manager.hpp>
#include <Nostalgia/managers/theatre_manager.hpp>
#include <Nostalgia/managers/render_manager.hpp>
#include <Nostalgia/managers/input_manager.hpp>
#include <Nostalgia/managers/event_manager.hpp>
#include <Nostalgia/managers/ui_manager.hpp>
#include <Nostalgia/settings/engine.hpp>
#include <Nostalgia/ui/implementor.hpp>
#include <Nostalgia/theatre/thing_factory.hpp>
#include <thread>

bool RoseTinted::m_sIsRunning{false};

void RoseTinted::m_sApplicationRuntimeLoop()
{
    do // app loop
    {

    } while(m_sIsRunning);
}

void RoseTinted::TriggerGameOver()
{
    g_pTheatreManager->ShutdownTheatre();
    GUI::OpenAll();
    MainWindow()->SetMouseMode(IWindow::MOUSE_MODE_VISIBLE);
}

void RoseTinted::Stop()
{ IManager::Stop(); }

int RoseTinted::Main()
{
    mMainWindow = IWindow::CreateNewWindow(IWindow::Properties{std::format("Rose-Tinted Glasses (Using Nostalgia v" NOSTALGIA_VERSION_STRING ")")});

    auto& imgui_impl{UI_Implementor::Create<ImGui_Implementor>()};
    auto& main_menu{imgui_impl->CreateSolution<ImGuiMainMenu>()};
    auto& debugger{imgui_impl->CreateSolution<ImGuiDebugger>()};

    IManager::Add(g_pPhysicsManager);
    IManager::Add(g_pTheatreManager);
    IManager::Add(g_pRenderManager);
    IManager::Add(g_pInputManager);
    IManager::Add(g_pEventManager);
    IManager::Add(g_pUIManager);

    IManager::InitAllManagers();

    ThingFactory::AddThing(&ThingFactory::ThingMakerTemplate<RoseTintedPlayer3D>, "RoseTintedPlayer3D", ThingType::NostalgiaPlayer3D);
    ThingFactory::AddThing(&ThingFactory::ThingMakerTemplate<CollisionTester3D>, "CollisionTester3D", ThingType::Collider3D);

    g_pInputManager->SetAction({"toggle_main_menu", Key::Escape});
    g_pInputManager->SetAction({"toggle_fullscreen", Key::F10});
    g_pInputManager->SetAction({"+forward",  Key::W});
    g_pInputManager->SetAction({"+backward", Key::S});
    g_pInputManager->SetAction({"+left",     Key::A});
    g_pInputManager->SetAction({"+right",    Key::D});

    m_sIsRunning = true;
    std::thread app_loop_thread{m_sApplicationRuntimeLoop};
    IManager::Start(); // manager loop
    m_sIsRunning = false;
    IManager::ShutdownAllManagers();
    IManager::RemoveAll();
    app_loop_thread.join();
    return mExitValue;
}

void RoseTinted::Event(AppEvent* inEvent)
{
    if(inEvent->IsEvent(AppEvent::WindowClose))
        { Application()->Stop(); }
}

void RoseTinted::Input(InputEvent* event)
{
    if((event->IsJustPressed(Key::Q) and event->IsModifierActive(Key::Mod_Control))
        or event->IsJustPressed(Key::F8))
        { EventManager::Queue()->add<AppEvent>(AppEvent::WindowClose); }
}
