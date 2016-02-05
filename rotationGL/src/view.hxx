#ifndef _VIEW_HXX_
#define _VIEW_HXX_

#include "cs237.hxx"
#include "scene.hxx"
#include "render.hxx"
// additional imports
#include "mesh.hxx"
#include "obj.hxx"
#include <list>

enum RotationDirection {
  NONE = 0,
  UP,
  DOWN,
  LEFT,
  RIGHT
};

/*! \brief The current state of the view */
struct View {
  // window state
  GLFWwindow		*win;   // the application window
  int     wid, ht;      // window dimensions
  bool    isVis;        // true, when the window is visible
  bool		shouldExit;	  // set to true when the application should exit
  bool		needsRedraw;	// set to true when the display is out of date
  bool restart;         // gameplay restart state
  bool gamePlayLoop;    // is the game running?

  // Camera state
  cs237::vec3f	camPos;  // camera position in world space
  cs237::vec3f	camAt;   // camera look-at point in world space
  cs237::vec3f	camUp;   // camera up vector in world space
  float		fov;		       // Horizontal field of view specified by scene

  //camera rotations
  cs237::vec3f modelRotation;
  cs237::vec3f cameraRotation;
  cs237::vec4f xRotationAxis;
  cs237::vec4f yRotationAxis;
  RotationDirection rotating;

  // view transformation 
  cs237::mat4f	modelViewMat;	// the obstacles' model-view matrix
  cs237::mat4f  playerMVM;    // the player's model-view matrix
  bool MVMinitialized;        // prevents reinitialization overhead
  cs237::mat4f	projectionMat;// the current projection matrix

  // renderers
  Renderer    *tRender; // the renderer for textured player
  Renderer		*bRender;	// the renderer for textured boxes

  // animation state
  double _lastStep, _lastFrameTime, startTime;

  // scene info
  int numObjects;
  std::vector<Mesh> meshObjects;
  std::vector<float> modelScales;

  // functions
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
