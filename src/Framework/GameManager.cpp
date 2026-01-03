#include "Framework/GameManager.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Framework/ObjectBase.hpp"
#include "Framework/SpriteManager.hpp"
#include "Framework/TextBase.hpp"
#include "utils.hpp"

static void displayCallback() { GameManager::Instance().Update(); }

static void keyboardDownEventCallback(unsigned char key, int x, int y) {
    GameManager::Instance().KeyDownEvent(key, x, y);
}
static void keyboardUpEventCallback(unsigned char key, int x, int y) {
    GameManager::Instance().KeyUpEvent(key, x, y);
}

static void specialKeyboardDownEventCallback(int key, int x, int y) {
    GameManager::Instance().SpecialKeyDownEvent(key, x, y);
}
static void specialKeyboardUpEventCallback(int key, int x, int y) {
    GameManager::Instance().SpecialKeyUpEvent(key, x, y);
}

static void mouseButtonEventCallback(int button, int state, int x, int y) {
    y = WINDOW_HEIGHT - y;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        GameManager::Instance().MouseDownEvent(x, y);
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        GameManager::Instance().MouseUpEvent(x, y);
    }
}

static void timerCallback(int) {
    GameManager::Instance().Update();
    glutTimerFunc(MS_PER_FRAME, &timerCallback, 0);
}

void displayText(double x, double y, double z, const char *str, bool centering,
                 void *font = GLUT_BITMAP_HELVETICA_10) {
    if (centering) {
        const unsigned char *temp = reinterpret_cast<const unsigned char *>(str);
        int pixelLength = 0;
        while (*temp != '\0') {
            pixelLength += glutBitmapWidth(font, *temp);
            temp++;
        }
        // int pixelLength = glutBitmapLength(font, reinterpret_cast<const unsigned
        // char*>(str));
        x = x - ((double)pixelLength / (double)WINDOW_WIDTH);
    }

    glPushMatrix();
    glLoadIdentity();

    glRasterPos3f(x, y, z);

    while (*str != '\0') {
        glutBitmapCharacter(font, *str);
        str++;
    }

    // glutBitmapString(font, reinterpret_cast<const unsigned char*>(str));

    glPopMatrix();
}

GameManager::GameManager()
    : m_gameState(GameManager::GameState::TITLE), m_pressedKeys(), m_mousePos(0, 0),
      m_pause(false) {}

void GameManager::Play(int argc, char **argv, std::shared_ptr<WorldBase> world) {
    m_world = world;

    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(0, 0);

    glutInit(&argc, argv);

    glutCreateWindow("Runaway");
    glutKeyboardFunc(&keyboardDownEventCallback);
    glutKeyboardUpFunc(&keyboardUpEventCallback);
    glutSpecialFunc(&specialKeyboardDownEventCallback);
    glutSpecialUpFunc(&specialKeyboardUpEventCallback);
    glutMouseFunc(&mouseButtonEventCallback);
    glutDisplayFunc(&displayCallback);
    glutMotionFunc(
        [](int x, int y) { GameManager::Instance().MouseMoveEvent(x, WINDOW_HEIGHT - y); });
    glutPassiveMotionFunc(
        [](int x, int y) { GameManager::Instance().MouseMoveEvent(x, WINDOW_HEIGHT - y); });
    glutTimerFunc(MS_PER_FRAME, &timerCallback, 0);

    // Initialize SpriteManager. Sprites will be loaded during the starting
    // screen.
    SpriteManager::Instance();
    glutMainLoop();
}

void GameManager::Update() {
    if (m_pause)
        return;
    if (GetKey(KeyCode::QUIT)) {
        exit(EXIT_SUCCESS);
    }
    switch (m_gameState) {
    case GameManager::GameState::TITLE:
        if (!SpriteManager::Instance().IsLoaded()) {
            if (!SpriteManager::Instance().LoadSprites()) {
                Prompt(
                    "ERROR: Failed to load assets!",
                    ("You may need to change ASSET_DIR in utils.hpp. It's currently " + ASSET_DIR)
                        .c_str());
                break;
            }
        }
        Prompt("Runaway", "Press Enter to start");
        if (GetKey(KeyCode::ENTER)) {
            m_world->Init();
            m_gameState = GameManager::GameState::ANIMATING;
            Display();
        }
        break;
    case GameManager::GameState::ANIMATING: {
        LevelStatus status = m_world->Update();
        Display();
        switch (status) {
        case LevelStatus::ONGOING:
            break;
        case LevelStatus::LOSING:
            ShowGameOver();
            m_world->CleanUp();
            m_gameState = GameManager::GameState::PROMPTING;
            break;
        }
        break;
    }
    case GameManager::GameState::PROMPTING:
        if (GetKey(KeyCode::ENTER)) {
            m_world->Init();
            m_gameState = GameManager::GameState::ANIMATING;
            Display();
        }
        break;
    case GameManager::GameState::GAMEOVER:
        if (GetKey(KeyCode::ENTER)) {
            exit(EXIT_SUCCESS);
        }
        break;
    default:
        break;
    }
}

