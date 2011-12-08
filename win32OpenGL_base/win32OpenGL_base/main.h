#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <stdio.h>			//sprintf
#include "common\wglext.h"	//расширения WGL
#include "common\glext.h"	//расширеня OpenGL
#include "common\gltools.h"	//инструменты мужиков, написавших книжку.. Ну там сферы всякие
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //Обработчик окна
BOOL APIENTRY StartDlgProc(HWND, UINT, WPARAM, LPARAM); //Обработчик стартового диалога
void FindBestPF(HDC, GLint *Format, GLint *MSFormat); //Подбирает лучший пиксельный формат с мультисемплингом и без. Если что-то не нашел = ноль туда кладёт
BOOL ShowStartOpts(); //Показывает диалог с настройками
void ChangeSize(GLsizei w, GLsizei h); //Меняет проекцию и поле просмотра
void RenderSciene(); //Рисует всю сцену
void Setup(HDC); //Настраивает контекст визуализации
void Shutdown(); //Освобождает контекст визуализации
void DrawObjs(bool updateLocation); //Рисует сферы и тор. Если Shadow != 0, то рисует тени
void DrawGround();//Рисует землю

struct AnQube		//Описывает анимацию куба из вращающихся кубиков)
{
	GLfloat side;	//Размер свтороны большого куба
	GLint segs;		//Число маленьких кубиков на сторону
	GLfloat period;	//Период начала анимации в секундах
	GLfloat shift;	//Смещение между началом анимации следующих кубиков в сеекундах
	GLTVector3 axis;//Вектор, вокруг которого будет происходить вращение маленьких кубиков
	GLfloat speed;	//Скорость вращения маленьких кубиков вокруг заданного вектора по часовой стрелке
	GLfloat fullTurn;//Угол, после поворота на который вращение останавливается
};

void RenderQube(AnQube*, bool updateRotation);//Обновляет структуру куба и отображает его
void InitAnQubeArrays(AnQube *qb, GLfloat Verts[][3], GLfloat Texts[][2], GLfloat Norms[][3]); //Инициализирует массивы начальным положением "собранного" куба
void InitLittleQube(GLTVector3 *firstVert, GLTVector3 *firstNorm, GLfloat side); //Размещает вершины маленького кубика в центром в начале координат

void SetupShaderEnv();//Получает адреса необходимых функйций. Запускать до установки шейдеров!
GLhandleARB InstallShaider(const char *vertexPath, const char *fragmentPath);//Загружает код из файлов и создаёт полноценный програмный объект

/*Методы рендеринга*/
#define RM_glBeginGlEnd 1
#define RM_viewTable	2
#define RM_vertexArray	3
/*Методы освещения*/
#define LM_Oldschool	1
#define LM_Vertex		2
#define	LM_Fragment		3
#define LM_Clock		4