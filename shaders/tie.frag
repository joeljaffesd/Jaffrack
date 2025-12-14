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
    vec2 uv = 0.03 * vPos.xy;
    //float t = (u_time * (0.001*(flux*3 )))+onset; //+ (cent);
    //float t = (u_time * 0.01); //XXX simplify back to whats in shadertoy example
    float t = (u_time * 0.01) + onset;

    //t += flux;
    //t +=cent;
    float k = cos(t);
    float l = sin(t);
    float s = 0.2+(onset/10.0); //+ (onset / u_time);

    // for(int i=0; i<64; ++i) {
        // uv  = abs(uv) - s*flux;//-onset;    // Mirror
        // uv *= mat2(k,-l,l,k); // Rotate
        // s  *= .95156;///(t+1);         // Scale
    // }
    // XXX simplify back to shadertoy example
    for(int i=0; i<80; ++i) {
        uv  = abs(uv) - s;//-onset;    // Mirror
        uv *= mat2(k+s,-l,l,k); // Rotate
        s  *= .95156;///(t+1);         // Scale
    }

    float x = .5 + .5*cos(6.28318*(40.0*length(uv))) * u_time;

    

    //fragColor =  vec4(vec3(x),1);
     //fragColor =  x* vec4(1,2*flux,3,1);
    //fragColor = vec4(vec3(x), 1);
    fragColor = .5 + .5*cos(6.28318*(40.0*length(uv))*vec4(-1,2+(u_time/500.0),3+flux,1)); //u time makes it grainy over time

}
