// Main.c

//Based on project 2 stub code by Prof John Reppy 
//  and project 2 solution code by Jack Ellenberger (2015)

#include "cs237.hxx"
#include "scene.hxx"
#include "view.hxx"

/*! \brief Set the viewport to the current size of the framebufffer.
 *         We use the framebuffer size instead of the window size, because it
 *         is different on Apple's retina displays.
 */
static void SetViewport (GLFWwindow *win)
{
  int fbWid, fbHt;
  glfwGetFramebufferSize (win, &fbWid, &fbHt);
  glViewport(0, 0 , fbWid, fbHt);
}


static void Error (int err, const char *msg)
{
  std::cerr << "[GLFW ERROR " << err << "] " << msg << std::endl;
}

// A helper function to call View->Render() and swap buffers.
// Necissary because glfwSetWindowRefreshCallback()'s second param
void Display (GLFWwindow *win)
{
  // start by grabbing the glfw window under the pointer
  View *view = (View *)glfwGetWindowUserPointer(win);
  view->Render ();
  glfwSwapBuffers (win);
} 

// a helper function to feed to glfwSetWindowIconifyCallback
// 
void Visible (GLFWwindow *win, int state)
{
    View *view = (View *)glfwGetWindowUserPointer(win);
    view->isVis = (state == GL_FALSE);
}

// allow for the window to be reshaped
void Reshape (GLFWwindow *win, int wid, int ht)
{
  View *view = (View *)glfwGetWindowUserPointer(win);

  view->wid = wid;
  view->ht = ht;

  SetViewport (win);

  view->InitProjMatrix();
}

// manage keyboard input
void Key (GLFWwindow *win, int key, int scancode, int action, int mods)
{
  View *view = (View *)glfwGetWindowUserPointer(win);
	switch (key) {
    case GLFW_KEY_Q:  // quit
      glfwSetWindowShouldClose (win, true);
      break;
    case GLFW_KEY_R:  //restart
      view->restart = true;
      view->gamePlayLoop = true;
      break;
    case GLFW_KEY_W:  // wireframe mode 
      view->mode = WIREFRAME;
      break;
    case GLFW_KEY_F:  // flat shading mode
      view->mode = FLAT_SHADING;
      break;
    case GLFW_KEY_D: // diffuse lighting mode
      view->mode = LIGHTING;
      break;
    case GLFW_KEY_T: // textured mode
      view->mode = TEXTURING;
      break;
    case GLFW_KEY_UP:
      view->rotating = (view->rotating == DOWN ? NONE : UP);
      break;
    case GLFW_KEY_DOWN:
      view->rotating = (view->rotating == UP ? NONE : DOWN);
      break;
    case GLFW_KEY_LEFT:
      view->rotating = (view->rotating == RIGHT ? NONE : LEFT);
      break;
    case GLFW_KEY_RIGHT:
      view->rotating = (view->rotating == LEFT ? NONE : RIGHT);
      break;
    default:
      break; //do nothing
  }
  if (action == GLFW_RELEASE){
    view->rotating = NONE;
  }
}


int main(int argc, const char **argv)
{
  // first we load the scene file
  Scene scene;
  if (scene.Load("../resources/objects")) {
    return EXIT_FAILURE;
  }

  glfwSetErrorCallback (Error);

  glfwInit ();

  // Check the GLFW version
  {
    int GLFWmajor;
    glfwGetVersion (&GLFWmajor, NULL, NULL);
    if (GLFWmajor < 3) {
        std::cerr << "CS237 requires GLFW 3.0 or later" << std::endl;
        exit (EXIT_FAILURE);
    }
  }

  glfwWindowHint (GLFW_RESIZABLE, GL_TRUE);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window = glfwCreateWindow(scene.Width(), 
                                        scene.Height(),
                                        "RotationGL", 
                                        NULL, NULL);
  if (window == nullptr)
   exit (EXIT_FAILURE);

  // Check the OpenGL version
  {
    GLint major, minor;
    glGetIntegerv (GL_MAJOR_VERSION, &major);
    glGetIntegerv (GL_MINOR_VERSION, &minor);
    if ((major < 4) || ((major == 4) && (minor < 1))) {
      std::cerr << "CS237 requires OpenGL 4.1 or later; found " << major << "." << minor << std::endl;
      exit (EXIT_FAILURE);
    }
  }
  glfwMakeContextCurrent (window);
  SetViewport (window);

  // Create the new view we will use
  View *view = new View(scene, window);

  // Initialize renderers and matrices
  view->InitRenderers ();
  view->InitProjMatrix ();
  view->InitModelViewMatrix ();

  // set up callbacks
  glfwSetWindowRefreshCallback (window, Display);
  glfwSetWindowSizeCallback    (window, Reshape);
  glfwSetWindowIconifyCallback (window, Visible);
  glfwSetKeyCallback           (window, Key);

  // display a frame as soon as it's ready
  while (! glfwWindowShouldClose(window)) {
    Display (window);      
    view->Animate();
    //usleep(100);
    glfwPollEvents();
  }
  glfwTerminate ();
  return EXIT_SUCCESS;  
}

