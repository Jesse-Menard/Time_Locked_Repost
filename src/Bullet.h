#pragma once
#ifndef __BULLET__
#define __BULLET__

#include "DisplayObject.h"
#include "WeaponType.h"
#include "Particle.h"

class Bullet : public DisplayObject
{
public:
    Bullet();
    Bullet(glm::vec2 position, glm::vec2 direction, WeaponType type, bool playerOwned, bool seeking = false, float seekingStrength = 0);
    ~Bullet() override;

    void Init(glm::vec2 position, WeaponType type);

    void Draw() override;
    void Update() override;
    void Clean() override;

    int GetDuration() const;
    bool GetIsPlayerOwned() const;
    WeaponType GetWeaponType() const;
    int GetParticleCtr() const;

    // Seek functions
    bool isSeeking() const;
    [[nodiscard]] glm::vec2 GetTargetDirection() const;
    [[nodiscard]] glm::vec2 GetTargetPosition() const;
    void SetTargetDirection(glm::vec2 targetPosition);
    void SetTargetPosition(glm::vec2 targetPosition);
    void Seek();

    void Move();
    int m_duration;
private:
    std::string m_textureName;
    WeaponType m_weaponType;

    bool m_isPlayerOwner;
    float m_speed;
    float m_renderAngle;
    glm::vec2 m_direction;
    float m_directionAngle;

    // Seek variables
    bool m_isSeeking;
    float m_turnRate;
    glm::vec2 m_targetNormal;
    glm::vec2 m_targetPosition;

    std::vector<Particle*> m_Particles;
    int m_particleCtr;

    // Boss bools
    bool m_sides;
    bool m_tops;
};

#endif // Defined __BULLET__