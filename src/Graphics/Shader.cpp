// Stephan Vedder 2015
#include "Shader.hpp"
#include <fstream>
#include <vector>
#include <iostream>


using namespace Graphics;

Shader::Shader() : m_program(0), m_shaders({{0, 0, 0}})
{
    m_program = glCreateProgram();
}

Shader::~Shader()
{
    if (m_shaders[VERTEX_SHADER])
        glDeleteShader(m_shaders[VERTEX_SHADER]);

    if (m_shaders[GEOMETRY_SHADER])
        glDeleteShader(m_shaders[GEOMETRY_SHADER]);

    if (m_shaders[FRAGMENT_SHADER])
        glDeleteShader(m_shaders[FRAGMENT_SHADER]);

    if (m_program)
        glDeleteProgram(m_program);
}


void Shader::LoadFromMemory(const Type type, const std::string & content)
{

    switch (type)
    {
    case VERTEX_SHADER:
        m_shaders[type] = glCreateShader(GL_VERTEX_SHADER);
        break;
    case GEOMETRY_SHADER:
        m_shaders[type] = glCreateShader(GL_GEOMETRY_SHADER);
        break;
    case FRAGMENT_SHADER:
        m_shaders[type] = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    }

    const GLchar* src = reinterpret_cast<const GLchar*>(content.c_str());
    const GLint size = content.size();
    glShaderSource(m_shaders[type], 1, &src, &size);

    glCompileShader(m_shaders[type]);

    GLint status = 0;
    glGetShaderiv(m_shaders[type], GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint logSize = 0;
        glGetShaderiv(m_shaders[type], GL_INFO_LOG_LENGTH, &logSize);
        std::vector<GLchar> compileLog(logSize);
        glGetShaderInfoLog(m_shaders[type], logSize, &logSize, &compileLog[0]);
        std::cout << &compileLog[0] << std::endl;
    }

    glAttachShader(m_program, m_shaders[type]);
}

void Shader::Link()
{
    GLint status = 0;
    glLinkProgram(m_program);

    glGetProgramiv(m_program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint logSize = 0;
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logSize);
        std::vector<GLchar> linkLog(logSize);
        glGetProgramInfoLog(m_program, logSize, NULL, &linkLog[0]);
        std::cout << &linkLog[0] << std::endl;
    }
}

void Shader::Use() const
{
    glUseProgram(m_program);
}

void Shader::UnUse() const
{
    glUseProgram(0);
}

void Graphics::Shader::AddUniform(const std::string & name)
{
    GLint attr = glGetUniformLocation(m_program, name.c_str());
    m_uniformLocationList[name] = attr;
}

void Graphics::Shader::AddAttribute(const std::string & name)
{
    GLint index = m_attributeList.size();
    glBindAttribLocation(m_program, index, name.c_str());
    m_attributeList[name] = index;
}

GLuint Graphics::Shader::Uniform(const std::string& name)
{

    return m_uniformLocationList[name];
}

GLuint Graphics::Shader::Attribute(const std::string& name)
{
    return m_attributeList[name];
}