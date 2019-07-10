#version 330 core

/**
\file PhongMultipleLights.glsl

\brief Fragment shader that will process up to 10 light sources.

\param [in] position --- vec4 vertex position from memory.

\param [in] color --- vec4 vertex color from memory.

\param [in] normal --- vec3 normal vector from memory.

\param [out] fColor --- vec4 output color to the frame buffer.

\param [uniform] Lt --- Light struct containing a single light attribute set.

\param [uniform] Mat --- Material struct containing a single material attribute set.

\param [uniform] eye --- vec3 position of the viewer/camera.

\param [uniform] GlobalAmbient --- vec4 global ambient color vector.

*/


struct Light
{
    bool on;             ///< Light on or off.
    vec4 position;       ///< Position of the light.
    vec3 spotDirection;  ///< Direction of the spot light.
    vec4 ambient;        ///< Ambient color of the light.
    vec4 diffuse;        ///< Diffuse color of the light.
    vec4 specular;       ///< Specular color of the light.
    float spotCutoff;    ///< Spot cutoff angle.
    float spotExponent;  ///< Spot falloff exponent.
    vec3 attenuation;    ///< Attenuation vector, x = constant, y = linear, z = quadratic.
};

struct Material
{
    vec4 ambient;     ///< Ambient color of the material.
    vec4 diffuse;     ///< Diffuse color of the material.
    vec4 specular;    ///< Specular color of the material.
    vec4 emission;    ///< Emission color of the material.
    float shininess;  ///< Shininess exponent of the material.
};

in vec4 position;
in vec4 color;
in vec3 normal;

uniform Light Lt[10];
uniform Material Mat;
uniform vec3 eye;
uniform vec4 GlobalAmbient;
uniform int numLights;

out vec4 fColor;

void main()
{
    float deg = 0.017453292519943296;

    vec4 cc = vec4(0.0);
    bool usingLights = false;
    vec4 globalAmbientPortion = Mat.ambient*GlobalAmbient;

    for (int i = 0; i < numLights; i++)
    {
        if (Lt[i].on)
        {
            usingLights = true;
            vec3 n = normalize(normal);
            vec3 l = normalize(vec3(Lt[i].position)-vec3(position));
            vec3 r = normalize(2.0*dot(l,n)*n - l);
            vec3 v = normalize(eye-vec3(position));
            float lightDistance =length(vec3(Lt[i].position)-vec3(position));

            float dfang = max(0.0, dot(l, n));
            float specang = max(0.0, dot(r, v));
            if (dfang == 0)
                specang = 0;

            float attenuation = 1.0 / (Lt[i].attenuation[0] +
                                       Lt[i].attenuation[1] * lightDistance +
                                       Lt[i].attenuation[2] * lightDistance * lightDistance);

            float spotCos = dot(l, -normalize(Lt[i].spotDirection));
            float SpotCosCutoff = cos(Lt[i].spotCutoff*deg);  // assumes that spotCutoff is in degrees

            float spotFactor = 1.0;
            if (spotCos < SpotCosCutoff && Lt[i].spotCutoff < 179.9)  // Only fade if a spotlight
            {
                float range = 1 + SpotCosCutoff;
                spotFactor = pow(1 - (SpotCosCutoff - spotCos)/range, Lt[i].spotExponent);
            }

            vec4 ambientPortion = Mat.ambient*Lt[i].ambient;
            vec4 diffusePortion = Mat.diffuse*Lt[i].diffuse*dfang*attenuation*spotFactor;
            vec4 specularPortion = Mat.specular*Lt[i].specular*pow(specang, Mat.shininess)*attenuation*spotFactor;

            vec4 c = ambientPortion + diffusePortion + specularPortion;
            cc += min(c, vec4(1.0));
        }
    }

    cc = min(cc + globalAmbientPortion + Mat.emission, vec4(1.0));

    if (usingLights)
        fColor = (0.5*cc) + (0.5*color);
    else
        fColor = color;
}
