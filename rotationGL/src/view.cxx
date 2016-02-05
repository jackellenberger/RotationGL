#include "cs237.hxx"
#include "view.hxx"

#define NEAR_Z    0.25f    //Should these be constants? UNCLEAR
#define FAR_Z     500.0f
#define TIME_STEP 0.001

cs237::color4f boxColors[3] = {cs237::color4f(1.0f,0.0f,0.0f,1.0f),
                          cs237::color4f(0.0f,1.0f,0.0f,1.0f),
                          cs237::color4f(0.0f,0.0f,1.0f,1.0f)};

cs237::vec3f boxFaces[6] = {cs237::vec3f(0.0f,1.0f,0.0f),
                            cs237::vec3f(0.0f,-1.0f,0.0f),
                            cs237::vec3f(1.0f,0.0f,0.0f),
                            cs237::vec3f(-1.0f,0.0f,0.0f),
                            cs237::vec3f(0.0f,0.0f,1.0f),
                            cs237::vec3f(0.0f,0.0f,-1.0f)};

/* View initialization.
 */
View::View (Scene const &scene, GLFWwindow *win)
{
  // link the window and the view together
  glfwSetWindowUserPointer (win, this);
  this->win = win;
  this->MVMinitialized = false;

  /* view info */
  this->wid		= scene.Width();
  this->ht		= scene.Height();
  this->isVis		= GL_TRUE;
  this->shouldExit	= false;
  this->needsRedraw	= true;
  this->gamePlayLoop = true;
  this->restart = true;

  /* initialize the camera */
  this->camPos	= scene.CameraPos();
  this->camAt		= scene.CameraLookAt();
  this->camUp		= scene.CameraUp();
  float hFOVrad = (float(scene.HorizontalFOV()) * M_PI/180.f);
  float vFOV = (180.0f/M_PI) * (2.0f * atan(tan(hFOVrad/2.0f) * (float(ht)/float(wid))));
  this->fov		= vFOV; //scene.HorizontalFOV();

  this->cameraRotation = cs237::vec3f(0.0f,0.0f,0.0f);
  this->modelRotation = cs237::vec3f(0.0f,0.0f,0.0f);

  /* initialize the rendering state */
  this->numObjects = scene.NumObjects();
  // meshObjects contains the player mesh, followed by obstacles 
  this->meshObjects = createMeshes(scene);

  // modelScales holds the the size of each of the meshObjects
  modelScales.push_back(1.0f);  
  for (int i = 1; i < numObjects; i++){
    modelScales.push_back(20.0f * i);
    meshObjects[i].openFace = boxFaces[rand() % 6];
  }
  this->_lastStep = this->_lastFrameTime = this->startTime = glfwGetTime();
}

void View::InitRenderers ()
{
  this->tRender  = new TexturingRenderer();
  this->bRender  = new BoxRenderer();
}

/* InitProjMatrix:
 *
 * initialize the projection matrix based on the view state.
 */
void View::InitProjMatrix ()
{
    //float vFOV = 2.0 * atan( tan(this->fov / 2.0) * (this->wid / this->ht));
    this->projectionMat = cs237::perspective (
      this->fov,
      (float)this->wid / (float)this->ht,
      NEAR_Z,
      FAR_Z);
}

/* InitModelViewMatrix:
 *
 * initialize the model-view matrix based on the view state.
 */
void View::InitModelViewMatrix ()
{
  if (restart) {
    restart = false;
    this->MVMinitialized = false;
    this->modelRotation = cs237::vec3f(0);
    modelScales[0] = 1.0f;  //player scale
    for (int i = 1; i < numObjects; i++){
      modelScales[i] = (20.0f * i);
      meshObjects[i].openFace = boxFaces[rand() % 6];
    }
  }
  if (this->MVMinitialized == false){
    cs237::mat4f modelViewMat = cs237::lookAt(this->camPos,
                                              this->camAt,
                                              this->camUp);
    modelViewMat = modelViewMat
                 * cs237::translate(this->camPos)
                 * cs237::rotateX(this->cameraRotation.x)
                 * cs237::rotateY(this->cameraRotation.y)
                 * cs237::rotateZ(this->cameraRotation.z)
                 * cs237::translate(this->camPos * -1.0f);
    // set the view variables to the modelViewMat we just declared
    this->playerMVM = this->modelViewMat = modelViewMat;
    // declare our initial rotation axis
    this->xRotationAxis = cs237::vec4f(1,0,0,0);
    this->yRotationAxis = cs237::vec4f(0,1,0,0);
    // rotate the earth so chicago is pointing at you (I live in chicago)
    this->playerMVM = playerMVM 
      * cs237::rotate(90.0f, cs237::vec3f(yRotationAxis))
      * cs237::rotate(-45.0f, cs237::vec3f(0,0,1));
    this->MVMinitialized = true;
  }
  // declare a rotation matrix based on the existing rotation
  cs237::mat4f rotation = cs237::mat4f(1)
                        * cs237::rotate(modelRotation.x, cs237::vec3f(xRotationAxis))
                        * cs237::rotate(modelRotation.y, cs237::vec3f(yRotationAxis));
  // update the axes of rotation depending on movement
  xRotationAxis = xRotationAxis * rotation;
  yRotationAxis = yRotationAxis * rotation;
  // rotate the obstacles
  modelViewMat *= rotation;    
}

