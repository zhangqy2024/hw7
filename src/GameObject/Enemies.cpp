#include "GameObject/Enemies.hpp"
#include "GameObject/Projectiles.hpp"
#include "utils.hpp"

// Goblin implementation
Goblin::Goblin(pGameWorld world, int x, int y)
    : GameObject(ImageID::GOBLIN, x, y, LayerID::ENEMIES, 20, 48, world, 5, 1, AnimID::IDLE),
      m_throwTimer(0), m_throwAnimationTimer(0), m_isThrowing(false) {
    EnableHpText(50);
}

void Goblin::UpdateActions() {
    if (!IsAlive()) {
        return;
    }

    // Move left
    MoveTo(GetX() - SPEED, GetY());

    // Check if out of screen
    if (GetX() <= 0) {
        SetHP(0);
        return;
    }

    // Handle throwing axe
    if (!m_isThrowing) {
        m_throwTimer++;
        if (m_throwTimer >= THROW_INTERVAL) {
            m_isThrowing = true;
            m_throwAnimationTimer = 0;
            m_throwTimer = 0;
            PlayAnimation(AnimID::THROW);
        }
    } else {
        m_throwAnimationTimer++;
        if (m_throwAnimationTimer >= THROW_ANIMATION_DURATION) {
            // Create axe at goblin's position
            GetWorld()->Instantiate(std::make_shared<Axe>(GetWorld(), GetX(), GetY()));
            m_isThrowing = false;
            PlayAnimation(AnimID::IDLE);
        }
    }
}

// Plant implementation
Plant::Plant(pGameWorld world, int x, int y)
    : GameObject(ImageID::PLANT, x, y, LayerID::PROJECTILES, 25, 25, world, MAX_HP_LIMIT, 1,
                 AnimID::NO_ANIMATION) {}

void Plant::UpdateActions() {
    if (!IsAlive()) {
        return;
    }

    // Move left
    MoveTo(GetX() - SPEED, GetY());

    // Check if out of screen
    if (GetX() <= 0) {
        SetHP(0);
    }
}

// Bird implementation
Bird::Bird(pGameWorld world, int x, int y)
    : GameObject(ImageID::BIRD, x, y, LayerID::ENEMIES, 25, 25, world, 1, 1, AnimID::IDLE) {
    EnableHpText(40);
}

void Bird::UpdateActions() {
    if (!IsAlive()) {
        return;
    }

    // Move left
    MoveTo(GetX() - SPEED, GetY());

    // Check if out of screen
    if (GetX() <= 0) {
        SetHP(0);
    }
}

// Pig implementation
Pig::Pig(pGameWorld world, int x, int y)
    : GameObject(ImageID::PIG, x, y, LayerID::ENEMIES, 20, 48, world, 5, 1, AnimID::IDLE),
      m_speed(INITIAL_SPEED), m_rushTimer(0), m_isRushing(false) {
    EnableHpText(50);
}

void Pig::UpdateActions() {
    if (!IsAlive()) {
        return;
    }

    // Check if should start rushing
    if (!m_isRushing) {
        m_rushTimer++;
        if (m_rushTimer >= RUSH_DELAY) {
            m_isRushing = true;
            m_speed = RUSH_SPEED;
            PlayAnimation(AnimID::RUSH);
        }
    }

    // Move left
    MoveTo(GetX() - m_speed, GetY());

    // Check if out of screen
    if (GetX() <= 0) {
        SetHP(0);
    }
}
