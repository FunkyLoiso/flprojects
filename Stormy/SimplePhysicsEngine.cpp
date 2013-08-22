#include "SimplePhysicsEngine.h"

#include <QLineF>
#include <QVector2D>
#include <QTransform>
#include <qmath.h>

#include "quartic/quartic.hpp"

#include "Glass.h"


void SimplePhysicsEngine::update(Glass* glass, qreal timePassed_s)
{
	//timePassed_s = 0.001;
	m_glass = glass;
	m_time_s = timePassed_s;
	for(Glass::TParticlesMap::Iterator pi = m_glass->particles.begin(); pi != m_glass->particles.end(); ++pi)
	{// 1. Write projected speeds and positions for all particles 
		writeProjectedSpeedAndPosition(*pi);
		pi->passive = false;// mark that this particle can initiate collisions
	}
	for(Glass::TParticlesMap::Iterator pi = m_glass->particles.begin(); pi != m_glass->particles.end(); ++pi)
	{// 2. Do collision detection and corresponding projected speed and location modifications for each particle
		if(!pi->passive) doCollisions(*pi, 0);
	}
	for(Glass::TParticlesMap::Iterator pi = m_glass->particles.begin(); pi != m_glass->particles.end(); ++pi)
	{// 3. Write projected to actual after all collisions were detected
		//writeActualSpeedAndPosition(*pi);
		pi->speed = pi->projectedSpeed;
		pi->pos = pi->projectedPos;
	}
}

void SimplePhysicsEngine::writeProjectedSpeedAndPosition(Particle& p)
{
	QVector2D dv = m_gravity * m_time_s;
	QVector2D dp = (p.speed + dv/2) * m_time_s;

	p.projectedSpeed = p.speed + dv;
	p.projectedPos = p.pos + dp;
	p.posTime = 0.0;
}

void SimplePhysicsEngine::doCollisions(Particle& _p, int level)
{
	//Q_ASSERT(level < 50);
	//Collision c;
	//if(findFirstCollision(p, c) && level < 50)
	//{
	//	processCollision(c);//updates projected speed and position
	//	doCollisions(*c.particle, level+1);	//recursion is fishy here...
	//	if(c.type == Collision::WithParticle) doCollisions(*c.otherParticle, level+1);
	//}
	//p.passive = true;

	QVector<Particle*> ps;
	ps.push_back(&_p);
	for(int i = 0; i < ps.size(); ++i)
	{
		Particle& p = *ps[i];
		Collision c;
		if(findFirstCollision(p, c))
		{
			processCollision(c);
			if(c.type == Collision::WithParticle)
				ps.push_back(c.otherParticle);
			ps.push_back(&p);
		}
		else p.passive = true;
	}
}

