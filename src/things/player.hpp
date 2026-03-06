#ifndef PLAYER_H
#define PLAYER_H

#include <Nostalgia/Nostalgia.hpp>
#include <Nostalgia/theatre/things/thinkers/3d/nostalgia_player_3d.hpp>

class RoseTintedPlayer3D : public NostalgiaPlayer3D
{
public:
    virtual void SetVariables(Farg<TheatreFile::ThingData>) override;
    virtual Shared<TheatreFile::ThingData> GetVariables() const override;
    virtual void Tick() override;
    virtual void Ready() override;

    ID GetMainColliderID() const;

    bool mCaptureMouse{false},
        mCaptureKeyboard{false};

protected:
    ID mMainColliderID{},
        mCockPitID{};
    bool mEnableCollision{true};
    glm::vec3 mVelocity{0.0f},
        mLookWish{0.0f},
        mMovementDirection{0.0f},
        mCockpitOffset{0.0f};
};

#endif // PLAYER_H
