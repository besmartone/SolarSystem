#version 400 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in vec3 Tangents;

uniform mat4 model;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 viewPosition;

out vec3 Normal;
out vec2 TexCoords;
out vec4 ModelPosition;
out mat3 TBN;
out vec3 Eye;


void main()
{
	vec4 modelPosition =  view * model * vec4(vertex, 1.0);

	Normal = vec4(transpose(inverse(model)) * vec4(normals, 1.0f)).xyz;

	vec3 T = normalize(vec3(model * vec4(Tangents,   1.0)));
    vec3 N = normalize(vec3(model * vec4(normals,   1.0)));

	vec3 B = cross(T, N);

	Eye = viewPosition;

    TBN = mat3(T, B, N);

	ModelPosition = model * vec4(vertex, 1.0);

	TexCoords = texCoords.xy;

	gl_Position = projection * modelPosition;
}