void GameManager::KeyDownEvent(unsigned char key, int x, int y) {
    KeyCode keyCode = ToKeyCode(key);
    if (keyCode != KeyCode::NONE) {
        if (m_pressedKeys.find(keyCode) == m_pressedKeys.end()) {
            m_pressedKeys.insert({keyCode, true});
        }
    }
}
void GameManager::KeyUpEvent(unsigned char key, int x, int y) {
    KeyCode keyCode = ToKeyCode(key);
    if (keyCode != KeyCode::NONE) {
        if (m_pressedKeys.find(keyCode) != m_pressedKeys.end()) {
            m_pressedKeys.erase(keyCode);
        }
    }
}

void GameManager::SpecialKeyDownEvent(int key, int x, int y) {
    KeyCode keyCode = SpecialToKeyCode(key);
    if (keyCode != KeyCode::NONE) {
        if (m_pressedKeys.find(keyCode) == m_pressedKeys.end()) {
            m_pressedKeys.insert({keyCode, true});
        }
    }
}

void GameManager::SpecialKeyUpEvent(int key, int x, int y) {
    KeyCode keyCode = SpecialToKeyCode(key);
    if (keyCode != KeyCode::NONE) {
        if (m_pressedKeys.find(keyCode) != m_pressedKeys.end()) {
            m_pressedKeys.erase(keyCode);
        }
    }
}

void GameManager::MouseMoveEvent(int x, int y) { m_mousePos = std::make_pair(x, y); }

void GameManager::MouseDownEvent(int x, int y) {
    ObjectBase::ClickAt(x, y);
    if (m_pressedKeys.find(KeyCode::FIRE1) == m_pressedKeys.end()) {
        m_pressedKeys.insert({KeyCode::FIRE1, true});
    }
}

void GameManager::MouseUpEvent(int x, int y) {
    if (m_pressedKeys.find(KeyCode::FIRE1) != m_pressedKeys.end()) {
        m_pressedKeys.erase(KeyCode::FIRE1);
    }
}

bool GameManager::GetKey(KeyCode key) const {
    return m_pressedKeys.find(key) != m_pressedKeys.end();
}

bool GameManager::GetKeyDown(KeyCode key) {
    auto keyEntry = m_pressedKeys.find(key);
    if (keyEntry != m_pressedKeys.end()) {
        if (keyEntry->second) {
            m_pressedKeys[keyEntry->first] = false;
            return true;
        } else {
            return false;
        }
    }
    return false;
}

void GameManager::Display() {
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Display all GameObjects
    ObjectBase::DisplayAllObjects(
        [=](ImageID imageID, AnimID animID, double x, double y, std::size_t frame) {
            return GameManager::Instance().DrawOneObject(imageID, animID, x, y, frame);
        });

    // Display all Texts in game
    TextBase::DisplayAllTexts(
        [=](int x, int y, const std::string &text, double r, double g, double b, bool centering) {
            glPushAttrib(GL_CURRENT_BIT);
            glColor3f(r, g, b);
            displayText(NormalizeCoord(x, WINDOW_WIDTH), NormalizeCoord(y, WINDOW_HEIGHT), 0,
                        text.c_str(), centering, GLUT_BITMAP_HELVETICA_18);
            glPopAttrib();
        });

    glutSwapBuffers();
}

double GameManager::NormalizeCoord(double pixels, double totalPixels) const {
    return 2.0 * pixels / totalPixels - 1.0;
}

int GameManager::DenormalizeCoord(double normalizedCoord, double totalPixels) const {
    return std::round((normalizedCoord + 1.0) / 2.0 * totalPixels);
}

inline void GameManager::Rotate(double x, double y, double degrees, double &xout,
                                double &yout) const {
    static const double PI = 4 * atan(1.0);
    double theta = (degrees / 360.0) * (2 * PI);
    xout = x * cos(theta) + y * sin(theta);
    yout = y * cos(theta) - x * sin(theta);
}

