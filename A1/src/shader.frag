#version 330

// uniform vec3 frag_color;
uniform vec4 color;
out vec4 finalColor;

void main()
{
     //gl_FragColor = color
    finalColor = color;
    // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
