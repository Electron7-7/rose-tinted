#ifndef ROSE_TINTED_H
#define ROSE_TINTED_H

#include <Nostalgia/Nostalgia.hpp>
#include <Nostalgia/application/application.hpp>
#include <Nostalgia/components/event_handling.hpp>

class RoseTinted final : public IApplication
{
public:
    int Main() final;
    void Stop() final;
    const char* Name() final { return "Rose Tinted"; }
    void Input(InputEvent*) final;
    void Event(AppEvent*) final;

private:
    int  mExitValue{0};

    static bool m_sIsRunning;
    static void m_sApplicationRuntimeLoop();
};

#endif // ROSE_TINTED_H
