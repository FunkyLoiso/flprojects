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
	for(Glass::TParticlesMap::Iterator pi = m_glass->particles.begin(); pi != m_glass->particles.end(); ++pi)
	{// 1. Write projected speeds and positions for all particles 
		pi->passive = false;// mark that this particle can initiate collisions
		pi->dbg_level = 0;
	}
	for(Glass::TParticlesMap::Iterator pi = m_glass->particles.begin(); pi != m_glass->particles.end(); ++pi)
	{// 2. Do collision detection and corresponding projected speed and location modifications for each particle
		pi->dbg_level = 0;
		if(!pi->passive) doCollisions(*pi, 0);
	}
	for(Glass::TParticlesMap::Iterator pi = m_glass->particles.begin(); pi != m_glass->particles.end(); ++pi)
	{// 3. Write projected to actual after all collisions were detected
		//writeActualSpeedAndPosition(*pi);

		qreal timeLeft = m_time_s - pi->posTime;

		QVector2D dv = m_gravity * timeLeft;
		QVector2D dp = (pi->speed + dv/2)*timeLeft;

		pi->speed += dv;
		pi->pos += dp;

		pi->passive = false;
		pi->posTime = 0.0;
	}
}

//void SimplePhysicsEngine::writeProjectedSpeedAndPosition(Particle& p)
//{
//	QVector2D dv = m_gravity * m_time_s;
//	QVector2D dp = (p.speed + dv/2) * m_time_s;
//
//	p.projectedSpeed = p.speed + dv;
//	p.projectedPos = p.pos + dp;
//	p.posTime = 0.0;
//}

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

	//QVector<Particle*> ps;
	//ps.push_back(&_p);
	//for(int i = 0; i < ps.size(); ++i)
	//{
	//	Particle& p = *ps[i];
	//	Collision c;
	//	if(findFirstCollision(p, c))
	//	{
	//		++p.dbg_level;
	//		processCollision(c);
	//		
	//		if(c.type == Collision::WithParticle)
	//		{
	//			++c.otherParticle->dbg_level;
	//			ps.push_back(c.otherParticle);
	//		}
	//		ps.push_back(&p);
	//		
	//	}
	//	else p.passive = true;
	//}

	for(;;)
	{
		for(Glass::TParticlesMap::Iterator pi = m_glass->particles.begin(); pi != m_glass->particles.end(); ++pi)
		{
			Collision c;
			if(findFirstCollision(*pi, c))
			{
				colls.insert(c.contactTime_s+c.particle->posTime, c);
			}
		}
		if(!colls.isEmpty())
		{
			processCollision(*colls.begin());
			colls.clear();
		}
		else
		{
			break;
		}
	}
}

