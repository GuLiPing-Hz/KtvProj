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
	//�޶�����
	void		funClamp(const float max);
	//���
	float		funDot(const COS_Vector3 *v);
	//ģ��
	float		funLength();
	//����нǻ���
	float		funAngle(const COS_Vector3 *v = 0) const;
	//��λ����
	COS_Vector3&	funNormalize();
	//��X����תx����,��Y����תy����,��Z����תz����,��������ϵ
	COS_Vector3& funRotateX(const float x);
	COS_Vector3& funRotateY(const float y);
	COS_Vector3& funRotateZ(const float z);
	COS_Vector3& funRotate(const float x,const float y,const float z);
	//�������ᣨ��vAΪ���㣬dAΪ������תrA����
	COS_Vector3& funRotate(const COS_Vector3& vA,COS_Vector3& dA,const float rA);
	//�ƶ�����
	COS_Vector3& funTranslate(const float tX,const float tY,const float tZ);
	COS_Vector3& funTranslate(const float tT);
	//��С�仯
	COS_Vector3& funScaling(const float sX,const float sY,const float sZ);
	COS_Vector3& funScaling(const float sS);
private:
	void fillMartix();
public:
	float m_x;
	float m_y;
	float m_z;
};
