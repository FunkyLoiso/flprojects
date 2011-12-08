#include "main.h"			//���������� �������
//#include "Maths\MATRIX4X4.h"
#include "Maths/Maths.h"

struct Options //���������, ���������� ����� ������������
{
	DEVMODE Mode;//����� �����������
	GLint PixelFormat;//���������� ������
	GLint PixelFormatMS;//���������� ������ � ����������������
	GLboolean FullScreen;//������ �����?
	GLboolean FSAA;//������������� �����������?
	GLboolean VSync;//������������ �������������?
	GLint TexFilter;//����� ������ ������� ������������
	GLfloat AnisotropyLevel;//������� ����������� 
	GLint RenderingMethod;//����� ����������
	GLboolean WF;//�����
	GLint LightingMethod;//��� ���������
};
struct
{
	GLint LightPos;
	GLint LightColor;
	GLint MaterialColor;
	GLint SpecSharpness;
	GLint Texture0;
	GLint Time;
}Uniforms;

#pragma region ���������� ����������
//////////////////////////////////////////////////////////////////////////////////////////////////
static HINSTANCE GlobalHInstance = NULL;		//����� ����������
static LPCTSTR AppName = "win32 meets OpenGL";	//��� ����������
static GLint FontList;							//������� ����������� ��� ������
static Options Opts;							//�����
static LARGE_INTEGER CounterFreq;				//������� ���������� �������� ������������������
static LARGE_INTEGER FPSCount;					//��� �������� �������� �������� ������������������, ���� ��������� ����� ������ �� ����
static LARGE_INTEGER FrameTimer;				//�������� �������� ������������������ ��� ������������ �������� �����, ��� ������� "������"
static POINTS lastMousePoint;					//��������� ���� � ������� ���

static GLhandleARB VertexLighting;
static GLhandleARB FragmentLighting;
static GLhandleARB ClockShader;

MATRIX4X4 lightProjectionMatrix, lightViewMatrix;
MATRIX4X4 cameraProjectionMatrix, cameraViewMatrix;

GLuint offscreenBuffer;

long wndW = 0, wndH = 0;

//Size of shadow map
const int shadowMapSize=2048;
//Textures
GLuint shadowMapTexture;

#define FPS_P 10

#pragma region �������� �����
////////////////////////////////////////////////////////////////////////////////////////
#define N_SPHERES 1			//����� ����
#define FOV 60					//���� ������ ������
GLTFrame SphFrames[N_SPHERES];	//������ ��������� ����
GLTFrame CamFrame;				//��������� ������
GLTFrame LightFrame;
AnQube qb;						//�����))
/*���� � ���������*/
GLfloat LightPos[4] = {-10.0f, 10.0f, 5.0f};	//��������� ��������� �����
GLfloat NoLight[4] = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat LowLight[4] = {0.2f, 0.2f, 0.2f, 0.2f};
GLfloat BrightLight[4] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat SpecSharpness = 16.0f;			//������ ����� ��� �������
GLTMatrix ShadowMatrix;					//������� ���� (���������� ������ GLfloat[16])
#define	GROUND_TEX	0
#define	TORUS_TEX	1
#define	SPHERE_TEX	2
#define QB_TEX		3
#define	N_TEXTURES	4
GLuint TexObjs[N_TEXTURES];
const char* TexFiles[] = {"grass.tga", "wood.tga", "orb.tga", "Qb_2.tga"};

GLuint TorusList, SphereList, GroundList, QbList;	//������� �����������
////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion
/*��������� �� ������� ����������*/
PFNWGLGETPIXELFORMATATTRIBIVARBPROC GetPixFormatARB = NULL;
PFNGLWINDOWPOS2IPROC WindowPos = NULL;
PFNWGLSWAPINTERVALEXTPROC SwapInterval = NULL;
/*������� ��� ������ � ���������*/
PFNGLUNIFORM1FARBPROC glUniform1fARB;
PFNGLUNIFORM1IARBPROC glUniform1iARB;
PFNGLUNIFORM3FARBPROC glUniform3fARB;
PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
PFNGLUNIFORM4FARBPROC glUniform4fARB;
PFNGLUNIFORM4FVARBPROC glUniform4fvARB;
PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
/*��������������*/
PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
/*FBO*/
PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT;
PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
void DoMagic()
{
	glPushMatrix();

	glLoadIdentity();
	//gluLookAt(CamFrame.vLocation[0], CamFrame.vLocation[1], CamFrame.vLocation[2],
	//	0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f);
	gltApplyCameraTransform(&CamFrame);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraViewMatrix);

	glLoadIdentity();
	gluPerspective(140.0f, 1.0f, 0.1f, 40.0f);//������ ���� ����� ��� �������
	glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);

	glLoadIdentity();
	gluLookAt(	LightPos[0], LightPos[1], LightPos[2],
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);

	glPopMatrix();


#define pass1
#define pass2
#define pass3
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, offscreenBuffer);
	//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_DEPTH_ATTACHMENT_EXT, shadowMapTexture, 0);
	//First pass - from light's point of view
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(lightProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(lightViewMatrix);
	//gltApplyCameraTransform(&LightFrame);

	//Use viewport the same size as the shadow map
	glViewport(0, 0, shadowMapSize, shadowMapSize);

	//Draw back faces into the shadow map
	glCullFace(GL_FRONT);

	//Disable color writes, and use flat shading for speed
	glShadeModel(GL_FLAT);
	glColorMask(0, 0, 0, 0);

	//Draw the scene
	//DrawScene(angle);
#ifdef pass1
	//DrawGround();
	glActiveTextureARB(GL_TEXTURE0_ARB);
	DrawObjs(true);
#endif

	//Read the depth buffer into the shadow map texture
	//glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, shadowMapSize, shadowMapSize);

	//restore states
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glColorMask(1, 1, 1, 1);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	//2nd pass - Draw from camera's point of view
	glClear(GL_DEPTH_BUFFER_BIT);

	//gltApplyCameraTransform(&CamFrame);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(cameraProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(cameraViewMatrix);

	glViewport(0, 0, wndW, wndH);

	//Use dim light to represent shadowed areas
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, LowLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LowLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, NoLight);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	//DrawScene(angle);
#ifdef pass2
	glActiveTextureARB(GL_TEXTURE0_ARB);
	DrawGround();
	DrawObjs(false);
#endif

	//3rd pass
	//Draw with bright light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, BrightLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, BrightLight);

	//Calculate texture matrix for projection
	//This matrix takes us from eye space to the light's clip space
	//It is postmultiplied by the inverse of the current view matrix when specifying texgen
	//static GLTMatrix biasMatrix = {0.5f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 0.5f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 0.5f, 0.0f,
	//	0.5f, 0.5f, 0.5f, 1.0f};
	static MATRIX4X4 biasMatrix(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);	//bias from [-1, 1] to [0, 1]
	MATRIX4X4 textureMatrix=biasMatrix*lightProjectionMatrix*lightViewMatrix;
	//GLTMatrix textureMatrix;
	//gltMultiplyMatrix(lightProjectionMatrix, lightViewMatrix, textureMatrix);
	//gltMultiplyMatrix(biasMatrix, textureMatrix, textureMatrix);

	//GLTVector4 row1 = {textureMatrix[0], textureMatrix[4], textureMatrix[8], textureMatrix[12]};
	//GLTVector4 row2 = {textureMatrix[1], textureMatrix[5], textureMatrix[9], textureMatrix[13]};
	//GLTVector4 row3 = {textureMatrix[2], textureMatrix[6], textureMatrix[10], textureMatrix[14]};
	//GLTVector4 row4 = {textureMatrix[3], textureMatrix[7], textureMatrix[11], textureMatrix[15]};

	//Set up texture coordinate generation.
	glActiveTextureARB(GL_TEXTURE1_ARB);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, textureMatrix.GetRow(0));
	glEnable(GL_TEXTURE_GEN_S);

	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, textureMatrix.GetRow(1));
	glEnable(GL_TEXTURE_GEN_T);

	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, textureMatrix.GetRow(2));
	glEnable(GL_TEXTURE_GEN_R);

	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, textureMatrix.GetRow(3));
	glEnable(GL_TEXTURE_GEN_Q);

	//Bind & enable shadow map texture
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glEnable(GL_TEXTURE_2D);

	//Enable shadow comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);

	//Shadow comparison should be true (ie not in shadow) if r<=texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);

	//Shadow comparison should generate an INTENSITY result
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);

	//Set alpha test to discard false comparisons
	glAlphaFunc(GL_GEQUAL, 0.99f);
	glEnable(GL_ALPHA_TEST);

	//glEnable(GL_TEXTURE_2D);

	//DrawScene(angle);
#ifdef pass3
	glActiveTextureARB(GL_TEXTURE0_ARB);
	DrawGround();
	DrawObjs(false);
#endif
	glActiveTextureARB(GL_TEXTURE1_ARB);
	//Disable textures and texgen
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);

	//Restore other states
	glDisable(GL_LIGHTING);
	glDisable(GL_ALPHA_TEST);
	//glActiveTextureARB(GL_TEXTURE0_ARB);
}
#pragma endregion
/*��������, ���� ��������� ������� ����, ��� ������������ � ������������ ����*/
void ChangeSize(GLsizei w, GLsizei h)
{
	if(h == 0) h = 1;//����� �� ���� ������� �� ����
	glViewport(0, 0, w, h);//����� ������� ����, � ������� ����� ���������������. ���������� �� ������� ������ ����
	glMatrixMode(GL_PROJECTION);//����� �������������� ���� ������ ��������
	glLoadIdentity();//��������� � ���� ��������� �������
	gluPerspective(FOV, (GLfloat)w / (GLfloat)h, 0.1, 1024);//������ �������, ����������� ����������� ����� (� ������������) � ���������� �����������
	glMatrixMode(GL_MODELVIEW);//������� ���� ��������� ������ ������� � ����� ������� ���� ��������� �������
	glLoadIdentity();

	gluPerspective(FOV, (float)w/h, 0.1f, 1024.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);
	glLoadIdentity();

	//shadowMapSize = h;
	wndW = w;
	wndH = h;

}
/*������ ����� ��� �������*/
void DrawGround()
{
	/*�������� ��������, ������� �������� ��������������� �� �����..*/
	glBindTexture(GL_TEXTURE_2D, TexObjs[GROUND_TEX]);//������� ������� ���������� ��������
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//���������� �������� �� ��� s (�� ���������������� x)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//���������� �� t (��������������� y)

	glCallList(GroundList);//�������� ������ ����������� � �����
}