bool SimplePhysicsEngine::findFirstCollision(Particle& p, Collision& out_collision) const
{
	static const qreal MIN_TIME = 0.0;
	qreal timeLeft = m_time_s - p.posTime;//time until end of frame for this particle
	qreal minTime = timeLeft;//*1.1 to make sure minTime initially greather than timeLeft
	QVector2D acceleration = m_gravity;

	//determine bounding rect of p
	QRectF boundingRect = getBoundingRect(p, acceleration, timeLeft);
	
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

		if(!rectIntersectsLine(boundingRect, vert1, vert2)) continue;

		//2. Determine contact time
		qreal contactTime = minTime/**1.1*/;
		QVector2D directionVector;
		//if(contactVert == NULL)
		//contact with an edge
		{
			qreal x1, y1, x2, y2;//points on the line
			directionVector = vert2 - vert1;
			directionVector.normalize();
			x1 = vert1.x() - p.radius * directionVector.y();
			y1 = vert1.y() + p.radius * directionVector.x();
			x2 = vert2.x() - p.radius * directionVector.y();
			y2 = vert2.y() + p.radius * directionVector.x();

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
					contactPoint = p.pos + (p.speed + acceleration*t1/2) * t1;
					if((contactPoint-vert1).length() < v1v2Length && (contactPoint-vert2).length() < v1v2Length)
					{
						QVector2D speedAtContact = p.speed + acceleration*t1;
						QVector2D normalVector(-directionVector.y(), directionVector.x());//outer nornal
						if(QVector2D::dotProduct(normalVector, speedAtContact) < 0.0)
						{
							contactTime = t1;
						}
					}
				}
				if(t2 > 0 && t2 < contactTime)
				{
					contactPoint = p.pos + (p.speed + acceleration*t2/2) * t2;
					if((contactPoint-vert1).length() < v1v2Length && (contactPoint-vert2).length() < v1v2Length)
					{
						QVector2D speedAtContact = p.speed + acceleration*t2;
						QVector2D normalVector(-directionVector.y(), directionVector.x());
						if(QVector2D::dotProduct(normalVector, speedAtContact) < 0.0)
						{
							contactTime = t2;
						}
					}
				}
				//if(qAbs(t1) < MIN_TIME || qAbs(t2) < MIN_TIME) contactTime = 0.0;
			}
		}

		//check the first vertex
		{
			qreal posx_vx = p.pos.x() - vert1.x();
			qreal posy_vy = p.pos.y() - vert1.y();

			A = acceleration.x()*acceleration.x()/4 + acceleration.y()*acceleration.y()/4;
			B = acceleration.x()*p.speed.x() + acceleration.y()*p.speed.y();
			C = p.speed.x()*p.speed.x() + p.speed.y()*p.speed.y() + acceleration.x()*posx_vx + acceleration.y()*posy_vy;
			D = 2*p.speed.x()*posx_vx + 2*p.speed.y()*posy_vy;
			E = posx_vx*posx_vx + posy_vy*posy_vy - p.radius*p.radius;

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
					//QVector2D speedAtContactPoint = p.speed + acceleration * root;
					Particle pAtContact = p.moved(acceleration, root);
					if(QVector2D::dotProduct(pAtContact.speed, vert1-pAtContact.pos) > 0.0)
					{
						contactTime = root;
						contactVertex = true;
					}
				}
				//if(qAbs(root) < MIN_TIME)
				//{
				//	contactTime = 0.0;
				//	break;
				//}
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
	for(Glass::TParticlesMap::Iterator pi2 = m_glass->particles.begin(); pi2 != m_glass->particles.end(); ++pi2)
	{
		if(p == *pi2) continue;
		qreal dt = p.posTime-pi2->posTime;
		if(dt < 0.0) continue;//p2 is from the future, we will consider this collision when searching for p2 collisions

		Particle p2 = pi2->moved(acceleration, dt);//warp p2 forward in time to the moment p.posTime

		QRectF p2BoundingRect = getBoundingRect(p2, acceleration, timeLeft);
		if(!boundingRect.intersects(p2BoundingRect)) continue;
		
		qreal minDistance = p.radius+p2.radius;

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
			qreal contactTime = minTime/**1.1*/;
			if(t1 > 0 && t1 < contactTime)
			{
				Particle p1Contact = p.moved(acceleration, t1);
				Particle p2Contact = p2.moved(acceleration, t1);
				if(QVector2D::dotProduct(p1Contact.speed-p2Contact.speed, p2Contact.pos-p1Contact.pos) > 0.0)
				{
					contactTime = t1;
				}
			}
			if(t2 > 0 && t2 < contactTime) 
			{
				Particle p1Contact = p.moved(acceleration, t2);
				Particle p2Contact = p2.moved(acceleration, t2);
				if(QVector2D::dotProduct(p1Contact.speed-p2Contact.speed, p2Contact.pos-p1Contact.pos) > 0.0)
				{
					contactTime = t2;
				}
			}
			//if(qAbs(t1) < MIN_TIME || qAbs(t2) < MIN_TIME) contactTime = 0.0;


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
	//QVector2D dv = m_gravity * c.contactTime_s;
	//QVector2D dp = (p.speed + dv/2)*c.contactTime_s;

	//p.pos += dp;
	//p.speed += dv;
	//p.posTime += c.contactTime_s;

	if(c.type == Collision::WithParticle)
	{//with particle
		Particle& p2 = *c.otherParticle;
		p2.move(m_gravity, -p2.posTime + p.posTime);//move p2 to mutual contact time
		//QVector2D dp2 = (p2.speed + dv/2)*c.contactTime_s;
		//
		//p2.pos += dp2;
		//p2.speed += dv;
		//p2.posTime += c.contactTime_s;

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
		//qreal timeLeft = m_time_s - p.posTime;
		//dv = m_gravity * timeLeft;

		//dp = (p.speed + dv/2)*timeLeft;
		//p.projectedPos = p.pos + dp;
		//p.projectedSpeed = p.speed + dv;

		//dp2 = (p2.speed + dv/2)*timeLeft;
		//p2.projectedPos = p2.pos + dp2;
		//p2.projectedSpeed = p2.speed + dv;
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

		//qreal timeLeft = m_time_s - p.posTime;

		//dv = m_gravity * timeLeft;
		//dp = (p.speed + dv/2)*timeLeft;

		//p.projectedPos = p.pos + dp;
		//p.projectedSpeed = p.speed + dv;
	}
}

