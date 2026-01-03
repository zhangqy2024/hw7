#ifndef GAMEWORLD_HPP__
#define GAMEWORLD_HPP__

#include <list>
#include <memory>

#include "Framework/TextBase.hpp"
#include "Framework/WorldBase.hpp"
#include "GameObject/GameObject.hpp"
#include "utils.hpp"

class GameWorld : public WorldBase, public std::enable_shared_from_this<GameWorld>
{
public:
    GameWorld();
    virtual ~GameWorld();

    void Init() override;

    LevelStatus Update() override;

    void CleanUp() override;

    // 这些 override
    // 并没有实际意义。而是给同学们的提示，可以使用这些函数获取输入。 Redundant
    // overrides just for hinting: Use these functions for input.
    bool GetKeyDown(KeyCode key) const override { return WorldBase::GetKeyDown(key); }
    bool GetKey(KeyCode key) const override { return WorldBase::GetKey(key); }
    std::pair<int, int> GetMousePos() const override { return WorldBase::GetMousePos(); }

    void Instantiate(std::shared_ptr<GameObject> newGameObject);

    void ScrollLeft(int pixels);
    void addScore() { m_score++; }
    int GetScore() { return m_score; }

    // private:
    //     void SpawnEnemies();
    //     void CheckCollisions();
    //     void RemoveDeadObjects();

    //     std::list<std::shared_ptr<GameObject>> m_gameObjects;
    //     std::shared_ptr<GameObject> m_player;
    //     int m_clock;
    //     int m_score;
    //     TextBase *m_scoreText;
    //     TextBase *m_gameOverText;
    //     bool m_isGameOver;
    // };

    // #endif // !GAMEWORLD_HPP__