/*������ ������� ����: ��������� �����, ����������� ��� � �����*/
void DrawObjs( bool updateLocation)
{
	const GLfloat rotSpeed = 30;//�������� �������� ���� � �������� �������� �������� ����� (������� � �������)
	static GLfloat yRot = 0;	//���� �������� ��� ��������. ������, ��� ��� ��� ����������� ������� �������� ��������, �������)
	if(updateLocation)
	{
		LARGE_INTEGER current;
		QueryPerformanceCounter(&current);
		GLfloat time = (GLfloat)(current.QuadPart - FrameTimer.QuadPart)/(GLfloat)CounterFreq.QuadPart;
		yRot += rotSpeed * time;
		glColor4f(1, 1, 1, 1);
	}
	//else glColor4f(0, 0, 0, 0.75);//���� ���� - �� �������� ���������� ������
	/*������ ��������� ������� ������������� �����*/

	//���
	//gltDrawUnitAxes();
	//�������� �����
	glMaterialfv(GL_FRONT, GL_SPECULAR, BrightLight);
	glBindTexture(GL_TEXTURE_2D, TexObjs[QB_TEX]);
	glPushMatrix();
	glTranslatef(0, 5, 0);
	RenderQube(&qb, updateLocation);
	//gltDrawCube(5);
	glPopMatrix();
	glMaterialfv(GL_FRONT, GL_SPECULAR, NoLight);


	glBindTexture(GL_TEXTURE_2D, TexObjs[SPHERE_TEX]);
	for(int i = 0; i < N_SPHERES; i++)
	{
		glPushMatrix();
		gltApplyActorTransform(&SphFrames[i]);//������������� � �����, �������� ������ SpheresList[i]
		glCallList(SphereList);//��������� �������������� ������� �����������
		glPopMatrix();
	}
	/*������ ��������� ����� � ���*/
	glPushMatrix();
	{
		glTranslatef(0, 0.1f, -2.5f);
		glPushMatrix();
		{
			glRotatef(-yRot * 2, 0, 1, 0);//����� �������� ������ ������������ ��� � ��� ���� ������� ����
			glTranslatef(1, 0, 0);//� ��������� �� ���� �� ���������� 1
			glCallList(SphereList);//������ "���������" �����
		}
		glPopMatrix();
		glMaterialfv(GL_FRONT, GL_SPECULAR, BrightLight);//��� - ����������
		glRotatef(yRot, 0, 1, 0);//������ ������������ ��� (y)
		glBindTexture(GL_TEXTURE_2D, TexObjs[TORUS_TEX]);//������� �������� ��� ���� ������� ���������� ���������
		glCallList(TorusList);//�������� ������������� ������� ����������� ����
		glMaterialfv(GL_FRONT, GL_SPECULAR, NoLight);//������ �������� ������� ���������� ������, �.�. �� �������)
	}
	glPopMatrix();
}

/*���������� ��� ������� �����*/
void RenderSciene()
{
	static GLint Frames = 0;//������� ����� �� ���, ����� �� ��������� fps
	static GLfloat fps = 0;	//fps
	char buffer[64];		//����� ��� fps
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);//������� ��� ������������ ������: �����, ������� � ���������
	//glDisable(GL_TEXTURE_2D);//�������� ���������������, ����� ������������ ���������
	/*����� ��� �������*/
	SYSTEMTIME time;
	GetLocalTime(&time);
	glUniform1fARB(Uniforms.Time, time.wHour*3600.0 + time.wMinute*60.0 + time.wSecond);
	glPushMatrix();
	//{
		//gltApplyCameraTransform(&CamFrame);	//���������� ������� ���, ����� ����������� ��� ���, ��� ���� CamFrame.
	//										//������, ��������� ������� gltApplyActorTransform
		glLightfv(GL_LIGHT0, GL_POSITION, LightPos);//�������� �������� �����
		glColor3f(1.0f, 1.0f, 1.0f);
	//	DrawGround();//������ �����
	//	/*������ ����*/
	//	if(Opts.LightingMethod == LM_Oldschool)
	//	{
	//		glDisable(GL_DEPTH_TEST);//���������	�������� �������
	//		glDisable(GL_LIGHTING);//				���������
	//		glDisable(GL_TEXTURE_2D);//				���������� ��������
	//		glEnable(GL_BLEND);		//��������		���������� ��������������� ������
	//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//����� �������, �� ������� ����� ����������� �����. �������� ����������� ����
	//		//�� ������ �������, ������� ��� ���� - �� ������ � ���������� ����������
	//		glEnable(GL_STENCIL_TEST);//			�������� ���������
	//		glPushMatrix();
	//		{
	//			glMultMatrixf(ShadowMatrix);//	�������� �� ������� ����, ������ ��� ������� ������������ �� "�����"
	//			DrawObjs(true);
	//		}
	//		glPopMatrix();
	//		glDisable(GL_STENCIL_TEST);	//��������� ��������
	//		glDisable(GL_BLEND);		//			����������
	//		glEnable(GL_LIGHTING);		//��������	���������
	//		glEnable(GL_TEXTURE_2D);	//			���������� ��������
	//		glEnable(GL_DEPTH_TEST);	//			�������� �������
	//	}

	//	DrawObjs(false);			//"���������" ������ �������
	//}
	DoMagic();
	glPopMatrix();
	
	/*������� ������� ���������� ������ (���������� ������ ����� ����)*/
	Frames++;
	if(Frames == FPS_P)
	{
		GLfloat Time;			//����� ����� ������
		LARGE_INTEGER Current;	//������� �������� �������� ������������������
		QueryPerformanceCounter(&Current);//�������� ������ �������� �������� ������������������
		Time = (GLfloat)(Current.QuadPart - FPSCount.QuadPart)/(GLfloat)CounterFreq.QuadPart;	//������� ����� ����� ������ ��� ������� ���������
		//�������� ������������������, ������� �� ������� ��� ����������
		fps = FPS_P / Time;
		Frames = 0;//������� ������� ������
		QueryPerformanceCounter(&FPSCount);//��������� ������� �������� �������� ������������������ ��� ���������� ��������
	}
	/*���� �������� ���������� ��������� ����, ���������� ������� ����� ������, � ��������, ���� ���� ����������� �������������� � VSync*/
	if(WindowPos != NULL)
	{
		int Row = 10;			//���������� ������ �� x �� ���� ����
		glDisable(GL_DEPTH_TEST);//���������	�������� �� �������
		glDisable(GL_LIGHTING);//				���������
		glDisable(GL_TEXTURE_2D);//				���������� ��������
		glUseProgramObjectARB(NULL);//��������� ������ ��� ������
		glColor3f(0.0f, 0.8f, 0.0f);		//���� ������
		/*������������� ��������� � ���������� ���������*/
		WindowPos(0, Row);		//��������� �� ������ �������
		glListBase(FontList);//---------------------------------------------------------------------------------------!!!!!!!!!!
		glCallLists(strlen("�������� �������:)") + 1, GL_UNSIGNED_BYTE, "�������� �������:)");
		Row += 15;//���������� �� ��������� �������
		if(Opts.FSAA && Opts.PixelFormatMS != 0)
		{
			//���� �������������� ������������� �������
			WindowPos(0, Row);
			glCallLists(strlen("MultiSampling") + 1, GL_UNSIGNED_BYTE, "MultiSampling");
			Row += 15;
		}
		if(SwapInterval != NULL && Opts.VSync)
		{
			//���� �������������� ������������ �������������
			WindowPos(0, Row);
			glCallLists(strlen("VSync on") + 1, GL_UNSIGNED_BYTE, "VSync on");
			Row += 15;
		}
		/*������� ����� ����������*/
		WindowPos(0, Row);
		switch(Opts.TexFilter)
		{
		case GL_NEAREST:
			glCallLists(strlen("(T)���������� ������: 1 NEAREST"), GL_UNSIGNED_BYTE, "(T)���������� ������: 1 NEAREST");
			break;
		case GL_LINEAR:
			glCallLists(strlen("(T)���������� ������: 2 LINEAR"), GL_UNSIGNED_BYTE, "(T)���������� ������: 2 LINEAR");
			break;
		case GL_NEAREST_MIPMAP_NEAREST:
			glCallLists(strlen("(T)���������� ������: 3 NEAREST_MIPMAP_NEAREST"), GL_UNSIGNED_BYTE, "(T)���������� ������: 3 NEAREST_MIPMAP_NEAREST");
			break;
		case GL_LINEAR_MIPMAP_NEAREST:	
			glCallLists(strlen("(T)���������� ������: 4 LINEAR_MIPMAP_NEAREST"), GL_UNSIGNED_BYTE, "(T)���������� ������: 4 LINEAR_MIPMAP_NEAREST");
			break;
		case GL_NEAREST_MIPMAP_LINEAR:	
			glCallLists(strlen("(T)���������� ������: 5 NEAREST_MIPMAP_LINEAR"), GL_UNSIGNED_BYTE, "(T)���������� ������: 5 NEAREST_MIPMAP_LINEAR");
			break;
		case GL_LINEAR_MIPMAP_LINEAR:	
			glCallLists(strlen("(T)���������� ������: 6 LINEAR_MIPMAP_LINEAR(�����������)"), GL_UNSIGNED_BYTE, "(T)���������� ������: 6 LINEAR_MIPMAP_LINEAR(�����������)");
			break;
		case GL_TEXTURE_MAX_ANISOTROPY_EXT: 
			glCallLists(strlen("(T)���������� ������: 7 MAX_ANISOTROPY"), GL_UNSIGNED_BYTE, "(T)���������� ������: 7 MAX_ANISOTROPY");
			break;
		}
		Row += 15;
		/*������ ������ �����������*/
		WindowPos(0, Row);
		sprintf(buffer, "(F)������� �����������: %.f", Opts.AnisotropyLevel);
		glCallLists(strlen(buffer) + 1, GL_UNSIGNED_BYTE, buffer);
		Row += 15;
		/*������ ����� ���������*/
		WindowPos(0, Row);
		switch(Opts.LightingMethod)
		{
		case LM_Oldschool:
			glCallLists(strlen("(L)����� ���������: ����������� ��������"), GL_UNSIGNED_BYTE, "(L)����� ���������: ����������� ��������");
			break;
		case LM_Vertex:
			glCallLists(strlen("(L)����� ���������: ����������� � �������"), GL_UNSIGNED_BYTE, "(L)����� ���������: ����������� � �������");
			break;
		case LM_Fragment:
			glCallLists(strlen("(L)����� ���������: ������������� � �������"), GL_UNSIGNED_BYTE, "(L)����� ���������: ������������� � �������");
			break;
		case LM_Clock:
			glCallLists(strlen("(L)����� ���������: ������������� � ������� + ����������� �������� � ������"), GL_UNSIGNED_BYTE, "(L)����� ���������: ������������� � ������� + ����������� �������� � ������");
			break;
		}
		Row += 15;
		/*������ ��� ����������*/
		WindowPos(0, Row);
		sprintf(buffer, "(R)��� ����������: ");
		switch(Opts.RenderingMethod)
		{
		case RM_glBeginGlEnd: strcat(buffer, "glBegin/glEnd"); break;
		case RM_viewTable: strcat(buffer, "��������� ����� � ������� �����������"); break;
		case RM_vertexArray: strcat(buffer, "���� ��� � ��������, ������� ������ �������"); break;
		}
		glCallLists(strlen(buffer) + 1, GL_UNSIGNED_BYTE, buffer);
		Row += 15;
		/*fps ���������*/
		sprintf_s(buffer, 64, "FPS: %.2f", fps);
		WindowPos(0, Row);
		glCallLists(strlen(buffer) + 1, GL_UNSIGNED_BYTE, buffer);
		/*�������� �������� �������, ��������� � �������� �������*/
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		switch(Opts.LightingMethod)//�������� ������ �������
		{
		case LM_Vertex: glUseProgramObjectARB(VertexLighting);
			break;
		case LM_Fragment: glUseProgramObjectARB(FragmentLighting);
			break;
		case LM_Clock:	glUseProgramObjectARB(ClockShader);
			break;
		}
	}
}

