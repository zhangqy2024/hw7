#ifndef GAMEOBJECT_HPP__
#define GAMEOBJECT_HPP__

#include "Framework/ObjectBase.hpp"
#include <memory>

class TextBase;

// Declares the class name GameWorld so that its pointers can be used.
class GameWorld;
using pGameWorld = std::shared_ptr<GameWorld>;

class GameObject : public ObjectBase, public std::enable_shared_from_this<GameObject> {
public:
    using std::enable_shared_from_this<GameObject>::shared_from_this;
    enum class Type { UI, Player, Enemy, ProjectilePlayer, ProjectileEnemy };

    GameObject(ImageID imageID, int x, int y, LayerID layer, int width, int height,
               std::shared_ptr<GameWorld> world, int hp, int attack,
               AnimID animID = AnimID::NO_ANIMATION);
    virtual ~GameObject();

    virtual void Update() override;
    void OnClick() override;
    virtual void OnCollision(std::shared_ptr<GameObject> other) {}

    virtual void UpdateActions() {}

    std::shared_ptr<GameWorld> GetWorld() const;
    bool IsAlive() const;
    int GetAttack() const;

    int GetHP() const;
    void SetHP(int hp);

    void TakeDamage(int damage);

    void DisableHpText();

    virtual GameObject::Type GetType() const { return Type::UI; }

    bool IsCollidingWith(std::shared_ptr<GameObject> other) const;

protected:
    void EnableHpText(int offsetY);
    void UpdateHpText();
    void DestroyHpText();

    // A handy definition of a large number,
    // For simplicity I don't want to use INT_MAX or std::numeric_limits.
    static const int MAX_HP_LIMIT = 9999999;

private:
    std::shared_ptr<GameWorld> m_world;
    int m_hp;
    int m_attack;
    bool m_hpTextEnabled;
    int m_hpTextOffsetY;
    std::unique_ptr<TextBase> m_hpText;
};

#endif // !GAMEOBJECT_HPP__
