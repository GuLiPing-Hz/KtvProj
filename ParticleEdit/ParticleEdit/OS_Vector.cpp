#include "OS_Vector.h"

D3DXMATRIX g_matrix;

//���� 1/sqrt(x) �ٶȱ�����4��
//ţ�ٵ��� y = x + f(x)/f'(x),��x����η��̵Ľ��ƽ�
float OS_InvSqrt(float x)
{
	union
	{
		int intPart;
		float floatPart;
	} convertor;

	convertor.floatPart = x;
	convertor.intPart = 0x5f3759df - (convertor.intPart >> 1);//0x5f37642f ֻ��һ��ţ�ٵ�������õģ����ǵڶ��κ󾫶Ƚ���
	//0x5f375a86 ʵ�ʲ�����õĳ���
	return convertor.floatPart*(1.5f - 0.4999f*x*convertor.floatPart*convertor.floatPart);
}

bool OS_MatrixMultiply(D3DXMATRIX* pOut,const D3DXMATRIX* pM1,const D3DXMATRIX* pM2)
{
	if (pM1 == NULL || pM2 == NULL || pOut == NULL)
	{
		return false;
	}

	D3DXMATRIX tmp;
	memset(&tmp,0,sizeof(tmp));
	for (int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			for(int k=0;k<4;k++)
			{
				tmp.m[i][j] += pM1->m[i][k]*pM2->m[k][j];
			}
		}
	}

	memcpy(pOut,&tmp,sizeof(D3DXMATRIX));

	return true;
}

COS_Vector3::COS_Vector3(void)
:m_x(0.0f)
,m_y(0.0f)
,m_z(0.0f)
{
	fillMartix();
}

COS_Vector3::COS_Vector3(float x,float y,float z)
:m_x(x)
,m_y(y)
,m_z(z)
{
	fillMartix();
}

COS_Vector3::COS_Vector3(const COS_Vector3& v)
{
	m_x = v.m_x;
	m_y = v.m_y;
	m_z = v.m_z;

	fillMartix();
}

COS_Vector3::~COS_Vector3(void)
{
}

void COS_Vector3::fillMartix()
{
	memset(g_matrix,0,sizeof(g_matrix));

	g_matrix._11 = m_x;
	g_matrix._12 = m_y;
	g_matrix._13 = m_z;
	g_matrix._14 = 1.0f;
}

COS_Vector3& COS_Vector3::operator-()
{
	m_x = -m_x;
	m_y = -m_y;
	m_z = - m_z;

	return *this;
}

COS_Vector3 COS_Vector3::operator+(const COS_Vector3& v)
{
	return COS_Vector3 (m_x + v.m_x,m_y + v.m_y,m_z + v.m_z);
}
COS_Vector3 COS_Vector3::operator-(const COS_Vector3& v)
{
	return COS_Vector3 (m_x - v.m_x,m_y - v.m_y,m_z - v.m_z);
}
COS_Vector3 COS_Vector3::operator*(const float v)
{
	return COS_Vector3 (m_x*v,m_y*v,m_z*v);
}
COS_Vector3 COS_Vector3::operator/(const float v)
{
	return COS_Vector3(m_x/v,m_y/v,m_z/v);
}

COS_Vector3& COS_Vector3::operator+=(const COS_Vector3& v)
{
	m_x += v.m_x;
	m_y += v.m_y;
	m_z += v.m_z;
	fillMartix();
	return *this;
}
COS_Vector3& COS_Vector3::operator-=(const COS_Vector3& v)
{
	m_x -= v.m_x;
	m_y -= v.m_y;
	m_z -= v.m_z;
	fillMartix();
	return *this;
}

bool COS_Vector3::operator==(const COS_Vector3& v)
{
	return ((m_x==v.m_x) && (m_y==v.m_y) && (m_z==v.m_z));
}

bool COS_Vector3::operator!=(const COS_Vector3& v)
{
	return !(*this == v);
}