/*����������� ����� �������� �������. ������ ������ � �� ������, ��������� ��������, ������ ������� �����������*/
void Setup(HDC hDC)
{
	//glShadeModel(GL_FLAT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLTVector3 ground[3] = {{0, -0.4f, 0},	//��� �����, �������� ��������� ����� (��� �������� ������� ����)
							{1, -0.4f, 0},
							{0, -0.4f, -1}};
	/*��� ��� ����������)*/
	qb.axis[0] = 1;
	qb.axis[1] = 0;	//������ ��� z
	qb.axis[2] = -5;
	qb.period = 5.0f;
	qb.segs = 10;
	qb.shift = 0.1f;
	qb.side = 5.0f;
	qb.speed = 120.0f;
	qb.fullTurn = 360.0f;
	/*����������� �����*/
	HFONT Font;					//����� ������
	LOGFONT lf;					//�������� ������
	lf.lfHeight = 15;
	lf.lfWidth = 0;		//... v__v
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_BLACK;
	lf.lfItalic = FALSE;
	lf.lfUnderline = FALSE;
	lf.lfStrikeOut = FALSE;
	lf.lfCharSet = RUSSIAN_CHARSET;
	//lf.lfCharSet = ANSI_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	strcpy_s(lf.lfFaceName, strlen("Arial") + 1, "Arial");
	/*������ ����� � ������� �����������*/
	Font = CreateFontIndirect(&lf);//�������������� ����� ������ ����������, ������� ���� ��� ����������
	SelectObject(hDC, Font);//�������� ���� ����� ��� ������ ���������
	FontList = glGenLists(256);//������ ������� ����������� ��� ������ 128�� �������� (������)
	wglUseFontBitmaps(hDC, 0, 256, FontList);	//������ ������� �������. �� ������ ������ ���������, ����� ������ ����,
	//����� ������, � ����� ������� �����������
	DeleteObject(Font);//����������� �����
	glClearColor(LowLight[0], LowLight[1], LowLight[2], LowLight[3]);//���
	/*			��������
	/*�������� � ������ ����� ������������� �� ��������, ���� � ��� ���-�� ������.
	/*���� ������� ��������� ���������������, �� ������ ����� ������ ����, ��� �������� ��������� ����� ����
	/*(�.�. ����, ��� ��� �� ������). ��� ��� ����, ����� ���� �� "������������"*/
	//glStencilOp(GL_KEEP, GL_INCR, GL_INCR);	//��������� �������� � ������ ��������� �� ���� ��� �������:
	////���� �������� ��������� �� ������(������� �������� �� ��� �������),
	////���� �������� ������, �� ������������ �������� ������ �������
	////���� � �� � ������ �������� ������ (��� ������ �������� + �������� ������� ���������)
	//glClearStencil(0);	//��������, ������� ����� ��������� ����� ���������
	//glStencilFunc(GL_EQUAL, 0x0, 0x1);	//�������� ��������� ������� ������ �� �������, ��� ������� �������� � ������, ��������� �������� � 0x01
	////����� ����� 0x0&0x01 (�� ���� ���������, ����� ��� �����, ������, ��� ����������������� ���������,
	////����� ���� ��������� �������� � ������ "�������������" ������, ���� 0x11 & 0x01 = 0x01)
	//glCullFace(GL_BACK);//������ ����� ��������������� �� ��������������� (�������� ��, ��� ����� �������, �������� ������ �����..)
	//glFrontFace(GL_CCW);//��� ������ �������� �������� ��� �������������, ������� �������� ���� ������ ������ ������� ��������,
	////��� �� ���������� ��������.. ����� ������) ��, � ������, ��� ��� ����� ���� ��� ���������,
	////�� �� ���������, ��� ��� ��� ���� ������� �� ������ �� ������, ���� ������� �� ��������)
	//glEnable(GL_CULL_FACE);//��������	��������� (���� ����� �� ������ ��������� ������ ������)
	//glEnable(GL_DEPTH_TEST);//			�������� �������
	///*�������� ���������*/
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, NoLight);//���������� ���� (�� ���� ������ ���)
	//glLightfv(GL_LIGHT0, GL_AMBIENT, LowLight);	//�������� �������� �����. ���������� ���������
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, BrightLight);//��������� ���������. ����� ����������� �� ���������� �����������. ������� ������� ������� �� ���� �������
	//glLightfv(GL_LIGHT0, GL_SPECULAR, BrightLight);//���������� ���������. ���������� ������������ ������. ��������� �����
	//glEnable(GL_LIGHTING);	//�������� ���������
	//glEnable(GL_LIGHT0);	//� �������� �����, ��� ���������
	//gltMakeShadowMatrix(ground, LightPos, ShadowMatrix);//�������� ������� ���� (������� �������� �� �����.. �� ���� �� ���� ��������� ����� ���� ���������))
	//glEnable(GL_COLOR_MATERIAL);//�������� ������������ ������� ��������� � ��� ������
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);	//�������� (��������� ����������� � ���������� �����) ��������� �������� ������ ���������������
	////������������� �������� ���, ����� ��������������� ����� �����
	//glMateriali(GL_FRONT, GL_SHININESS, 128);	//������� "������" ������, ��� ����� ����� [0, 128], ��� ������ � ���� �����
	gltInitFrame(&CamFrame);//�������������� ����� - ������ � ������ �������, ���� - y, ����� - -z
	for(int i = 0; i < N_SPHERES; i++)//�������� ��������� �����
	{
		gltInitFrame(&SphFrames[i]);//������ ����� � ������, ���� y, ���� -z
		SphFrames[i].vLocation[0] = (GLfloat)(((rand() % 400) - 200) * 0.1);//�������� ��������� ����� �� ���� 40x40 � ����� 0.1
		SphFrames[i].vLocation[1] = 0;//���� ���� ���� ���, ��� ��� - -0.4 �� ������
		SphFrames[i].vLocation[2] = (GLfloat)(((rand() % 400) - 200) * 0.1);
	}
	gltInitFrame(&LightFrame);
	LightFrame.vLocation[0] = LightPos[0];
	LightFrame.vLocation[1] = LightPos[1];
	LightFrame.vLocation[2] = LightPos[2];

	glEnable(GL_TEXTURE_2D);//�������� ���������� ��������
	glGenTextures(N_TEXTURES, TexObjs);//������ � ������� TexObjs N_TESTURES ���������� "���" �������, ������� ����� ����� ������������
	for(int i = 0; i < N_TEXTURES; i++)//�������� ��������
	{
		GLbyte* tex;//��������� �� ������, ���� ��������� ��������
		GLint w, h, comps;//������, ������ (ljk;ys ,snm cntgtym. ldjqrb) � ����� ����������� ����� (1 - ���������, 3 - RGB, 4 - RGBA)
		GLenum format;//������ �������� ��������
		glBindTexture(GL_TEXTURE_2D, TexObjs[i]);//������ �������� (��� ���� ������) ������
		tex = gltLoadTGA(TexFiles[i], &w, &h, &comps, &format);//��������� ��������, �������� ��������� �� �� � ��������� � ����������
		gluBuild2DMipmaps(GL_TEXTURE_2D, comps, w, h, format, GL_UNSIGNED_BYTE, tex);	//������ �� �������� �� ������ ������� � �����������. ��� �����
		//������� � ��������� ������ ���������������
		free(tex);//���� �������� ��� ������ �� �����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Opts.TexFilter);//���������� � �������� �����������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Opts.TexFilter);//���������� � �������� ����������� ������� � ����� �������� - �����������(� �� ����������..?)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//��� ���������� �� ������ �� ���������� �������, ����������� �� ������ ��������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//�� �� ����� �� ������ ���
	}
	/*�������� ��������� �� ������� ����������, ���� ��� ����������*/
	WindowPos = (PFNGLWINDOWPOS2IPROC)wglGetProcAddress("glWindowPos2i");//��� ����������� ������� �� ����
	SwapInterval = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");//��� ������������ �������������, ��� � �����
	glUniform1fARB = (PFNGLUNIFORM1FARBPROC)wglGetProcAddress("glUniform1fARB");
	glUniform1iARB = (PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1iARB");
	glUniform3fARB = (PFNGLUNIFORM3FARBPROC)wglGetProcAddress("glUniform3fARB");
	glUniform3fvARB = (PFNGLUNIFORM3FVARBPROC)wglGetProcAddress("glUniform3fvARB");
	glUniform4fARB = (PFNGLUNIFORM4FARBPROC)wglGetProcAddress("glUniform4fARB");
	glUniform4fvARB = (PFNGLUNIFORM4FVARBPROC)wglGetProcAddress("glUniform4fvARB");
	glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)wglGetProcAddress("glGetUniformLocationARB");
	glUseProgramObjectARB =(PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress("glUseProgramObjectARB");

	glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
	glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");

	glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
	glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
	glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
	if(SwapInterval != NULL)
	{
		if(Opts.VSync) SwapInterval(1);	//�������� �������� �������������
		else	SwapInterval(0);		//��� ��������� ���, ���� �� ��� �� �������� ����)
	}
	if(Opts.FSAA && Opts.PixelFormatMS != NULL)glEnable(GL_MULTISAMPLE_ARB);//��������� ��������������
	if(gltIsExtSupported("GL_EXT_separate_specular_color"))	//���� �������� ���������� ��������������� ����������� ����� (����� ��� ������ �� ��������� �����)
	{														//�� ������ ��� � �������
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	}
	QueryPerformanceFrequency(&CounterFreq);//�������������� �� ��� �������� ������������������
	QueryPerformanceCounter(&FPSCount);

	FrameTimer = FPSCount;	//������� ������� �������� �������� ������������������ � ������ ������, ����� ����� ���������, ����� ������� ������ � �������
							//������������ ����� � �� ������� �� ��� ����� ����� ��������� ������
#pragma region ������ ������� �����������
	TorusList = glGenLists(4);	//������ ������.
	SphereList = TorusList + 1;	//��� ������� - ������ ���� ����
	GroundList = TorusList + 2;
	QbList = TorusList + 3;
	glNewList(TorusList, GL_COMPILE);//���� �������������, �� �� �����������
	{
		gltDrawTorus(0.35f, 0.15f, 60, 30);//-----------------------------------------------------------------------------��������� ����-----------------------
	}
	glEndList();
	glNewList(SphereList, GL_COMPILE);
	{
		gltDrawSphere(0.3f, 100, 100);//-----------------------------------------------------------------------------------��������� ����-----------------------
	}
	glEndList();
	glNewList(GroundList, GL_COMPILE);
	{
		GLfloat side = 40;//����� �������
		GLfloat texSide = 1;//������ ������� ����� "������"
		GLfloat y = -0.4f;//��� �� ������ �����
		/*������ ��� ��������� �����*/
		glBegin(GL_QUADS);
		{
			glNormal3f(0, 1, 0);

			glTexCoord2f(0, 0);
			glVertex3f(-side/2, y, side/2);
			glTexCoord2f(side/texSide, 0);
			glVertex3f(side/2, y, side/2);
			glTexCoord2f(side/texSide, side/texSide);
			glVertex3f(side/2, y, -side/2);
			glTexCoord2f(0, side/texSide);
			glVertex3f(-side/2, y, -side/2);


		}
		glEnd();
	}
	glEndList();
	glNewList(QbList, GL_COMPILE);
	{
		gltDrawCube(qb.side/qb.segs);
	}
	glEndList();
#pragma endregion
#pragma region ����������� �������
	SetupShaderEnv();
	/*������ ������������ ���������*/
	VertexLighting = InstallShaider("VertexLighting/VS.cpp", "VertexLighting/FS.cpp");
	/*������ ������������� ���������*/
	FragmentLighting = InstallShaider("FragmentLighting/VS.cpp", "FragmentLighting/FS.cpp");
	/*����*/
	ClockShader = InstallShaider("Clock/VS.cpp", "Clock/FS.cpp");
	Uniforms.Time = glGetUniformLocationARB(ClockShader, "Time");
#pragma endregion

	//Load identity modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Shading states
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Depth states
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	//We use glScale when drawing the scene
	glEnable(GL_NORMALIZE);

	//Create the shadow map texture
	//glGenTextures(1, &shadowMapTexture);
	//glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	//glTexImage2D(	GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize, 0,
	//	GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	int shadowMapWidth = shadowMapSize;
	int shadowMapHeight = shadowMapSize;

	GLenum FBOstatus;

	// Try to use a texture depth component
	glGenTextures(1, &shadowMapTexture);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);

	// GL_LINEAR does not make sense for depth texture. However, next tutorial shows usage of GL_LINEAR and PCF
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Remove artifact on the edges of the shadowmap
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	// No need to force GL_DEPTH_COMPONENT24, drivers usually give you the max precision if available
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a framebuffer object
	glGenFramebuffersEXT(1, &offscreenBuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, offscreenBuffer);

	// Instruct openGL that we won't bind a color texture with the currently binded FBO
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// attach the texture to FBO depth attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, shadowMapTexture, 0);

	// check FBO status
	//FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	//if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
	//	printf("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO\n");

	// switch back to window-system-provided framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	//Use the color as the ambient and diffuse material
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	//White specular material color, shininess 16
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 16.0f);

	//glGenFramebuffersEXT(1, &offscreenBuffer);
}
/*����������� �������*/
void Shutdown()
{
	/*������ ������� �����������*/
	glDeleteLists(FontList, 256);
	glDeleteLists(TorusList, 3);//���, �����, �����
	glDeleteTextures(N_TEXTURES, TexObjs);//������� ��������
}
/*����� �����*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;						//��������� ����������
	WNDCLASS wc;					//��������� ������ ����
	HWND hWnd;						//����� ����
	UINT Style, StyleX;				//������ ��� ������ ����
	//GlobalHInstance = hInstance;	//��������� ����� ����������

	if(DialogBoxA(hInstance, MAKEINTRESOURCEA(DIALOG), NULL, StartDlgProc) == FALSE) return 0;//���� �� ���������� ������ ��������
	if(Opts.FullScreen)
	{
		if(ChangeDisplaySettingsA(&Opts.Mode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			MessageBoxA(NULL, "�� ������� ������� � ������������� ����� � �������� �����������", "������ ����..", MB_OK | MB_ICONSTOP);
			return -1;
		}
	}
	/*������������ ����� ����*/
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	//��������� CS_OWNDC � ���� ������ ����� ���� �������� ����������, �� ���� ����� ������������
	//BeginPaint() - EndPaint() � �������, ��� ��������� ����� ���������� � �� ��� ����� �����..)
	//������ ��� ��������� ���������� ���� ��������� ����������������, ���� ��� ���������� ��
	//��������� � �����������
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;			//�������� ��� ���� ������� �� �����
	wc.lpszMenuName	= NULL;
	wc.lpszClassName = AppName;
	if(RegisterClassA(&wc) == NULL) return 0;//������������ ����� ����
	if(Opts.FullScreen)//�������� ����� ����
	{
		Style = WS_POPUP;//��� ���������
		StyleX = WS_EX_TOPMOST;//����� �������
	}
	else
	{
		Style = WS_OVERLAPPEDWINDOW;//�������
		StyleX = 0;
	}
	/*������ ����, � ������� ����� �������� �������*/
	hWnd = CreateWindowExA(StyleX, wc.lpszClassName, AppName, Style, 0, 0, Opts.Mode.dmPelsWidth, Opts.Mode.dmPelsHeight, NULL, NULL, hInstance, NULL);
	if(hWnd == NULL) return 0;
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	while(GetMessageA(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
	/*����������� ���������, ����������� ����� �������*/
	if(Opts.FullScreen) ChangeDisplaySettingsA(NULL, 0);//������, �� ������ ���������
	return msg.wParam;
}

/*������������ ��� ��������� ���� ���������*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HGLRC GLRC;	//���������� �������� ������������ ��� OpenGL
	static HDC hDC;		//�������� ���������� (GDI)

	switch(message)
	{
	case WM_CREATE:	//�������� ������, ��������� OpenGL
		{
			hDC = GetDC(hWnd);//��������� ��������, ��������������� ���� (�� ��� ������ � ������ ���������, �������?))
			/*�����, ��������, ���������, ��� ��� ��������� ������������� ����������� �������*/
			FindBestPF(hDC, &Opts.PixelFormat, &Opts.PixelFormatMS);
			if(Opts.FSAA && Opts.PixelFormatMS != NULL)
			{
				SetPixelFormat(hDC, Opts.PixelFormatMS, NULL);//��������� ���� - ���������� �������� �������, ��� ����� ������ ��� �����, ��� ����-�� ������
			}
			else
			{
				SetPixelFormat(hDC, Opts.PixelFormat, NULL);
			}
			GLRC = wglCreateContext(hDC);	//������ �������� ������������ ( RC - rendering context, ��?) ��� ������ ��������� ����������, ��-��
			wglMakeCurrent(hDC, GLRC);		//� ������ ��� ������� ��� ���� �� ��������� ����������

			GLfloat maxAnisotropy;			//��������� ������������ ������� ������������ ���������� ����� ����, ��� ���������� ���������� ������
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			Opts.AnisotropyLevel = maxAnisotropy;

			Setup(hDC);	//������� ��������� ��������� ������
		}
		break;
	case WM_KEYDOWN:	//�������� ������� �������
		switch(wParam)
		{
		case VK_ESCAPE: DestroyWindow(hWnd);
			break;

		case 0x56://V - ������������ �������������
			{
				Opts.VSync = !Opts.VSync;
				SwapInterval(int(Opts.VSync));
			}
			break;
		case 0x54://T - ����� ���������� �������
			{
				switch(Opts.TexFilter)
				{
				case GL_NEAREST:
					Opts.TexFilter++;
					break;
				case GL_LINEAR:
					Opts.TexFilter = GL_NEAREST_MIPMAP_NEAREST;
					break;
				case GL_NEAREST_MIPMAP_NEAREST:
				case GL_LINEAR_MIPMAP_NEAREST:
				case GL_NEAREST_MIPMAP_LINEAR:
					Opts.TexFilter++;
					break;
				case GL_LINEAR_MIPMAP_LINEAR:
					Opts.TexFilter = GL_NEAREST;
					break;
				}
				for(int i = 0; i < N_TEXTURES; i++)//�������� ������� ��������� ������
				{
					GLbyte* tex;//��������� �� ������, ���� ��������� ��������
					GLint w, h, comps;//������, ������ (ljk;ys ,snm cntgtym. ldjqrb) � ����� ����������� ����� (1 - ���������, 3 - RGB, 4 - RGBA)
					GLenum format;//������ �������� ��������
					glBindTexture(GL_TEXTURE_2D, TexObjs[i]);//������ �������� (��� ���� ������) ������
					tex = gltLoadTGA(TexFiles[i], &w, &h, &comps, &format);//��������� ��������, �������� ��������� �� �� � ��������� � ����������
					gluBuild2DMipmaps(GL_TEXTURE_2D, comps, w, h, format, GL_UNSIGNED_BYTE, tex);	//������ �� �������� �� ������ ������� � �����������. ��� �����
					//������� � ��������� ������ ���������������
					free(tex);//���� �������� ��� ������ �� �����
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Opts.TexFilter);//���������� � �������� �����������
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Opts.TexFilter);//���������� � �������� ����������� ������� � ����� �������� - �����������(� �� ����������..?)
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//��� ���������� �� ������ �� ���������� �������, ����������� �� ������ ��������
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//�� �� ����� �� ������ ���

					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, Opts.AnisotropyLevel);//������� ������� �����������
				}
			}
			break;
		case 0x46://F - ������� ������������ ����������
			{

				static GLint maxAnisotropy = 0;
				if(!maxAnisotropy) glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
				Opts.AnisotropyLevel < maxAnisotropy ? Opts.AnisotropyLevel++ : Opts.AnisotropyLevel = 1;
				for(int i = 0; i < N_TEXTURES; i++)//�������� ������� ��������� ������
				{
					GLbyte* tex;//��������� �� ������, ���� ��������� ��������
					GLint w, h, comps;//������, ������ (ljk;ys ,snm cntgtym. ldjqrb) � ����� ����������� ����� (1 - ���������, 3 - RGB, 4 - RGBA)
					GLenum format;//������ �������� ��������
					glBindTexture(GL_TEXTURE_2D, TexObjs[i]);//������ �������� (��� ���� ������) ������
					tex = gltLoadTGA(TexFiles[i], &w, &h, &comps, &format);//��������� ��������, �������� ��������� �� �� � ��������� � ����������
					gluBuild2DMipmaps(GL_TEXTURE_2D, comps, w, h, format, GL_UNSIGNED_BYTE, tex);	//������ �� �������� �� ������ ������� � �����������. ��� �����
					//������� � ��������� ������ ���������������
					free(tex);//���� �������� ��� ������ �� �����
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Opts.TexFilter);//���������� � �������� �����������
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Opts.TexFilter);//���������� � �������� ����������� ������� � ����� �������� - �����������(� �� ����������..?)
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//��� ���������� �� ������ �� ���������� �������, ����������� �� ������ ��������
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//�� �� ����� �� ������ ���

					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, Opts.AnisotropyLevel);//������� ������� �����������
				}
			}
			break;
		case 0x52://R - ��� ����������
			if(Opts.RenderingMethod == 3) Opts.RenderingMethod = 1;
			else Opts.RenderingMethod++;
			break;
		case 0x50://P - �����/����������
			Opts.WF ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			Opts.WF = !Opts.WF;
			break;
		case 0x4c://L - ����� ���������
			if(Opts.LightingMethod == 4) Opts.LightingMethod = 1;
			else Opts.LightingMethod++;
			switch(Opts.LightingMethod)
			{
			case LM_Oldschool: glUseProgramObjectARB(NULL);
				break;
			case LM_Vertex: 
				glUseProgramObjectARB(VertexLighting);
				break;
			case LM_Fragment:
				glUseProgramObjectARB(FragmentLighting);
				break;
			case LM_Clock:
				glUseProgramObjectARB(ClockShader);
			}
		}
		break;
	case WM_ACTIVATE:	//������������ ����������� � �����. ��� ����� ���� ��� ����� ������� � ������������� (���� ��������), ��� �������� �����
		{
			if(Opts.FullScreen)
			{
				/*������ ��������� ������������ ����*/
				WINDOWPLACEMENT wp;
				wp.length = sizeof(WINDOWPLACEMENT);
				wp.flags = WPF_RESTORETOMAXIMIZED;	//����������
				wp.ptMaxPosition.x = 0;
				wp.ptMaxPosition.y = 0;
				wp.ptMinPosition.x = 0;
				wp.ptMinPosition.y = 0;
				wp.rcNormalPosition.bottom = Opts.Mode.dmPelsHeight;//� ������������� ��������� ��� ��������� ������ �������
				wp.rcNormalPosition.left = 0;
				wp.rcNormalPosition.right = Opts.Mode.dmPelsWidth;
				wp.rcNormalPosition.top = 0;
				if(LOWORD(wParam) == WA_INACTIVE)//���� ������������� �� ����
				{
					wp.showCmd = SW_SHOWMINNOACTIVE;//���������� � ����, ����� ��������� ����, �� ���� �������
					SetWindowPlacement(hWnd, &wp);
					ShowCursor(TRUE);
				}
				else//���� ������� �� ���� �������������
				{
					wp.showCmd = SW_RESTORE;//���������������, ����� �������� ����
					SetWindowPlacement(hWnd, &wp);
					ShowCursor(FALSE);
				}
			}
		}
		break;
	case WM_DESTROY:	//���� �������
		{
			Shutdown();//����������� �������
			wglMakeCurrent(hDC, NULL);	//�������� ����� ��������� ������������
			wglDeleteContext(GLRC);		//� ������ ���
			PostQuitMessage(0);			//�������� ���������� ��������� ����� �������� ����
		}
		break;
	case WM_SIZE:	//������ ���� ���������
		ChangeSize(LOWORD(lParam), HIWORD(lParam));//������������ ����� ��������� � ���� ���������
		break;
	case WM_PAINT:	//���� ����������������
		{
			LARGE_INTEGER Current;
			/*������� ����������*/
			if(GetFocus() == hWnd)//���� ���� ���� � ������, �� ������� ����������
			{
				GLfloat Time, Linear;
				/*������� �����, ��������� � ��������� ������������ �����*/
				QueryPerformanceCounter(&Current);
				Time = (GLfloat)(Current.QuadPart - FrameTimer.QuadPart) / (GLfloat)CounterFreq.QuadPart;
				/*�������� ������ ����������� �� �������. �� ������ �������� � ������� ����� ������������, �� ������)
				/*�������� � ����������� � �������*/
				const GLfloat linSpeed = 5.0f;

				Linear = Time * linSpeed;
				/*������� ���������� � ���� �����-�� ������ ������ - ����������/�������*/
				if(GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57)) gltMoveFrameForward(&CamFrame, Linear);
				if(GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x53)) gltMoveFrameForward(&CamFrame, -Linear);
				if(GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41)) gltMoveFrameRight(&CamFrame, Linear);
				if(GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44)) gltMoveFrameRight(&CamFrame,-Linear);
				if(GetAsyncKeyState(VK_SPACE)) gltTranslateFrameWorld(&CamFrame, 0, Linear, 0);
				if(GetAsyncKeyState(VK_SHIFT)) gltTranslateFrameWorld(&CamFrame, 0, -Linear, 0);
			}
