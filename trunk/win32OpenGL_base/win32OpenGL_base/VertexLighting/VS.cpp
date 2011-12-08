/*Прощитывает освещение точечным источником света*/
void PointLight(	in int light, in vec3 pos, in vec3 norm,
						inout vec4 amb,
						inout vec4 diff,
						inout vec4 spec)
{
	vec3 lightVec = gl_LightSource[light].position.xyz - pos;//Вектор от точки до источника света
	float lightDist = length(lightVec);//Расстояние от точки до источника
	lightVec = normalize(lightVec);//Нормируем
	float attenuation = 1.0;
	if(gl_LightSource[light].constantAttenuation != 1.0, gl_LightSource[light].linearAttenuation != 0.0 || gl_LightSource[light].quadraticAttenuation != 0.0)
	{
		float attenuation = 1.0/(	gl_LightSource[light].constantAttenuation +
											gl_LightSource[light].linearAttenuation * lightDist +
											gl_LightSource[light].quadraticAttenuation * lightDist * lightDist);
	}
	vec3 refVec = reflect(lightVec, norm);
	float KDiff = max(dot(norm, lightVec), 0.0);
	float KSpec = 0.0;
	if(KDiff > 0.0)
	{
		KSpec = max(dot(refVec, normalize(pos)), 0.0);
		KSpec = pow(KSpec, /*gl_FrontMaterial.shininess*/128.0);
	}
	amb += gl_LightSource[light].ambient;
	diff += gl_LightSource[light].diffuse * KDiff * attenuation;
	spec += gl_LightSource[light].specular * KSpec * attenuation;
}
//**Точка входа**/
void main(void)
{
	vec4 amb = vec4(0.0), diff = vec4(0.0), spec = vec4(0.0);
	PointLight(0, vec3(gl_ModelViewMatrix * gl_Vertex), normalize(gl_NormalMatrix * gl_Normal), amb, diff, spec);
	gl_FrontColor = amb + diff;
	gl_FrontSecondaryColor = spec;
  
   gl_TexCoord[0] = gl_MultiTexCoord0;
   gl_Position = ftransform();
}