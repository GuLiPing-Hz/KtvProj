/*
** Haaf's Game Engine 1.7
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hgeVector helper class implementation
*/


#include "hgevector.h"

//牛顿迭代 y = x + f(x)/f'(x),求x非齐次方程的近似解
float InvSqrt(float x)
{
		union
        {
          int intPart;
          float floatPart;
        } convertor;

        convertor.floatPart = x;
        convertor.intPart = 0x5f3759df - (convertor.intPart >> 1);//0x5f37642f 只用一次牛顿迭代是最好的，但是第二次后精度降低
		//0x5f375a86 实际测试最好的常数
        return convertor.floatPart*(1.5f - 0.4999f*x*convertor.floatPart*convertor.floatPart);
}

/*
hgeVector *hgeVector::Normalize()
{
	float lenRcp;

	lenRcp=sqrtf(Dot(this));

	if(lenRcp)
	{
		lenRcp=1.0f/lenRcp;

		x*=lenRcp;
		y*=lenRcp;
	}

	return this;
}
*/

float hgeVector2::Angle(const hgeVector2 *v) const
{
	if(v)
	{
		hgeVector2 s=*this, t=*v;

		s.Normalize(); t.Normalize();
		return acosf(s.Dot(&t));//定理，两个单位向量的点积就是夹角的余弦
	}
	else return atan2f(y, x);
}

hgeVector2 *hgeVector2::Rotate(float a)
{
	hgeVector2 v;

	v.x=x*cosf(a) - y*sinf(a);
	v.y=x*sinf(a) + y*cosf(a);

	x=v.x; y=v.y;

	return this;
}