#pragma region ���������� �������� �����
			//////////////////////////////////////////////////////////////////////////////////////////////////
			const GLfloat MouseSpeed = 0.002f;//������� �� ������� ��������)

			static RECT wndRect;
			if(wndRect.left == 0) GetWindowRect(hWnd, &wndRect);//������� ������� ������� ����
			static int cx = (wndRect.right + wndRect.left) / 2;//������� ����� ����
			static int cy = (wndRect.top + wndRect.bottom) / 2;
			POINT cur;		//������� ��������� �������
			GetCursorPos(&cur);//������� �� ����� ������� ��������� �������

			GLfloat xAngle = (cx - cur.x) * MouseSpeed;
			GLfloat yAngle = (cy - cur.y) * MouseSpeed;
			gltRotateFrameLocalX(&CamFrame, -yAngle);	//�������� ������ x
			gltRotateFrameWorldY(&CamFrame, xAngle);	//� ������ �

			SetCursorPos(cx, cy);
			//////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion
			RenderSciene();		//�������� �����
			SwapBuffers(hDC);	//����������� ������
			FrameTimer = Current;//� ����� ����� ������ ��� �������������� ����� ������� � FrameTimer
			//ValidateRect(hWnd, NULL);//���� �� ����������� ������������, WM_PAINT ����� ���������� ����� � ����� �� ������������..
		}
		break;
	default: return DefWindowProcA(hWnd, message, wParam, lParam);
	}
	return(0L);//...oO
}
/*������������ ������ ��� �������*/
BOOL APIENTRY StartDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		{
			GLint PF;		//���������� ������
			HDC hDC;		//�������� ����������� ����
			HGLRC GLRC;		//�������� ������������ OpenGL
			DEVMODE Mode;	//����� ������ �������
			GLuint curMode;
			GLuint curW;	//������� ���������
			GLuint curH;
			char Buffer[64];//����� ��� �����, ������?
			HWND ComboBox;	//����� ������ (��� �� ����?)
			PIXELFORMATDESCRIPTOR pfd = {
				sizeof(PIXELFORMATDESCRIPTOR),
				1,			//������
				PFD_DRAW_TO_WINDOW |	// ������������ ������ � ����
				PFD_SUPPORT_OPENGL |	// ������������ OpenGL
				PFD_DOUBLEBUFFER,		// ������ � ������� ������������
				PFD_TYPE_RGBA,			// RGBA 
				32,						// ������� �����
				0, 0, 0, 0, 0, 0,		// �� ������������ ��� ������ ������
				0, 0, 0, 0, 0,
				16,						// ����� �������
				8,						// ����� ���������
				0, 0, 0, 0, 0, 0};		//�����-�� �������������� ������, �����-�� ��������������� ���������, ���-�� ����������.. ��� �� ���� ���
				/*������������� ����� ������������*/
				Opts.FSAA = false;
				Opts.FullScreen = false;
				Opts.VSync = false;
				Opts.TexFilter = GL_LINEAR_MIPMAP_LINEAR;
				Opts.RenderingMethod = RM_glBeginGlEnd;
				Opts.WF = false;
				Opts.LightingMethod = LM_Oldschool;
				/*�������� "���������" �������� ������������, ����� ������ ���������� �������*/
				hDC = GetDC(hDlg);
				PF = ChoosePixelFormat(hDC, &pfd);	//����� �������� ������� �� ���, ��� ������ � ���������
				SetPixelFormat(hDC, PF, &pfd);		//������ ��� ������� ��� ������ �������
				DescribePixelFormat(hDC, PF, sizeof(PIXELFORMATDESCRIPTOR), &pfd);//��������� ���������� � ��������� � ��� �� ���������
				GLRC = wglCreateContext(hDC);	//������ �������� ������������
				wglMakeCurrent(hDC, GLRC);		//� ��������� �������
				SetDlgItemTextA(hDlg, L_VENDOR, (LPCSTR)glGetString(GL_VENDOR));//��������� ������� � �������
				SetDlgItemTextA(hDlg, L_CARD, (LPCSTR)glGetString(GL_RENDERER));
				SetDlgItemTextA(hDlg, L_VER, (LPCSTR)glGetString(GL_VERSION));
				FindBestPF(hDC, &Opts.PixelFormat, &Opts.PixelFormatMS);//������� ���������� ������� � ���������������� � ��� ����
				wglMakeCurrent(hDC, NULL);	//�������� �������� ������������
				wglDeleteContext(GLRC);		//� ������� ���
				/*������������ ������� ����������� � ����������*/
				curMode = 0;
				curW = GetSystemMetrics(SM_CXSCREEN);//������� �����
				curH = GetSystemMetrics(SM_CYSCREEN);
				ComboBox = GetDlgItem(hDlg, CB_RES);
				while(EnumDisplaySettingsA(NULL, curMode, &Mode))
				{
					if(Mode.dmBitsPerPel < 16)
					{
						curMode++;
						continue;
					}
					GLint item;
					sprintf_s(Buffer, 64, "%d x %d x %dbpp @%dhz", Mode.dmPelsWidth, Mode.dmPelsHeight, Mode.dmBitsPerPel, Mode.dmDisplayFrequency);
					item = SendMessageA(ComboBox, CB_ADDSTRING, 0, (LPARAM)Buffer);//������� ������������ �������
					SendMessageA(ComboBox, CB_SETITEMDATA, item, curMode);//� ��������� ���� ������� ������ ����� ������ ������
					if(Mode.dmPelsHeight == curH && Mode.dmPelsWidth == curW)
						SendMessageA(ComboBox, CB_SETCURSEL, item, 0);
					curMode++;
				}
				/*��������� ��������� ����������*/
				CheckDlgButton(hDlg, CHB_FS, BST_CHECKED);//������������� �����
				if(Opts.PixelFormatMS != NULL)//������������ �������� ������ ���� ��������
					EnableWindow(GetDlgItem(hDlg, CHB_MS), TRUE);
				else
					EnableWindow(GetDlgItem(hDlg, CHB_MS), FALSE);
				return(TRUE);//������� ��������, ��� �� ������
		}
		break;
	case WM_COMMAND:	//��������� ������
		{
			if(LOWORD(wParam) == B_START)
			{
				/*��������� ��������� ����� �����������*/
				HWND ComboBox = GetDlgItem(hDlg, CB_RES);
				GLint mode = SendMessageA(ComboBox, CB_GETCURSEL, 0, 0);//������� ������� ����� ������� �������
				mode = SendMessageA(ComboBox, CB_GETITEMDATA, mode, 0);	//� ������ - ��������, ����������� � ���� �������
				EnumDisplaySettingsA(NULL, mode, &Opts.Mode);//��������� � ����� ���������, ����������� ����� ������� ��� �������� �������
				if(IsDlgButtonChecked(hDlg, CHB_FS))	Opts.FullScreen = true;	//������ �����
				else									Opts.FullScreen = false;
				if(IsDlgButtonChecked(hDlg, CHB_MS))	Opts.FSAA = true;		//FSAA
				else									Opts.FSAA = false;
				if(IsDlgButtonChecked(hDlg, CHB_VS))	Opts.VSync = true;		//VSync
				else									Opts.VSync = false;
				EndDialog(hDlg, TRUE);//���������� ������ � ��������� �����
			}
			if(LOWORD(wParam) == B_CANCEL)
				EndDialog(hDlg, FALSE);
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, FALSE);
		break;
	}
	return FALSE;
}
/*����� ����������� ������� � ��������� ����������, ��������� � ���������� ����� ���������� ������ � ���������������� (����, ���� ����) � ���*/
void FindBestPF(HDC hDC, GLint *Format, GLint *MSFormat)
{
	*Format = 0;
	*MSFormat = 0;
	/*���� �� ���������� ��� ������ ����������� ������� �� ����������?*/
	if(gltIsWGLExtSupported(hDC, "WGL_ARB_pixel_format"))
		if(GetPixFormatARB == NULL) GetPixFormatARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");
	if(GetPixFormatARB != NULL)//������� �������� ������������ ���������� wgl
	{
		int BestMS = 0;
		int Results[9];
		int Attribs[9] = {	WGL_SUPPORT_OPENGL_ARB,
			WGL_ACCELERATION_ARB,
			WGL_DRAW_TO_WINDOW_ARB,
			WGL_DOUBLE_BUFFER_ARB,
			WGL_PIXEL_TYPE_ARB,
			WGL_DEPTH_BITS_ARB,
			WGL_STENCIL_BITS_ARB,
			WGL_SAMPLE_BUFFERS_ARB,
			WGL_SAMPLES_ARB};
		int NFormats[] = {0};	//������� ���������, ������� ����� ��������
		int attrib[] = {WGL_NUMBER_PIXEL_FORMATS_ARB};
		GetPixFormatARB(hDC, 1, 0, 1, attrib, NFormats);//������� ����� ��������
		for(int i = 0; i < NFormats[0]; i++)//������ ������ ������
		{
			GetPixFormatARB(hDC, i+1, 0, 9, Attribs, Results);	//������� ���� ������(����� ����������, ����� �������, ���� �����-��, ����� ���������,
			//��������, ���� ������� ���������� �� ������ ���������)
			/*�������� ������*/
			if(Results[0] == 1 && Results[1] == WGL_FULL_ACCELERATION_ARB && Results[2] == 1)//��������� OpenGL, ������ ���������, ��������� � ����
				if(Results[3] == 1)//������� �����������
					if(Results[4] == WGL_TYPE_RGBA_ARB)//������ ����
						if(Results[5] >= 16)//������� ������ ���� ����� 16�� �����
							if(Results[6] > 0)//� ��������� ���� ���� �����-�� �������
							{
								//���� ������ - �������� �� �������������. ���������, ��� � ���� � ����������������
								if(Results[7] == 1)//���� ��������������
								{
									if(Results[8] > BestMS)//���� � ����� ������� �������������� ������, ��� � ��������� �� �����
									{
										*MSFormat = i;
										BestMS = Results[8];
									}
								}
								else//��� ���������������
								{
									//�������� ��� "�������� �������"
									*Format = i;
								}
							}
		}
	}
	else//���� ���������� �� �������������� - ���� ����������� ��������
	{
		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),
			1,			//������
			PFD_DRAW_TO_WINDOW |	// ������������ ������ � ����
			PFD_SUPPORT_OPENGL |	// ������������ OpenGL
			PFD_DOUBLEBUFFER,		// ������ � ������� ������������
			PFD_TYPE_RGBA,			// RGBA 
			32,						// ������� �����
			0, 0, 0, 0, 0, 0,		// �� ������������ ��� ������ ������
			0, 0, 0, 0, 0, 0,
			24,						// ����� �������
			8,						// ����� ���������
			0, 0, 0, 0, 0, 0};		//�����-�� �������������� ������, �����-�� ��������������� ���������, ���-�� ����������.. ��� �� ���� ���
			*Format = ChoosePixelFormat(hDC, &pfd);//������� ����� ������� ������ � ������� ��� ����� � ���������� ��� ��������
	}
}

