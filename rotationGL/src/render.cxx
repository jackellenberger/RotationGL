/*! \file render.cxx
 *
 * \author John Reppy
 */

/* CMSC23700 Project 1 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "render.hxx"
#include <map>
/* The path to the shaders; this is usually set from the compiler command-line.
 * but it defaults to a path relative to the build directory.
 */
#ifndef SHADER_DIR
#  define SHADER_DIR "../shaders/"
#endif

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

/***** class WireframeRenderer member functions *****/

WireframeRenderer::WireframeRenderer()
{
    //std::cout<<"WireframeRenderer::WireframeRenderer\n";
    CS237_CHECK( this->_shader = LoadShader("../shaders/wffsShader"));

    CS237_CHECK( this->modelViewLoc = _shader->UniformLocation("modelView") );
    CS237_CHECK( this->projectionLoc = _shader->UniformLocation("projection") );
    CS237_CHECK( this->translationLoc = _shader->UniformLocation("translation") );
    

    this->colorLoc = _shader->UniformLocation("vColor"); 
}

WireframeRenderer::~WireframeRenderer () { }

void WireframeRenderer::Enable (cs237::mat4f const &projectionMat)
{
      //std::cout<<"WireframeRenderer::Enable\n";

    CS237_CHECK( _shader->Use() );
    //std::cout<<colorLoc<<"\t"<<samplerLoc<<"\n";
    CS237_CHECK( cs237::setUniform(projectionLoc,projectionMat) );
    CS237_CHECK( glEnable(GL_DEPTH_TEST) );
}
void WireframeRenderer::Render (cs237::mat4f const &modelViewMat, Mesh *mesh)
{
      //std::cout<<"WireframeRenderer::Render\n";

  CS237_CHECK( cs237::setUniform (translationLoc, mesh->position) );
  CS237_CHECK( cs237::setUniform (modelViewLoc, modelViewMat) );
  CS237_CHECK( cs237::setUniform (colorLoc, mesh->color) );
  CS237_CHECK( glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) );
  CS237_CHECK( mesh->Draw() );

}

/***** class FlatShadingRenderer member functions *****/

FlatShadingRenderer::FlatShadingRenderer()
{
    //std::cout<<"FlatShadingRenderer::FlatShadingRenderer\n";
    CS237_CHECK( this->_shader = LoadShader("../shaders/wffsShader"));

    CS237_CHECK( this->modelViewLoc = _shader->UniformLocation("modelView") );
    CS237_CHECK( this->projectionLoc = _shader->UniformLocation("projection") );
    CS237_CHECK( this->translationLoc = _shader->UniformLocation("translation") );

    this->colorLoc = _shader->UniformLocation("vColor"); 
}

FlatShadingRenderer::~FlatShadingRenderer (){ }

void FlatShadingRenderer::Enable (cs237::mat4f const &projectionMat)
{
      //std::cout<<"FlatShadingRenderer::Enable\n";

    CS237_CHECK( _shader->Use() );
    CS237_CHECK( cs237::setUniform(projectionLoc,projectionMat) );
    CS237_CHECK( glEnable(GL_DEPTH_TEST) );
}

void FlatShadingRenderer::Render (cs237::mat4f const &modelViewMat, Mesh *mesh)
{
  //std::cout<<"FlatShadingRenderer::Render\n";
  
  CS237_CHECK( cs237::setUniform (translationLoc, mesh->position) );
  CS237_CHECK( cs237::setUniform (modelViewLoc, modelViewMat) );
  CS237_CHECK( cs237::setUniform (colorLoc, mesh->color) );
  CS237_CHECK( glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) );
  CS237_CHECK( mesh->Draw() );
}

/***** class LightingRenderer member functions *****/

