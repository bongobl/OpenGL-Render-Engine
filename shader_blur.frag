uniform sampler2D texture;
in vec3 pos;

void main()
{
	float blurRadius = 0.000;
    vec2 offx = vec2(blurRadius, 0.0);
    vec2 offy = vec2(0.0, blurRadius);
	vec2 pixelPosition = vec2( (pos.x / 2.0) + 0.5 , (pos.y /2.0) + 0.5);

    vec4 pixel = texture2D(texture, pixelPosition)               * 4.0 +
                 texture2D(texture, pixelPosition - offx)        * 2.0 +
                 texture2D(texture, pixelPosition + offx)        * 2.0 +
                 texture2D(texture, pixelPosition - offy)        * 2.0 +
                 texture2D(texture, pixelPosition + offy)        * 2.0 +
                 texture2D(texture, pixelPosition - offx - offy) * 1.0 +
                 texture2D(texture, pixelPosition - offx + offy) * 1.0 +
                 texture2D(texture, pixelPosition + offx - offy) * 1.0 +
                 texture2D(texture, pixelPosition + offx + offy) * 1.0;

    gl_FragColor =  pixel / 16.0;


}
