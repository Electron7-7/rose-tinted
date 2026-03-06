#ifndef IMGUI_DEBUGGER_H
#define IMGUI_DEBUGGER_H

#include <Nostalgia/Nostalgia.hpp>
#include <Nostalgia/ui/solution.hpp>

class ImGuiDebugger : public UI_Solution
{
public:
    void Update() final;
    void Input(InputEvent*) final;

    static void SetOpen(bool inIsOpen);

private:
    static bool m_sOpen;
};

#endif // IMGUI_DEBUGGER_H
