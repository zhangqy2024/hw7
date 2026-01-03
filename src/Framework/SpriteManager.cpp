#define GL_GLEXT_PROTOTYPES
#include "Framework/SpriteManager.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "utils.hpp"

SpriteManager::SpriteManager() : m_spriteInfos() {
    m_spriteInfos.insert({EncodeAnim(ImageID::BACKGROUND, AnimID::NO_ANIMATION),
                          SpriteInfo{"game_bg.png", 2 * WINDOW_WIDTH, WINDOW_HEIGHT,
                                     2 * WINDOW_WIDTH, WINDOW_HEIGHT}});
    m_spriteInfos.insert({EncodeAnim(ImageID::PLAYER, AnimID::IDLE),
                          SpriteInfo{"run.png", 100 * 6, 100 * 2, 100, 100, 6, 12}});
    m_spriteInfos.insert({EncodeAnim(ImageID::PLAYER, AnimID::BOOST),
                          SpriteInfo{"run.png", 500 * 6, 500 * 2, 500, 500, 6, 12}});
    m_spriteInfos.insert({EncodeAnim(ImageID::BULLET, AnimID::NO_ANIMATION),
                          SpriteInfo{"player_bullet.png", 10, 10, 10, 10}});
    m_spriteInfos.insert({EncodeAnim(ImageID::PLAYER, AnimID::JUMP),
                          SpriteInfo{"jump.png", 90 * 4, 90, 90, 90, 4, 4}});
    m_spriteInfos.insert(
        {EncodeAnim(ImageID::GOBLIN, AnimID::IDLE),
         SpriteInfo{"goblin_Idle_24f_remake.png", 100 * 6, 100 * 4, 100, 100, 6, 24}});
    m_spriteInfos.insert(
        {EncodeAnim(ImageID::GOBLIN, AnimID::THROW),
         SpriteInfo{"goblin_throw_24f_remake.png", 100 * 6, 100 * 4, 100, 100, 6, 24}});
    m_spriteInfos.insert(
        {EncodeAnim(ImageID::GOBLIN, AnimID::HURT),
         SpriteInfo{"goblin_hurt_24f_remake.png", 100 * 6, 100 * 4, 100, 100, 6, 24}});
    m_spriteInfos.insert({EncodeAnim(ImageID::AXE, AnimID::NO_ANIMATION),
                          SpriteInfo{"axe_rotation_grid.png", 6 * 25, 4 * 25, 25, 25, 6, 24}});
    m_spriteInfos.insert({EncodeAnim(ImageID::PLANT, AnimID::NO_ANIMATION),
                          SpriteInfo{"plant.png", 80, 110, 80, 110}});
    m_spriteInfos.insert(
        {EncodeAnim(ImageID::PIG, AnimID::IDLE), SpriteInfo{"newpig.png", 80, 80, 80, 80}});
    m_spriteInfos.insert(
        {EncodeAnim(ImageID::PIG, AnimID::HURT), SpriteInfo{"newpig.png", 80, 80, 80, 80}});
    m_spriteInfos.insert(
        {EncodeAnim(ImageID::PIG, AnimID::RUSH), SpriteInfo{"newpigrush.png", 80, 80, 80, 80}});
    m_spriteInfos.insert({EncodeAnim(ImageID::BIRD, AnimID::IDLE),
                          SpriteInfo{"bird_24.png", 6 * 50, 4 * 50, 50, 50, 6, 24}});
    m_spriteInfos.insert({EncodeAnim(ImageID::BIRD, AnimID::HURT),
                          SpriteInfo{"bird_24.png", 6 * 50, 4 * 50, 50, 50, 6, 24}});
}

bool SpriteManager::LoadSprites() {
    if (m_loaded) {
        return true;
    }

    glEnable(GL_DEPTH_TEST);

    // OpenGL 纹理坐标原点在左下角，图片数据通常在左上角，所以需要翻转
    stbi_set_flip_vertically_on_load(true);

    for (auto &asset : m_spriteInfos) {
        int width, height, nrChannels;
        unsigned char *data =
            stbi_load(asset.second.filename.c_str(), &width, &height, &nrChannels, 0);

        if (data) {
            GLuint textureID;
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);

            // 设置纹理环绕方式
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            // 设置纹理过滤 (对应 SOIL_FLAG_MIPMAPS)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // 检测是 RGB 还是 RGBA (透明通道)
            GLenum format = GL_RGB;
            if (nrChannels == 4)
                format = GL_RGBA;

            // 生成纹理
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE,
                         data);

            // 生成 Mipmap (替代 SOIL 的自动生成)
            glGenerateMipmap(GL_TEXTURE_2D);

            // 释放由 stb_image 分配的内存
            stbi_image_free(data);

            // 保存 ID
            asset.second.texture = textureID;
        } else {
            std::cout << "Error loading " << asset.second.filename << std::endl
                      << "-- STB Image loading error : " << stbi_failure_reason() << std::endl;
            return false;
        }
    }

    m_loaded = true;
    return true;
}

SpriteInfo SpriteManager::GetSpriteInfo(ImageID imageID, AnimID animID) const {
    auto it = m_spriteInfos.find(EncodeAnim(imageID, animID));
    if (it == m_spriteInfos.end()) {
        return SpriteInfo("", 0, 0, 0, 0);
    } else {
        return it->second;
    }
}