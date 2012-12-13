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
    vec3 C;
    float r;
    vec4 color;
    float shininess;
};

uniform Sphere spheres[10];
uniform int numSpheres;

uniform vec3 c;
uniform float r;
uniform vec3 L;

varying vec2 pixel;

float near_clip = 1.0;

bool intersect_sphere(Ray r, Sphere s, out float t)
{
    float a = r.D.x*r.D.x + r.D.y*r.D.y + r.D.z*r.D.z;
    float b = 2.0*(r.D.x*(r.A.x-s.C.x)+r.D.y*(r.A.y-s.C.y)+r.D.z*(r.A.z-s.C.z));
    float c = (r.A.x-s.C.x)*(r.A.x-s.C.x) + (r.A.y-s.C.y)*(r.A.y-s.C.y) + (r.A.z-s.C.z)*(r.A.z-s.C.z) + -s.r*s.r;

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
    int sphere_index = -1;
    float min_t = 1000.0;
    for(int i = 0; i < numSpheres; i++)
    {
        float t = 0.0;
        
        if(intersect_sphere(ray, spheres[i], t))
        {
            if(t < min_t)
            {
                min_t = t;
                sphere_index = i;
            }
        }
    }
    
    if(sphere_index != -1)
    {
        float t = min_t;
        
        vec3 P = ray.A+t*ray.D;
        vec3 N = normalize(P-spheres[sphere_index].C);
        vec3 I = normalize(L-P);
        vec3 R = reflect(I, N);
        vec3 V = normalize(P-ray.A);
        
        vec4 diffuse = spheres[sphere_index].color * max(0.0,dot(I,N));
        vec4 specular = vec4(1.0) * pow(max(0.0,dot(R,V)), spheres[sphere_index].shininess);
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
