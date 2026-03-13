#ifndef PLAYER_H
#define PLAYER_H

#include <Nostalgia/Nostalgia.hpp>
#include <Nostalgia/theatre/things/thinkers/3d/nostalgia_player_3d.hpp>

class RoseTintedPlayer3D : public NostalgiaPlayer3D
{
public:
    SET_VARIABLES_OVERRIDE
    GET_VARIABLES_OVERRIDE
    READY_OVERRIDE
    virtual void Shutdown() override;
    virtual void Tick() override;

    ID GetMainColliderID() const;

    bool mCaptureMouse{false},
        mCaptureKeyboard{false};

    static float m_sMaxSpeed;
    static float m_sVelocityMultiplier;
    static float m_sAngularFriction;
    static float m_sLinearFriction;

protected:
    friend class ImGuiDebugger;
    ID mMainColliderID{},
        mCockPitID{};
    bool mEnableCollision{true};
    float mThrust{0.0f},
        mYaw{0.0f},
        mRoll{0.0f},
        mPitch{0.0f};
    glm::vec3 mLookWish{0.0f},
        mCockpitOffset{0.0f};
};

#endif // PLAYER_H
