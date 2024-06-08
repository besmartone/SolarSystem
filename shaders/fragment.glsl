#version 400 core

struct Light
{
	vec3 position;
	vec3 color;
	vec3 direction;
	vec3 spotLightDirection;

	bool isDirection; // used to note if its a directional light
	bool isPoint; // used to note if its a point light
	bool isSpot; // used to note if its a spot light

	float spotexponent;
	float cutoff;
	float innercutoff;
	float intensity;

	float kc; float kl; float kq;

	float ambientIntensity;

	float specularIntensity;
};

uniform Light[2] light;


in vec3 Normal;
in vec2 TexCoords;
in vec4 ModelPosition;
in mat3 TBN;
in vec3 Eye;

uniform float isSpec_map;
uniform float isNorm_map;
uniform float height_scale;

out vec4 FragmentColor;

uniform sampler2D Texture;
uniform sampler2D normal_map;
uniform sampler2D spec_map;
uniform sampler2D bump_map;

uniform float Emit;

uniform vec3 color;

vec3 Point(vec3 normal, int index, vec2 Tex)
{
	vec3 lightDirection = light[index].position - ModelPosition.xyz;

	vec3 ambient = light[index].ambientIntensity * vec3(1.0f);

	vec3 norm = normal.xyz;

	float d = length(lightDirection);

	/*float attenuation =	1.0f /	(light[index].kc +
							   light[index].kl * d +
                                  light[index].kq * d * d); */

								  float attenuation =	1.0f;

	vec3 N = normalize(norm);
	vec3 L = normalize(lightDirection);

	vec3 diffuse = max(dot(N, L), 0.0) * light[index].intensity * light[index].color * attenuation;

	vec3 V = normalize(Eye - ModelPosition.xyz);
	vec3 R = reflect(-L, N);

	vec3 Half = normalize(L + V);

	vec3 use_spec = vec3(1.0f);

	if(isSpec_map != 0.0f)
	{
		use_spec = texture(spec_map, Tex).rgb;
	}

	vec3 specular = pow(max(dot(N, Half), 0.0), 64.0f) * light[index].color * attenuation * light[index].specularIntensity * use_spec;
	
	vec3 LightOutPut = (ambient + diffuse + specular);
	//vec3 LightOutPut = vec3(N);
	return LightOutPut;
}



//meant to displace textures to look more rough
vec2 Bump(vec2 texCoords)
{ 
	vec3 viewDir = normalize(Eye - ModelPosition.xyz);

    float height =  texture(bump_map, texCoords).r;  
	
	vec3 res = viewDir * height;
	res = res / res.z;

    vec2 p = (viewDir.xy / viewDir.z) * (height * height_scale);

    return texCoords - p.xy;    
} 


void main()
{
	vec3 used_normal = Normal;
	vec2 used_texture_coords = TexCoords.st;
	if(isNorm_map != 0.0f)
	{
			used_texture_coords = Bump(TexCoords.st);

		    // obtain normal from normal map in range [0,1]
			used_normal = texture(normal_map, used_texture_coords).rgb;
			
			// transform normal vector to range [-1,1]
			
			used_normal = normalize(used_normal * 2.0 - 1.0);  
			
			used_normal = (TBN * used_normal);//transformation to tangent space
	}

	vec3 LightOutPut2 = (Point(used_normal, 1, used_texture_coords));

	if(Emit != 1.0f)
	{
		if(color != vec3(0.0f))
		{
			FragmentColor = vec4(color, 1.0f)  * vec4(LightOutPut2, 1.0f);
		}
		else
		{
			FragmentColor =  texture(Texture, used_texture_coords) * vec4(LightOutPut2, 1.0f);
		}
	}
	else
	{
				if(color != vec3(0.0f))
		{
			FragmentColor = vec4(color, 1.0f);
		}
		else
		{
			FragmentColor =  texture(Texture, used_texture_coords);
		}
	}

	//FragmentColor = vec4(ModelPosition);

}