bool SimplePhysicsEngine::findFirstCollision(Particle& p, Collision& out_collision) const
{
	qreal minTime = m_time_s*1.1;//slightly more so that minTime > m_time_s is garanted
	QVector2D acceleration = m_gravity;
	
	//1. Check for collisions with the border (both edges and vertices)
	for(QPolygonF::ConstIterator bi = m_glass->border.begin(); bi != m_glass->border.end(); ++bi)
	{
		//now we determine whether the final position corresponds to a contact with border
		QVector2D vert1 = QVector2D(*bi);
		QVector2D vert2;
		QVector2D* contactVert = NULL; //Will hold pointer to point1 or point2 if we have a possible contact with a point, not an edge
		if(bi != m_glass->border.end()-1)	vert2 = QVector2D(*(bi+1));
		else								vert2 = QVector2D(*(m_glass->border.begin()));
		qreal dstVert1 = (p.projectedPos - vert1).length();
		qreal dstVert2 = (p.projectedPos - vert2).length();
		qreal dstV1V2 = (vert1 - vert2).length();

		qreal halfPerimeter;
		qreal dst;

		if(dstVert1 > QVector2D(dstVert2, dstV1V2).length())
		{//possible contact with vertex2
			contactVert = &vert2;
			dst = dstVert2;
		}
		else if(dstVert2 > QVector2D(dstVert1, dstV1V2).length())
		{//possible contact with vertex1
			contactVert = &vert1;
			dst = dstVert1;
		}
		else
		{//possible contact with the edge
			halfPerimeter = (dstVert1 + dstVert2 + dstV1V2) / 2;
			dst = 2/dstV1V2*qSqrt(halfPerimeter *	(halfPerimeter - dstV1V2)	* 
				(halfPerimeter - dstVert1)	* 
				(halfPerimeter - dstVert2)		);
		}

		if(dst < p.radius)
		{//contact
			//2. Determine contact time
			qreal contactTime = minTime*1.1;
			QVector2D directionVector;
			if(contactVert == NULL)
			{//contact with an edge
				qreal x1, y1, x2, y2;//points on the line
				{
					directionVector = vert2 - vert1;
					directionVector.normalize();
					x1 = vert1.x() - p.radius * directionVector.y();
					y1 = vert1.y() + p.radius * directionVector.x();
					x2 = vert2.x() - p.radius * directionVector.y();
					y2 = vert2.y() + p.radius * directionVector.x();
				}

				qreal A, B, C;//quadratic equation coefficients
				{
					qreal dx = x1 - x2;
					qreal dy = y1 - y2;
					if(qFuzzyIsNull(dx))
					{
						A = acceleration.x()/2;
						B = p.speed.x();
						C = p.pos.x() - x1;
					}
					else if(qFuzzyIsNull(dy))
					{
						A = acceleration.y()/2;
						B = p.speed.y();
						C = p.pos.y() - y1;
					}
					else
					{
						A = acceleration.y()*dx/(2*dy) - acceleration.x()/2;
						B = p.speed.y()*dx/dy - p.speed.x();
						C = (p.pos.y() - (x1*y2 - x2*y1)/dx)*dx/dy - p.pos.x();
					}
				}
				//now solve the At^2 + Bt + C = 0 for intersection times
				if(qFuzzyIsNull(A))
				{//linear equation, acceleration is zero
					contactTime = -C/B;
				}
				else
				{//quadratic
					qreal t1, t2;
					bool solved = magnet::math::quadraticSolve(B/A, C/A, t1, t2);
					if(solved)
					{
						//if(t1 < 0)		contactTime = t2;
						//else if(t2 < 0)	contactTime = t1;
						//else			contactTime = qMin(t1, t2);
						if(t1 > 0)
						{
							if(t1 < contactTime) contactTime = t1;
						}
						if(t2 > 0)
						{
							if(t2 < contactTime) contactTime = t2;
						}
						if(t1 < 0 && t2 < 0)
						{//two negative roots => initial pos is already contacting!
							contactTime = qMax(t1, t2);
						}

					}
				}

				//QVector2D contactPoint = p.pos + (p.speed + m_gravity*time/2)*time;
				//int neverCock = true;
			}
			else
			{//contact with a vertex
				qreal A, B, C, D, E;//Quartic equation coefficients
				{
					qreal posx_vx = p.pos.x() - contactVert->x();
					qreal posy_vy = p.pos.y() - contactVert->y();

					A = acceleration.x()*acceleration.x()/4 + acceleration.y()*acceleration.y()/4;
					B = acceleration.x()*p.speed.x() + acceleration.y()*p.speed.y();
					C = p.speed.x()*p.speed.x() + p.speed.y()*p.speed.y() + acceleration.x()*posx_vx + acceleration.y()*posy_vy;
					D = 2*p.speed.x()*posx_vx + 2*p.speed.y()*posy_vy;
					E = posx_vx*posx_vx + posy_vy*posy_vy - p.radius*p.radius;

					qreal roots[4];	//the roots will be here
					size_t nRoots;	//number of roots
					if(qFuzzyIsNull(A))
					{//quadratic equation (acceleration is zero)
						bool solved = magnet::math::quadraticSolve(D/C, E/C, roots[0], roots[1]);
						nRoots = solved ? 2 : 0;
					}
					else
					{//quartic equation
						nRoots = magnet::math::quarticSolve(B/A, C/A, D/A, E/A, roots[0], roots[1], roots[2], roots[3]);
					}

					for(int i = 0; i < nRoots; ++i)
					{
						qreal root = roots[i];
						if(root > 0.0 && root < contactTime) contactTime = root;
					}
				}
			}

			if(contactTime < minTime)
			{
				out_collision.type = contactVert ? Collision::WithVertex : Collision::WithEdge;
				out_collision.particle = &p;
				out_collision.contactTime_s = contactTime;
				out_collision.contactVal = contactVert ? *contactVert : directionVector;

				minTime = contactTime;
			}
		}
	}
	// 2. Now check for collisions with other particles
	for(Glass::TParticlesMap::Iterator pi2 = m_glass->particles.begin(); pi2 != m_glass->particles.end(); ++pi2)
	{
		Particle& p2 = *pi2;
		if(p == p2) continue;

		
		qreal minDistance = p.radius+p2.radius;
		if((p.projectedPos - p2.projectedPos).length() < minDistance)
		{//contact
			qreal A, B, C;//Quadratic equation coefficients
			{
				QVector2D dPos(p2.pos - p.pos);
				QVector2D dSpeed(p2.speed - p.speed);

				A = dSpeed.x()*dSpeed.x() + dSpeed.y()*dSpeed.y();
				B = 2*(dSpeed.x()*dPos.x() + dSpeed.y()*dPos.y());
				C = dPos.x()*dPos.x() + dPos.y()*dPos.y() - minDistance*minDistance;
			}

			qreal t1, t2;
			bool solved = magnet::math::quadraticSolve(B/A, C/A, t1, t2);
			if(solved)
			{
				qreal contactTime = minTime*1.1;
				//if(t1 < 0)		contactTime = t2;
				//else if(t2 < 0)	contactTime = t1;
				//else			contactTime = qMin(t1, t2);
				if(t1 > 0 && t1 < contactTime) contactTime = t1;
				if(t2 > 0 && t2 < contactTime) contactTime = t2;


				if(contactTime < minTime)
				{
					out_collision.type = Collision::WithParticle;
					out_collision.particle = &p;
					out_collision.otherParticle = &p2;
					out_collision.contactTime_s = contactTime;

					minTime = contactTime;
				}
			}
		}
	}
	return minTime < m_time_s;
}

