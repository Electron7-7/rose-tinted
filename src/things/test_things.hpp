#ifndef TEST_THINGS_H
#define TEST_THINGS_H

#include <Nostalgia/Nostalgia.hpp>
#include <Nostalgia/things/thinkers/3d/collider_3d.hpp>

class CollisionTester3D : public Collider3D
{
public:
    SUPER(Collider3D)
    READY_OVERRIDE
    SET_VARIABLES_OVERRIDE
    GET_VARIABLES_OVERRIDE

    void Update() override;
    void Tick() override;

    bool mHadHit{false};
    std::vector<JPH::BodyID> mHitBodyIDs{};

protected:
    void OnContactAdded(ID inOtherColliderID,
        Farg<JPH::Body> inBody1,
        Farg<JPH::Body> inBody2,
        Farg<JPH::ContactManifold> manifold,
        JPH::ContactSettings& ioSettings) override;

private:
    glm::vec3 mMovementDirection{0.0f};
    float mMovementSpeed{1.0f};
    float mGravity{0.0f};
    ID mPlayerColliderID{};
};

#endif // TEST_THINGS_H
