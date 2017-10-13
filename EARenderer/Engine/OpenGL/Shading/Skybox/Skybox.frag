#version 400 core

// Uniforms

uniform samplerCube uCubeMapTexture;
uniform bool uIsHDR;

// Input

in vec3 oEyeDirection;

// Output

out vec4 oFragmentColor;

// Functions

vec3 ReinhardToneMapAndGammaCorrect(vec3 color) {
    vec3 mappedColor = color / (color + vec3(1.0));
    vec3 gammaCorrectedColor = pow(mappedColor, vec3(1.0 / 2.2));
    return gammaCorrectedColor;
}

void main() {
    oFragmentColor = textureLod(uCubeMapTexture, oEyeDirection, 1);
    
    if (uIsHDR) {
        oFragmentColor = vec4(ReinhardToneMapAndGammaCorrect(oFragmentColor.rgb), 1.0);
    }
}