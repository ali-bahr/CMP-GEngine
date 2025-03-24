#version 330

uniform sampler2D tex;
in vec2 tex_coord;
out vec4 frag_color;

#define STRENGTH 0.005

void main(){
    // Sample red channel from slightly left position
    float r = texture(tex, tex_coord - vec2(STRENGTH, 0.0)).r;
    
    // Sample green channel from original position (center)
    float g = texture(tex, tex_coord).g;
    
    // Sample blue channel from slightly right position
    float b = texture(tex, tex_coord + vec2(STRENGTH, 0.0)).b;
    
    // Combine channels while keeping original alpha
    vec4 original = texture(tex, tex_coord);
    frag_color = vec4(r, g, b, original.a);
}