LightingRenderer::LightingRenderer()
{
    //std::cout<<"LightingRenderer::LightingRenderer\n";
    CS237_CHECK( this->_shader = LoadShader("../shaders/lightingShader"));
    
    CS237_CHECK( this->modelViewLoc = _shader->UniformLocation("modelView") );
    CS237_CHECK( this->projectionLoc = _shader->UniformLocation("projection") );
    CS237_CHECK( this->translationLoc = _shader->UniformLocation("translation") );
    CS237_CHECK( this->lDirectionLoc = _shader->UniformLocation("direction") );
    CS237_CHECK( this->lAmbientLoc = _shader->UniformLocation("ambient") );
    CS237_CHECK( this->lIntensityLoc = _shader->UniformLocation("intensity") );
    CS237_CHECK( this->lDiffuseLoc = _shader->UniformLocation("diffuse") );

    this->colorLoc = _shader->UniformLocation("vColor"); 
}

LightingRenderer::~LightingRenderer (){ }

void LightingRenderer::Enable (cs237::mat4f const &projectionMat)
{
      //std::cout<<"LightingRenderer::Enable\n";

    CS237_CHECK( _shader->Use() );
    CS237_CHECK( cs237::setUniform(projectionLoc,projectionMat) );
    CS237_CHECK( glEnable(GL_DEPTH_TEST) );
}

void LightingRenderer::Render (cs237::mat4f const &modelViewMat, Mesh *mesh)
{
  //std::cout<<"LightingRenderer::Render\n";
  
  CS237_CHECK( cs237::setUniform (translationLoc, mesh->position) );
  CS237_CHECK( cs237::setUniform (modelViewLoc, modelViewMat) );
  CS237_CHECK( cs237::setUniform (lDirectionLoc, mesh->lightDir) );
  CS237_CHECK( cs237::setUniform (lAmbientLoc, mesh->ambientLight) );
  CS237_CHECK( cs237::setUniform (lIntensityLoc, mesh->lightIntensity) );
  CS237_CHECK( cs237::setUniform (lDiffuseLoc, mesh->diffuse) );
  CS237_CHECK( cs237::setUniform (colorLoc, mesh->color) );

  CS237_CHECK( glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) );
  CS237_CHECK( mesh->Draw() );
}

/***** class TexturingRenderer member functions *****/

TexturingRenderer::TexturingRenderer()
{
    //std::cout<<"TexturingRenderer::TexturingRenderer\n";
    CS237_CHECK( this->_shader = LoadShader("../shaders/textureShader"));
    
    CS237_CHECK( this->modelViewLoc = _shader->UniformLocation("modelView") );
    CS237_CHECK( this->projectionLoc = _shader->UniformLocation("projection") );
    CS237_CHECK( this->translationLoc = _shader->UniformLocation("translation") );
    CS237_CHECK( this->lDirectionLoc = _shader->UniformLocation("direction") );
    CS237_CHECK( this->lAmbientLoc = _shader->UniformLocation("ambient") );
    CS237_CHECK( this->lIntensityLoc = _shader->UniformLocation("intensity") );
    CS237_CHECK( this->lDiffuseLoc = _shader->UniformLocation("diffuse") );

    this->samplerLoc = _shader->UniformLocation("sampler"); 
}

TexturingRenderer::~TexturingRenderer (){ }

void TexturingRenderer::Enable (cs237::mat4f const &projectionMat)
{
      //std::cout<<"TexturingRenderer::Enable\n";
    CS237_CHECK( _shader->Use() );
    CS237_CHECK( cs237::setUniform(projectionLoc,projectionMat) );
    CS237_CHECK( glEnable(GL_DEPTH_TEST) );
}

void TexturingRenderer::Render (cs237::mat4f const &modelViewMat, Mesh *mesh)
{
  CS237_CHECK( cs237::setUniform (translationLoc, mesh->position) );
  CS237_CHECK( cs237::setUniform (modelViewLoc, modelViewMat) );
  CS237_CHECK( cs237::setUniform (lDirectionLoc, mesh->lightDir) );
  CS237_CHECK( cs237::setUniform (lAmbientLoc, mesh->ambientLight) );
  CS237_CHECK( cs237::setUniform (lIntensityLoc, mesh->lightIntensity) );
  CS237_CHECK( cs237::setUniform (lDiffuseLoc, mesh->diffuse) );

  CS237_CHECK( glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) );
  CS237_CHECK(glActiveTexture(GL_TEXTURE0));    
  mesh->texture->Bind();
  cs237::setUniform(samplerLoc, 0);
  CS237_CHECK( mesh->Draw() );
}