QRectF SimplePhysicsEngine::getBoundingRect(Particle p, QVector2D acceleration, qreal timeLeft) const
{
	QRectF boundingRect;

	//expand radius a little bit so that bounding rect is a bit bigger and no border errors are present
	p.radius *= 1.01;//1%

	//x
	if(qFuzzyIsNull(acceleration.x()))
	{
		qreal dx = p.speed.x()*timeLeft;
		if(dx > 0.0)
		{
			boundingRect.setLeft(p.pos.x()-p.radius);
			boundingRect.setRight(p.pos.x()+dx+p.radius);
		}
		else
		{
			boundingRect.setLeft(p.pos.x()+dx-p.radius);
			boundingRect.setRight(p.pos.x()+p.radius);
		}
	}
	else
	{
		qreal critTimeX = -2.0*p.speed.x()/acceleration.x();//point where speed changes sign
		if(critTimeX < 0.0 || critTimeX > timeLeft)
		{
			qreal dx = (p.speed.x() + acceleration.x()*timeLeft/2)*timeLeft;
			if(dx > 0.0)
			{
				boundingRect.setLeft(p.pos.x()-p.radius);
				boundingRect.setRight(p.pos.x()+dx+p.radius);
			}
			else
			{
				boundingRect.setLeft(p.pos.x()+dx-p.radius);
				boundingRect.setRight(p.pos.x()+p.radius);
			}
		}
		else
		{
			qreal dxCrit = (p.speed.x() + acceleration.x()*critTimeX/2)*critTimeX;
			if(dxCrit > 0.0)
			{
				boundingRect.setLeft(p.pos.x()-p.radius);
				boundingRect.setRight(p.pos.x()+dxCrit+p.radius);
			}
			else
			{
				boundingRect.setLeft(p.pos.x()+dxCrit-p.radius);
				boundingRect.setRight(p.pos.x()+p.radius);
			}
		}
	}

	//y
	if(qFuzzyIsNull(acceleration.y()))
	{
		qreal dy = p.speed.y()*timeLeft;
		if(dy > 0.0)
		{
			boundingRect.setTop(p.pos.y()-p.radius);
			boundingRect.setBottom(p.pos.y()+dy+p.radius);
		}
		else
		{
			boundingRect.setTop(p.pos.y()+dy-p.radius);
			boundingRect.setBottom(p.pos.y()+p.radius);
		}
	}
	else
	{
		qreal critTimeY = -2.0*p.speed.y()/acceleration.y();//point where speed changes sign
		if(critTimeY < 0.0 || critTimeY > timeLeft)
		{
			qreal dy = (p.speed.y() + acceleration.y()*timeLeft/2)*timeLeft;
			if(dy > 0.0)
			{
				boundingRect.setTop(p.pos.y()-p.radius);
				boundingRect.setBottom(p.pos.y()+dy+p.radius);
			}
			else
			{
				boundingRect.setTop(p.pos.y()+dy-p.radius);
				boundingRect.setBottom(p.pos.y()+p.radius);
			}
		}
		else
		{
			qreal dyCrit = (p.speed.y() + acceleration.y()*critTimeY/2)*critTimeY;
			if(dyCrit > 0.0)
			{
				boundingRect.setTop(p.pos.y()-p.radius);
				boundingRect.setBottom(p.pos.y()+dyCrit+p.radius);
			}
			else
			{
				boundingRect.setTop(p.pos.y()+dyCrit-p.radius);
				boundingRect.setBottom(p.pos.y()+p.radius);
			}
		}
	}

	return boundingRect;
}

bool SimplePhysicsEngine::rectIntersectsLine(const QRectF& rect, QVector2D p1, QVector2D p2) const
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
	if(p1.y() > rect.bottom() && p2.y() > rect.bottom()) return false;	//line lays lower
	if(p1.y() < rect.top() && p2.y() < rect.top()) return false;		//line lays higher

	return true;
}