void SimplePhysicsEngine::processCollision(Collision& c)
{
	static bool zero = false;
	if(zero) c.contactTime_s = 0;
	Particle& p = *c.particle;
	
	QVector2D dv = m_gravity * c.contactTime_s;
	QVector2D dp = (p.speed + dv/2)*c.contactTime_s;

	p.pos += dp;
	p.speed += dv;
	p.posTime += c.contactTime_s;

	if(c.type == Collision::WithParticle)
	{//with particle
		Particle& p2 = *c.otherParticle;
		QVector2D dp2 = (p2.speed + dv/2)*c.contactTime_s;
		
		p2.pos += dp2;
		p2.speed += dv;
		p2.posTime += c.contactTime_s;

		QVector2D pToP2(p2.pos - p.pos);
		QVector2D directionVector(-pToP2.y(), pToP2.x());

		qreal ang = -qAtan2(directionVector.y(), directionVector.x());

		QTransform tr;
		tr.rotateRadians(ang);

		QVector2D v1 = QVector2D(tr.map(p.speed.toPointF()));
		QVector2D v2 = QVector2D(tr.map(p2.speed.toPointF()));

		qreal m1 = p.mass, m2 = p2.mass;
		qreal y1 = (m1*v1.y() - m2/**m_restitution*/*(v1.y() - v2.y()) + m2*v2.y())/(m1 + m2);
		qreal y2 = y1 + /*m_restitution**/(v1.y() - v2.y());

		v1.setY(y1);
		v2.setY(y2);

		p.speed = QVector2D( tr.inverted().map(v1.toPointF()) );
		p2.speed = QVector2D( tr.inverted().map(v2.toPointF()) );

		//now calculate the projected speed and pos for remaining time
		qreal timeLeft = m_time_s - p.posTime;
		dv = m_gravity * timeLeft;

		dp = (p.speed + dv/2)*timeLeft;
		p.projectedPos = p.pos + dp;
		p.projectedSpeed = p.speed + dv;

		dp2 = (p2.speed + dv/2)*timeLeft;
		p2.projectedPos = p2.pos + dp2;
		p2.projectedSpeed = p2.speed + dv;
	}
	else
	{//with edge or vertex
		QVector2D directionVector;
		if(c.type == Collision::WithEdge)
		{//direction vector is present
			directionVector = c.contactVal;
		}
		else
		{//direction vector is vector from pos to contact vertex, rotated 90 degrees
			QVector2D posToVertex(c.contactVal - p.pos);
			directionVector.setX(-posToVertex.y());
			directionVector.setY(posToVertex.x());
		}
		qreal ang = -qAtan2(directionVector.y(), directionVector.x());
		
		QTransform tr;
		tr.rotateRadians(ang);

		p.speed = QVector2D(tr.map(p.speed.toPointF()));
		p.speed.setY(p.speed.y() * -1/*m_restitution*/);
		p.speed = QVector2D(tr.inverted().map(p.speed.toPointF()));

		qreal timeLeft = m_time_s - p.posTime;

		dv = m_gravity * timeLeft;
		dp = (p.speed + dv/2)*timeLeft;

		p.projectedPos = p.pos + dp;
		p.projectedSpeed = p.speed + dv;
	}
}

qreal SimplePhysicsEngine::cosBetweenVectors(QVector2D v1, QVector2D v2)
{
	return	QVector2D::dotProduct(v1, v2) / (v1.length() * v2.length());

}
