#include "main.h"			//Объявления функций
//#include "Maths\MATRIX4X4.h"
#include "Maths/Maths.h"

struct Options //Структура, содержащая опции визуализации
{
	DEVMODE Mode;//Режим отображения
	GLint PixelFormat;//Пиксельный формат
	GLint PixelFormatMS;//Пиксельный формат с мультисемплингом
	GLboolean FullScreen;//Полный экран?
	GLboolean FSAA;//Полноэкранное сглаживание?
	GLboolean VSync;//Вертикальная синхронизация?
	GLint TexFilter;//Какой фильтр текстур использовать
	GLfloat AnisotropyLevel;//Уровень анизотропии 
	GLint RenderingMethod;//Метод рендеринга
	GLboolean WF;//Сетка
	GLint LightingMethod;//Тип освещения
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

#pragma region Глобальные переменные
//////////////////////////////////////////////////////////////////////////////////////////////////
static HINSTANCE GlobalHInstance = NULL;		//хендл приложения
static LPCTSTR AppName = "win32 meets OpenGL";	//имя приложения
static GLint FontList;							//Таблица отображения для шрифта
static Options Opts;							//Опции
static LARGE_INTEGER CounterFreq;				//частота увеличения счётчика производительности
static LARGE_INTEGER FPSCount;					//тут хранится значение счётчика производительности, пока следующая сотня кадров не менёт
static LARGE_INTEGER FrameTimer;				//значение счётчика производительности при визуализации прошлого кадра, для расчёта "физики"
static POINTS lastMousePoint;					//положение мыши в прошлый раз

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

#pragma region свойства сцены
////////////////////////////////////////////////////////////////////////////////////////
#define N_SPHERES 1			//число сфер
#define FOV 60					//Угол обзора камеры
GLTFrame SphFrames[N_SPHERES];	//массив положений сфер
GLTFrame CamFrame;				//положение камеры
GLTFrame LightFrame;
AnQube qb;						//Кубик))
/*Свет и материалы*/
GLfloat LightPos[4] = {-10.0f, 10.0f, 5.0f};	//положение источника света
GLfloat NoLight[4] = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat LowLight[4] = {0.2f, 0.2f, 0.2f, 0.2f};
GLfloat BrightLight[4] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat SpecSharpness = 16.0f;			//Размер блика для шейдера
GLTMatrix ShadowMatrix;					//матрица тени (одномерный массив GLfloat[16])
#define	GROUND_TEX	0
#define	TORUS_TEX	1
#define	SPHERE_TEX	2
#define QB_TEX		3
#define	N_TEXTURES	4
GLuint TexObjs[N_TEXTURES];
const char* TexFiles[] = {"grass.tga", "wood.tga", "orb.tga", "Qb_2.tga"};

GLuint TorusList, SphereList, GroundList, QbList;	//Таблицы отображения
////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion
/*Указатели на функции расширения*/
PFNWGLGETPIXELFORMATATTRIBIVARBPROC GetPixFormatARB = NULL;
PFNGLWINDOWPOS2IPROC WindowPos = NULL;
PFNWGLSWAPINTERVALEXTPROC SwapInterval = NULL;
/*Функции для работы с шейдерами*/
PFNGLUNIFORM1FARBPROC glUniform1fARB;
PFNGLUNIFORM1IARBPROC glUniform1iARB;
PFNGLUNIFORM3FARBPROC glUniform3fARB;
PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
PFNGLUNIFORM4FARBPROC glUniform4fARB;
PFNGLUNIFORM4FVARBPROC glUniform4fvARB;
PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
/*Мультитекстуры*/
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
	gluPerspective(140.0f, 1.0f, 0.1f, 40.0f);//должны быть видны все объекты
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
/*Вызываем, если изменился рназмер окна, для согласования с координатами окна*/
void ChangeSize(GLsizei w, GLsizei h)
{
	if(h == 0) h = 1;//Чтобы не было деления на ноль
	glViewport(0, 0, w, h);//Задаём участок окна, в котором будем визуализировать. Координаты от нижнего левого угла
	glMatrixMode(GL_PROJECTION);//Будет использоваться стек матриц проекций
	glLoadIdentity();//Загружаем в стек единичную матрицу
	gluPerspective(FOV, (GLfloat)w / (GLfloat)h, 0.1, 1024);//Создаёт матрицу, описывающую наблюдаемый объём (с перспективой) в глобальных координатах
	glMatrixMode(GL_MODELVIEW);//Сделаем стек наблюденя модели текущим и также запишем туда единичную матрицу
	glLoadIdentity();

	gluPerspective(FOV, (float)w/h, 0.1f, 1024.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);
	glLoadIdentity();

	//shadowMapSize = h;
	wndW = w;
	wndH = h;

}
/*Рисуем землю как квадрат*/
void DrawGround()
{
	/*Настроим текстуру, которая мозаикой распространится по земле..*/
	glBindTexture(GL_TEXTURE_2D, TexObjs[GROUND_TEX]);//Выбрали текущую двухмерную текстуру
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//Повторение текстуры по оси s (по пространственной x)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//Повторение по t (пространствнной y)

	glCallList(GroundList);//Вызываем список отображения с землёй
}

/*Рисуем объекты мира: статичные сферы, вращающиеся тор и сферу*/
void DrawObjs( bool updateLocation)
{
	const GLfloat rotSpeed = 30;//Скорость вращения тора и половина скорости вращения сферы (градусы в секунду)
	static GLfloat yRot = 0;	//Угол поворота для анимации. Статик, так что при последубщих вызовах сохранит значение, красота)
	if(updateLocation)
	{
		LARGE_INTEGER current;
		QueryPerformanceCounter(&current);
		GLfloat time = (GLfloat)(current.QuadPart - FrameTimer.QuadPart)/(GLfloat)CounterFreq.QuadPart;
		yRot += rotSpeed * time;
		glColor4f(1, 1, 1, 1);
	}
	//else glColor4f(0, 0, 0, 0.75);//Цвет тени - на четверть прозрачный чёрный
	/*рисуем случайным образом расположенные сферы*/

	//оси
	//gltDrawUnitAxes();
	//Нарисуем Кубик
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
		gltApplyActorTransform(&SphFrames[i]);//Передвигаемся в место, заданное актёром SpheresList[i]
		glCallList(SphereList);//Исполняет прекомпиленную таблицу отображения
		glPopMatrix();
	}
	/*рисуем подвижную сферу и тор*/
	glPushMatrix();
	{
		glTranslatef(0, 0.1f, -2.5f);
		glPushMatrix();
		{
			glRotatef(-yRot * 2, 0, 1, 0);//Сфера крутится вокруг вертикальной оси в два раза быстрее тора
			glTranslatef(1, 0, 0);//и находится от него на расстоянии 1
			glCallList(SphereList);//рисуем "подвижную" сферу
		}
		glPopMatrix();
		glMaterialfv(GL_FRONT, GL_SPECULAR, BrightLight);//тор - зеркальный
		glRotatef(yRot, 0, 1, 0);//вокруг вертикальной оси (y)
		glBindTexture(GL_TEXTURE_2D, TexObjs[TORUS_TEX]);//Сделаем текстуру для тора текущей двухмерной текстурой
		glCallList(TorusList);//Выполним прекомпилённую таблицу отображения тора
		glMaterialfv(GL_FRONT, GL_SPECULAR, NoLight);//Теперь материал блестит бесцветным цветом, т.е. не блестит)
	}
	glPopMatrix();
}

