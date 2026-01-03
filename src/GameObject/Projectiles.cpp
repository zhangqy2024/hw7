#include "GameObject/Projectiles.hpp"
#include "utils.hpp"

// Bullet implementation
Bullet::Bullet(pGameWorld world, int x, int y)
    : GameObject(ImageID::BULLET, x, y, LayerID::PROJECTILES, 10, 10, world, 1, 1,
                 AnimID::NO_ANIMATION) {}

void Bullet::UpdateActions() {
    if (!IsAlive()) {
        return;
    }

    // Move right
    MoveTo(GetX() + SPEED, GetY());

    // Check if out of screen
    if (GetX() >= WINDOW_WIDTH) {
        SetHP(0);
    }
}

// Axe implementation
Axe::Axe(pGameWorld world, int x, int y)
    : GameObject(ImageID::AXE, x, y, LayerID::PROJECTILES, 25, 25, world, 1, 1,
                 AnimID::NO_ANIMATION) {}

void Axe::UpdateActions() {
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