#include "SimplePhysicsEngine.h"

#include <QLineF>
#include <QVector2D>
#include <QRectF>
#include <QTransform>
#include <qmath.h> 

#include "quartic/quartic.hpp"

#include "Glass.h"


void SimplePhysicsEngine::update(Glass* glass, qreal timePassed_s)
{
	//timePassed_s = 2.0;
	m_glass = glass;
	m_time_s = timePassed_s;
	//for(Glass::TParticlesMap::Iterator pi = m_glass->particles.begin(); pi != m_glass->particles.end(); ++pi)
	//{
	//	pi->dbg_level = 0;
	//}

	//do all collisions for all particles
	doCollisions();

#ifndef NDEBUG
	QRectF borderRect = m_glass->border.boundingRect();
#endif
	for(Glass::TParticlesVector::Iterator pi = m_glass->particles.begin(); pi != m_glass->particles.end(); ++pi)
	{//move all particles to the end of the frame
		qreal timeLeft = m_time_s - pi->posTime();
		pi->move(m_gravity, timeLeft);

		//pi->passive = false;
		pi->setPosTime(0.0);
#ifndef NDEBUG
		Q_ASSERT(borderRect.contains(pi->pos().toPointF()));
#endif
	}
}

void SimplePhysicsEngine::doCollisions()
{
	for(;;)
	{
		qreal firstCollisionTime = m_time_s*1.1;
		Collision firstCollision;
		for(Glass::TParticlesVector::Iterator pi = m_glass->particles.begin(); pi != m_glass->particles.end(); ++pi)
		{
			Collision c;
			if(findFirstCollision(*pi, c))
			{
				qreal collisionTime = c.contactTime_s+c.particle->posTime();
				if(collisionTime < firstCollisionTime)
				{
					firstCollisionTime = collisionTime;
					firstCollision = c;
				}
			}
		}
		if(firstCollisionTime < m_time_s)
		{
			processCollision(firstCollision);
		}
		else
		{
			break;
		}
	}
}

