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

float near_clip = 1.0;

bool intersect_sphere(Ray r, Sphere s, out float t)
{
    float a = r.D.x*r.D.x + r.D.y*r.D.y + r.D.z*r.D.z;
    float b = 2.0*(r.D.x*(r.A.x-s.c.x)+r.D.y*(r.A.y-s.c.y)+r.D.z*(r.A.z-s.c.z));
    float c = (r.A.x-s.c.x)*(r.A.x-s.c.x) + (r.A.y-s.c.y)*(r.A.y-s.c.y) + (r.A.z-s.c.z)*(r.A.z-s.c.z) + -s.r*s.r;

    float discriminant = b*b-4.0*a*c;
    
    if(discriminant < 0.0)
        return false;
    else
    {
        float sqrt_qty = sqrt(discriminant);
        float t1 = (-b+sqrt_qty)/(2.0*a);
        float t2 = (-b-sqrt_qty)/(2.0*a);
        
        if(t1 > near_clip)
        {
            if(t2 > near_clip)
            {
                t = min(t1, t2);
            }
            else
            {
                t = t1;
            }
        }
        else
        {
            if(t2 > near_clip)
            {
                t = t2;
            }
            else
            {
                return false;
            }
        }
        
        return true;
    }
}

bool trace(Ray ray, out vec4 color)
{
    Sphere sphere;
    sphere.c = c;
    sphere.r = r;
    vec4 surfaceColor = vec4(1.0, 1.0, 0.0, 1.0);
    float t = 0.0;
    float shininess = 128.0;
    
    if(intersect_sphere(ray, sphere, t))
    {
        vec3 P = ray.A+t*ray.D;
        vec3 N = normalize(P-sphere.c);
        vec3 I = normalize(L-P);
        vec3 R = reflect(I, N);
        vec3 V = normalize(P-ray.A);
        
        vec4 diffuse = surfaceColor * max(0.0,dot(I,N));
        vec4 specular = vec4(1.0) * pow(max(0.0,dot(R,V)),shininess);
        color = diffuse + specular;
        
        return true;
    }
    else
    {
        return false;
    }
}

void main()
{
    Ray ray;
    ray.A = vec3(0, 0, -1);
    ray.D = normalize(vec3(pixel.x, pixel.y, 1)-ray.A);
    
    vec4 color;
    if(trace(ray, color))
        gl_FragColor = clamp(color, 0.0, 1.0);
    else
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
}
