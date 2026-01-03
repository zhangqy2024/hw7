#include "GameWorld/GameWorld.hpp"

#include "Framework/TextBase.hpp"
#include "GameObject/Background.hpp"
#include "GameObject/Enemies.hpp"
#include "GameObject/Player.hpp"
#include "GameObject/Projectiles.hpp"

GameWorld::GameWorld()
    : m_player(nullptr), m_clock(0), m_score(0), m_scoreText(nullptr), m_gameOverText(nullptr),
      m_isGameOver(false) {}

GameWorld::~GameWorld() {
    if (m_scoreText != nullptr) {
        delete m_scoreText;
    }
    if (m_gameOverText != nullptr) {
        delete m_gameOverText;
    }
}

void GameWorld::Init() {
    // Initialize game data
    m_clock = 0;
    m_score = 0;
    m_isGameOver = false;

    // Create background
    Instantiate(std::make_shared<Background>(shared_from_this()));

    // Create player
    m_player = std::make_shared<Player>(shared_from_this());
    Instantiate(m_player);

    // Create score text (delete old one if exists)
    if (m_scoreText != nullptr) {
        delete m_scoreText;
    }
    m_scoreText = new TextBase(WINDOW_WIDTH - 160, 8, "Score: 0", 1.0, 1.0, 1.0);
}

LevelStatus GameWorld::Update() {
    m_clock++;

    // Spawn enemies
    SpawnEnemies();

    // Update all game objects
    for (auto &obj : m_gameObjects) {
        obj->Update();
    }

    // Check collisions
    CheckCollisions();

    // Remove dead objects
    RemoveDeadObjects();

    // Check if player is dead
    if (m_player == nullptr || !m_player->IsAlive()) {
        // Display game over score (only once)
        if (!m_isGameOver) {
            // Hide the in-game score text so the losing screen doesn't show duplicate scores.
            if (m_scoreText != nullptr) {
                delete m_scoreText;
                m_scoreText = nullptr;
            }

            // Hide all HP texts on losing screen.
            for (auto &obj : m_gameObjects) {
                obj->DisableHpText();
            }

            // Show final score on a separate line to avoid overlapping the framework's message.
            if (m_gameOverText != nullptr) {
                delete m_gameOverText;
            }
            m_gameOverText = new TextBase(WINDOW_WIDTH / 2, 80,
                                          "Score: " + std::to_string(m_score), 1.0, 1.0, 1.0,
                                          true);
            m_isGameOver = true;
        }
        return LevelStatus::LOSING;
    }

    // Update score text
    if (m_scoreText != nullptr) {
        m_scoreText->SetText("Score: " + std::to_string(m_score));
    }

    return LevelStatus::ONGOING;
}

void GameWorld::CleanUp() {
    m_gameObjects.clear();
    m_player = nullptr;

    if (m_scoreText != nullptr) {
        delete m_scoreText;
        m_scoreText = nullptr;
    }

    if (m_gameOverText != nullptr) {
        delete m_gameOverText;
        m_gameOverText = nullptr;
    }

    m_isGameOver = false;
}

void GameWorld::Instantiate(std::shared_ptr<GameObject> newGameObject) {
    m_gameObjects.push_back(newGameObject);
}

void GameWorld::SpawnEnemies() {
    // Calculate spawn rate based on time (difficulty increase)
    int difficulty = m_clock / 450;  // Difficulty increases every 15 seconds (450 ticks)
    int baseInterval = 80;           // Base spawn interval
    int spawnInterval = std::max(30, baseInterval - difficulty * 5);

    // Spawn goblins
    if (m_clock % spawnInterval == 0) {
        int y = 125 + randInt(-5, 5);
        Instantiate(std::make_shared<Goblin>(shared_from_this(), WINDOW_WIDTH, y));
    }

    // Spawn plants
    if (m_clock % (spawnInterval + 30) == 10) {
        Instantiate(std::make_shared<Plant>(shared_from_this(), WINDOW_WIDTH, 100));
    }

    // Spawn birds
    if (m_clock % (spawnInterval + 40) == 15) {
        int y = 215 + randInt(-10, 10);
        Instantiate(std::make_shared<Bird>(shared_from_this(), WINDOW_WIDTH, y));
    }

    // Spawn pigs
    if (m_clock % (spawnInterval + 50) == 20) {
        Instantiate(std::make_shared<Pig>(shared_from_this(), WINDOW_WIDTH, 100));
    }
}

void GameWorld::CheckCollisions() {
    for (auto it1 = m_gameObjects.begin(); it1 != m_gameObjects.end(); ++it1) {
        if (!(*it1)->IsAlive())
            continue;

        for (auto it2 = std::next(it1); it2 != m_gameObjects.end(); ++it2) {
            if (!(*it2)->IsAlive())
                continue;

            if (!(*it1)->IsCollidingWith(*it2))
                continue;

            auto type1 = (*it1)->GetType();
            auto type2 = (*it2)->GetType();

            // Bullet hits enemy
            if (type1 == GameObject::Type::ProjectilePlayer &&
                type2 == GameObject::Type::Enemy) {
                (*it2)->TakeDamage((*it1)->GetAttack());
                (*it1)->SetHP(0);
                if (!(*it2)->IsAlive()) {
                    m_score++;
                }
            } else if (type2 == GameObject::Type::ProjectilePlayer &&
                       type1 == GameObject::Type::Enemy) {
                (*it1)->TakeDamage((*it2)->GetAttack());
                (*it2)->SetHP(0);
                if (!(*it1)->IsAlive()) {
                    m_score++;
                }
            }
            // Enemy projectile or enemy hits player
            else if (type1 == GameObject::Type::ProjectileEnemy &&
                     type2 == GameObject::Type::Player) {
                (*it2)->TakeDamage((*it1)->GetAttack());
                (*it1)->SetHP(0);
            } else if (type2 == GameObject::Type::ProjectileEnemy &&
                       type1 == GameObject::Type::Player) {
                (*it1)->TakeDamage((*it2)->GetAttack());
                (*it2)->SetHP(0);
            } else if (type1 == GameObject::Type::Enemy && type2 == GameObject::Type::Player) {
                (*it2)->TakeDamage((*it1)->GetAttack());
                (*it1)->SetHP(0);
            } else if (type2 == GameObject::Type::Enemy && type1 == GameObject::Type::Player) {
                (*it1)->TakeDamage((*it2)->GetAttack());
                (*it2)->SetHP(0);
            }
        }
    }
}

void GameWorld::RemoveDeadObjects() {
    m_gameObjects.remove_if([](std::shared_ptr<GameObject> obj) { return !obj->IsAlive(); });
}
