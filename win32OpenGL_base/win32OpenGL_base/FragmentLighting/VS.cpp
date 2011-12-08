varying vec3 Position;
varying vec3 Normal;

void main(void)
{
   Position = vec3(gl_ModelViewMatrix * gl_Vertex);//Положение вершины относительно точки обзора
   Normal = normalize(gl_NormalMatrix * gl_Normal);
   gl_Position = ftransform();
   gl_TexCoord[0] = gl_MultiTexCoord0;
}