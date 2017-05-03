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

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}


class WindowContext {
private:
    float *pixels = nullptr;
    
    unsigned int width = 0, height = 0;
    const char *title = nullptr;
    
    GLFWwindow* window = nullptr;
    
    const char* vertex_shader_text =
    "attribute vec2 a_Position;\n"
    "attribute vec2 a_TexCoord;\n"
    "varying vec2 v_TexCoord;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(a_Position, 0.0, 1.0);\n"
    "    v_TexCoord = a_TexCoord;\n"
    "}\n";
    
    const char* fragment_shader_text =
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
    
    bool initWindow() {
        // Initialize GLFW
        glfwSetErrorCallback(error_callback);
        
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
        GLuint vertex_buffer;
        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        GLuint element_buffer;
        glGenBuffers(1, &element_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
        
        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
        glCompileShader(vertex_shader);
        
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
        glCompileShader(fragment_shader);
        
        GLuint program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);
        glUseProgram(program);
        
        GLint a_Position_loc = glGetAttribLocation(program, "a_Position");
        GLint a_TexCoord_loc = glGetAttribLocation(program, "a_TexCoord");
        GLint u_Sampler_loc = glGetUniformLocation(program, "u_Sampler");
        glEnableVertexAttribArray(a_Position_loc);
        glVertexAttribPointer(a_Position_loc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) 0);
        glEnableVertexAttribArray(a_TexCoord_loc);
        glVertexAttribPointer(a_TexCoord_loc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) (sizeof(float) * 2));
        glUniform1i(u_Sampler_loc, 0);
        
        GLuint texture_id;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // Initialize pixels
        pixels = new GLfloat[width * height * 4];
        
        return true;
    }
    
public:
    WindowContext(int width, int height, const char *title): width(width), height(height), title(title) {
        initWindow();
    }
    
    bool windowWillClose() {
        return glfwWindowShouldClose(window);
    }
    
    void presentScene() {
        glClear(GL_COLOR_BUFFER_BIT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, pixels);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    void setPixel(unsigned int x, unsigned int y, float r, float g, float b, float a) {
        *(pixels + (x + width * y) * 4 + 0) = r;
        *(pixels + (x + width * y) * 4 + 1) = g;
        *(pixels + (x + width * y) * 4 + 2) = b;
        *(pixels + (x + width * y) * 4 + 3) = a;
    }
    
    ~WindowContext() {
        delete[] pixels;
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

#endif /* WindowContext_hpp */
