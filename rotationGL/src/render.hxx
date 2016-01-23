/*! \file render.hxx
 *
 * \author John Reppy
 */

/* CMSC23700 Project 1 sample code (Autumn 2014)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

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
    cs237::ShaderProgram	*_shader;	//!< the shader program

    /** YOUR CODE HERE **/
    GLint modelViewLoc;
    GLint projectionLoc;
    GLint colorLoc;
    GLint translationLoc;
    GLint samplerLoc;
    GLint lDirectionLoc;
    GLint lAmbientLoc;
    GLint lIntensityLoc;
    GLint lDiffuseLoc;
    GLint edgeLoc;

    //Renderer (cs237::ShaderProgram *sh);


};

class WireframeRenderer : public Renderer {
  public:
    WireframeRenderer ();
    virtual ~WireframeRenderer ();

    void Enable (cs237::mat4f const &projectionMat);

    /** YOUR CODE HERE **/
    void Render (cs237::mat4f const &modelViewMat, Mesh *mesh);
    //Renderer (cs237::ShaderProgram *sh);
};

class FlatShadingRenderer : public Renderer {
  public:
    FlatShadingRenderer ();
    virtual ~FlatShadingRenderer ();

    void Enable (cs237::mat4f const &projectionMat);

    /** YOUR CODE HERE **/
    void Render (cs237::mat4f const &modelViewMat, Mesh *mesh);
    //Renderer (cs237::ShaderProgram *sh);
};

class LightingRenderer : public Renderer {
  public:
    LightingRenderer ();
    virtual ~LightingRenderer ();

    void Enable (cs237::mat4f const &projectionMat);

    /** YOUR CODE HERE **/
    void Render (cs237::mat4f const &modelViewMat, Mesh *mesh);
    //Renderer (cs237::ShaderProgram *sh);
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
