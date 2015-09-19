#version 410
in vec2 vTexCoord;
in vec4 vNormal; //N
in vec4 vTangent;
in vec4 vBiTangent;
in vec4 vPosition;


out vec4 FragColor;

//
uniform vec3 CameraPos;
uniform vec3 LightColor = vec3(1, 1, 1);
uniform vec3 LightDir = normalize(vec3(-10,-10,-10)); //L
uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;
uniform sampler2D NormalMap;


//Material Colors
vec4 kA = texture(DiffuseMap, vTexCoord);	//Ambient Material Color
vec4 kD = texture(DiffuseMap, vTexCoord);	//Diffuse Material Color
vec4 kS = texture(SpecularMap, vTexCoord);	//Specular Material Color
vec4 kN = texture(NormalMap, vTexCoord);

//Light Colors
uniform vec4 iA = vec4(.25f,.25f,.25f,1); 			//Ambient Light Color
uniform vec4 iD = vec4(1,1,1,1);			//Diffuse Light Color
uniform vec4 iS = vec4(1,1,1,1);			//Specular Light Color
uniform float iSpecPower = 15.f;		//Specular Light Power



	void main() {
		//TBN
		mat3 TBN = mat3(vTangent, vBiTangent, vNormal);

		vec3 n = kN.xyz * 2 -1;

		vec3 N = normalize(TBN * n).xyz;

		//Ambient Light
		vec4 Ambient = vec4(kA * iA);

		//Lambert term
		float NdL = max(0.0f, dot(N, -LightDir));
		vec4 Diffuse = vec4(kD * iD * NdL);
		Diffuse.a = 1;

		//float d = max(0, dot(normalize(vNormal.xyz), -LightDir));
		vec3 R = reflect(LightDir, N);
		
		vec3 E = normalize(CameraPos- vPosition.xyz);

		float specTerm = max(0, dot(E, R));
		specTerm = pow(specTerm, iSpecPower); //Spec Term
		vec4 Specular = vec4(kS * specTerm * iS);
		

		/*
			
		*/
		//Diffuse = clamp(Diffuse,0.0f,1.0f);
		//Specular = clamp(Specular,0.0f,1.0f);
		FragColor = Ambient + ((Diffuse) + (Specular));

		//FragColor = Diffuse;
		//FragColor = vec4(N,1);
	}

/*
	#version 410
in vec2 vTexCoord;
in vec4 vNormal; //N
in vec4 vPosition;


out vec4 FragColor;

//Material Colors
uniform vec3 kA = texture(DiffuseMap, vTexCoord);	//Ambient Material Color
uniform vec3 kD = texture(DiffuseMap, vTexCoord);	//Diffuse Material Color
uniform vec3 kS = texture(DiffuseMap, vTexCoord);								//Specular Material Color

//Light Colors
uniform vec3 iA = vec3(1, 1, 1); 	//Ambient Light Color
uniform vec3 iD = vec3(1,1,1);				//Diffuse Light Color
uniform vec3 iS = vec3(1,1,1);				//Specular Light Color
uniform float iSpecPower = 50.f;			//Specular Light Power

//

uniform vec3 CameraPos;
uniform vec3 LightColor = vec3(1, 1, 1);
uniform vec3 LightDir = vec3(0,1,0); //L
uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;

	void main() {
		//Ambient Light
		vec3 Ambient = kA * iA;


		//Diffuse Light
		float NdL = max(0.0f, dot(vNormal.xyz, -LightDir)); //Lambert Term
		vec3 Diffuse = (kD * NdL) * iD;


		//Specular Light
		vec3 R = reflect(LightDir, vNormal.xyz); // Reflected Light	
		vec3 E = normalize(CameraPos - vPosition.xyz); //Surface to eye vector

		float specTerm = max(0.0f, dot(E, R));
		specTerm = pow(specTerm, iSpecPower); //Spec Term

		vec3 Specular = (kS * specTerm * iS);
		
		FragColor = vec4(Ambient + (Diffuse + Specular), 1);
		//FragColor = vec4(Ambient + Diffuse);
	}*/