void View::Animate ()
{
    double now = glfwGetTime();
    double dt = now - this->_lastStep;
    if (dt >= TIME_STEP) {
      this->_lastStep = now;
      if (gamePlayLoop){
        // loop over the obstacles
        for (int i = 1; i < numObjects; i++){
          // if our obstacle gets to be the size of the player, test for intersection
          if (this->modelScales[i] < 1.5){
            cs237::vec3f openFace = cs237::vec3f(this->modelViewMat * cs237::vec4f(this->meshObjects[i].openFace,0));
            // see if the red circle on earth is inside or outside the obstacle
            float offAngle = cs237::__detail::dot(openFace,cs237::vec3f(0,0,0.2));
            // if we the red circle intersects a wall, game over
            if (offAngle < 0.16){
              this->gamePlayLoop = false;
            }
            // if the player escaped the obstacle, regenerate the obstacle with
            //  a new color and new open face, and set it to be large again
            else{
              this->meshObjects[i].color = boxColors[rand() %3];
              this->meshObjects[i].openFace = boxFaces[rand() % 6];
              this->modelScales[i] = 30.0f;
            }
          }
          // at each timestep decrease the size of the obstacles
          else
            this->modelScales[i] = (this->modelScales[i] - 5.0f*(dt)) - 0.001*(now - startTime);
        }
        // take the keyboard input from main.cxx and turn it into obstacle rotation
        switch(this->rotating){
          case UP:
            this->modelRotation.x = -100.0f*(dt);
            break;
          case DOWN:
            this->modelRotation.x = 100.0f*(dt);
            break;
          case LEFT:
            this->modelRotation.y = 100.0f*(dt);
            break;
          case RIGHT:
            this->modelRotation.y = -100.0f*(dt);
            break;
          case NONE:
            this->modelRotation.y = this->modelRotation.x = 0.0;
          default:
            break;
          }
        this->InitModelViewMatrix();
      }
    }
}

void View::Render ()
{
  /* clear the screen */
  glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // render the player model: a textured sphere (could be any object)
  tRender->Enable(this->projectionMat);
  tRender->Render(this->playerMVM, &(this->meshObjects[0]));

  // blend the obstacles together based on alpha, so you can see one through another
  glEnable( GL_BLEND );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // enable the box render and render each box in turn, scaled by their respective size. 
  bRender->Enable (this->projectionMat);
  for (int i = 1; i < this->meshObjects.size(); i++)
    bRender->Render(modelViewMat * cs237::scale(modelScales[i]), &(this->meshObjects[i]));
  
  glDisable( GL_BLEND );
    
}


std::vector<Mesh> View::createMeshes(Scene const &scene)
{
  std::vector<Mesh> meshes;

  std::vector<SceneObj>::const_iterator thisObject;
  for (thisObject = scene.beginObjs();  thisObject != scene.endObjs();  thisObject++) {
    // new Mesh() sets the arrays and buffers
    Mesh* thisMesh = new Mesh(GL_TRIANGLES);
    //load in data from SceneObj object
    int modelID = thisObject->model;
    thisMesh->model = modelID;
    thisMesh->position = thisObject->pos;
    thisMesh->color = boxColors[rand() % 3];
    thisMesh->openFace = boxFaces[rand() % 6];
    //load in data from the Model that is referred to by SceneObject->Model
    // first, get the model
    const OBJ::Model* thisModel = scene.Model(modelID);
    OBJ::Group thisGroup        = thisModel->Group(0);
    OBJ::Material thisMaterial  = thisModel->Material(thisGroup.material);
    // then get everything else. We probably won't need much of this again, but why not keep it.
    thisMesh->nVerts    = thisGroup.nVerts;  
    thisMesh->nIndices  = thisGroup.nIndices;
    thisMesh->verts     = thisGroup.verts;  
    thisMesh->indices   = thisGroup.indices;
    // texturing
    thisMesh->norms     = thisGroup.norms;
    thisMesh->txtCoords = thisGroup.txtCoords;

    cs237::image2d * image = scene.TextureByName(thisMaterial.diffuseMap);
    thisMesh->texture = new cs237::texture2D(GL_TEXTURE_2D, image);
    {
      thisMesh->texture->Bind(); 
      thisMesh->texture->Parameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
      thisMesh->texture->Parameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    }
    // lighting - much of this is unused now, but will be useful in future projects
    thisMesh->illum       = thisMaterial.illum;
    thisMesh->ambient     = thisMaterial.ambient;  
    thisMesh->diffuse     = thisMaterial.diffuse;  
    thisMesh->specular    = thisMaterial.specular;
    thisMesh->shininess   = thisMaterial.shininess;
    thisMesh->ambientMap  = thisMaterial.ambientMap;
    thisMesh->diffuseMap  = thisMaterial.diffuseMap;
    thisMesh->specularMap = thisMaterial.specularMap;
    thisMesh->normalMap   = thisMaterial.normalMap;  
    thisMesh->lightDir    = scene.LightDir();
    thisMesh->lightIntensity = scene.LightIntensity();
    thisMesh->ambientLight = scene.AmbientLight();
    // create vao, vbuf, ebuf for the mesh.
    thisMesh->LoadVertices(thisMesh->nVerts, thisMesh->verts);
    thisMesh->LoadIndices(thisMesh->nIndices, thisMesh->indices);
    thisMesh->LoadTexCoords (thisMesh->nVerts, thisMesh->txtCoords);
    thisMesh->LoadNormals (thisMesh->nVerts, thisMesh->norms);
    // add our mesh to the list.
    meshes.push_back(*thisMesh);
  }
  return meshes;
}

