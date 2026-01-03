#ifndef PROJECTILES_HPP__
#define PROJECTILES_HPP__

#include "GameObject/GameObject.hpp"

class Bullet : public GameObject {
public:
    Bullet(pGameWorld world, int x, int y);

    void UpdateActions() override;
    GameObject::Type GetType() const override { return GameObject::Type::ProjectilePlayer; }

private:
    static const int SPEED = 10;
};

class Axe : public GameObject {
public:
    Axe(pGameWorld world, int x, int y);

    void UpdateActions() override;
    GameObject::Type GetType() const override { return GameObject::Type::ProjectileEnemy; }

private:
    static const int SPEED = 10;
};

#endif // !PROJECTILES_HPP__
