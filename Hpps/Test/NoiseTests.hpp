//
//  NoiseTests.hpp
//  Garden
//
//  Created by Nycshisan on 2018/4/27.
//  Copyright © 2018年 陈十三. All rights reserved.
//

#ifndef NoiseTests_h
#define NoiseTests_h

#include "Helper.hpp"

// Test Perlin Noise
#define TEST_NAME perlin

DEF_ATTRIBUTE {
public:
    vec4 pos;
};

DEF_UNIFORM {
public:
    const unsigned permutation[512] = {
        151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69,
        142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219,
        203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68,
        175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133,
        230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73,
        209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109,
        198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85,
        212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152,
        2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110,
        79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144,
        12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199,
        106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114,
        67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,
        151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69,
        142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219,
        203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68,
        175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133,
        230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73,
        209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109,
        198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85,
        212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152,
        2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110,
        79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144,
        12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199,
        106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114,
        67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,
    };
};

#define Varying_perlin NullVarying

DEF_PIPELINE;

DEF_VS {
    vertex.position = attribute.pos;
}
DEF_FS {
    float x = frag.pixelX / 8.f, y = frag.pixelY / 8.f;
    auto xi = unsigned(x), yi = unsigned(y);
    float xf = x - xi, yf = y - yi;
    xi &= 0xffu; yi &= 0xffu;
    
    auto hash = [&uniform](unsigned x, unsigned y) -> unsigned { return uniform.permutation[uniform.permutation[x] + y]; };
    auto hlb = hash(xi, yi), hlt = hash(xi, yi + 1), hrb = hash(xi + 1, yi), hrt = hash(xi + 1, yi + 1);
    
    
    auto grad = [](unsigned h, float xf, float yf) -> float {
        switch (h & 0x7u) {
            case 0x0: return xf + 2 * yf;
            case 0x1: return 2 * xf + yf;
            case 0x2: return -xf + 2 * yf;
            case 0x3: return -2 * xf + yf;
            case 0x4: return -xf - 2 * yf;
            case 0x5: return -2 * xf - yf;
            case 0x6: return xf - 2 * yf;
            case 0x7: return 2 * xf - yf;
            default: return 0;
        }
    };
    
    float glb = grad(hlb, xf, yf), glt = grad(hlt, xf, yf - 1.0f), grb = grad(hrb, xf - 1.0f, yf), grt = grad(hrt, xf - 1.0f, yf - 1.0f);
    
    auto lerp = [](float a, float b, float x) -> float { return a + x * (b - a); };
    
    auto fade = [](float t) -> float { return t * t * t * (t * (t * 6 - 15) + 10); };
    float u = fade(xf), v = fade(yf);
    
    float ib = lerp(glb, grb, u), it = lerp(glt, grt, u);
    float i = lerp(ib, it, v);
    
    float noise = std::min(1.f, std::max(0.f, (i + 1.5f) / 3.f));
    fragColor = { noise, noise, noise, 1.0 };
}

DEF_PREPARE {
    pipeline.vertexShader = VS;
    pipeline.fragmentShader = FS;
    
    ATTRIBUTE attr;
    
    attr.pos = {-1.0, 1.0, 0.0, 1.0};
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {-1.0, -1.0, 0.0, 1.0};
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {1.0, -1.0, 0.0, 1.0};
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {1.0, 1.0, 0.0, 1.0};
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {-1.0, 1.0, 0.0, 1.0};
    pipeline.attributeBuffer.emplace_back(attr);
}

DEF_DRAW {
    pipeline.drawArrays(Triangles, 0, 3);
    pipeline.drawArrays(Triangles, 2, 3);
}

#undef TEST_NAME

// Main
void _main(WindowContext *context) {
    Pipeline_perlin pipeline_perlin(context);
    
    prepare(pipeline_perlin);
    
    while (!context->windowWillClose()) {
        draw(pipeline_perlin);
        context->presentScene();
    }
}

#endif /* NoiseTests_h */