/*�������������� ���, �������� ��� ��������*/
void RenderQube(AnQube *qb, bool updateRotation)
{
	/*����� ������������ ��������*/
	static GLfloat halfSide = qb->side/2;
	static GLfloat segSize = qb->side/qb->segs;
	static GLfloat halfSegSize = qb->side/qb->segs/2;


	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);
	static LARGE_INTEGER periodStart = current;
	static GLfloat time = 0;
	if(updateRotation) time = (GLfloat)(current.QuadPart - periodStart.QuadPart) / (GLfloat)CounterFreq.QuadPart;//�����, ��������� � ������ ������� ��������
	if(time > qb->period)//
	{
		periodStart = current;
		time = 0;
	}
	//������ �������� ������ � �������� ��
	static bool first = true;
	if(Opts.RenderingMethod == RM_vertexArray)
	{
#pragma region ���� � ��������
		static GLTVector3 *Verts = (GLfloat(*)[3])malloc(sizeof(GLfloat) * qb->segs * qb->segs * qb->segs * 72);//����� ������� * ����� ������ * ����� �� ����� * ����������� �������
		static GLTVector2 *Texts = (GLfloat(*)[2])malloc(sizeof(GLfloat) * qb->segs * qb->segs * qb->segs * 48);
		static GLTVector3 *Norms = (GLfloat(*)[3])malloc(sizeof(GLfloat) * qb->segs * qb->segs * qb->segs * 72);
		/*p.s. ����. GLfloat(*)[3] - ���� ��������� �� ������ �� ��� �������, ��� ��� ��� ������.. ��� �������� ������ ������ ������ ��� �� �����������..)*/
		//���� ������ ��� ��������� - �������������� ����
		if(first)
		{
			InitAnQubeArrays(qb, Verts, Texts, Norms);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);

			glVertexPointer(3, GL_FLOAT, 0, Verts);
			glTexCoordPointer(2, GL_FLOAT, 0, Texts);
			glNormalPointer(GL_FLOAT, 0, Norms);
			first = false;
		}
		/*�������� ��. ���������� ������� �������� ��� ������� ������ � ����, ������� ��� � ����� �������� ��� ���������� ��� ���� ��������� �������*/
		for(int y = 0; y < qb->segs; y++)
		{
			GLfloat angle = qb->speed * (time - qb->shift * y);//���� �������� � ������ ������� ��� �������� ����
			if(angle < 0) angle = 0;//���� ������� ����� ���� ��� �� ������
			if(angle >= qb->fullTurn) angle = qb->fullTurn;//���� ������ ������ ��� ���������
			/*���������� ������� �������� ��� �������� ������ ������*/
			GLint yShift = y * qb->segs * qb->segs * 24;
			GLTMatrix rotMat;		//������� �������� ������
			GLTMatrix temp;
			GLTMatrix normRotMat;	//������� �������� ��������
			gltRotationMatrix((GLfloat)gltDegToRad(angle), qb->axis[0], qb->axis[1], qb->axis[2], rotMat);
			memcpy(&temp, &rotMat, sizeof(GLTMatrix));
			gltTransposeMatrix(temp);
			gltInvertMatrix(temp, normRotMat);
			//���������� ���������� ����� �������� � ���� ����� �������
			rotMat[12] = -halfSide + halfSegSize;
			rotMat[13] = halfSide - halfSegSize - y*segSize;
			rotMat[14] = halfSide - halfSegSize;
			InitLittleQube(Verts + yShift, Norms + yShift, segSize);
			for(int i = 0; i < 24; i++)
			{
				gltTransformPoint(Verts + yShift + i, rotMat);
				gltTransformPoint(Norms + yShift + i, normRotMat);
			}
			/*������ ������� ��������� ������ ����� � ������������� � ������������ ������*/
			for(GLint z = 0; z < qb->segs; z++)
				for(GLint x = 0; x < qb->segs; x++)
				{
					if(x == 0 && z == 0)
					{
						continue;//������ ����� �� �������
					}
					GLint xShift = x * 24;
					GLint zShift = z * qb->segs * 24;
					GLint vert = 0;
					do
					{
						(*(Verts + yShift + zShift + xShift + vert))[0] = (*(Verts + yShift + vert))[0] + x * segSize;
						(*(Verts + yShift + zShift + xShift + vert))[1] = (*(Verts + yShift + vert))[1];
						(*(Verts + yShift + zShift + xShift + vert))[2] = (*(Verts + yShift + vert))[2] - z * segSize;

						(*(Norms + yShift + zShift + xShift + vert))[0] = (*(Norms + yShift + vert))[0];
						(*(Norms + yShift + zShift + xShift + vert))[1] = (*(Norms + yShift + vert))[1];
						(*(Norms + yShift + zShift + xShift + vert))[2] = (*(Norms + yShift + vert))[2];
					}while(++vert < 24);
				}
		}
		static int count = qb->segs * qb->segs * qb->segs * 24;
		glDrawArrays(GL_QUADS, 0, count);
