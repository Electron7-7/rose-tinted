#include "./rose_tinted.hpp"
#include "gui/imgui_implementor.hpp"
#include "gui/imgui_main_menu.hpp"
#include "things/player.hpp"
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

std::string gToggleFullscreen{"ToggleFullscreen"};

Shared<Thing> sPlayerMaker()
{ return MakeShared<RoseTintedPlayer3D>(); }

void RoseTinted::Stop()
{ IManager::Stop(); }

int RoseTinted::Main()
{
    mMainWindow = IWindow::CreateWindow(IWindow::Properties{std::format("Rose-Tinted Glasses")});

    auto& imgui_impl{UI_Implementor::Create<ImGui_Implementor>()};
    auto& main_menu{imgui_impl->CreateSolution<ImGuiMainMenu>()};

    IManager::Add(g_pPhysicsManager);
    IManager::Add(g_pTheatreManager);
    IManager::Add(g_pRenderManager);
    IManager::Add(g_pInputManager);
    IManager::Add(g_pEventManager);
    IManager::Add(g_pUIManager);

    IManager::InitAllManagers();

    ThingFactory::AddThing(&sPlayerMaker, "RoseTintedPlayer3D", ThingType::NostalgiaPlayer3D);

    g_pInputManager->SetAction({"toggle_main_menu", Key::Escape});
    g_pInputManager->SetAction({gToggleFullscreen, Key::F10});
    g_pInputManager->SetAction({"+forward",  Key::W});
    g_pInputManager->SetAction({"+backward", Key::S});
    g_pInputManager->SetAction({"+left",     Key::A});
    g_pInputManager->SetAction({"+right",    Key::D});

    IManager::Start(); // gameloop
    IManager::ShutdownAllManagers();
    IManager::RemoveAll();

    return 0;
}

void RoseTinted::Event(AppEvent* inEvent)
{
    if(inEvent->IsEvent(AppEvent::WindowClose))
        { Application()->Stop(); }
}

void RoseTinted::Input(InputEvent* event)
{
    if(event->IsActive(gToggleFullscreen))
    {
        MainWindow()->SetWindowMode((MainWindow()->GetWindowMode() == IWindow::WINDOW_MODE_WINDOWED)
            ? IWindow::WINDOW_MODE_FULLSCREEN
            : IWindow::WINDOW_MODE_WINDOWED);
    }
    else if((event->IsJustPressed(Key::Q) and event->IsModifierActive(Key::Mod_Control))
        or event->IsJustPressed(Key::F8))
        { EventManager::Queue()->add<AppEvent>(AppEvent::WindowClose); }
}
