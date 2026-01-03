#include "GameObject/GameObject.hpp"

#include "Framework/TextBase.hpp"
#include "GameWorld/GameWorld.hpp"
#include "utils.hpp"

GameObject::GameObject(ImageID imageID, int x, int y, LayerID layer, int width, int height,
                       std::shared_ptr<GameWorld> world, int hp, int attack, AnimID animID)
    : ObjectBase(imageID, x, y, layer, width, height, animID), m_world(world), m_hp(hp),
      m_attack(attack), m_hpTextEnabled(false), m_hpTextOffsetY(0), m_hpText(nullptr) {}

GameObject::~GameObject() {}

void GameObject::Update() {
    if (!IsAlive()) {
        return;
    }

    UpdateActions();
    UpdateHpText();
}

void GameObject::OnClick() {}

std::shared_ptr<GameWorld> GameObject::GetWorld() const { return m_world; }

bool GameObject::IsAlive() const { return m_hp > 0; }

int GameObject::GetAttack() const { return m_attack; }

int GameObject::GetHP() const { return m_hp; }

void GameObject::SetHP(int hp) {
    m_hp = hp;
    if (m_hp <= 0) {
        DestroyHpText();
    } else {
        UpdateHpText();
    }
}

void GameObject::TakeDamage(int damage) {
    m_hp -= damage;
    if (m_hp <= 0) {
        DestroyHpText();
    } else {
        UpdateHpText();
    }
}

void GameObject::DisableHpText() {
    m_hpTextEnabled = false;
    DestroyHpText();
}

void GameObject::EnableHpText(int offsetY) {
    m_hpTextEnabled = true;
    m_hpTextOffsetY = offsetY;
    UpdateHpText();
}

void GameObject::UpdateHpText() {
    if (!m_hpTextEnabled) {
        return;
    }

    if (m_hp <= 0 || m_hp >= MAX_HP_LIMIT / 2) {
        DestroyHpText();
        return;
    }

    if (m_hpText == nullptr) {
        m_hpText = std::make_unique<TextBase>(GetX(), GetY() + m_hpTextOffsetY, "", 1.0, 1.0,
                                              1.0, true);
    }

    m_hpText->MoveTo(GetX(), GetY() + m_hpTextOffsetY);
    m_hpText->SetText("HP: " + std::to_string(m_hp));
}

void GameObject::DestroyHpText() { m_hpText.reset(); }

bool GameObject::IsCollidingWith(std::shared_ptr<GameObject> other) const {
    if (other == nullptr) {
        return false;
    }

    int left1 = GetX() - GetWidth() / 2;
    int right1 = GetX() + GetWidth() / 2;
    int bottom1 = GetY() - GetHeight() / 2;
    int top1 = GetY() + GetHeight() / 2;

    int left2 = other->GetX() - other->GetWidth() / 2;
    int right2 = other->GetX() + other->GetWidth() / 2;
    int bottom2 = other->GetY() - other->GetHeight() / 2;
    int top2 = other->GetY() + other->GetHeight() / 2;

    return (right1 > left2 && left1 < right2 && top1 > bottom2 && bottom1 < top2);
}