#pragma endregion
	}
	else
	{
		first = true;
#pragma region ��������� ����� �� ������
		for(GLint y = 0; y < qb->segs; y++)//��� ������ �����������
		{
			glPushMatrix();
			{
				glTranslatef(-halfSide + halfSegSize,  halfSide - segSize*(y+0.5f), halfSide - halfSegSize);//���������� � ����� �������� ������ ���������� ���� �����������
				int x = 0;
				int z = 0;
				do
				{
					glPushMatrix();
					if(qb->speed * (time - qb->shift*y) < qb->fullTurn && qb->speed * (time - qb->shift*y) > 0)glRotatef(qb->speed * (time - qb->shift*y), qb->axis[0], qb->axis[1], qb->axis[2]);//������������, ������ ���� ��� �� ������� ������� �������
					switch(Opts.RenderingMethod)
					{
					case RM_glBeginGlEnd:
						//glFrontFace(GL_CW);
						gltDrawCube(qb->side/qb->segs);
						//glutSolidCube(qb->side/qb->segs);
						break;
					case RM_viewTable:
						glCallList(QbList);
						break;
					}
					glPopMatrix();
					if(++x >= qb->segs)
					{
						x = 0, z++;
						glTranslatef(-qb->side + qb->side/qb->segs, 0, -qb->side/qb->segs);
					}
					else glTranslatef(qb->side/qb->segs, 0, 0);
					if(z >= qb->segs) break;
				}
				while(1);
			}
			glPopMatrix();
		}
#pragma endregion
	}
}
/*��������������� ������� ��� ����, ���������� ������ � ��������� ���������*/
void InitAnQubeArrays(AnQube *qb, GLfloat Verts[][3], GLfloat Texts[][2], GLfloat Norms[][3])
{
	static GLfloat segSize = qb->side/qb->segs;
	static GLfloat halfSide = qb->side/2;
	const GLfloat e = 0.02f;

	GLint vertCounter = 0;
	for(GLfloat y = halfSide - segSize; y >= -halfSide - e; y -= segSize)//�����������
		for(GLfloat x = -halfSide; x <= halfSide - segSize + e; x += segSize)//�������
			for(GLfloat z = halfSide; z >= -halfSide + segSize - e; z -= segSize)//������ � �������
			{
				//�������� �����
				Norms[vertCounter][0] = 0;
				Norms[vertCounter][1] = 0;
				Norms[vertCounter][2] = 1.0f;
				for(int i = 1; i < 4; i++)
				{
					memcpy(Norms[vertCounter], Norms[vertCounter+i], sizeof(GLfloat)*3);
				}

				Texts[vertCounter][0] = 0.34f;
				Texts[vertCounter][1] = 0;
				Verts[vertCounter][0] = x;			//0
				Verts[vertCounter][1] = y;
				Verts[vertCounter++][2] = z;

				Texts[vertCounter][0] = 0.66f;
				Texts[vertCounter][1] = 0;
				Verts[vertCounter][0] = x + segSize;	//3
				Verts[vertCounter][1] = y;
				Verts[vertCounter++][2] = z;

				Texts[vertCounter][0] = 0.66f;
				Texts[vertCounter][1] = 0.45f;
				Verts[vertCounter][0] = x + segSize;	//2
				Verts[vertCounter][1] = y + segSize;
				Verts[vertCounter++][2] = z;

				Texts[vertCounter][0] = 0.34f;
				Texts[vertCounter][1] = 0.45f;
				Verts[vertCounter][0] = x;			//1
				Verts[vertCounter][1] = y + segSize;
				Verts[vertCounter++][2] = z;

				//������ �����
				Norms[vertCounter][0] = 1.0f;
				Norms[vertCounter][1] = 0;
				Norms[vertCounter][2] = 0;
				for(int i = 1; i < 4; i++)
				{
					memcpy(Norms[vertCounter], Norms[vertCounter+i], sizeof(GLfloat)*3);
				}

				Texts[vertCounter][0] = 0.72f;
				Texts[vertCounter][1] = 0;
				Verts[vertCounter][0] = x + segSize;	//3
				Verts[vertCounter][1] = y;
				Verts[vertCounter++][2] = z;

				Texts[vertCounter][0] = 0.99f;
				Texts[vertCounter][1] = 0;
				Verts[vertCounter][0] = x + segSize;	//5
				Verts[vertCounter][1] = y;
				Verts[vertCounter++][2] = z - segSize;

				Texts[vertCounter][0] = 0.99f;
				Texts[vertCounter][1] = 0.45f;
				Verts[vertCounter][0] = x + segSize;	//6
				Verts[vertCounter][1] = y + segSize;
				Verts[vertCounter++][2] = z - segSize;

				Texts[vertCounter][0] = 0.72f;
				Texts[vertCounter][1] = 0.45f;
				Verts[vertCounter][0] = x + segSize;	//2
				Verts[vertCounter][1] = y + segSize;
				Verts[vertCounter++][2] = z;

				//������ �����
				Norms[vertCounter][0] = 0;
				Norms[vertCounter][1] = 0;
				Norms[vertCounter][2] = -1.0f;
				for(int i = 1; i < 4; i++)
				{
					memcpy(Norms[vertCounter], Norms[vertCounter+i], sizeof(GLfloat)*3);
				}

				Texts[vertCounter][0] = 0.72f;
				Texts[vertCounter][1] = 0.55f;
				Verts[vertCounter][0] = x + segSize;	//5
				Verts[vertCounter][1] = y;
				Verts[vertCounter++][2] = z - segSize;

				Texts[vertCounter][0] = 1.0f;
				Texts[vertCounter][1] = 0.55f;
				Verts[vertCounter][0] = x;			//4
				Verts[vertCounter][1] = y;
				Verts[vertCounter++][2] = z - segSize;

				Texts[vertCounter][0] = 1.0f;
				Texts[vertCounter][1] = 1.0f;
				Verts[vertCounter][0] = x;			//7
				Verts[vertCounter][1] = y + segSize;
				Verts[vertCounter++][2] = z - segSize;

				Texts[vertCounter][0] = 0.72f;
				Texts[vertCounter][1] = 1.0f;
				Verts[vertCounter][0] = x + segSize;	//6
				Verts[vertCounter][1] = y + segSize;
				Verts[vertCounter++][2] = z - segSize;

				//����� �����
				Norms[vertCounter][0] = -1.0f;
				Norms[vertCounter][1] = 0;
				Norms[vertCounter][2] = 0;
				for(int i = 1; i < 4; i++)
				{
					memcpy(Norms[vertCounter], Norms[vertCounter+i], sizeof(GLfloat)*3);
				}

				Texts[vertCounter][0] = 0;
				Texts[vertCounter][1] = 0;
				Verts[vertCounter][0] = x;			//4
				Verts[vertCounter][1] = y;
				Verts[vertCounter++][2] = z - segSize;

				Texts[vertCounter][0] = 0.28f;
				Texts[vertCounter][1] = 0;
				Verts[vertCounter][0] = x;			//0
				Verts[vertCounter][1] = y;
				Verts[vertCounter++][2] = z;

				Texts[vertCounter][0] = 0.28f;
				Texts[vertCounter][1] = 0.45f;
				Verts[vertCounter][0] = x;			//1
				Verts[vertCounter][1] = y + segSize;
				Verts[vertCounter++][2] = z;

				Texts[vertCounter][0] = 0;
				Texts[vertCounter][1] = 0.45f;
				Verts[vertCounter][0] = x;			//7
				Verts[vertCounter][1] = y + segSize;
				Verts[vertCounter++][2] = z - segSize;

				//������� �����
				Norms[vertCounter][0] = 0;
				Norms[vertCounter][1] = 1.0f;
				Norms[vertCounter][2] = 0;
				for(int i = 1; i < 4; i++)
				{
					memcpy(Norms[vertCounter], Norms[vertCounter+i], sizeof(GLfloat)*3);
				}

				Texts[vertCounter][0] = 0.34f;
				Texts[vertCounter][1] = 0.55f;
				Verts[vertCounter][0] = x;			//1
				Verts[vertCounter][1] = y + segSize;
				Verts[vertCounter++][2] = z;

				Texts[vertCounter][0] = 0.66f;
				Texts[vertCounter][1] = 0.55f;
				Verts[vertCounter][0] = x + segSize;	//2
				Verts[vertCounter][1] = y + segSize;
				Verts[vertCounter++][2] = z;

				Texts[vertCounter][0] = 0.66f;
				Texts[vertCounter][1] = 1.0f;
				Verts[vertCounter][0] = x + segSize;	//6
				Verts[vertCounter][1] = y + segSize;
				Verts[vertCounter++][2] = z - segSize;

				Texts[vertCounter][0] = 0.34f;
				Texts[vertCounter][1] = 1.0f;
				Verts[vertCounter][0] = x;			//7
				Verts[vertCounter][1] = y + segSize;
				Verts[vertCounter++][2] = z - segSize;

				//������ �����
				Norms[vertCounter][0] = 0;
				Norms[vertCounter][1] = -1.0f;
				Norms[vertCounter][2] = 0;
				for(int i = 1; i < 4; i++)
				{
					memcpy(Norms[vertCounter], Norms[vertCounter+i], sizeof(GLfloat)*3);
				}

				Texts[vertCounter][0] = 0.28f;
				Texts[vertCounter][1] = 1.0f;
				Verts[vertCounter][0] = x + segSize;	//3
				Verts[vertCounter][1] = y;
				Verts[vertCounter++][2] = z;

				Texts[vertCounter][0] = 0;
				Texts[vertCounter][1] = 1.0f;
				Verts[vertCounter][0] = x;			//0
				Verts[vertCounter][1] = y;
				Verts[vertCounter++][2] = z;

				Texts[vertCounter][0] = 0;
				Texts[vertCounter][1] = 0.55f;
				Verts[vertCounter][0] = x;			//4
				Verts[vertCounter][1] = y;
				Verts[vertCounter++][2] = z - segSize;

				Texts[vertCounter][0] = 0.28f;
				Texts[vertCounter][1] = 0.55f;
				Verts[vertCounter][0] = x + segSize;	//5
				Verts[vertCounter][1] = y;
				Verts[vertCounter++][2] = z - segSize;	
			}
}