/*вызывается для каждого кадра*/
void RenderSciene()
{
	static GLint Frames = 0;//считает кадры до ста, чтобы мы высчитали fps
	static GLfloat fps = 0;	//fps
	char buffer[64];		//буфер для fps
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);//Очищаем все используемые бцферы: цвета, глубины и трафарета
	//glDisable(GL_TEXTURE_2D);//Запретим текстурирование, чтобы пронаблюдать освещение
	/*Время для шейдера*/
	SYSTEMTIME time;
	GetLocalTime(&time);
	glUniform1fARB(Uniforms.Time, time.wHour*3600.0 + time.wMinute*60.0 + time.wSecond);
	glPushMatrix();
	//{
		//gltApplyCameraTransform(&CamFrame);	//Перекоряим матрицу так, чтобы наблюдатель был там, где актёр CamFrame.
	//										//Видимо, действует обратно gltApplyActorTransform
		glLightfv(GL_LIGHT0, GL_POSITION, LightPos);//Помещаем источник света
		glColor3f(1.0f, 1.0f, 1.0f);
	//	DrawGround();//рисуем землю
	//	/*рисуем тени*/
	//	if(Opts.LightingMethod == LM_Oldschool)
	//	{
	//		glDisable(GL_DEPTH_TEST);//выключаем	проверку глубины
	//		glDisable(GL_LIGHTING);//				освещение
	//		glDisable(GL_TEXTURE_2D);//				двухмерные текстуры
	//		glEnable(GL_BLEND);		//включаем		смешивание накладывающихся цветов
	//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//Задаём функцию, по которой будут смешиваться цвета. Умножаем поступающий цвет
	//		//по первой формуле, который уже есть - по второй и складываем результаты
	//		glEnable(GL_STENCIL_TEST);//			проверку трафарета
	//		glPushMatrix();
	//		{
	//			glMultMatrixf(ShadowMatrix);//	Умножаем на матрицу тени, теперь все объекты проецируются на "землю"
	//			DrawObjs(true);
	//		}
	//		glPopMatrix();
	//		glDisable(GL_STENCIL_TEST);	//выключаем трафарет
	//		glDisable(GL_BLEND);		//			смешивание
	//		glEnable(GL_LIGHTING);		//включаем	освещение
	//		glEnable(GL_TEXTURE_2D);	//			двухмерные текстуры
	//		glEnable(GL_DEPTH_TEST);	//			проверку глубины
	//	}

	//	DrawObjs(false);			//"нормально" рисуем объекты
	//}
	DoMagic();
	glPopMatrix();
	
	/*рассчёт частоты обновления кадров (производим каждый сотый кадр)*/
	Frames++;
	if(Frames == FPS_P)
	{
		GLfloat Time;			//Время сотни кадров
		LARGE_INTEGER Current;	//текущее значение счётчика производительности
		QueryPerformanceCounter(&Current);//получаем теущее значение счётчика производительности
		Time = (GLfloat)(Current.QuadPart - FPSCount.QuadPart)/(GLfloat)CounterFreq.QuadPart;	//считаем время сотни кадров как разницу показаний
		//счётчика производительности, делённое на частоту его увеличения
		fps = FPS_P / Time;
		Frames = 0;//обнулем счётчик кадров
		QueryPerformanceCounter(&FPSCount);//Сохраняем текущее значение счётчика производительности для следующего рассчёта
	}
	/*Если доступно расширение положения окна, отображаем частоту смены кадров, и сообщаем, если была активирован мультисемплинг и VSync*/
	if(WindowPos != NULL)
	{
		int Row = 10;			//координата текста по x от низа окна
		glDisable(GL_DEPTH_TEST);//отключаем	проверку по глубине
		glDisable(GL_LIGHTING);//				освещение
		glDisable(GL_TEXTURE_2D);//				двухмерные текстуры
		glUseProgramObjectARB(NULL);//Отключаем шейдер для текста
		glColor3f(0.0f, 0.8f, 0.0f);		//цвет текста
		/*устанавливаем положение и отображаем сообщение*/
		WindowPos(0, Row);		//Переехали на нижнюю строчку
		glListBase(FontList);//---------------------------------------------------------------------------------------!!!!!!!!!!
		glCallLists(strlen("Душистый майоран:)") + 1, GL_UNSIGNED_BYTE, "Душистый майоран:)");
		Row += 15;//поднимемся на следующую строчку
		if(Opts.FSAA && Opts.PixelFormatMS != 0)
		{
			//если использовалась множественная выборка
			WindowPos(0, Row);
			glCallLists(strlen("MultiSampling") + 1, GL_UNSIGNED_BYTE, "MultiSampling");
			Row += 15;
		}
		if(SwapInterval != NULL && Opts.VSync)
		{
			//если использовалась вертикальная синхронизация
			WindowPos(0, Row);
			glCallLists(strlen("VSync on") + 1, GL_UNSIGNED_BYTE, "VSync on");
			Row += 15;
		}
		/*Напишем режим фильтрации*/
		WindowPos(0, Row);
		switch(Opts.TexFilter)
		{
		case GL_NEAREST:
			glCallLists(strlen("(T)Текстурный фильтр: 1 NEAREST"), GL_UNSIGNED_BYTE, "(T)Текстурный фильтр: 1 NEAREST");
			break;
		case GL_LINEAR:
			glCallLists(strlen("(T)Текстурный фильтр: 2 LINEAR"), GL_UNSIGNED_BYTE, "(T)Текстурный фильтр: 2 LINEAR");
			break;
		case GL_NEAREST_MIPMAP_NEAREST:
			glCallLists(strlen("(T)Текстурный фильтр: 3 NEAREST_MIPMAP_NEAREST"), GL_UNSIGNED_BYTE, "(T)Текстурный фильтр: 3 NEAREST_MIPMAP_NEAREST");
			break;
		case GL_LINEAR_MIPMAP_NEAREST:	
			glCallLists(strlen("(T)Текстурный фильтр: 4 LINEAR_MIPMAP_NEAREST"), GL_UNSIGNED_BYTE, "(T)Текстурный фильтр: 4 LINEAR_MIPMAP_NEAREST");
			break;
		case GL_NEAREST_MIPMAP_LINEAR:	
			glCallLists(strlen("(T)Текстурный фильтр: 5 NEAREST_MIPMAP_LINEAR"), GL_UNSIGNED_BYTE, "(T)Текстурный фильтр: 5 NEAREST_MIPMAP_LINEAR");
			break;
		case GL_LINEAR_MIPMAP_LINEAR:	
			glCallLists(strlen("(T)Текстурный фильтр: 6 LINEAR_MIPMAP_LINEAR(трилинейная)"), GL_UNSIGNED_BYTE, "(T)Текстурный фильтр: 6 LINEAR_MIPMAP_LINEAR(трилинейная)");
			break;
		case GL_TEXTURE_MAX_ANISOTROPY_EXT: 
			glCallLists(strlen("(T)Текстурный фильтр: 7 MAX_ANISOTROPY"), GL_UNSIGNED_BYTE, "(T)Текстурный фильтр: 7 MAX_ANISOTROPY");
			break;
		}
		Row += 15;
		/*Укажем уровнь анизотропии*/
		WindowPos(0, Row);
		sprintf(buffer, "(F)Уровень анизотропии: %.f", Opts.AnisotropyLevel);
		glCallLists(strlen(buffer) + 1, GL_UNSIGNED_BYTE, buffer);
		Row += 15;
		/*Укажем метод освещения*/
		WindowPos(0, Row);
		switch(Opts.LightingMethod)
		{
		case LM_Oldschool:
			glCallLists(strlen("(L)Метод освещения: Стандартный конвейер"), GL_UNSIGNED_BYTE, "(L)Метод освещения: Стандартный конвейер");
			break;
		case LM_Vertex:
			glCallLists(strlen("(L)Метод освещения: Повершинный в шейдере"), GL_UNSIGNED_BYTE, "(L)Метод освещения: Повершинный в шейдере");
			break;
		case LM_Fragment:
			glCallLists(strlen("(L)Метод освещения: Пофрагментный в шейдере"), GL_UNSIGNED_BYTE, "(L)Метод освещения: Пофрагментный в шейдере");
			break;
		case LM_Clock:
			glCallLists(strlen("(L)Метод освещения: Пофрагментный в шейдере + процедурная текстура с часами"), GL_UNSIGNED_BYTE, "(L)Метод освещения: Пофрагментный в шейдере + процедурная текстура с часами");
			break;
		}
		Row += 15;
		/*Укажем тип рендеринга*/
		WindowPos(0, Row);
		sprintf(buffer, "(R)Тип рендеринга: ");
		switch(Opts.RenderingMethod)
		{
		case RM_glBeginGlEnd: strcat(buffer, "glBegin/glEnd"); break;
		case RM_viewTable: strcat(buffer, "Маленький кубик в таблице отображения"); break;
		case RM_vertexArray: strcat(buffer, "Весь куб в массивах, которые меняем ручками"); break;
		}
		glCallLists(strlen(buffer) + 1, GL_UNSIGNED_BYTE, buffer);
		Row += 15;
		/*fps изобразим*/
		sprintf_s(buffer, 64, "FPS: %.2f", fps);
		WindowPos(0, Row);
		glCallLists(strlen(buffer) + 1, GL_UNSIGNED_BYTE, buffer);
		/*включаем проверку глубины, освещение и текстуры обратно*/
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		switch(Opts.LightingMethod)//Включаем шейдер обратно
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

/*Настраиваем перед запуском графики. Создаём шрифты и их растры, загружаем текстуры, создаём таблицы отображения*/
void Setup(HDC hDC)
{
	//glShadeModel(GL_FLAT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLTVector3 ground[3] = {{0, -0.4f, 0},	//Три точки, задаюшие плоскость земли (для создания матрицы тени)
							{1, -0.4f, 0},
							{0, -0.4f, -1}};
	/*Куб мой крутящийся)*/
	qb.axis[0] = 1;
	qb.axis[1] = 0;	//Вокруг оси z
	qb.axis[2] = -5;
	qb.period = 5.0f;
	qb.segs = 10;
	qb.shift = 0.1f;
	qb.side = 5.0f;
	qb.speed = 120.0f;
	qb.fullTurn = 360.0f;
	/*настраиваем шрифт*/
	HFONT Font;					//Хендл шрифта
	LOGFONT lf;					//Свойства шрифта
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
	/*Создаём шрифт и таблицу отображения*/
	Font = CreateFontIndirect(&lf);//Инициализируем хендл шрифта структурой, которую тока что зафигарили
	SelectObject(hDC, Font);//Выбираем этот шрифт для нашего контекста
	FontList = glGenLists(256);//Создаём таблицы отображения для первых 128ми символов (глифов)
	wglUseFontBitmaps(hDC, 0, 256, FontList);	//Создаём битмэпы шрифтов. Из шрифта какого контекста, какой первый глиф,
	//скока глифов, в какую таблицу отображения
	DeleteObject(Font);//освобождаем шрифт
	glClearColor(LowLight[0], LowLight[1], LowLight[2], LowLight[3]);//Фон
	/*			Трафарет
	/*Значение в буфере будет увеличиваться на единичку, если в нем кто-то рисует.
	/*Если функция трафарета активизированна, то писать можно тольео туда, где значение трафарета равно нулю
	/*(т.е. туда, где ещё не писали). Это для того, чтобы тени не "складывались"*/
	//glStencilOp(GL_KEEP, GL_INCR, GL_INCR);	//Увеличить значение в буфере трафарета во всех трёх случаях:
	////если проверка трафарета не прошла(функцию проверки мы ещё зададим),
	////если трафарет прошли, но провалиласть проверка буфера глубины
	////если и ту и другую проверку прошли (или только трафарет + проверка глубины отключена)
	//glClearStencil(0);	//Значение, которым будет очищаться буфер трафарета
	//glStencilFunc(GL_EQUAL, 0x0, 0x1);	//Проверку трафарета пройдут только те пиксели, для которых значение в буфере, логисески сложеное с 0x01
	////будет равно 0x0&0x01 (не могу придумать, зачем эта маска, видимо, для многоступенчатого трафарета,
	////можно ведь некоторые единички в буфере "замаскировать" нулями, типа 0x11 & 0x01 = 0x01)
	//glCullFace(GL_BACK);//задние части многоугольников не визуализируются (например то, что можно увидеть, находясь внутри сферы..)
	//glFrontFace(GL_CCW);//Они делают передней стороной тот многоугольник, верщины которого были против часово стрелки указанны,
	////это не дефолтовое значение.. Зачем только) Ну, в смысле, они все сферы свои так создавали,
	////не по умолчанию, так что без этой строчки не увидим мы ничего, пока изнутри не взглянем)
	//glEnable(GL_CULL_FACE);//включаем	отсечение (чуть ранее мы задали отсечение задних граней)
	//glEnable(GL_DEPTH_TEST);//			проверку глубины
	///*Настроим освещение*/
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, NoLight);//Рассеянный свет (со всех сторон идёт)
	//glLightfv(GL_LIGHT0, GL_AMBIENT, LowLight);	//Настроим источник свера. Рассеянный компонент
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, BrightLight);//Диффузный компонент. Имеет направление но отражается произвольно. Яркость объекта зависит от угла падения
	//glLightfv(GL_LIGHT0, GL_SPECULAR, BrightLight);//Отраженный компонент. Отражается параллельным пучком. Порождает блики
	//glEnable(GL_LIGHTING);	//разрешим освещение
	//glEnable(GL_LIGHT0);	//и источник света, что настроили
	//gltMakeShadowMatrix(ground, LightPos, ShadowMatrix);//Создадим матрицу тени (матрицу проекции на землю.. Та тока на одну плоскость можно тень отбросить))
	//glEnable(GL_COLOR_MATERIAL);//Включаем согласование свойств материала с его цветом
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);	//Свойства (отражение рассеянного и диффузного света) материала передних граней многоугольников
	////автоматически задаются так, чтобы соответствовать цвету грани
	//glMateriali(GL_FRONT, GL_SHININESS, 128);	//Степень "блеска" граней, чем болше число [0, 128], тем меньше и ярче блику
	gltInitFrame(&CamFrame);//Инициализирует актёра - ставит в начало отсчёта, верх - y, вперёд - -z
	for(int i = 0; i < N_SPHERES; i++)//Случайно разместим сферы
	{
		gltInitFrame(&SphFrames[i]);//ставим сферу в начало, верх y, перёд -z
		SphFrames[i].vLocation[0] = (GLfloat)(((rand() % 400) - 200) * 0.1);//Случайно расставим сферы на поле 40x40 с шагом 0.1
		SphFrames[i].vLocation[1] = 0;//чуть выше пола они, ибо пол - -0.4 по игрику
		SphFrames[i].vLocation[2] = (GLfloat)(((rand() % 400) - 200) * 0.1);
	}
	gltInitFrame(&LightFrame);
	LightFrame.vLocation[0] = LightPos[0];
	LightFrame.vLocation[1] = LightPos[1];
	LightFrame.vLocation[2] = LightPos[2];

	glEnable(GL_TEXTURE_2D);//Разрешим двухмерные текстуры
	glGenTextures(N_TEXTURES, TexObjs);//Создаёт в массиве TexObjs N_TESTURES уникальных "имён" текстур, которые потом будем использовать
	for(int i = 0; i < N_TEXTURES; i++)//Загрузим текстуры
	{
		GLbyte* tex;//Указатель на память, куда загрузили текстуру
		GLint w, h, comps;//ширина, высота (ljk;ys ,snm cntgtym. ldjqrb) и число компонентов цвета (1 - яркостное, 3 - RGB, 4 - RGBA)
		GLenum format;//Формат упаковки пикселей
		glBindTexture(GL_TEXTURE_2D, TexObjs[i]);//Делаем текстуру (она пока пустая) теушей
		tex = gltLoadTGA(TexFiles[i], &w, &h, &comps, &format);//Загрущаем текстуру, получаем указатель на неё и параметры в переменные
		gluBuild2DMipmaps(GL_TEXTURE_2D, comps, w, h, format, GL_UNSIGNED_BYTE, tex);	//Делаем из картинки по адресу тестуру с мипмапингом. Она будет
		//текушей в указанном режиме текстурирования
		free(tex);//Сама картинка нам больше не нужна
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Opts.TexFilter);//Увеличение с линейной фильтрацией
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Opts.TexFilter);//Уменьшение с линейной фильтрацией уровней и между уровнями - трилинейная(а не билинейная..?)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//При фильтрации на стыках не используем пиксели, находящиеся за гранью текстуры
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//То же самое по другой оси
	}
	/*получаем указатели на функции расширения, если они существуют*/
	WindowPos = (PFNGLWINDOWPOS2IPROC)wglGetProcAddress("glWindowPos2i");//для расстановки шрифтов по окну
	SwapInterval = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");//для вертикальной синхронизации, как я понял
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
		if(Opts.VSync) SwapInterval(1);	//Включает ожидание синхросигнала
		else	SwapInterval(0);		//Или отключает его, если мы его не включили сами)
	}
	if(Opts.FSAA && Opts.PixelFormatMS != NULL)glEnable(GL_MULTISAMPLE_ARB);//Разрешаем мультисемплинг
	if(gltIsExtSupported("GL_EXT_separate_specular_color"))	//если доступно расширение дополнительного зеркального цвета (нужен для бликов на текстурах вроде)
	{														//то делаем тор с бликами
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	}
	QueryPerformanceFrequency(&CounterFreq);//Инициализируем всё для счётчика производительности
	QueryPerformanceCounter(&FPSCount);

	FrameTimer = FPSCount;	//Заносим текущее значение счётчика производительности в таймер камеры, чтобы потом посчитать, скока времени прошло с прошлой
							//визуализации кадра и на сколько за это время могла переехать камера
