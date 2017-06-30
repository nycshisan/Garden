//
//  WindowContext.hpp
//  WindowContext
//
//  Created by Nycshisan on 2017/4/27.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef WindowContext_hpp
#define WindowContext_hpp

// This ignoring is to suppress the documentation issues of GLFW
#pragma clang diagnostic ignored "-Wdocumentation"

#include <cstdio>
#include <cassert>
#include <utility>
#include <unistd.h>
#include <memory>
#include <x86intrin.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vec.hpp"


typedef unsigned int coord_t;

class WindowContext {
private:
    GLfloat *pixelsFG = nullptr, *pixelsBG = nullptr;
    
    double lastFrameTime = 0.0;
    double fpsBoundary = 0.0;
    double targetInterval;
    
    char titleBuffer[128];
    
    GLFWwindow* window = nullptr;
    
    const char* vertexShaderText =
    "attribute vec2 a_Position;\n"
    "attribute vec2 a_TexCoord;\n"
    "varying vec2 v_TexCoord;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(a_Position, 0.0, 1.0);\n"
    "    v_TexCoord = a_TexCoord;\n"
    "}\n";
    
    const char* fragmentShaderText =
    "varying vec2 v_TexCoord;\n"
    "uniform sampler2D u_Sampler;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = texture2D(u_Sampler, v_TexCoord);\n"
    "}\n";
    
    GLfloat vertices[16] = {
    //  Position      Texcoords
        -1.0f, +1.0f, 0.0f, 0.0f, // Top-left
        +1.0f, +1.0f, 1.0f, 0.0f, // Top-right
        +1.0f, -1.0f, 1.0f, 1.0f, // Bottom-right
        -1.0f, -1.0f, 0.0f, 1.0f  // Bottom-left
    };
    
    GLuint elements[6] = {
        0, 1, 2,
        2, 3, 0
    };
    
    static void errorCallback(int error, const char* description) {
        fprintf(stderr, "Error: %s\n", description);
    }
    
    bool initWindow();
    
    void clearPixel(GLfloat *p) {
        memset(p, 0, sizeof(GLfloat) * width * height * 4);
    }
    
    void boundFPS() {
        if (fpsBoundary > 0) {
            double now = glfwGetTime();
            double currentInterval = now - lastFrameTime;
            lastFrameTime = now;
            int wait = int((targetInterval - currentInterval) * 1000000);
            if (wait > 0) {
                usleep(wait);
            }
        }
    }
    
    void updateFPS() {
        double interval = glfwGetTime() - lastFrameTime;
        double fps = 1 / interval;
        sprintf(titleBuffer, "%s - FPS: %.1f", title, fps);
        glfwSetWindowTitle(window, titleBuffer);
    }
    
    
public:
    unsigned int width = 0, height = 0;
    const char *title = nullptr;
    
    WindowContext(int width, int height, const char *title): width(width), height(height), title(title) {
        initWindow();
        clearPixel(pixelsFG);
        clearPixel(pixelsBG);
        lastFrameTime = glfwGetTime();
    }
    
    bool windowWillClose() {
        return glfwWindowShouldClose(window);
    }
    
    void presentScene() {
        static int respondCount = 0, respondInterval = 3;
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, pixelsFG);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        std::swap(pixelsFG, pixelsBG);
        clearPixel(pixelsBG);
        updateFPS();
        boundFPS();
        
        if (respondCount == 0) {
            glfwPollEvents();
        }
        ++respondCount;
        respondCount %= respondInterval;
    }
    
    void setPixel(coord_t x, coord_t y, const vec4 &color) {
        assert(x >= 0 && x < width);
        assert(y >= 0 && y < height);
        GLfloat *position = (pixelsFG + (x + width * y) * 4);
        
        _mm_stream_ps(position, color.m128);
    }
    
    void setFPSBoundary(double fps) {
        assert(fps > 0.0);
        fpsBoundary = fps;
        targetInterval = 1 / fpsBoundary;
    }
    
    ~WindowContext() {
        delete[] pixelsFG;
        delete[] pixelsBG;
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

bool WindowContext::initWindow() {
    // Initialize GLFW
    glfwSetErrorCallback(errorCallback);
    
    if (!glfwInit()) {
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    
    if (!window) {
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(window);
    
    glfwSwapInterval(1);
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    
    
    // OpenGL configuration
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    GLuint elementBuffer;
    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderText, NULL);
    glCompileShader(vertexShader);
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderText, NULL);
    glCompileShader(fragmentShader);
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glUseProgram(program);
    
    GLint aPositionLoc = glGetAttribLocation(program, "a_Position");
    GLint aTexCoordLoc = glGetAttribLocation(program, "a_TexCoord");
    GLint uSamplerLoc = glGetUniformLocation(program, "u_Sampler");
    glEnableVertexAttribArray(aPositionLoc);
    glVertexAttribPointer(aPositionLoc, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void*) 0);
    glEnableVertexAttribArray(aTexCoordLoc);
    glVertexAttribPointer(aTexCoordLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void*) (sizeof(GLfloat) * 2));
    glUniform1i(uSamplerLoc, 0);
    
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Initialize pixels
    pixelsFG = new GLfloat[width * height * 4];
    pixelsBG = new GLfloat[width * height * 4];
    
    return true;
}

#endif /* WindowContext_hpp */