COS_Vector3& COS_Vector3::operator=(const COS_Vector3& v)
{
	if (this != &v)
	{
		m_x = v.m_x;
		m_y = v.m_y;
		m_z = v.m_z;
		fillMartix();
	}

	return *this;
}

void		COS_Vector3::funClamp(const float max)
{ 
	if(funLength() > max)
	{ 
		funNormalize(); 
		m_x *= max; 
		m_y *= max; 
		m_z *= max;
	} 
}

//���
float		COS_Vector3::funDot(const COS_Vector3 *v)
{
	if (v)
	{
		return (m_x*v->m_x + m_y*v->m_y + m_z*v->m_z);
	}
	else
	{
		return 0.0f;
	}
}
//ģ��
float		COS_Vector3::funLength()
{
	return sqrtf(funDot(this));
}
//����нǽǶ�
float		COS_Vector3::funAngle(const COS_Vector3 *v) const
{
	if(v)
	{
		COS_Vector3 s=*this, t=*v;

		s.funNormalize(); t.funNormalize();
		return acosf(s.funDot(&t));//����������λ�����ĵ�����Ǽнǵ�����
	}
	else 
	{
		return atan2f(m_y, m_x);
	}

}
//��λ����
COS_Vector3&	COS_Vector3::funNormalize()
{
	float rc=OS_InvSqrt(funDot(this)); 
	m_x*=rc; 
	m_y*=rc; 
	m_z*=rc;
	return *this;
}

COS_Vector3& COS_Vector3::funRotateX(const float x)
{
	if (x)
	{
		D3DXMATRIX multiMatrix(1.0f	,.0f		,.0f		,.0f
													,.0f	,cosf(x),sinf(x)	,.0f
													,.0f	,-sinf(x),cosf(x),.0f
													,.0f	,.0f		,.0f		,1.0f);
		OS_MatrixMultiply(&g_matrix,&g_matrix,&multiMatrix);

// 		float tmp_y = m_y*cos(x)-m_z*sin(x);
// 		float tmp_z = m_y*sin(x)+m_z*cos(x);

		m_y = g_matrix._12;
		m_z = g_matrix._13;
	}
	return *this;
}

COS_Vector3& COS_Vector3::funRotateY(const float y)
{
	if (y)
	{
		D3DXMATRIX multiMatrix(cosf(y)	,.0f	,-sinf(y),.0f
													,.0f		,1.0f	,.0f		,.0f
													,sinf(y)	,.0f	,cosf(y),.0f
													,.0f		,.0f	,.0f		,1.0f);
		OS_MatrixMultiply(&g_matrix,&g_matrix,&multiMatrix);

// 		float tmp_z = m_z*cos(y)-m_x*sin(y);
// 		float tmp_x = m_z*sin(y)+m_x*cos(y);

		m_x = g_matrix._11;
		m_z = g_matrix._13;
	}
	return *this;
}