#pragma region Строим таблицы отображения
	TorusList = glGenLists(4);	//Создаём списки.
	SphereList = TorusList + 1;	//Имя второго - первый плюс один
	GroundList = TorusList + 2;
	QbList = TorusList + 3;
	glNewList(TorusList, GL_COMPILE);//Лист компилируется, но не исполняется
	{
		gltDrawTorus(0.35f, 0.15f, 60, 30);//-----------------------------------------------------------------------------настройка тора-----------------------
	}
	glEndList();
	glNewList(SphereList, GL_COMPILE);
	{
		gltDrawSphere(0.3f, 100, 100);//-----------------------------------------------------------------------------------настройка сфер-----------------------
	}
	glEndList();
	glNewList(GroundList, GL_COMPILE);
	{
		GLfloat side = 40;//длина стороны
		GLfloat texSide = 1;//Размер стороны одной "плитки"
		GLfloat y = -0.4f;//где по игрику лежит
		/*рисуем сам квадратик земли*/
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
#pragma region Настраиваем шейдеры
	SetupShaderEnv();
	/*Шейдер повершинного освещения*/
	VertexLighting = InstallShaider("VertexLighting/VS.cpp", "VertexLighting/FS.cpp");
	/*Шейдер попиксельного освещения*/
	FragmentLighting = InstallShaider("FragmentLighting/VS.cpp", "FragmentLighting/FS.cpp");
	/*Часы*/
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
/*освобождаем ресурсы*/
void Shutdown()
{
	/*удалёем таблицы отображения*/
	glDeleteLists(FontList, 256);
	glDeleteLists(TorusList, 3);//тор, сфела, земля
	glDeleteTextures(N_TEXTURES, TexObjs);//удаляем текстуры
}
/*Точка входа*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;						//Структура сообщениея
	WNDCLASS wc;					//Структура класса окна
	HWND hWnd;						//Хендл окна
	UINT Style, StyleX;				//Память для стилей окна
	//GlobalHInstance = hInstance;	//Сохраняем хендл приложения

	if(DialogBoxA(hInstance, MAKEINTRESOURCEA(DIALOG), NULL, StartDlgProc) == FALSE) return 0;//Если не получается диалог показать
	if(Opts.FullScreen)
	{
		if(ChangeDisplaySettingsA(&Opts.Mode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			MessageBoxA(NULL, "Не удалось перейти в полноэкранный режим с заданным разрешением", "полная жопа..", MB_OK | MB_ICONSTOP);
			return -1;
		}
	}
	/*регистрируем стиль окна*/
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	//Благодаря CS_OWNDC у окна всегда будет свой контекст устройства, не надо будет использовать
	//BeginPaint() - EndPaint() и бояться, что контексты вдруг закончатся и на мне дадут новый..)
	//Первые два параметра заставляют окно полностью перерисовываться, если оно поменялось по
	//вертикали и горизонтали
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;			//кисточка для фона конечно не нужна
	wc.lpszMenuName	= NULL;
	wc.lpszClassName = AppName;
	if(RegisterClassA(&wc) == NULL) return 0;//регистрируем класс окна
	if(Opts.FullScreen)//Выбираем стили окна
	{
		Style = WS_POPUP;//Без заголовка
		StyleX = WS_EX_TOPMOST;//Самое верхнее
	}
	else
	{
		Style = WS_OVERLAPPEDWINDOW;//обычное
		StyleX = 0;
	}
	/*Создаём окно, в которое будем выводить графику*/
	hWnd = CreateWindowExA(StyleX, wc.lpszClassName, AppName, Style, 0, 0, Opts.Mode.dmPelsWidth, Opts.Mode.dmPelsHeight, NULL, NULL, hInstance, NULL);
	if(hWnd == NULL) return 0;
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	while(GetMessageA(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
	/*закончилась программа, восстановим режим дмсплея*/
	if(Opts.FullScreen) ChangeDisplaySettingsA(NULL, 0);//Видимо, на дефолт скидывает
	return msg.wParam;
}

/*Обрабатываем все сообщения окна рисования*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HGLRC GLRC;	//Постоянный контекст визуализации для OpenGL
	static HDC hDC;		//Контекст устройства (GDI)

	switch(message)
	{
	case WM_CREATE:	//Создание окошка, настройка OpenGL
		{
			hDC = GetDC(hWnd);//Сохраняем контекст, предоставленный окну (мы ему личный в классе прописали, помните?))
			/*Экран, возможно, изменился, так что повторяем инициализацию пиксельного формата*/
			FindBestPF(hDC, &Opts.PixelFormat, &Opts.PixelFormatMS);
			if(Opts.FSAA && Opts.PixelFormatMS != NULL)
			{
				SetPixelFormat(hDC, Opts.PixelFormatMS, NULL);//Последний нуль - логическое описание формата, оно нужно только для логов, или чего-то такого
			}
			else
			{
				SetPixelFormat(hDC, Opts.PixelFormat, NULL);
			}
			GLRC = wglCreateContext(hDC);	//Создаём контекст визуализации ( RC - rendering context, нэ?) для нашего контекста устройства, хе-хе
			wglMakeCurrent(hDC, GLRC);		//И делаем его текущим для того же контекста устройства

			GLfloat maxAnisotropy;			//Установим максимальный уровень анизотропной фильтрации после того, как установили пиксельный формат
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			Opts.AnisotropyLevel = maxAnisotropy;

			Setup(hDC);	//Проведём первичную настройку окошка
		}
		break;
	case WM_KEYDOWN:	//Проверим нажатие клавишь
		switch(wParam)
		{
		case VK_ESCAPE: DestroyWindow(hWnd);
			break;

		case 0x56://V - вертикальная синхронизация
			{
				Opts.VSync = !Opts.VSync;
				SwapInterval(int(Opts.VSync));
			}
			break;
		case 0x54://T - режим фильтрации текстур
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
				for(int i = 0; i < N_TEXTURES; i++)//Текстуры придётся загрузить заново
				{
					GLbyte* tex;//Указатель на память, куда загрузили текстуру
					GLint w, h, comps;//ширина, высота (ljk;ys ,snm cntgtym. ldjqrb) и число компонентов цвета (1 - яркостное, 3 - RGB, 4 - RGBA)
					GLenum format;//Формат упаковки пикселей
					glBindTexture(GL_TEXTURE_2D, TexObjs[i]);//Делаем текстуру (она пока пустая) теушей
					tex = gltLoadTGA(TexFiles[i], &w, &h, &comps, &format);//Загрущаем текстуру, получаем указатель на неё и параметры в переменные
					gluBuild2DMipmaps(GL_TEXTURE_2D, comps, w, h, format, GL_UNSIGNED_BYTE, tex);	//Делаем из картинки по адресу тестуру с мипмапингом. Она будет
					//текушей в указанном режиме текстурирования
					free(tex);//Сама картинка нам больше не нужна
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Opts.TexFilter);//Увеличение с линейной фильтрацией
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Opts.TexFilter);//Уменьшение с линейной фильтрацией уровней и между уровнями - трилинейная(а не билинейная..?)
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//При фильтрации на стыках не используем пиксели, находящиеся за гранью текстуры
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//То же самое по другой оси

					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, Opts.AnisotropyLevel);//Зададим уровень анизотропии
				}
			}
			break;
		case 0x46://F - уровень анизотропной фильтрации
			{

				static GLint maxAnisotropy = 0;
				if(!maxAnisotropy) glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
				Opts.AnisotropyLevel < maxAnisotropy ? Opts.AnisotropyLevel++ : Opts.AnisotropyLevel = 1;
				for(int i = 0; i < N_TEXTURES; i++)//Текстуры придётся загрузить заново
				{
					GLbyte* tex;//Указатель на память, куда загрузили текстуру
					GLint w, h, comps;//ширина, высота (ljk;ys ,snm cntgtym. ldjqrb) и число компонентов цвета (1 - яркостное, 3 - RGB, 4 - RGBA)
					GLenum format;//Формат упаковки пикселей
					glBindTexture(GL_TEXTURE_2D, TexObjs[i]);//Делаем текстуру (она пока пустая) теушей
					tex = gltLoadTGA(TexFiles[i], &w, &h, &comps, &format);//Загрущаем текстуру, получаем указатель на неё и параметры в переменные
					gluBuild2DMipmaps(GL_TEXTURE_2D, comps, w, h, format, GL_UNSIGNED_BYTE, tex);	//Делаем из картинки по адресу тестуру с мипмапингом. Она будет
					//текушей в указанном режиме текстурирования
					free(tex);//Сама картинка нам больше не нужна
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Opts.TexFilter);//Увеличение с линейной фильтрацией
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Opts.TexFilter);//Уменьшение с линейной фильтрацией уровней и между уровнями - трилинейная(а не билинейная..?)
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//При фильтрации на стыках не используем пиксели, находящиеся за гранью текстуры
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//То же самое по другой оси

					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, Opts.AnisotropyLevel);//Зададим уровень анизотропии
				}
			}
			break;
		case 0x52://R - тип рендеринга
			if(Opts.RenderingMethod == 3) Opts.RenderingMethod = 1;
			else Opts.RenderingMethod++;
			break;
		case 0x50://P - сетка/заполнение
			Opts.WF ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			Opts.WF = !Opts.WF;
			break;
		case 0x4c://L - режим освещения
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
	case WM_ACTIVATE:	//Обрабатываем манипуляции с окном. Оно может быть или самым главным и полноэкранным (если фулскрин), или свёрнцтым совем
		{
			if(Opts.FullScreen)
			{
				/*Создаём структуру расположения окна*/
				WINDOWPLACEMENT wp;
				wp.length = sizeof(WINDOWPLACEMENT);
				wp.flags = WPF_RESTORETOMAXIMIZED;	//Развёрнутое
				wp.ptMaxPosition.x = 0;
				wp.ptMaxPosition.y = 0;
				wp.ptMinPosition.x = 0;
				wp.ptMinPosition.y = 0;
				wp.rcNormalPosition.bottom = Opts.Mode.dmPelsHeight;//С максимальными размерами для заданного режима дисплея
				wp.rcNormalPosition.left = 0;
				wp.rcNormalPosition.right = Opts.Mode.dmPelsWidth;
				wp.rcNormalPosition.top = 0;
				if(LOWORD(wParam) == WA_INACTIVE)//Если переключились от окна
				{
					wp.showCmd = SW_SHOWMINNOACTIVE;//Показываем в трее, фокус оставляем тому, на кого перешли
					SetWindowPlacement(hWnd, &wp);
					ShowCursor(TRUE);
				}
				else//Если обратно на окно переключились
				{
					wp.showCmd = SW_RESTORE;//Разворачиваемся, фокус отбираем себе
					SetWindowPlacement(hWnd, &wp);
					ShowCursor(FALSE);
				}
			}
		}
		break;
	case WM_DESTROY:	//Окно удалётся
		{
			Shutdown();//Освобождаем ресурсы
			wglMakeCurrent(hDC, NULL);	//Отменяем выбор контекста визуализации
			wglDeleteContext(GLRC);		//и удалёем его
			PostQuitMessage(0);			//Сообщаем приложению закрыться после закрытия окна
		}
		break;
	case WM_SIZE:	//Размер окна изменился
		ChangeSize(LOWORD(lParam), HIWORD(lParam));//Модифицируеи объём отсечения и поле просмотра
		break;
	case WM_PAINT:	//Окно перерисовывается
		{
			LARGE_INTEGER Current;
			/*Опросим клавиатуру*/
			if(GetFocus() == hWnd)//если наше окно в фокусе, то опросим клавиатуру
			{
				GLfloat Time, Linear;
				/*получим время, прошедшее с последней визуализации кадра*/
				QueryPerformanceCounter(&Current);
				Time = (GLfloat)(Current.QuadPart - FrameTimer.QuadPart) / (GLfloat)CounterFreq.QuadPart;
				/*движение камеры управляется по времени. Мы задяём линейную и считаем время меджукадрами, всё просто)
				/*Скорость в координатах в секунду*/
				const GLfloat linSpeed = 5.0f;

				Linear = Time * linSpeed;
				/*Опросим клавиатуру и если какие-то кнопки нажаты - передвинем/повернём*/
				if(GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57)) gltMoveFrameForward(&CamFrame, Linear);
				if(GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x53)) gltMoveFrameForward(&CamFrame, -Linear);
				if(GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41)) gltMoveFrameRight(&CamFrame, Linear);
				if(GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44)) gltMoveFrameRight(&CamFrame,-Linear);
				if(GetAsyncKeyState(VK_SPACE)) gltTranslateFrameWorld(&CamFrame, 0, Linear, 0);
				if(GetAsyncKeyState(VK_SHIFT)) gltTranslateFrameWorld(&CamFrame, 0, -Linear, 0);
			}
