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
    float reflectiveness;
    float diffuse;
    float specular;
};

struct PointLight
{
    vec3 position;
    vec4 diffuseColor;
    vec4 specularColor;
};

uniform Sphere spheres[10];
uniform int numSpheres;

uniform PointLight lights[4];
uniform int numLights;

int depth_max = 2;

varying vec2 pixel;

bool intersect_sphere(Ray r, Sphere s, float near_clip, out float t)
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
        
        // this is a branch free way of calculating the following:
        // t = smaller of (t1,t2) that is still greater than near_clip
        t = float(t1>near_clip && t1<t2)*t1 + float(t2>near_clip && t2<t1)*t2;
        
        return t1>near_clip || t2>near_clip;
    }
}

bool trace2(Ray ray, out vec4 color, int depth)
{
    int sphere_index = -1;
    float min_t = 1000.0;
    for(int i = 0; i < numSpheres; i++)
    {
        float t = 0.0;
        
        if(intersect_sphere(ray, spheres[i], 0.1, t) && t < min_t)
        {
            min_t = t;
            sphere_index = i;
        }
    }
    
    if(sphere_index != -1)
    {
        color = vec4(0, 0, 0, 0);
        
        Sphere sphere = spheres[sphere_index];
        
        float t = min_t;
        
        vec3 P = ray.A+t*ray.D;
        vec3 N = normalize(P-sphere.C);
        
        for(int l = 0; l < numLights; l++)
        {
            Ray toLight;
            toLight.A = P;
            toLight.D = normalize(lights[l].position-P);
            
            bool skip_light = false;
            
            for(int i = 0; i < numSpheres; i++)
            {
                float tL = 0.0;
                if(i != sphere_index && intersect_sphere(toLight, spheres[i], 0.1, tL))
                {
                    skip_light = true;
                    break;
                }
            }
            
            if(!skip_light)
            {
                vec3 I = normalize(lights[l].position-P);
                vec3 R = reflect(I, N);
                vec3 V = normalize(P-ray.A);
                
                vec4 diffuse = sphere.color * lights[l].diffuseColor * max(0.0,dot(I,N));
                vec4 specular = lights[l].specularColor * pow(max(0.0,dot(R,V)), sphere.shininess);
                color += diffuse + specular;
            }
        }
        
//        if(sphere.reflectiveness > 0.0)
//        {
//            Ray reflector;
//            reflector.A = P;
//            reflector.D = reflect(normalize(P-ray.A), N);
//            
//            vec4 reflectColor = vec4(0, 0, 0, 0);
//            //            if(trace(reflector, reflectColor, depth+1))
//            //                color += clamp(reflectColor*sphere.reflectiveness, 0.0, 1.0);
//        }
        
        return true;
    }
    else
    {
        return false;
    }
}

bool trace(Ray ray, out vec4 color, int depth)
{
    if(depth > depth_max)
        return false;
    
    int sphere_index = -1;
    float min_t = 1000.0;
    for(int i = 0; i < numSpheres; i++)
    {
        float t = 0.0;
        
        if(intersect_sphere(ray, spheres[i], 1.0, t) && t < min_t)
        {
            min_t = t;
            sphere_index = i;
        }
    }
    
    if(sphere_index != -1)
    {
        color = vec4(0, 0, 0, 0);
        
        Sphere sphere = spheres[sphere_index];
        
        float t = min_t;
        
        vec3 P = ray.A+t*ray.D;
        vec3 N = normalize(P-sphere.C);
        
        for(int l = 0; l < numLights; l++)
        {
            Ray toLight;
            toLight.A = P;
            toLight.D = normalize(lights[l].position-P);
            
            bool skip_light = false;
            
            for(int i = 0; i < numSpheres; i++)
            {
                float tL = 0.0;
                if(i != sphere_index && intersect_sphere(toLight, spheres[i], 0.1, tL))
                {
                    skip_light = true;
                    break;
                }
            }
            
            if(!skip_light)
            {
                vec3 I = normalize(lights[l].position-P);
                vec3 R = reflect(I, N);
                vec3 V = normalize(P-ray.A);
                
                vec4 diffuse = sphere.diffuse * sphere.color * lights[l].diffuseColor * max(0.0,dot(I,N));
                vec4 specular = sphere.specular * lights[l].specularColor * pow(max(0.0,dot(R,V)), sphere.shininess);
                color += diffuse + specular;
            }
        }
        
        if(sphere.reflectiveness > 0.0)
        {
            Ray reflector;
            reflector.A = P;
            reflector.D = reflect(normalize(P-ray.A), N);
            
            vec4 reflectColor = vec4(0, 0, 0, 0);
            if(trace2(reflector, reflectColor, depth+1))
                color += clamp(reflectColor*sphere.reflectiveness, 0.0, 1.0);
        }
        
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
    if(trace(ray, color, 1))
        gl_FragColor = clamp(color, 0.0, 1.0);
    else
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
}
