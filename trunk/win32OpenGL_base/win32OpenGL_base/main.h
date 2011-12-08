#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <stdio.h>			//sprintf
#include "common\wglext.h"	//���������� WGL
#include "common\glext.h"	//��������� OpenGL
#include "common\gltools.h"	//����������� �������, ���������� ������.. �� ��� ����� ������
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //���������� ����
BOOL APIENTRY StartDlgProc(HWND, UINT, WPARAM, LPARAM); //���������� ���������� �������
void FindBestPF(HDC, GLint *Format, GLint *MSFormat); //��������� ������ ���������� ������ � ���������������� � ���. ���� ���-�� �� ����� = ���� ���� �����
BOOL ShowStartOpts(); //���������� ������ � �����������
void ChangeSize(GLsizei w, GLsizei h); //������ �������� � ���� ���������
void RenderSciene(); //������ ��� �����
void Setup(HDC); //����������� �������� ������������
void Shutdown(); //����������� �������� ������������
void DrawObjs(bool updateLocation); //������ ����� � ���. ���� Shadow != 0, �� ������ ����
void DrawGround();//������ �����

struct AnQube		//��������� �������� ���� �� ����������� �������)
{
	GLfloat side;	//������ �������� �������� ����
	GLint segs;		//����� ��������� ������� �� �������
	GLfloat period;	//������ ������ �������� � ��������
	GLfloat shift;	//�������� ����� ������� �������� ��������� ������� � ���������
	GLTVector3 axis;//������, ������ �������� ����� ����������� �������� ��������� �������
	GLfloat speed;	//�������� �������� ��������� ������� ������ ��������� ������� �� ������� �������
	GLfloat fullTurn;//����, ����� �������� �� ������� �������� ���������������
};

void RenderQube(AnQube*, bool updateRotation);//��������� ��������� ���� � ���������� ���
void InitAnQubeArrays(AnQube *qb, GLfloat Verts[][3], GLfloat Texts[][2], GLfloat Norms[][3]); //�������������� ������� ��������� ���������� "����������" ����
void InitLittleQube(GLTVector3 *firstVert, GLTVector3 *firstNorm, GLfloat side); //��������� ������� ���������� ������ � ������� � ������ ���������

void SetupShaderEnv();//�������� ������ ����������� ��������. ��������� �� ��������� ��������!
GLhandleARB InstallShaider(const char *vertexPath, const char *fragmentPath);//��������� ��� �� ������ � ������ ����������� ���������� ������

/*������ ����������*/
#define RM_glBeginGlEnd 1
#define RM_viewTable	2
#define RM_vertexArray	3
/*������ ���������*/
#define LM_Oldschool	1
#define LM_Vertex		2
#define	LM_Fragment		3
#define LM_Clock		4