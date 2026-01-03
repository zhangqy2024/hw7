#ifndef WORLDBASE_HPP__
#define WORLDBASE_HPP__

#include <iostream>
#include <memory>
#include <set>

#include "utils.hpp"

class WorldBase {
public:
    WorldBase();
    virtual ~WorldBase();

    virtual void Init() = 0;

    virtual LevelStatus Update() = 0;

    virtual void CleanUp() = 0;

    virtual bool GetKeyDown(KeyCode key) const;
    virtual bool GetKey(KeyCode key) const;
    virtual std::pair<int, int> GetMousePos() const;
    void addScore() { score++; }
    int GetScore() { return score; }

private:
    int score = 0;
};

#endif // !WORLDBASE_HPP__
