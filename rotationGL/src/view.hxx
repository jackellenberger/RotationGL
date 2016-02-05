/*! \file view.hxx
 *
 * \brief type definitions for tracking the view state.
 *
 * \author John Reppy
 */

/* CMSC23700 Project 1 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _VIEW_HXX_
#define _VIEW_HXX_

#include "cs237.hxx"
#include "scene.hxx"
#include "render.hxx"
// additional imports
#include "mesh.hxx"
#include "obj.hxx"
#include <list>

  enum RenderMode {
      WIREFRAME = 0,      //!< render scene as a wireframe
      FLAT_SHADING,       //!< render the scene using flat shading
      LIGHTING,           //!< render the scene using the directional lighting
      TEXTURING,          //!< render the scene using directional lighting and textures
      NUM_RENDER_MODES    //!< == to the number of rendering modes
  };

  enum RotationDirection {
    NONE = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT
  };

/*! \brief The current state of the view */
struct View {
    GLFWwindow		*win;		//!< the application window
    bool		shouldExit;	//!< set to true when the application should exit
    bool		needsRedraw;	//!< set to true when the display is out of date
  // Camera state
    cs237::vec3f	camPos;		//!< camera position in world space
    cs237::vec3f	camAt;		//!< camera look-at point in world space
    cs237::vec3f	camUp;		//!< camera up vector in world space
    float		fov;		//!< Horizontal field of view specified by scene
  //camera rotations
    float xRot;
    float yRot;
    float zRot;
    cs237::vec3f modelRotation;
    cs237::vec3f cameraRotation;
    cs237::vec4f xRotationAxis;
    cs237::vec4f yRotationAxis;
    RotationDirection rotating;
    std::vector<float> modelScales;
    bool restart;
  // view info 
    cs237::mat4f	modelViewMat;	//!< the current model-view matrix
    cs237::mat4f  playerMVM; //!< the current model-view matrix
    bool MVMinitialized;
    bool gamePlayLoop;
    cs237::mat4f	projectionMat;	//!< the current projection matrix
    int			wid, ht;	//!< window dimensions
    bool		isVis;		//!< true, when the window is visible
  // rendering state
    RenderMode		mode;		//!< the current rendering mode
    Renderer		*wfRender;	//!< the renderer for wireframe
    Renderer    *fsRender;  //!< the renderer for flat shading
    Renderer    *dlRender;  //!< the renderer for directional / diffuse lighting
    Renderer    *tRender; //!< the renderer for texture
    Renderer		*bRender;	//!< the renderer for texture

    double _lastStep, _lastFrameTime, startTime;
  // scene info
/** YOUR CODE HERE **/
    int numObjects;
    std::vector<Mesh> meshObjects;

    int textureCount;

    View (Scene const &scene, GLFWwindow *win);
    void Animate();

    std::vector<Mesh> createMeshes(Scene const &scene);


  /*! \brief initialize the renderers by loading and compiling their shaders.
   *  Note that this function needs to be called after the current
   *  OpenGL context has been set.
   */
    void InitRenderers ();

  /*! \brief initialize the projection matrix based on the current camera state. */
    void InitProjMatrix ();

  /*! \brief initialize the model-view matrix based on the current camera state. */
    void InitModelViewMatrix ();

  /*! \brief render the state of the scene
   */
    void Render ();

};

#endif /* !_VIEW_HXX_ */
