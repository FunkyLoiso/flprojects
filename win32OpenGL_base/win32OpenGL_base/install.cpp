#include "main.h"

//PFNGLPROGRAMSTRINGARBPROC glProgramStringARB;
//PFNGLBINDPROGRAMARBPROC glBindProgramARB;
//PFNGLDELETEPROGRAMSARBPROC glDeleteProgramsARB;
//PFNGLGENPROGRAMSARBPROC glGenProgramsARB;
//PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB;
//PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB;
//PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
//PFNGLGETHANDLEARBPROC glGetHandleARB;
//PFNGLDETACHOBJECTARBPROC glDetachObjectARB;
PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
//PFNGLVALIDATEPROGRAMARBPROC glValidateProgramARB;
//PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
//PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
//PFNGLUNIFORM1FARBPROC glUniform1fARB;
//PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
//PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
//PFNGLACTIVETEXTUREPROC glActiveTexture;

/*������������� ��������� �� ������� ���������� � ��������� �� �����������*/
void SetupShaderEnv()
{
	glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress("glCreateShaderObjectARB");
	glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSourceARB");
	glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShaderARB");
	glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress("glGetObjectParameterivARB");
	glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress("glCreateProgramObjectARB");
	glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)wglGetProcAddress("glAttachObjectARB");
	glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgramARB");
}
/*��������� ���� � ������ �������� � ���������� ��������� �� ����*/
GLubyte *LoadShaderText(const char *fileName)
{
    GLubyte *shaderText = NULL;
    GLint shaderLength = 0;
    FILE *fp;

    fp = fopen(fileName, "r");
    if (fp != NULL)
    {
        while (fgetc(fp) != EOF)
        {
            shaderLength++;
        }
        rewind(fp);
        shaderText = (GLubyte *)malloc(shaderLength+1);
        if (shaderText != NULL)
        {
            fread(shaderText, 1, shaderLength, fp);
        }
        shaderText[shaderLength] = '\0';
        fclose(fp);
    }

    return shaderText;
}
/*������������� ������*/
GLhandleARB InstallShaider(const char *vertexPath, const char *fragmentPath)
{
	GLhandleARB VS = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	GLhandleARB FS = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	
	const GLcharARB *buff[1];//�������� �������� ���
	buff[0] = (GLcharARB*)LoadShaderText(vertexPath);
	glShaderSourceARB(VS, 1, buff, NULL);
	buff[0] = (GLcharARB*)LoadShaderText(fragmentPath);
	glShaderSourceARB(FS, 1, buff, NULL);
	
	glCompileShaderARB(VS);//������������
	glCompileShaderARB(FS);
	GLint flag;
	glGetObjectParameterivARB(VS, GL_OBJECT_COMPILE_STATUS_ARB, &flag);//������� ��������� ����������
	if(!flag)
	{
		int err = glGetError();
		while(0)
		{
		}
	}
	glGetObjectParameterivARB(FS, GL_OBJECT_COMPILE_STATUS_ARB, &flag);
	if(!flag)
	{
		int err = glGetError();
		while(0)
		{
		}
	}

	GLhandleARB prog = glCreateProgramObjectARB();

	glAttachObjectARB(prog, VS);//����������� ������� � ������������ �������
	glAttachObjectARB(prog, FS);

	glLinkProgramARB(prog);//���������� ����������� ������
	glGetObjectParameterivARB(prog, GL_OBJECT_LINK_STATUS_ARB, &flag);//�������, ���� ����������
	if(!flag)
	{
		int err = glGetError();
		while(0)
		{
		}
	}

	return prog;
}