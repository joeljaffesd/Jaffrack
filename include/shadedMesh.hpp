#ifndef SHADED_MESH_HPP
#define SHADED_MESH_HPP

// std includes 
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

// al includes 
#include "al/graphics/al_Shader.hpp"
#include "al/graphics/al_Mesh.hpp"
#include "al/graphics/al_Graphics.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/graphics/al_Texture.hpp"

/**
 * @brief Mesh with associated ShaderProgram.
 */
class ShadedMesh : public al::VAOMesh {
protected:
  al::ShaderProgram mShader;
  al::Texture mTexture;

public:
  al::ShaderProgram& shader() {
    return this->mShader;
  }

  /// sphere vertices
  /// @param r sphere radius
  /// @param subdiv number of subdivisions
  void seed() {
    this->reset();
    al::addTexQuad(*this);
    this->setShaders("../../shaders/standard.vert", "../../shaders/julia.frag");
  }  

  // Load and set shaders. bool so we can immediately know if it worked.
  // Set vertex and fragment shaders from file paths
  inline bool setShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    std::string vertexSource = loadFile(vertexShaderPath);
    std::string fragmentSource = loadFile(fragmentShaderPath);

    if (vertexSource.empty() || fragmentSource.empty()) {
      std::cerr << "ShaderMesh Error: Shader source file empty.\n";
      return false;
    }

    if (!mShader.compile(vertexSource, fragmentSource)) {
      std::cerr << "ShaderMesh Error: Shader failed to compile.\n";
      mShader.printLog();
      return false;
    }

    std::cout << "ShaderMesh: Shaders compiled successfully.\n";
    return true;
  }

  /// Draw the sphere
  void draw(al::Graphics& g) {
    static bool first = true;
    if (first) {
      this->update();
      first = false;
    }
    g.shader(this->shader());
    g.draw(*this);
  }  

  // Uniform setters (will add overloads as needed)
  // Load a shader source file into a string
  inline std::string loadFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
      std::cerr << "Shader Error: Cannot open file " << filePath << "\n";
      return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf(); // Slurp contents into memory
    return buffer.str();    // Convert to usable string
  }


  // Set a single int uniform
  /// @param name The uniform name inside the shader
  /// @param value The int value to send
  inline void setUniformInt(const std::string& name, int value) {
    mShader.use();
    mShader.uniform(name.c_str(), value);
  }  

  // Set a single float uniform
  /// @param name The uniform name inside the shader
  /// @param value The float value to send
  inline void setUniformFloat(const std::string& name, float value) {
    mShader.use(); // 🚨 Make sure shader is active
    int loc = mShader.getUniformLocation(name.c_str());
    if (loc >= 0) {
      mShader.uniform(loc, value);
    } else {
      static std::unordered_map<std::string, bool> warned;
      if (warned.find(name) == warned.end()) {
        std::cerr << "[Warning] Uniform '" << name << "' not found in shader.\n";
        warned[name] = true;
      }
    }
  }

  // Set a vec3 uniform (3 floats: x, y, z)
  /// @param name The uniform name inside the shader
  /// @param value (x, y, z)
  inline void setUniformVec3f(const std::string& name, const al::Vec3f& vec) {
    mShader.use();
    mShader.uniform(name.c_str(), vec);
  }

  // Set a mat4 uniform (4x4 matrix)
  /// @param name The uniform name inside the shader
  /// @param value 4x4 matrix
  inline void setUniformMat4f(const std::string& name, const al::Mat4f& mat) {
    mShader.use();
    mShader.uniform(name.c_str(), mat);
  }

  //updating for spherical purposes, not sure if this will workl:
  inline void setMatrices(const al::Mat4f& view, const al::Mat4f& proj) {
    mShader.use(); //Important: bind before setting uniforms
    mShader.uniform("al_ModelViewMatrix", view);
    mShader.uniform("al_ProjectionMatrix", proj);
  }
};
#endif // SHADED_MESH_HPP