bool SimplePhysicsEngine::findFirstCollision(Particle& p, Collision& out_collision) const
{
	qreal timeLeft = m_time_s - p.posTime();//time until end of frame for this particle
	qreal minTime = timeLeft;
	QVector2D acceleration = m_gravity;

	//determine bounding rect for p
	QRectF boundingRect = p.boundingRect(acceleration, timeLeft);
	
	//1. Check for collisions with the border (both edges and vertices)
	for(QPolygonF::ConstIterator bi = m_glass->border.begin(); bi != m_glass->border.end(); ++bi)
	{
		// new concept - we always look for collision point and determine whether the time is within current frame
		qreal A, B, C, D, E;//Quartic equation coefficients

		QVector2D vert1 = QVector2D(*bi);
		QVector2D vert2;
		bool contactVertex = false;
		if(bi != m_glass->border.end()-1)	vert2 = QVector2D(*(bi+1));
		else								vert2 = QVector2D(*(m_glass->border.begin()));

		//check whether bounding rectangle intersects with this border
		if(!rectIntersectsLineSegment(boundingRect, vert1, vert2)) continue;

		//2. Determine contact time
		qreal contactTime = minTime;
		QVector2D directionVector;
		{
			qreal x1, y1, x2, y2;//points on the line
			directionVector = vert2 - vert1;
			directionVector.normalize();
			x1 = vert1.x() - p.radius() * directionVector.y();
			y1 = vert1.y() + p.radius() * directionVector.x();
			x2 = vert2.x() - p.radius() * directionVector.y();
			y2 = vert2.y() + p.radius() * directionVector.x();

			qreal dx = x1 - x2;
			qreal dy = y1 - y2;
			if(qFuzzyIsNull(dx))
			{
				A = acceleration.x()/2;
				B = p.speed().x();
				C = p.pos().x() - x1;
			}
			else if(qFuzzyIsNull(dy))
			{
				A = acceleration.y()/2;
				B = p.speed().y();
				C = p.pos().y() - y1;
			}
			else
			{
				A = acceleration.y()*dx/(2*dy) - acceleration.x()/2;
				B = p.speed().y()*dx/dy - p.speed().x();
				C = (p.pos().y() - (x1*y2 - x2*y1)/dx)*dx/dy - p.pos().x();
			}

			//now solve the At^2 + Bt + C = 0 for intersection times
			qreal t1, t2;
			bool solved = magnet::math::quadSolve(C, B, A, t1, t2);
			if(solved)
			{
				QVector2D contactPoint;
				qreal v1v2Length;
				v1v2Length = (vert1-vert2).length();

				//(x-x1)/(x2-x1)=(y-y1)/(y2-y1) ///@todo using line equation is probably faster
				// ?? only if speed at contact point is directed towards edge?
				if(t1 > 0 && t1 < contactTime)
				{
					contactPoint = p.pos() + (p.speed() + acceleration*t1/2) * t1;
					if((contactPoint-vert1).length() < v1v2Length && (contactPoint-vert2).length() < v1v2Length)
					{
						QVector2D speedAtContact = p.speed() + acceleration*t1;
						QVector2D normalVector(-directionVector.y(), directionVector.x());//outer nornal
						if(QVector2D::dotProduct(normalVector, speedAtContact) < 0.0)
						{
							contactTime = t1;
						}
					}
				}
				if(t2 > 0 && t2 < contactTime)
				{
					contactPoint = p.pos() + (p.speed() + acceleration*t2/2) * t2;
					if((contactPoint-vert1).length() < v1v2Length && (contactPoint-vert2).length() < v1v2Length)
					{
						QVector2D speedAtContact = p.speed() + acceleration*t2;
						QVector2D normalVector(-directionVector.y(), directionVector.x());
						if(QVector2D::dotProduct(normalVector, speedAtContact) < 0.0)
						{
							contactTime = t2;
						}
					}
				}
			}
		}

		//check the first vertex
		{
			qreal posx_vx = p.pos().x() - vert1.x();
			qreal posy_vy = p.pos().y() - vert1.y();

			A = acceleration.x()*acceleration.x()/4 + acceleration.y()*acceleration.y()/4;
			B = acceleration.x()*p.speed().x() + acceleration.y()*p.speed().y();
			C = p.speed().x()*p.speed().x() + p.speed().y()*p.speed().y() + acceleration.x()*posx_vx + acceleration.y()*posy_vy;
			D = 2*p.speed().x()*posx_vx + 2*p.speed().y()*posy_vy;
			E = posx_vx*posx_vx + posy_vy*posy_vy - p.radius()*p.radius();

			qreal roots[4];	//the roots will be here
			size_t nRoots;	//number of roots

			if(qFuzzyIsNull(A))
			{//quadratic equation (acceleration is zero)
				bool solved = magnet::math::quadSolve(E, D, C, roots[0], roots[1]);
				nRoots = solved ? 2 : 0;
			}
			else
			{//quartic equation
				nRoots = magnet::math::quarticSolve(B/A, C/A, D/A, E/A, roots[0], roots[1], roots[2], roots[3]);
			}

			for(int i = 0; i < nRoots; ++i)
			{
				qreal root = roots[i];
				if(root > 0.0 && root < contactTime)
				{
					Particle pAtContact = p.moved(acceleration, root);
					if(QVector2D::dotProduct(pAtContact.speed(), vert1-pAtContact.pos()) > 0.0)
					{
						contactTime = root;
						contactVertex = true;
					}
				}
			}
		}		

		if(contactTime < minTime)
		{
			out_collision.type = contactVertex ? Collision::WithVertex : Collision::WithEdge;
			out_collision.particle = &p;
			out_collision.contactTime_s = contactTime;
			//out_collision.contactTime_s = contactTime - MIN_TIME;
			out_collision.contactVal = contactVertex ? vert1 : directionVector;

			minTime = contactTime;
		}
	}
	// 2. Now check for collisions with other particles
	for(Glass::TParticlesVector::Iterator pi2 = m_glass->particles.begin(); pi2 != m_glass->particles.end(); ++pi2)
	{
		if(p == *pi2) continue;
		qreal dt = p.posTime()-pi2->posTime();
		if(dt < 0.0) continue;//p2 is from the future, we will consider this collision when searching for p2 collisions

		Particle p2 = pi2->moved(acceleration, dt);//warp p2 forward in time to the moment p.posTime

		//QRectF p2BoundingRect = p2.boundingRect(acceleration, timeLeft);
		//if(!boundingRect.intersects(p2BoundingRect)) continue;
		
		qreal minDistance = p.radius()+p2.radius();

		qreal A, B, C;//Quadratic equation coefficients
		{
			QVector2D dPos(p2.pos() - p.pos());
			QVector2D dSpeed(p2.speed() - p.speed());

			A = dSpeed.x()*dSpeed.x() + dSpeed.y()*dSpeed.y();
			B = 2*(dSpeed.x()*dPos.x() + dSpeed.y()*dPos.y());
			C = dPos.x()*dPos.x() + dPos.y()*dPos.y() - minDistance*minDistance;
		}

		qreal t1, t2;
		bool solved = magnet::math::quadraticSolve(B/A, C/A, t1, t2);
		if(solved)
		{
			qreal contactTime = minTime;
			if(t1 > 0 && t1 < contactTime)
			{
				Particle p1Contact = p.moved(acceleration, t1);
				Particle p2Contact = p2.moved(acceleration, t1);
				if(QVector2D::dotProduct(p1Contact.speed()-p2Contact.speed(), p2Contact.pos()-p1Contact.pos()) > 0.0)
				{
					contactTime = t1;
				}
			}
			if(t2 > 0 && t2 < contactTime) 
			{
				Particle p1Contact = p.moved(acceleration, t2);
				Particle p2Contact = p2.moved(acceleration, t2);
				if(QVector2D::dotProduct(p1Contact.speed()-p2Contact.speed(), p2Contact.pos()-p1Contact.pos()) > 0.0)
				{
					contactTime = t2;
				}
			}


			if(contactTime < minTime)
			{
				out_collision.type = Collision::WithParticle;
				out_collision.particle = &p;
				//*pi2 = p2;
				out_collision.otherParticle = &*pi2;
				out_collision.contactTime_s = contactTime;
				//out_collision.contactTime_s = contactTime - MIN_TIME;

				minTime = contactTime;
			}
		}
	}
	return minTime < timeLeft;
}

