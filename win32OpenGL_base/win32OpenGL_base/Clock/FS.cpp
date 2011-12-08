uniform bool Inverse;
uniform float Time;//Время в секундах(0.0 - 43200.0)

varying vec3 Position;
varying vec3 Normal;


/*Прощитывает освещение точечным источником света*/
void PointLight(   in int light, in vec3 pos, in vec3 norm,
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
      float attenuation = 1.0/(   gl_LightSource[light].constantAttenuation +
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
/*Отрисовывает часы*/
void ApplyClock(in float Time, inout vec4 Color, bool Inverse)
{
   vec4 HourColor = vec4(0.0, 0.7, 0.0, 1.0);
   vec4 MinColor = vec4(0.0, 0.0, 1.0, 1.0);
   vec4 SecColor = vec4(1.0, 0.0, 0.0, 1.0);
   
   float inv;
   Inverse ? (inv = -1.0) : (inv = 1.0);
   
   const float PIx2d12 = 0.52359877559829887307710723054658;//Два пи, делённое на двенадцать
   const float PIx2d60 = 0.10471975511965977461542144610932;//Два пи делённое на шестьдесят
   const float radius = 0.4;
   //половина ширины, длина попы, длина указки. Всё в относительных координатах как у текстуры [0.0, 1.0]
   const vec3 secHand =    vec3(0.0025, 0.05, 0.15);
   const vec3 minHand =    vec3(0.005, 0.04, 0.15);
   const vec3 hourHand =    vec3(0.007, 0.01, 0.075);
   
   float hours = Time / 3600.0;
   float mins = mod(Time/60.0, 60.0);
   float secs = mod(Time, 60.0);
   vec2 handCoord = fract(gl_TexCoord[0].st) - 0.5;//Коодинаты точки в пространстве часов
   /*Отметим центр и край часов*/
   if(all(lessThanEqual(handCoord, vec2(0.001))) && all(greaterThanEqual(handCoord, vec2(-0.001))))
   {
      Color = vec4(1.0);
      //discard;
      return;
   }
  if(length(handCoord) < radius + 0.01)
     {
        Color = vec4(smoothstep(radius - 0.01, radius + 0.01, length(handCoord)));
     }
   /*Теперь для каждой стрелки рассчитаем угол, повернём на него точку против
   часовой стрелки и посмотрим, попадает ли она в тело стрелки*/
   //Для секундной стрелки
   float A = inv * secs * PIx2d60;//Делим секунды на шестьдесят и умножаем на два пи - получаем угол в раддианах
   float cosA = cos(A), sinA = sin(A);
   mat2 rotMat = mat2(cosA, sinA, -sinA, cosA);//Создаём матрицу вращения стрелки до положения "12 часов"
   //Повернём точку этой матрицей и посмотрим, попадает ли она в стрелку
   vec2 tempCoord = rotMat * handCoord;
   if(step(secHand[0], abs(tempCoord.x)) != 1.0 && tempCoord.y >= -secHand[1] && tempCoord.y <= secHand[2])
   {
      Color = SecColor;
      return;
   }
   //Теперь минутная
   A = inv * mins * PIx2d60;
   cosA = cos(A);
   sinA = sin(A);
   rotMat = mat2(cosA, sinA, -sinA, cosA);//Создаём матрицу вращения стрелки до положения "12 часов"
   //Повернём точку этой матрицей и посмотрим, попадает ли она в стрелку
   tempCoord = rotMat * handCoord;
   if(step(minHand[0], abs(tempCoord.x)) != 1.0 && tempCoord.y >= -minHand[1] && tempCoord.y <= minHand[2])
   {
      Color = MinColor;
      return;
   }  
   //Теперь для часов
   A = inv * hours * PIx2d12;
   cosA = cos(A);
   sinA = sin(A);
   rotMat = mat2(cosA, sinA, -sinA, cosA);//Создаём матрицу вращения стрелки до положения "12 часов"
   //Повернём точку этой матрицей и посмотрим, попадает ли она в стрелку
   tempCoord = rotMat * handCoord;
   if(step(hourHand[0], abs(tempCoord.x)) != 1.0 && tempCoord.y >= -hourHand[1] && tempCoord.y <= hourHand[2])
   {
      Color = HourColor;
      //discard;
      return;
   }
}



void main(void)
{   
   vec4 amb = vec4(0.0), diff = vec4(0.0), spec = vec4(0.0), color = vec4(0.0);
   ApplyClock(Time, color, Inverse);
   PointLight(0, Position, Normal, amb, diff, spec);
   gl_FragColor = gl_FrontLightModelProduct.sceneColor + diff * color + spec;
}