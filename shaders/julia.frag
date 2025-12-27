#version 330 core

in vec3 vPos; //recieve from vert
in vec2 vUV;

out vec4 fragColor;

uniform float u_time;
uniform float onset;
uniform float cent;
uniform float flux;


// *** STARTER CODE INSPIRED BY : https://www.shadertoy.com/view/4lSSRy *** //

void main() {
    vec2 uv = 0.3 * vPos.xy;
    mediump float t = (u_time * 0.01) + onset;

    mediump float k = cos(t);
    mediump float l = sin(t);
    mediump float s = 0.2 + (onset/10.0);

    // XXX simplify back to shadertoy example
    for(int i = 0; i < 32; ++i) {
        uv  = abs(uv) - s;//-onset;    // Mirror
        uv *= mat2(k,-l,l,k); // Rotate
        s  *= .95156;///(t+1);         // Scale
    }

    mediump float x = .5 + .5 * cos(6.28318 * (40.0 * length(uv)));
    fragColor = .5 + .5 * cos(6.28318 * (40.0 * length(uv)) * vec4(-1,2 + (u_time / 500.0), 3 + flux, 1));
}
 