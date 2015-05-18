#version 330

uniform vec4 frag_color;
out vec4 finalColor;

void main()
{
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    finalColor =  frag_color;
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
