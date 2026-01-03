#ifndef UTILS_HPP__
#define UTILS_HPP__

#include <random>
#include <string>

// 注意：如果你遇到了 SOIL 图片素材加载失败的问题，你应当修改下面这个路径。
// 在 Mac OS/Linux 上，这个路径应当为你运行程序的终端所在位置到 assets/
// 文件夹的相对路径。 建议你在根目录下以 ./bulid/runaway
// 命令运行程序，这样下面的 ./assets/ 是正确的。
// 如果你是在windows上双击exe运行程序，那它就是相对于exe文件所在位置路径打开，即可能需要改成../assets/
// 之类的。

static const std::string ASSET_DIR = "./assets/";

// Returns a random integer within [min, max] (inclusive).
inline int randInt(int min, int max) {
    if (max < min)
        std::swap(max, min);
    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

enum class LevelStatus { ONGOING, LOSING };

enum class KeyCode {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    JUMP,
    FIRE1, // Left mouse button
    FIRE2, // Right mouse button
    ENTER, // Enter
    QUIT   // Esc
};

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

enum class ImageID {
    NONE = 0,
    BACKGROUND,
    PLAYER,
    BULLET,
    GOBLIN,
    AXE,
    PLANT,
    BOMBMAN,
    BIRD,
    PIG,
    GAME_OVER = 100
};

enum class AnimID { NO_ANIMATION = 0, IDLE, THROW, HURT, BOOST, RUSH, JUMP };

const int MAX_LAYERS = 7;

enum class LayerID { UI = 0, PLAYER, ENEMIES, PROJECTILES, BACKGROUND };

const int MS_PER_FRAME = 33;

#endif // !UTILS_HPP__
