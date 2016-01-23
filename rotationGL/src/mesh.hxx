/*! \file mesh.hxx
 *
 * \author YOUR_NAME
 */


#ifndef _MESH_HXX_
#define _MESH_HXX_

#include "cs237.hxx"

//! the information needed to render a mesh
struct Mesh {
    /**HINT: You could also provide member variables for the position and color inside this mesh */ 
    GLuint vaoId;
    GLuint vbufId;
    GLuint ebufId;
    GLuint tCoordId;
    GLuint normId;

    /**HINT: Create member variable for the primitive */
    GLenum prim;
    /* Data from SceneObj */
    int         model;  //!< the ID of the model that defines the object's mesh
    cs237::vec3f    position;    //!< the world-space coordinates of the object
    cs237::color4f  color;  //!< the color of the object

    /* Data from Model->Objects[0] */
    uint32_t        nVerts;     //!< the number of vertices in this group
    uint32_t        nIndices;   //!< the number of indices (3 * number of triangles)
    cs237::vec3f    *verts;     //!< array of texture coordinates (or nullptr)
    uint32_t        *indices;   //!< array of element indices that can be used to render
    // texturing
    cs237::vec3f    *norms;
    cs237::vec2f    *txtCoords;
    cs237::texture2D  *texture;
    int             illum;
    cs237::color3f  ambient;  
    cs237::color3f  diffuse;  
    cs237::color3f  specular;
    float           shininess;
    std::string     ambientMap;
    std::string     diffuseMap;
    std::string     specularMap;
    std::string     normalMap;  
    cs237::vec3f    lightDir;
    cs237::color3f  lightIntensity;
    cs237::color3f  ambientLight;
    int txtID;
    cs237::vec3f openEdge;

    /** HINT: I have provide these functions that helping load information about the mesh. You 
     * free to change anything to your liking */ 
    //! create a MeshInfo object by initializing its buffer Ids.  The buffer data is
    //! loaded separately.
    Mesh (GLenum p);

  //! initialize the vertex data buffers for the mesh
    void LoadVertices (int nVerts, const cs237::vec3f *verts);

  //! initialize the element array for the mesh
    void LoadIndices (int n, const uint32_t *indices);

    void LoadTexCoords (int nCoords, cs237::vec2f *tcoords);

    void LoadNormals (int nVerts, cs237::vec3f *norms);

  //! draw the mesh using a glDrawElements call
    void Draw ();

};

#endif // !_MESH_HXX_