#pragma region Обработаем движение мышки
			//////////////////////////////////////////////////////////////////////////////////////////////////
			const GLfloat MouseSpeed = 0.002f;//Градуса на пиксель смещения)

			static RECT wndRect;
			if(wndRect.left == 0) GetWindowRect(hWnd, &wndRect);//Получим текушие размеры окна
			static int cx = (wndRect.right + wndRect.left) / 2;//Получим центр окна
			static int cy = (wndRect.top + wndRect.bottom) / 2;
			POINT cur;		//Текущее положение курсора
			GetCursorPos(&cur);//Получим то самое текущее положение курсора

			GLfloat xAngle = (cx - cur.x) * MouseSpeed;
			GLfloat yAngle = (cy - cur.y) * MouseSpeed;
			gltRotateFrameLocalX(&CamFrame, -yAngle);	//Покрутим вокруг x
			gltRotateFrameWorldY(&CamFrame, xAngle);	//и вокруг у

			SetCursorPos(cx, cy);
			//////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion
			RenderSciene();		//Отрисуем сцену
			SwapBuffers(hDC);	//переключаем буферы
			FrameTimer = Current;//А время начла только что отрендеренного кадра запишем в FrameTimer
			//ValidateRect(hWnd, NULL);//Если не подтвержать правильность, WM_PAINT будет вызываться снова и снова до побагровения..
		}
		break;
	default: return DefWindowProcA(hWnd, message, wParam, lParam);
	}
	return(0L);//...oO
}
/*Обрабатывает диалог при запуске*/
BOOL APIENTRY StartDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		{
			GLint PF;		//Пиксельный формат
			HDC hDC;		//Контекст диалогового окна
			HGLRC GLRC;		//Контекст визуализации OpenGL
			DEVMODE Mode;	//Режим работы дисплея
			GLuint curMode;
			GLuint curW;	//текущие установки
			GLuint curH;
			char Buffer[64];//Буфер для строк, угадал?
			HWND ComboBox;	//Хендл списка (чем не окно?)
			PIXELFORMATDESCRIPTOR pfd = {
				sizeof(PIXELFORMATDESCRIPTOR),
				1,			//Версия
				PFD_DRAW_TO_WINDOW |	// поддерживаеи запись в окно
				PFD_SUPPORT_OPENGL |	// поддерживает OpenGL
				PFD_DOUBLEBUFFER,		// формат с двойной буферизацией
				PFD_TYPE_RGBA,			// RGBA 
				32,						// Глубина цвета
				0, 0, 0, 0, 0, 0,		// Не используются для выборв режима
				0, 0, 0, 0, 0,
				16,						// Буфер глубины
				8,						// Буфер трафарета
				0, 0, 0, 0, 0, 0};		//Какие-то дополнительные буферы, какие-то перекрывающиеся плоскости, что-то устаревшее.. Нам не надо это
				/*инициализация опций визуализации*/
				Opts.FSAA = false;
				Opts.FullScreen = false;
				Opts.VSync = false;
				Opts.TexFilter = GL_LINEAR_MIPMAP_LINEAR;
				Opts.RenderingMethod = RM_glBeginGlEnd;
				Opts.WF = false;
				Opts.LightingMethod = LM_Oldschool;
				/*создадим "временный" контекст визуализации, чтобы искать пиксельные форматы*/
				hDC = GetDC(hDlg);
				PF = ChoosePixelFormat(hDC, &pfd);	//Ищеим наиболее похожий на тот, что описан в структуре
				SetPixelFormat(hDC, PF, &pfd);		//Делаем его текущим для нашего диалого
				DescribePixelFormat(hDC, PF, sizeof(PIXELFORMATDESCRIPTOR), &pfd);//Сохраняем информацию о найденном в той же структуре
				GLRC = wglCreateContext(hDC);	//Создаём контекст визуализации
				wglMakeCurrent(hDC, GLRC);		//и делаемего текущим
				SetDlgItemTextA(hDlg, L_VENDOR, (LPCSTR)glGetString(GL_VENDOR));//Установим строчки в диалоге
				SetDlgItemTextA(hDlg, L_CARD, (LPCSTR)glGetString(GL_RENDERER));
				SetDlgItemTextA(hDlg, L_VER, (LPCSTR)glGetString(GL_VERSION));
				FindBestPF(hDC, &Opts.PixelFormat, &Opts.PixelFormatMS);//Находим пиксельные форматы с мультисемплингом и без него
				wglMakeCurrent(hDC, NULL);	//Отменяем контекст визуализации
				wglDeleteContext(GLRC);		//и удаляем его
				/*перечисление режимов отображения в комбобоксе*/
				curMode = 0;
				curW = GetSystemMetrics(SM_CXSCREEN);//Текущий режим
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
					item = SendMessageA(ComboBox, CB_ADDSTRING, 0, (LPARAM)Buffer);//Добавим составленную сторчку
					SendMessageA(ComboBox, CB_SETITEMDATA, item, curMode);//И присвоеим этой строчке данные ввиде номера режима
					if(Mode.dmPelsHeight == curH && Mode.dmPelsWidth == curW)
						SendMessageA(ComboBox, CB_SETCURSEL, item, 0);
					curMode++;
				}
				/*установим умолчания интерфейса*/
				CheckDlgButton(hDlg, CHB_FS, BST_CHECKED);//полноэкранный режим
				if(Opts.PixelFormatMS != NULL)//Антиальязинг доступен только если доступен
					EnableWindow(GetDlgItem(hDlg, CHB_MS), TRUE);
				else
					EnableWindow(GetDlgItem(hDlg, CHB_MS), FALSE);
				return(TRUE);//Говорит винмейну, что всё хорошо
		}
		break;
	case WM_COMMAND:	//Сообщения кнопок
		{
			if(LOWORD(wParam) == B_START)
			{
				/*Установим выбранный режим отображения*/
				HWND ComboBox = GetDlgItem(hDlg, CB_RES);
				GLint mode = SendMessageA(ComboBox, CB_GETCURSEL, 0, 0);//сначала получим номер текущей строчки
				mode = SendMessageA(ComboBox, CB_GETITEMDATA, mode, 0);	//а теперь - значение, соотнесённое с этой строкой
				EnumDisplaySettingsA(NULL, mode, &Opts.Mode);//сохраняет в опции струкруру, описывающую режим дисплея под выбраным номером
				if(IsDlgButtonChecked(hDlg, CHB_FS))	Opts.FullScreen = true;	//Полный экран
				else									Opts.FullScreen = false;
				if(IsDlgButtonChecked(hDlg, CHB_MS))	Opts.FSAA = true;		//FSAA
				else									Opts.FSAA = false;
				if(IsDlgButtonChecked(hDlg, CHB_VS))	Opts.VSync = true;		//VSync
				else									Opts.VSync = false;
				EndDialog(hDlg, TRUE);//Уничтожает диалог и возвращат штуку
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
/*Выбор пиксельного формата с желаемыми атрибутами, сохраняет в параметрах самый подходящий формат с мультисемплингом (ноль, если нету) и без*/
void FindBestPF(HDC hDC, GLint *Format, GLint *MSFormat)
{
	*Format = 0;
	*MSFormat = 0;
	/*есть ли расширение для поиска пиксельного формата по аттрибутам?*/
	if(gltIsWGLExtSupported(hDC, "WGL_ARB_pixel_format"))
		if(GetPixFormatARB == NULL) GetPixFormatARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");
	if(GetPixFormatARB != NULL)//Вначале пытаемся использовать расширение wgl
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
		int NFormats[] = {0};	//Сначала посчитаем, сколько всего форматов
		int attrib[] = {WGL_NUMBER_PIXEL_FORMATS_ARB};
		GetPixFormatARB(hDC, 1, 0, 1, attrib, NFormats);//Получим число форматов
		for(int i = 0; i < NFormats[0]; i++)//Изучим каждый формат
		{
			GetPixFormatARB(hDC, i+1, 0, 9, Attribs, Results);	//Получим само формат(хендл устройства, номер формата, слой какой-то, число атрибутов,
			//атрибуты, куда вернуть результаты по кадому аттрибуду)
			/*проверим формат*/
			if(Results[0] == 1 && Results[1] == WGL_FULL_ACCELERATION_ARB && Results[2] == 1)//поддкржка OpenGL, полное ускорение, рисование в оене
				if(Results[3] == 1)//Двойная буферизация
					if(Results[4] == WGL_TYPE_RGBA_ARB)//Полный цвет
						if(Results[5] >= 16)//Глубина больше либо равна 16ти битам
							if(Results[6] > 0)//У трафарета есть хоть какая-то глубина
							{
								//Этот формат - кандидат на использование. Посмотрим, как у него с мультисемплингом
								if(Results[7] == 1)//Есть мультисемплинг
								{
									if(Results[8] > BestMS)//Если у этого формата мультисемплинг глубже, чем у найденных до этого
									{
										*MSFormat = i;
										BestMS = Results[8];
									}
								}
								else//нет мультисемплинга
								{
									//Подходит для "обычного формата"
									*Format = i;
								}
							}
		}
	}
	else//Если расширение не поддерживается - ищем старомодным способом
	{
		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),
			1,			//Версия
			PFD_DRAW_TO_WINDOW |	// поддерживаеи запись в окно
			PFD_SUPPORT_OPENGL |	// поддерживает OpenGL
			PFD_DOUBLEBUFFER,		// формат с двойной буферизацией
			PFD_TYPE_RGBA,			// RGBA 
			32,						// Глубина цвета
			0, 0, 0, 0, 0, 0,		// Не используются для выборв режима
			0, 0, 0, 0, 0, 0,
			24,						// Буфер глубины
			8,						// Буфер трафарета
			0, 0, 0, 0, 0, 0};		//Какие-то дополнительные буферы, какие-то перекрывающиеся плоскости, что-то устаревшее.. Нам не надо это
			*Format = ChoosePixelFormat(hDC, &pfd);//Подберём самый похожий формат и сохрани его номер в переменную для обычного
	}
}

