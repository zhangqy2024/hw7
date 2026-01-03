#include "GameObject/Background.hpp"

Background::Background(pGameWorld world)
    : GameObject(ImageID::BACKGROUND, WINDOW_WIDTH, WINDOW_HEIGHT / 2, LayerID::BACKGROUND,
                 2 * WINDOW_WIDTH, WINDOW_HEIGHT, world, 1, 0) {}

void Background::Update() {
    MoveTo(GetX() - 4, GetY());
    if (GetX() <= 0) {
        MoveTo(GetX() + WINDOW_WIDTH, GetY());
    }
}