void SimplePhysicsEngine::processCollision(Collision& c)
{
	Particle& p = *c.particle;
	
	p.move(m_gravity, c.contactTime_s);

	if(c.type == Collision::WithParticle)
	{//with particle
		Particle& p2 = *c.otherParticle;
		p2.move(m_gravity, -p2.posTime() + p.posTime());//move p2 to mutual contact time

		QVector2D pToP2(p2.pos() - p.pos());
		QVector2D directionVector(-pToP2.y(), pToP2.x());

		qreal ang = -qAtan2(directionVector.y(), directionVector.x());

		QTransform tr;
		tr.rotateRadians(ang);

		QVector2D v1 = QVector2D(tr.map(p.speed().toPointF()));
		QVector2D v2 = QVector2D(tr.map(p2.speed().toPointF()));

		qreal m1 = p.mass(), m2 = p2.mass();
		qreal y1 = (m1*v1.y() - m2*(v1.y() - v2.y()) + m2*v2.y())/(m1 + m2);
		qreal y2 = y1 + (v1.y() - v2.y());

		v1.setY(y1);
		v2.setY(y2);

		p.setSpeed(QVector2D( tr.inverted().map(v1.toPointF()) ));
		p2.setSpeed(QVector2D( tr.inverted().map(v2.toPointF()) ));
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
			QVector2D posToVertex(c.contactVal - p.pos());
			directionVector.setX(-posToVertex.y());
			directionVector.setY(posToVertex.x());
		}
		qreal ang = -qAtan2(directionVector.y(), directionVector.x());
		
		QTransform tr;
		tr.rotateRadians(ang);

		QPointF newSpeed(tr.map( p.speed().toPointF() ));
		newSpeed.setY(newSpeed.y() * -1);
		p.setSpeed(QVector2D(tr.inverted().map(newSpeed)));
	}
}



bool SimplePhysicsEngine::rectIntersectsLineSegment(const QRectF& rect, QVector2D p1, QVector2D p2) const
{
	//F(x y) = (y2-y1)x + (x1-x2)y + (x2*y1-x1*y2) <- line equation
	qreal dy = p2.y()-p1.y();
	qreal dx = p1.x()-p2.x();
	qreal C = p2.x()*p1.y()-p1.x()*p2.y();

	bool a = dy*rect.topLeft().x() + dx*rect.topLeft().y() + C > 0.0;
	bool b = dy*rect.topRight().x() + dx*rect.topRight().y() + C > 0.0;
	bool c = dy*rect.bottomLeft().x() + dx*rect.bottomLeft().y() + C > 0.0;
	bool d = dy*rect.bottomRight().x() + dx*rect.bottomRight().y() + C > 0.0;

	if(a == b && b == c && c == d)
	{
		return false;//all corners on the same side of the line if all signs are the same
	}

	//x axis
	if(p1.x() > rect.right() && p2.x() > rect.right()) return false;	//line lays to the right
	if(p1.x() < rect.left() && p2.x() < rect.left()) return false;		//line lays to the left
	//y axis
	if(p1.y() > rect.bottom() && p2.y() > rect.bottom()) return false;	//line lays lower
	if(p1.y() < rect.top() && p2.y() < rect.top()) return false;		//line lays higher

	return true;
}