/*Перерисовывает куб, учитывая его анимацию*/
void RenderQube(AnQube *qb, bool updateRotation)
{
	/*Часто используемые значения*/
	static GLfloat halfSide = qb->side/2;
	static GLfloat segSize = qb->side/qb->segs;
	static GLfloat halfSegSize = qb->side/qb->segs/2;


	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);
	static LARGE_INTEGER periodStart = current;
	static GLfloat time = 0;
	if(updateRotation) time = (GLfloat)(current.QuadPart - periodStart.QuadPart) / (GLfloat)CounterFreq.QuadPart;//Время, прошедшее с начала периода анимации
	if(time > qb->period)//
	{
		periodStart = current;
		time = 0;
	}
	//Теперь нарисуем кубики и покрутим их
	static bool first = true;
	if(Opts.RenderingMethod == RM_vertexArray)
	{
#pragma region Кубы в массивах
		static GLTVector3 *Verts = (GLfloat(*)[3])malloc(sizeof(GLfloat) * qb->segs * qb->segs * qb->segs * 72);//число кубиков * число граней * точек на грань * компонентов вектора
		static GLTVector2 *Texts = (GLfloat(*)[2])malloc(sizeof(GLfloat) * qb->segs * qb->segs * qb->segs * 48);
		static GLTVector3 *Norms = (GLfloat(*)[3])malloc(sizeof(GLfloat) * qb->segs * qb->segs * qb->segs * 72);
		/*p.s. Ужос. GLfloat(*)[3] - есть указатель на массив из трёх флоутов, как раз наш вектор.. Про скобочки вокруг звезды просто так не догадаешься..)*/
		//Если первый раз запускаем - инициализируем кубы
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
		/*покрутим же. Рассчитаем матрицу поворота для первого кубика в слое, повернём его а затем перенесём его координаты для всех остальных кубиков*/
		for(int y = 0; y < qb->segs; y++)
		{
			GLfloat angle = qb->speed * (time - qb->shift * y);//Угол поворота с начала периода для текущего слоя
			if(angle < 0) angle = 0;//Если очередь этого слоя ещё не пришла
			if(angle >= qb->fullTurn) angle = qb->fullTurn;//Если полный оборот уже свершился
			/*рассчитаем матрицу поворота для ближнего левого кубика*/
			GLint yShift = y * qb->segs * qb->segs * 24;
			GLTMatrix rotMat;		//Матрица вращения вершин
			GLTMatrix temp;
			GLTMatrix normRotMat;	//Матрица вращения нормалей
			gltRotationMatrix((GLfloat)gltDegToRad(angle), qb->axis[0], qb->axis[1], qb->axis[2], rotMat);
			memcpy(&temp, &rotMat, sizeof(GLTMatrix));
			gltTransposeMatrix(temp);
			gltInvertMatrix(temp, normRotMat);
			//Трансляция повернутой точки задяется в этой части матрицы
			rotMat[12] = -halfSide + halfSegSize;
			rotMat[13] = halfSide - halfSegSize - y*segSize;
			rotMat[14] = halfSide - halfSegSize;
			InitLittleQube(Verts + yShift, Norms + yShift, segSize);
			for(int i = 0; i < 24; i++)
			{
				gltTransformPoint(Verts + yShift + i, rotMat);
				gltTransformPoint(Norms + yShift + i, normRotMat);
			}
			/*Теперь изменим кординаты других кубов в соответстывии с координатами первых*/
			for(GLint z = 0; z < qb->segs; z++)
				for(GLint x = 0; x < qb->segs; x++)
				{
					if(x == 0 && z == 0)
					{
						continue;//Первый кубик не трогаем
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
#pragma region Рисование кубов по одному
		for(GLint y = 0; y < qb->segs; y++)//Для каждой горизонтали
		{
			glPushMatrix();
			{
				glTranslatef(-halfSide + halfSegSize,  halfSide - segSize*(y+0.5f), halfSide - halfSegSize);//Перенесёмся в место ближнего левого квадратика этой горизонтали
				int x = 0;
				int z = 0;
				do
				{
					glPushMatrix();
					if(qb->speed * (time - qb->shift*y) < qb->fullTurn && qb->speed * (time - qb->shift*y) > 0)glRotatef(qb->speed * (time - qb->shift*y), qb->axis[0], qb->axis[1], qb->axis[2]);//Поворачиваем, только если ещё не сделали полного оборота
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
/*Инициавлизирует массивы для куба, располагая кубики в начальном положении*/
void InitAnQubeArrays(AnQube *qb, GLfloat Verts[][3], GLfloat Texts[][2], GLfloat Norms[][3])
{
	static GLfloat segSize = qb->side/qb->segs;
	static GLfloat halfSide = qb->side/2;
	const GLfloat e = 0.02f;

	GLint vertCounter = 0;
	for(GLfloat y = halfSide - segSize; y >= -halfSide - e; y -= segSize)//Горизонтали
		for(GLfloat x = -halfSide; x <= halfSide - segSize + e; x += segSize)//Столбцы
			for(GLfloat z = halfSide; z >= -halfSide + segSize - e; z -= segSize)//Кубики в столбце
			{
				//Передняя грань
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

				//Правая грань
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

				//Задняя грань
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

				//Левая грань
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

				//Верхняя грань
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

				//Нижняя грань
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

/*Ставит вершины и нормали по адресам так, чтобы кубик был в начальном положении*/
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

	//Передняя
	memcpy(firstVert, corners, GLTVector3Size);
	memcpy(firstVert+1, corners+3, GLTVector3Size);
	memcpy(firstVert+2, corners+2, GLTVector3Size);
	memcpy(firstVert+3, corners+1, GLTVector3Size);
	//Правая
	memcpy(firstVert+4, corners+3, GLTVector3Size);
	memcpy(firstVert+5, corners+5, GLTVector3Size);
	memcpy(firstVert+6, corners+6, GLTVector3Size);
	memcpy(firstVert+7, corners+2, GLTVector3Size);
	//Задняя
	memcpy(firstVert+8, corners+5, GLTVector3Size);
	memcpy(firstVert+9, corners+4, GLTVector3Size);
	memcpy(firstVert+10, corners+7, GLTVector3Size);
	memcpy(firstVert+11, corners+6, GLTVector3Size);
	//Левая
	memcpy(firstVert+12, corners+4, GLTVector3Size);
	memcpy(firstVert+13, corners+0, GLTVector3Size);
	memcpy(firstVert+14, corners+1, GLTVector3Size);
	memcpy(firstVert+15, corners+7, GLTVector3Size);
	//Верхняя
	memcpy(firstVert+16, corners+1, GLTVector3Size);
	memcpy(firstVert+17, corners+2, GLTVector3Size);
	memcpy(firstVert+18, corners+6, GLTVector3Size);
	memcpy(firstVert+19, corners+7, GLTVector3Size);
	//Нижняя
	memcpy(firstVert+20, corners+3, GLTVector3Size);
	memcpy(firstVert+21, corners, GLTVector3Size);
	memcpy(firstVert+22, corners+4, GLTVector3Size);
	memcpy(firstVert+23, corners+5, GLTVector3Size);
	/*А теперь нормали*/
	for(register int i = 0; i < 6; i++)
	{
		memcpy(firstNorm + i*4, norms + i, GLTVector3Size);
		memcpy(firstNorm + i*4 + 1, norms + i, GLTVector3Size);
		memcpy(firstNorm + i*4 + 2, norms + i, GLTVector3Size);
		memcpy(firstNorm + i*4 + 3, norms + i, GLTVector3Size);
	}
}