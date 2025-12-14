vec3 hsv2rgb(vec3 c)
{
    vec3 rgb = clamp(abs(mod(c.x * 6.0 + vec3(0,4,2), 6.0) - 3.0) - 1.0,
                     0.0, 1.0);
    return c.z * mix(vec3(1.0), rgb, c.y);
}

vec2 kaleido(vec2 uv)
{
	float th = atan(uv.y, uv.x);
	float r = pow(length(uv), .9);
	float f = 3.14159 / 3.5;

	th = abs(mod(th + f/4.0, f) - f/2.0) / (1.0 + r);
	//th = sin(th * 6.283 / f);

	return vec2(cos(th), sin(th)) * r * .1;
}

vec2 transform(vec2 at)
{
	vec2 v;
	float th = .02 * iTime;
	v.x = at.x * cos(th) - at.y * sin(th) - .2 * sin(th);
	v.y = at.x * sin(th) + at.y * cos(th) + .2 * cos(th);
	return v;
}

vec4 scene(vec2 at)
{
    // sample texture for structure only
    float v = texture(iChannel1, transform(at) * 2.0).r;

    float angle = atan(at.y, at.x);
    float radius = length(at);

    float hue = fract(angle / (2.0 * 3.14159) + iTime * 0.05);
    float sat = 0.85;
    float val = pow(v, 0.8) * (1.2 - radius);

    vec3 col = hsv2rgb(vec3(hue, sat, val));
    return vec4(col, 1.0);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 uv = fragCoord.xy / iResolution.xy;	
	uv.x = mix(-1.0, 1.0, uv.x);
	uv.y = mix(-1.0, 1.0, uv.y);
	uv.y *= iResolution.y / iResolution.x;
	fragColor = scene(kaleido(uv));
}
