#include "common/GLTools.h"

/*���������� ����� ��������:
 ___ ___ ___
|btm|top|bck|
|___|___|___|
|lft|frt|rgt|
|___|___|___|

*/

void gltDrawCube(GLfloat side)
{
	GLfloat h = side / 2;
	glBegin(GL_QUADS);
	{
		//�������� �����
		glNormal3f(0, 0, 1.0f);
		glTexCoord2f(0.34f, 0);
		glVertex3f(-h, -h, h);
		glTexCoord2f(0.66f, 0);
		glVertex3f(h, -h, h);
		glTexCoord2f(0.66f, 0.45f);
		glVertex3f(h, h, h);
		glTexCoord2f(0.34f, 0.45f);
		glVertex3f(-h, h, h);
		
		
		//������ �����
		glNormal3f(1.0f, 0, 0);
		glTexCoord2f(0.72f, 0);
		glVertex3f(h, -h, h);
		glTexCoord2f(1.0f, 0);
		glVertex3f(h, -h, -h);
		glTexCoord2f(1.0f, 0.45f);
		glVertex3f(h, h, -h);
		glTexCoord2f(0.72f, 0.45f);
		glVertex3f(h, h, h);
		
		
		//������ �����
		glNormal3f(0, 0, -1.0f);
		glTexCoord2f(0.72f, 0.55f);
		glVertex3f(h, -h, -h);
		glTexCoord2f(1.0f, 0.55f);
		glVertex3f(-h, -h, -h);
		glTexCoord2f(1, 1);
		glVertex3f(-h, h, -h);
		glTexCoord2f(0.72f, 1.0f);
		glVertex3f(h, h, -h);
		
		
		//����� �����
		glNormal3f(-1.0f, 0, 0);
		glTexCoord2f(0, 0);
		glVertex3f(-h, -h, -h);
		glTexCoord2f(0.28f, 0);
		glVertex3f(-h, -h, h);
		glTexCoord2f(0.28f, 0.45f);
		glVertex3f(-h, h, h);
		glTexCoord2f(0, 0.45f);
		glVertex3f(-h, h, -h);
		
		
		//������� �����
		glNormal3f(0, 1.0f, 0);
		glTexCoord2f(0.34f, 0.55f);
		glVertex3f(-h, h, h);
		glTexCoord2f(0.66f, 0.55f);
		glVertex3f(h, h, h);
		glTexCoord2f(0.66f, 1.0f);
		glVertex3f(h, h, -h);
		glTexCoord2f(0.34f, 1.0f);
		glVertex3f(-h, h, -h);
		
		
		//������ �����
		glNormal3f(0, -1.0f, 0);
		glTexCoord2f(0, 0.55f);
		glVertex3f(-h, -h, -h);
		glTexCoord2f(0.28f, 0.55f);
		glVertex3f(h, -h, -h);
		glTexCoord2f(0.28f, 1);
		glVertex3f(h, -h, h);
		glTexCoord2f(0, 1.0f);
		glVertex3f(-h, -h, h);
		
		
	}
	glEnd();
}