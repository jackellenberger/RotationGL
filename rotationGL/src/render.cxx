#include "render.hxx"
#include <map>
/* The path to the shaders; this is usually set from the compiler command-line.
 * but it defaults to a path relative to the build directory.
 */
#ifndef SHADER_DIR
#  define SHADER_DIR "../shaders/"
#endif

cs237::vec3f colors[3] = {cs237::vec3f(1.0f,0.0f,0.0f),
                          cs237::vec3f(0.0f,1.0f,0.0f),
                          cs237::vec3f(0.0f,0.0f,1.0f)};

//! Load a shader from the file system and compile and link it.  We keep
//! a cache of shaders that have already been loaded to allow two renderers
//! to share the same shader program without compiling it twice.
//
static cs237::ShaderProgram *LoadShader (std::string const & shaderPrefix)
{
  static std::map<std::string, cs237::ShaderProgram *> Shaders;

  auto it = Shaders.find(shaderPrefix);
  if (it == Shaders.end()) {
    // load, compile, and link the shader program
    cs237::VertexShader vsh((shaderPrefix + ".vsh").c_str());
    cs237::FragmentShader fsh((shaderPrefix + ".fsh").c_str());
    cs237::ShaderProgram *shader = new cs237::ShaderProgram (vsh, fsh);
    if (shader == nullptr) {
      std::cerr << "Cannot build " << shaderPrefix << std::endl;
      exit (1);
    }
    Shaders.insert (std::pair<std::string, cs237::ShaderProgram *>(shaderPrefix, shader));
    return shader;
  }
  else {
    return it->second;
  }
}

Renderer::~Renderer ()
{ }

/***** class TexturingRenderer member functions *****/

TexturingRenderer::TexturingRenderer()
{
    this->_shader = LoadShader("../shaders/textureShader");
    
    this->modelViewLoc = _shader->UniformLocation("modelView");
    this->projectionLoc = _shader->UniformLocation("projection");
    this->translationLoc = _shader->UniformLocation("translation");
    this->lDirectionLoc = _shader->UniformLocation("direction");
    this->lAmbientLoc = _shader->UniformLocation("ambient");
    this->lIntensityLoc = _shader->UniformLocation("intensity");
    this->lDiffuseLoc = _shader->UniformLocation("diffuse");
    this->samplerLoc = _shader->UniformLocation("sampler"); 
}

TexturingRenderer::~TexturingRenderer (){ }

void TexturingRenderer::Enable (cs237::mat4f const &projectionMat)
{
    _shader->Use();
    cs237::setUniform(projectionLoc,projectionMat);
    glEnable(GL_DEPTH_TEST);
}

void TexturingRenderer::Render (cs237::mat4f const &modelViewMat, Mesh *mesh)
{
  cs237::setUniform (translationLoc, mesh->position);
  cs237::setUniform (modelViewLoc, modelViewMat);
  cs237::setUniform (lDirectionLoc, mesh->lightDir);
  cs237::setUniform (lAmbientLoc, mesh->ambientLight);
  cs237::setUniform (lIntensityLoc, mesh->lightIntensity);
  cs237::setUniform (lDiffuseLoc, mesh->diffuse);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glActiveTexture(GL_TEXTURE0);    
  mesh->texture->Bind();
  cs237::setUniform(samplerLoc, 0);
  mesh->Draw();
}

/***** class BoxRenderer member functions *****/

BoxRenderer::BoxRenderer()
{
  this->_shader = LoadShader("../shaders/BoxShader");
  
  this->modelViewLoc = _shader->UniformLocation("modelView");
  this->projectionLoc = _shader->UniformLocation("projection");
  this->translationLoc = _shader->UniformLocation("translation");
  this->colorLoc = _shader->UniformLocation("color");
  this->openFaceLoc = _shader->UniformLocation("openFace");
  this->samplerLoc = _shader->UniformLocation("sampler"); 
}

BoxRenderer::~BoxRenderer (){ }

void BoxRenderer::Enable (cs237::mat4f const &projectionMat)
{
  _shader->Use();
  cs237::setUniform(projectionLoc,projectionMat);
  glEnable(GL_DEPTH_TEST);
}

void BoxRenderer::Render (cs237::mat4f const &modelViewMat, Mesh *mesh)
{
  cs237::setUniform (translationLoc, mesh->position);
  cs237::setUniform (modelViewLoc, modelViewMat);
  cs237::setUniform (colorLoc, mesh->color);
  cs237::setUniform (openFaceLoc, mesh->openFace);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glActiveTexture(GL_TEXTURE0);    
  mesh->texture->Bind();
  cs237::setUniform(samplerLoc, 0);
  mesh->Draw();
}


