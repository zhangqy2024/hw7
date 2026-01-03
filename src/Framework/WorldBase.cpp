#include "Framework/WorldBase.hpp"
#include "Framework/GameManager.hpp"

WorldBase::WorldBase() {}

WorldBase::~WorldBase() {}

bool WorldBase::GetKeyDown(KeyCode key) const { return GameManager::Instance().GetKeyDown(key); }

bool WorldBase::GetKey(KeyCode key) const { return GameManager::Instance().GetKey(key); }

std::pair<int, int> WorldBase::GetMousePos() const { return GameManager::Instance().GetMousePos(); }
