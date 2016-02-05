#ifndef _RENDER_HXX_
#define _RENDER_HXX_

#include "cs237.hxx"
#include "mesh.hxx"

//! an abstract base class that wraps a renderer behavior
//
class Renderer {
  public:

  //! enable the renderer
  //! \param projectMat the projection matrix for the current camera state
  virtual void Enable (cs237::mat4f const &projectionMat) = 0;

  virtual ~Renderer ();
  virtual void Render (cs237::mat4f const &modelViewMat, Mesh *mesh) = 0;

  protected:
    // our shader program
    cs237::ShaderProgram	*_shader;

    // we save the location of our uniforms into these variables
    GLint modelViewLoc;
    GLint projectionLoc;
    GLint colorLoc;
    GLint translationLoc;
    GLint samplerLoc;
    GLint lDirectionLoc;
    GLint lAmbientLoc;
    GLint lIntensityLoc;
    GLint lDiffuseLoc;
    GLint openFaceLoc;
};

class TexturingRenderer : public Renderer {
  public:
    TexturingRenderer ();
    virtual ~TexturingRenderer ();

    void Enable (cs237::mat4f const &projectionMat);
    void Render (cs237::mat4f const &modelViewMat, Mesh *mesh);
};

class BoxRenderer : public Renderer {
  public:
    BoxRenderer ();
    virtual ~BoxRenderer ();

    void Enable (cs237::mat4f const &projectionMat);
    void Render (cs237::mat4f const &modelViewMat, Mesh *mesh);
};

#endif // !_RENDER_HXX_
