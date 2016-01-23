/*! \file mesh.cxx
 *
 * \author YOUR_NAME
 */

#include "mesh.hxx"

/*! The locations of the standard mesh attributes.  The layout directives in the shaders
 * should match these values.
 */
const GLint CoordAttrLoc = 0; //!< location of vertex coordinates attribute
const GLint NormAttrLoc = 1;  //!< location of vertex normal attribute
const GLint TexCoordAttrLoc = 2;  //!< location of texture coordniate attribute

//! create a MeshInfo object by initializing its buffer Ids.  The buffer data is
//! loaded separately.
Mesh::Mesh(GLenum p)
{
    //std::cout<<"Mesh::Mesh\n";
    /** YOUR CODE HERE **/
    this->prim = p;
    this->vaoId = 0;
    this->vbufId = 0;
    this->ebufId = 0;
    this->tCoordId = 0;
    this->normId = 0;
    CS237_CHECK( glGenVertexArrays(1, &(this->vaoId)) );

}

void Mesh::LoadTexCoords (int nCoords, cs237::vec2f *tcoords)
{
    //std::cout<<"Mesh::LoadTexCoords\n";

    GLuint id;

    //gen buffer ids
    CS237_CHECK( glGenBuffers(1, &(id)) );
    CS237_CHECK( glBindVertexArray(this->vaoId) );

    //initialize vertex buffer
    this->tCoordId = id;
    CS237_CHECK( glBindBuffer(GL_ARRAY_BUFFER, this->tCoordId) );
    CS237_CHECK( glBufferData(GL_ARRAY_BUFFER, nCoords * sizeof(cs237::vec2f), tcoords, GL_STREAM_DRAW) );
    CS237_CHECK( glVertexAttribPointer(TexCoordAttrLoc, 2, GL_FLOAT, GL_FALSE, sizeof(cs237::vec2f), (GLvoid*) 0) );
    CS237_CHECK( glEnableVertexAttribArray(TexCoordAttrLoc) );

    //unbind
    CS237_CHECK( glBindVertexArray(0) );
    CS237_CHECK( glBindBuffer(GL_ARRAY_BUFFER,0) );
}

void Mesh::LoadNormals (int nVerts, cs237::vec3f *norms)
{
    //std::cout<<"Mesh::LoadNormals\n";

    GLuint id;

    //gen buffer ids
    CS237_CHECK( glGenBuffers(1, &(id)) );
    CS237_CHECK( glBindVertexArray(this->vaoId) );

    //initialize vertex buffer
    this->normId = id;
    CS237_CHECK( glBindBuffer(GL_ARRAY_BUFFER, this->normId) );
    CS237_CHECK( glBufferData(GL_ARRAY_BUFFER, nVerts * sizeof(cs237::vec3f), norms, GL_STREAM_DRAW) );
    CS237_CHECK( glVertexAttribPointer(NormAttrLoc, 3, GL_FLOAT, GL_FALSE, sizeof(cs237::vec3f), (GLvoid*) 0) );
    CS237_CHECK( glEnableVertexAttribArray(NormAttrLoc) );

    //unbind
    CS237_CHECK( glBindVertexArray(0) );
    CS237_CHECK( glBindBuffer(GL_ARRAY_BUFFER,0) );
}

//! initialize the vertex data buffers for the mesh
void Mesh::LoadVertices (int nVerts, const cs237::vec3f *verts)
{
    //std::cout<<"Mesh::LoadVertices\n";

    /** YOUR CODE HERE **/
    GLuint id;

    //gen buffer ids
    CS237_CHECK( glGenBuffers(1, &(id)) );
    CS237_CHECK( glBindVertexArray(this->vaoId) );

    //initialize vertex buffer
    this->vbufId = id;
    CS237_CHECK( glBindBuffer(GL_ARRAY_BUFFER, this->vbufId) );
    CS237_CHECK( glBufferData(GL_ARRAY_BUFFER, nVerts * sizeof(cs237::vec3f), verts, GL_STREAM_DRAW) );
    CS237_CHECK( glVertexAttribPointer(CoordAttrLoc, 3, GL_FLOAT, GL_FALSE, sizeof(cs237::vec3f), (GLvoid*) 0) );
    CS237_CHECK( glEnableVertexAttribArray(CoordAttrLoc) );

    //unbind
    CS237_CHECK( glBindVertexArray(0) );
    CS237_CHECK( glBindBuffer(GL_ARRAY_BUFFER,0) );
}

//! initialize the element array for the mesh
void Mesh::LoadIndices (int n, const uint32_t *indices)
{
    //std::cout<<"Mesh::LoadIndices\n";

    /** YOUR CODE HERE **/
    this->nIndices = n;

    CS237_CHECK( glBindVertexArray (this->vaoId) );
	CS237_CHECK( glGenBuffers (1, &this->ebufId) );
    
	CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, this->ebufId) );
	CS237_CHECK( glBufferData (GL_ELEMENT_ARRAY_BUFFER, n*sizeof(uint32_t), indices, GL_STATIC_DRAW) );
	
	CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0) );
    glBindVertexArray(0);
}

void Mesh::Draw ()
{
    //std::cout<<"Mesh::Draw\n";
    /** YOUR CODE HERE **/
    //bind to vao
    CS237_CHECK( glBindVertexArray(this->vaoId) );

    //bind ind array
    CS237_CHECK( glBindBuffer(GL_ARRAY_BUFFER, vbufId) );
    CS237_CHECK( glBindBuffer(GL_ARRAY_BUFFER, tCoordId) );
    CS237_CHECK( glBindBuffer(GL_ARRAY_BUFFER, normId) );
    CS237_CHECK( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebufId) );

    //render
    CS237_CHECK( glDrawElements(this->prim, this->nIndices, GL_UNSIGNED_INT, 0) );

    //clean
    CS237_CHECK( glBindBuffer(GL_ARRAY_BUFFER, 0) );
    CS237_CHECK( glBindVertexArray(0) );

}