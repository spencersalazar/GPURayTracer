// default.vert

uniform float teapot;
/*
  This simple GLSL vertex shader does exactly what 
  OpenGL would do -- It transforms the vertex positions
  using the default OpenGL transformation. It also passes
  through the texture coordinate, normal coordinate, and some
  other good stuff so we can use them in the fragment shader.
*/

// This 'varying' vertex output can be read as an input
// by a fragment shader that makes the same declaration.

varying vec2 pixel;

void main()
{
    pixel = gl_Vertex.xy;
    
    // Render the shape using standard OpenGL position transforms.
    gl_Position = ftransform();
}
