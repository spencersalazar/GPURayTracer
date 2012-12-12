// phong.frag

/*
 This fragment implements the Phong Reflection model.
 */

// The input image we will be filtering in this kernel.

struct Ray
{
    vec3 A;
    vec3 D;
};

struct Sphere
{
    vec3 c;
    float r;
};

uniform vec3 c;
uniform float r;
uniform vec3 L;

varying vec2 pixel;

bool intersect_sphere(Ray r, Sphere s, out float t)
{
    float c=dot(s.c,s.c)-s.r*s.r;
    float b=2.0
    *dot(s.c,r.D);
    float a=dot(r.D,r.D);
    
    float sqrt_qty = b*b-4.0*a*c;
    
    if(sqrt_qty < 0.0)
        return false;
    else
    {
        t = min((-b+sqrt(sqrt_qty))/(2.0*a),
                (-b-sqrt(sqrt_qty))/(2.0*a));
        return true;
    }
}

void main()
{
    Ray ray;
    ray.A = vec3(0, 0, 0);
    ray.D = normalize(vec3(pixel.x, pixel.y, 1)-ray.A);
    Sphere s;
    s.c = c;
    s.r = r;
    float t = 0.0;
    
    if(intersect_sphere(ray, s, t))
        gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
    else
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
}
