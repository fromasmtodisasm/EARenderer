//
//  GLLDRTextureCubemap.cpp
//  EARenderer
//
//  Created by Pavlo Muratov on 18.03.2018.
//  Copyright © 2018 MPO. All rights reserved.
//

#include "GLLDRTextureCubemap.hpp"
#include "StringUtils.hpp"
#include "stb_image.h"

#include <stdexcept>

namespace EARenderer {

#pragma mark - Lifecycle

    GLLDRTextureCubemap::GLLDRTextureCubemap(const Size2D& size, Filter filter) {
        std::array<void *, 6> nullptrs;
        nullptrs.fill(nullptr);
        initialize(size, filter, WrapMode::ClampToEdge, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, nullptrs);
    }
    
    GLLDRTextureCubemap::GLLDRTextureCubemap(const std::string& rightImagePath,
                                             const std::string& leftImagePath,
                                             const std::string& topImagePath,
                                             const std::string& bottomImagePath,
                                             const std::string& frontImagePath,
                                             const std::string& backImagePath)
    {
        int32_t width = 0;
        int32_t height = 0;
        int32_t components = 0;

        std::array<std::string, 6> paths = {
            rightImagePath, leftImagePath, topImagePath,
            bottomImagePath, frontImagePath, backImagePath
        };

        std::array<void *, 6> pixelDataArray;

        stbi_set_flip_vertically_on_load(true);

        for(GLuint i = 0; i < 6; i++) {
            stbi_uc *pixelData = stbi_load(paths[i].c_str(), &width, &height, &components, STBI_rgb_alpha);

            if (!pixelData) {
                throw std::runtime_error(string_format("Unable to read texture file: %s", paths[i].c_str()));
            }

            pixelDataArray[i] = pixelData;
        }

        GLint texComponents;
        switch (components) {
            case 1: texComponents = GL_RED; break;
            case 2: texComponents = GL_RG; break;
            case 3: texComponents = GL_RGB; break;
            default: texComponents = GL_RGBA; break;
        }

        initialize(Size2D(width, height), Filter::Trilinear, WrapMode::Repeat, texComponents, GL_RGBA, GL_UNSIGNED_BYTE, pixelDataArray);

        for (auto buffer : pixelDataArray) {
            stbi_image_free(buffer);
        }
    }

}