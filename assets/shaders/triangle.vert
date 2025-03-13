#version 330

// This vertex shader should be used to render a triangle whose normalized device coordinates are:
// (-0.5, -0.5, 0.0), ( 0.5, -0.5, 0.0), ( 0.0,  0.5, 0.0)
// And it also should send the vertex color as a varying to the fragment shader where the colors are (in order):
// (1.0, 0.0, 0.0), (0.0, 1.0, 0.0), (0.0, 0.0, 1.0)

out Varyings {
    vec3 color;
} vs_out;

// Currently, the triangle is always in the same position, but we don't want that.
// So two uniforms should be added: translation (vec2) and scale (vec2).
// Each vertex "v" should be transformed to be "scale * v + translation".
// The default value for "translation" is (0.0, 0.0) and for "scale" is (1.0, 1.0).

uniform vec2 translation = vec2(0.0, 0.0);
uniform vec2 scale = vec2(1.0, 1.0);



void main(){

    vec3 position;
    if(gl_VertexID == 0){
        position = vec3(-0.5, -0.5, 0.0);
    }else if(gl_VertexID == 1){
        position = vec3(0.5, -0.5, 0.0);
    }else{
        position = vec3(0.0, 0.5, 0.0);
    }

    position = vec3(scale.x * position.x + translation.x, scale.y * position.y + translation.y, 0.0);

    gl_Position = vec4(position, 1.0);

    if(gl_VertexID == 0){
        vs_out.color = vec3(1.0, 0.0, 0.0);
    }else if(gl_VertexID == 1){
        vs_out.color = vec3(0.0, 1.0, 0.0);
    }else{
        vs_out.color = vec3(0.0, 0.0, 1.0);
    }

}