/*������ ������� � ������� �� ������� ���, ����� ����� ��� � ��������� ���������*/
void InitLittleQube(GLTVector3 *firstVert, GLTVector3 *firstNorm, GLfloat side)
{
	static const size_t GLTVector3Size = sizeof(GLTVector3);
	static const GLfloat h = side/2;
	static const GLTVector3 corners[8] ={	{-h, -h, h},
											{-h, h, h},
											{h, h, h},
											{h, -h, h},
											{-h, -h, -h},
											{h, -h, -h},
											{h, h, -h},
											{-h, h, -h}};
	static const GLTVector3 norms[6] = {{0, 0, 1.0f},
										{1.0f, 0, 0},
										{0, 0, -1.0f},
										{-1.0, 0, 0},
										{0, 1.0f, 0},
										{0, -1.0f, 0}};

	//��������
	memcpy(firstVert, corners, GLTVector3Size);
	memcpy(firstVert+1, corners+3, GLTVector3Size);
	memcpy(firstVert+2, corners+2, GLTVector3Size);
	memcpy(firstVert+3, corners+1, GLTVector3Size);
	//������
	memcpy(firstVert+4, corners+3, GLTVector3Size);
	memcpy(firstVert+5, corners+5, GLTVector3Size);
	memcpy(firstVert+6, corners+6, GLTVector3Size);
	memcpy(firstVert+7, corners+2, GLTVector3Size);
	//������
	memcpy(firstVert+8, corners+5, GLTVector3Size);
	memcpy(firstVert+9, corners+4, GLTVector3Size);
	memcpy(firstVert+10, corners+7, GLTVector3Size);
	memcpy(firstVert+11, corners+6, GLTVector3Size);
	//�����
	memcpy(firstVert+12, corners+4, GLTVector3Size);
	memcpy(firstVert+13, corners+0, GLTVector3Size);
	memcpy(firstVert+14, corners+1, GLTVector3Size);
	memcpy(firstVert+15, corners+7, GLTVector3Size);
	//�������
	memcpy(firstVert+16, corners+1, GLTVector3Size);
	memcpy(firstVert+17, corners+2, GLTVector3Size);
	memcpy(firstVert+18, corners+6, GLTVector3Size);
	memcpy(firstVert+19, corners+7, GLTVector3Size);
	//������
	memcpy(firstVert+20, corners+3, GLTVector3Size);
	memcpy(firstVert+21, corners, GLTVector3Size);
	memcpy(firstVert+22, corners+4, GLTVector3Size);
	memcpy(firstVert+23, corners+5, GLTVector3Size);
	/*� ������ �������*/
	for(register int i = 0; i < 6; i++)
	{
		memcpy(firstNorm + i*4, norms + i, GLTVector3Size);
		memcpy(firstNorm + i*4 + 1, norms + i, GLTVector3Size);
		memcpy(firstNorm + i*4 + 2, norms + i, GLTVector3Size);
		memcpy(firstNorm + i*4 + 3, norms + i, GLTVector3Size);
	}
}