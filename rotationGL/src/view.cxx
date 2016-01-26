/*! \file view.c
 *
 * \brief This file defines the viewer operations.
 *
 * \author John Reppy
 */

/* CMSC23700 Project 1 sample code (Winter 2014)
 *
 * COPYRIGHT (c) 2014 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "view.hxx"

#define NEAR_Z    0.25f    //Should these be constants? UNCLEAR
#define FAR_Z     500.0f
#define TIME_STEP 0.001

cs237::color4f boxColors[3] = {cs237::color4f(1.0f,0.0f,0.0f,1.0f),
                          cs237::color4f(0.0f,1.0f,0.0f,1.0f),
                          cs237::color4f(0.0f,0.0f,1.0f,1.0f)};
cs237::vec3f boxEdges[6] = {cs237::vec3f(0.0f,1.0f,0.0f),
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
    this->mode		= WIREFRAME;
    this->textureCount = 0;

    this->numObjects = scene.NumObjects();
    this->meshObjects = createMeshes(scene);

    modelScales.push_back(1.0f);  //player scale
    for (int i = 1; i < numObjects; i++){
      modelScales.push_back(20.0f * i);
      meshObjects[i].openEdge = boxEdges[rand() % 6];
    }
    this->_lastStep = this->_lastFrameTime = this->startTime = glfwGetTime();
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
  if (this->MVMinitialized == false){
   cs237::mat4f modelViewM = cs237::lookAt (
      this->camPos,
      this->camAt,
      this->camUp);
    
    // place the player at the center
    this->MVMinitialized = true;

        modelViewM = modelViewM
        * cs237::translate(this->camPos)
        * cs237::rotateX(this->cameraRotation.x)
        * cs237::rotateY(this->cameraRotation.y)
        * cs237::rotateZ(this->cameraRotation.z)
        * cs237::translate(this->camPos * -1.0f);

        this->playerMVM = this->modelViewMat = modelViewM;

        this->xRotationAxis = cs237::vec4f(1,0,0,0);
        this->yRotationAxis = cs237::vec4f(0,1,0,0);

        // rotate the earth so chicago is pointing at you
        this->playerMVM = playerMVM 
          * cs237::rotate(90.0f, cs237::vec3f(yRotationAxis))
          * cs237::rotate(-45.0f, cs237::vec3f(0,0,1));

  }

    cs237::mat4f rotation = cs237::mat4f(1)
        * cs237::rotate(modelRotation.x, cs237::vec3f(xRotationAxis))
        * cs237::rotate(modelRotation.y, cs237::vec3f(yRotationAxis));

    xRotationAxis = xRotationAxis*rotation;
    yRotationAxis = yRotationAxis*rotation;

    modelViewMat *= rotation;
        
}

void View::InitRenderers ()
{
  /*
    this->wfRender = new WireframeRenderer();
    this->fsRender = new FlatShadingRenderer();
    this->dlRender = new LightingRenderer();*/
    this->tRender  = new TexturingRenderer();
    this->bRender  = new BoxRenderer();

}

void View::Animate ()
{
    double now = glfwGetTime();
    double dt = now - this->_lastStep;
    if (dt >= TIME_STEP) {
      this->_lastStep = now;
      if (gamePlayLoop){
        for (int i = 1; i < numObjects; i++){
          if (this->modelScales[i] < 1.5){
            cs237::vec3f openFace = cs237::vec3f(this->modelViewMat * cs237::vec4f(this->meshObjects[i].openEdge,0));
            float offAngle = cs237::__detail::dot(openFace,cs237::vec3f(0,0,0.2));
            if (offAngle < 0.16){
              std::cout<<"Fail: "<<offAngle<<"\t"<<modelScales[i]<<"\t"<<openFace<<"\n";
              this->gamePlayLoop = false;
              //return;
            }
            else{
              std::cout<<"Pass: "<<offAngle<<"\t"<<modelScales[i]<<"\t"<<openFace<<"\n";
              this->meshObjects[i].color = boxColors[rand() %3];
              this->meshObjects[i].openEdge = boxEdges[rand() % 6];
              this->modelScales[i] = 30.0f;
            }
          }
          else
            this->modelScales[i] = (this->modelScales[i] - 5.0f*(dt)) - 0.001*(now - startTime);
        }

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
            //this->MVMinitialized = false;
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
    glClearColor (0.0f, 0.0f, 0.0f, 1.0f);	// clear the surface
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  

      tRender->Enable(this->projectionMat);
    tRender->Render(this->playerMVM, &(this->meshObjects[0]));

      glEnable( GL_BLEND );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  bRender->Enable (this->projectionMat);
  for (int i = 1; i < this->meshObjects.size(); i++){
    bRender->Render(modelViewMat * cs237::scale(modelScales[i]), &(this->meshObjects[i]));
  }
    glDisable( GL_BLEND );
    

}


std::vector<Mesh> View::createMeshes(Scene const &scene)
{
  //std::cout<<"View::CreateMeshes\n";
  std::vector<Mesh> meshes;

  std::vector<SceneObj>::const_iterator thisObject;
  for (thisObject = scene.beginObjs();  thisObject != scene.endObjs();  thisObject++) {
      // new Mesh() sets the arrays and buffers
      Mesh* thisMesh = new Mesh(GL_TRIANGLES);
      //load in data from SceneObj object
      int modelID = thisObject->model; //save this, we need it twice
      thisMesh->model = modelID;
      thisMesh->position = thisObject->pos;
      thisMesh->color = boxColors[rand() % 3];
      thisMesh->openEdge = boxEdges[rand() % 6];
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
      thisMesh->txtID     = this->textureCount++;

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