// Draws one object and returns its next frame.
std::size_t GameManager::DrawOneObject(ImageID imageID, AnimID animID, double x, double y,
                                       std::size_t frame) const {
    SpriteInfo spriteInfo = SpriteManager::Instance().GetSpriteInfo(imageID, animID);
    if (spriteInfo.texture == 0) {
        return 0;
    }
    glPushMatrix();
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, spriteInfo.texture);

    // // An ugly patch-solution for having insuffient size on pole vaulter
    // spritesheets if (imageID == IMGID_POLE_VAULTING_ZOMBIE) {
    //   y += 20;
    // }

    double centerX = NormalizeCoord(x, WINDOW_WIDTH);
    double centerY = NormalizeCoord(y, WINDOW_HEIGHT);
    double halfW = spriteInfo.spriteWidth / 2.0;
    double halfH = spriteInfo.spriteHeight / 2.0;

    int spriteRow = frame / spriteInfo.cols, spriteCol = frame % spriteInfo.cols;
    double spriteWidth = (double)spriteInfo.spriteWidth / spriteInfo.totalWidth;
    double spriteHeight = (double)spriteInfo.spriteHeight / spriteInfo.totalHeight;

    glBegin(GL_QUADS);
    glTexCoord2f(spriteCol * spriteWidth, 1 - (spriteRow + 1) * spriteHeight);
    glVertex3f((float)(centerX - halfW / (WINDOW_WIDTH / 2)),
               (float)(centerY - halfH / (WINDOW_HEIGHT / 2)), 0);
    glTexCoord2f((spriteCol + 1) * spriteWidth, 1 - (spriteRow + 1) * spriteHeight);
    glVertex3f((float)(centerX + halfW / (WINDOW_WIDTH / 2)),
               (float)(centerY - halfH / (WINDOW_HEIGHT / 2)), 0);
    glTexCoord2f((spriteCol + 1) * spriteWidth, 1 - spriteRow * spriteHeight);
    glVertex3f((float)(centerX + halfW / (WINDOW_WIDTH / 2)),
               (float)(centerY + halfH / (WINDOW_HEIGHT / 2)), 0);
    glTexCoord2f(spriteCol * spriteWidth, 1 - spriteRow * spriteHeight);
    glVertex3f((float)(centerX - halfW / (WINDOW_WIDTH / 2)),
               (float)(centerY + halfH / (WINDOW_HEIGHT / 2)), 0);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glEnable(GL_DEPTH_TEST);

    glPopAttrib();
    glPopMatrix();
    return (frame + 1) % spriteInfo.frames;
}

void GameManager::Prompt(const char *title, const char *subtitle) const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 0.5f);
    displayText(0, 0.25, -1, title, true, GLUT_BITMAP_HELVETICA_18);
    glColor3f(1.0f, 1.0f, 1.0f);
    displayText(0, -0.2, -1, subtitle, true, GLUT_BITMAP_HELVETICA_12);
    glutSwapBuffers();
}

void GameManager::ShowGameOver() const {

    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // enables user to display end-game wave number text.
    TextBase::DisplayAllTexts(
        [=](int x, int y, const std::string &text, double r, double g, double b, bool centering) {
            glPushAttrib(GL_CURRENT_BIT);
            glColor3f(r, g, b);
            displayText(NormalizeCoord(x, WINDOW_WIDTH), NormalizeCoord(y, WINDOW_HEIGHT), 0,
                        text.c_str(), centering, GLUT_BITMAP_HELVETICA_18);
            glPopAttrib();
        });

    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    displayText(NormalizeCoord(WINDOW_WIDTH / 2, WINDOW_WIDTH), NormalizeCoord(50, WINDOW_HEIGHT),
                0, "Your score: [      ] Press Enter to restart.", true, GLUT_BITMAP_HELVETICA_18);
    glPopAttrib();
    glutSwapBuffers();
}

inline KeyCode GameManager::ToKeyCode(unsigned char key) const {
    switch (key) {
    case '\x1B':
        return KeyCode::QUIT;
    case '\r':
        return KeyCode::ENTER;
    case 'a':
    case 'A':
        return KeyCode::LEFT;
    case 'd':
    case 'D':
        return KeyCode::RIGHT;
    case ' ':
    case 'k':
    case 'K':
        return KeyCode::JUMP;
    case 'j':
    case 'J':
        return KeyCode::FIRE1;
    default:
        return KeyCode::NONE;
    }
}
inline KeyCode GameManager::SpecialToKeyCode(int key) const {
    switch (key) {
    case GLUT_KEY_UP:
        return KeyCode::UP;
    case GLUT_KEY_LEFT:
        return KeyCode::LEFT;
    case GLUT_KEY_DOWN:
        return KeyCode::DOWN;
    case GLUT_KEY_RIGHT:
        return KeyCode::RIGHT;
    default:
        return KeyCode::NONE;
    }
}