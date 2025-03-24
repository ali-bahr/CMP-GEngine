#version 330

uniform sampler2D tex;
in vec2 tex_coord;
out vec4 frag_color;

void main(){
    // Get the original color from the scene texture
    vec4 color = texture(tex, tex_coord);
    
    // Convert texture coordinates (0-1) to NDC space (-1 to 1)
    vec2 ndc = tex_coord * 2.0 - 1.0;
    
    // Calculate squared distance from center
    float distSquared = dot(ndc, ndc);
    
    // Apply vignette effect (darken pixels based on distance from center)
    float vignette = 1.0 / (1.0 + distSquared);
    
    // Apply the vignette effect to the color
    frag_color = color * vignette;
}