COS_Vector3& COS_Vector3::funRotateZ(const float z)
{
	if (z)
	{
		D3DXMATRIX multiMatrix(cosf(z)	,sinf(z)	,.0f	,.0f
													,-sinf(z),cosf(z),.0f	,.0f
													,.0f		,.0f		,1.0f	,.0f
													,.0f		,.0f		,.0f	,1.0f);
		OS_MatrixMultiply(&g_matrix,&g_matrix,&multiMatrix);

// 		float tmp_x = m_x*cos(z)-m_y*sin(z);
// 		float tmp_y = m_x*sin(z)+m_y*cos(z);

		m_x = g_matrix._11;
		m_y = g_matrix._12;
	}
	return *this;
}
//��X����תx����,��Y����תy����,��Z����תz����
COS_Vector3&	COS_Vector3::funRotate(const float x,const float y,const float z)
{
	funRotateX(x);
	funRotateY(y);
	funRotateZ(z);

	return *this;
}
//������� a��b��cΪA�ĵ�λ��������(a,b,c)
COS_Vector3& COS_Vector3::funRotate(const COS_Vector3& vA,COS_Vector3& dA,const float rA)
{
	dA.funNormalize();
	//ƽ��ԭ�㵽A����
	D3DXMATRIX tA(1.0f			,0.0f				,0.0f			,0.0f
								,0.0f			,1.0f				,0.0f			,0.0f
								,0.0f			,0.0f				,1.0f			,0.0f
								,-vA.m_x	,-vA.m_y		,-vA.m_z	,1.0f);
	OS_MatrixMultiply(&g_matrix,&g_matrix,&tA);
	//��ƽ��AO'A'��X����תr1�Ƕ�,r1�Ƕ���O'A��YOZƽ���ϵ�ͶӰO'A'��Z��н�
	//v*v=a*a+b*b�� cos(r1)=c/v�� sin(r1)=b/v 
	float _1_v = OS_InvSqrt(dA.m_x*dA.m_x+dA.m_y*dA.m_y);
	float cosa = dA.m_z*_1_v;
	float sina = dA.m_y*_1_v;
	D3DXMATRIX rX(1.0f	,0.0f		,0.0f			,0.0f
								,0.0f,cosa		,sina			,0.0f
								,0.0f,-sina	,cosa		,0.0f
								,0.0f,0.0f		,0.0f			,1.0f);
	OS_MatrixMultiply(&g_matrix,&g_matrix,&rX);
	//����O'A��Y����תr2�Ƕ���z���غϣ���z����ԭ�㿴��r2��˳ʱ�뷽������ȡ��ֵ
	//sin(r2)=a/1 cos(r2)=v/1 ��λ����ģ��
	float v = 1/_1_v;
	D3DXMATRIX rY(v				,0.0f	,dA.m_x	,0.0f
								,0.0f			,1.0f	,0.0f			,0.0f
								,-dA.m_x	,0.0f	,v				,0.0f
								,0.0f			,0.0f	,0.0f			,1.0f);
	OS_MatrixMultiply(&g_matrix,&g_matrix,&rY);
	//��������ϵ����Z����תrA
	funRotateZ(rA);
	//��rY��rX��tA����任
	D3DXMATRIX rY_1(v				,0.0f	,-dA.m_x	,0.0f
									,0.0f			,1.0f	,0.0f			,0.0f
									,dA.m_x	,0.0f	,v				,0.0f
									,0.0f			,0.0f	,0.0f			,1.0f);
	D3DXMATRIX rX_1(1.0f	,0.0f		,0.0f			,0.0f
									,0.0f,cosa		,-sina		,0.0f
									,0.0f,sina		,cosa		,0.0f
									,0.0f,0.0f		,0.0f			,1.0f);
	D3DXMATRIX tA_1(1.0f			,0.0f				,0.0f			,0.0f
									,0.0f			,1.0f				,0.0f			,0.0f
									,0.0f			,0.0f				,1.0f			,0.0f
									,vA.m_x	,vA.m_y		,vA.m_z	,1.0f);
	OS_MatrixMultiply(&g_matrix,&g_matrix,&rY_1);
	OS_MatrixMultiply(&g_matrix,&g_matrix,&rX_1);
	OS_MatrixMultiply(&g_matrix,&g_matrix,&tA_1);

	m_x = g_matrix._11;
	m_y = g_matrix._12;
	m_z = g_matrix._13;

	return *this;
}

COS_Vector3& COS_Vector3::funTranslate(const float tX,const float tY,const float tZ)
{
	m_x += tX;
	m_y += tY;
	m_z += tZ;
	fillMartix();
	return *this;
}

COS_Vector3& COS_Vector3::funTranslate(const float tT)
{
	return funTranslate(tT,tT,tT);
}

COS_Vector3& COS_Vector3::funScaling(const float sX,const float sY,const float sZ)
{
	m_x *= sX;
	m_y *= sY;
	m_z *= sZ;
	fillMartix();
	return *this;
}

COS_Vector3& COS_Vector3::funScaling(const float sS)
{
	return funScaling(sS,sS,sS);
}

