#pragma once

#include <d3d9.h>
#include <d3dx9math.h>

bool OS_MatrixMultiply(D3DXMATRIX* pOut,const D3DXMATRIX* pM1,const D3DXMATRIX* pM2);

class COS_Vector3
{
public:
	COS_Vector3(void);
	COS_Vector3(const COS_Vector3& v);
	COS_Vector3(float x,float y,float z);
	virtual ~COS_Vector3(void);

	COS_Vector3& operator-();

	COS_Vector3 operator+(const COS_Vector3& v);
	COS_Vector3 operator-(const COS_Vector3& v);
	COS_Vector3 operator*(const float v);
	COS_Vector3 operator/(const float v);

	COS_Vector3& operator+=(const COS_Vector3& v);
	COS_Vector3& operator-=(const COS_Vector3& v);

	bool operator==(const COS_Vector3& v);
	bool operator!=(const COS_Vector3& v);

	COS_Vector3& operator=(const COS_Vector3& v);
	//限定长度
	void		funClamp(const float max);
	//点积
	float		funDot(const COS_Vector3 *v);
	//模长
	float		funLength();
	//计算夹角弧度
	float		funAngle(const COS_Vector3 *v = 0) const;
	//单位向量
	COS_Vector3&	funNormalize();
	//绕X轴旋转x弧度,绕Y轴旋转y弧度,绕Z轴旋转z弧度,右手坐标系
	COS_Vector3& funRotateX(const float x);
	COS_Vector3& funRotateY(const float y);
	COS_Vector3& funRotateZ(const float z);
	COS_Vector3& funRotate(const float x,const float y,const float z);
	//绕任意轴（以vA为顶点，dA为方向）旋转rA弧度
	COS_Vector3& funRotate(const COS_Vector3& vA,COS_Vector3& dA,const float rA);
	//移动坐标
	COS_Vector3& funTranslate(const float tX,const float tY,const float tZ);
	COS_Vector3& funTranslate(const float tT);
	//大小变化
	COS_Vector3& funScaling(const float sX,const float sY,const float sZ);
	COS_Vector3& funScaling(const float sS);
private:
	void fillMartix();
public:
	float m_x;
	float m_y;
	float m_z;
};
