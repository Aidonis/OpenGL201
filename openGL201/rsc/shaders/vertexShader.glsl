#version 410
layout(location=0) in vec4 Position;
layout(location=1) in vec4 Normal;
layout(location=2) in vec4 Tangent;
layout(location=3) in vec2 TexCoord;


out vec4 vPosition;

out vec3 vNormal;
out vec3 vTangent;
out vec3 vBiTangent;

out vec2 vTexCoord;

uniform mat4 ProjectionView;


	void main(){
		vNormal     = Normal.xyz;
		vTangent    = Tangent.xyz;
		vBiTangent  = cross(vNormal, vTangent);

		vTexCoord   = TexCoord;
		vPosition   = Position;
		gl_Position = ProjectionView * Position;
	}