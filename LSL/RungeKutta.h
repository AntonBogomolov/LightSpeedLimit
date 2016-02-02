#pragma once

#include "../../tools/includes/glm/glm.hpp"

/**
*	����� ������ ��� ���������� ������� ���������������� ���������
*/
template <typename T>
class CRungeKutta
{
private:
	int N;						/**<����������� �������		*/
    
	T* YY;					/**< ���������� ����������	*/
	T* Y1;					/**< ���������� ����������	*/
	T* Y2;					/**< ���������� ����������	*/
	T* Y3;					/**< ���������� ����������	*/
	T* Y4;					/**< ���������� ����������	*/
	double t;					/**< ������� �����			*/
	double dt;

protected:
	T* Y;					/**< Y[0] - ���� �������, Y[i] - i-��� ����������� ������� */
public:
	CRungeKutta(const int N)
	{
		this->N = N;				// ��������� ����������� �������

		if (N < 1)
		{
			this->N = -1;			// ���� ����������� ������ �������, �� ���������� ���� ������
			return;					// � ����� �� ������������
		}

		Y = new T[N];		// ������� ������ �������
		YY = new T[N];		// � ���������� �������
		Y1 = new T[N];
		Y2 = new T[N];
		Y3 = new T[N];
		Y4 = new T[N];
	}

	virtual ~CRungeKutta()
	{

	}

	virtual void F(const double t, const T* Y, T* FY) = 0;

	void SetModelInitCondition(const double t0, const T* Y0)
	{
		t = t0;
		int i;
		for (i = 0; i < N; i++)
		{
			Y[i] = Y0[i];
		}
	}

	const T* GetCurrModelData() const
	{
		return Y;
	}

	const double GetModelTime() const
	{
		return t;
	}
	
	void NextModelStep(const double dt)
	{
		if (dt < 0) return;
		int i;

		F(t, Y, Y1);							// ���������� Y1
		for (i = 0; i < N; i++)
		{
			YY[i] = Y[i] + Y1[i] * (dt / 2.0);
		}
		F(t + dt / 2.0, YY, Y2);				// ���������� Y2
		for (i = 0; i < N; i++)
		{
			YY[i] = Y[i] + Y2[i] * (dt / 2.0);
		}
		F(t + dt / 2.0, YY, Y3);				// ���������� Y3

		for (i = 0; i < N; i++)
		{
			YY[i] = Y[i] + Y3[i] * dt;
		}
		F(t + dt, YY, Y4);					 // ���������� Y4

		for (i = 0; i < N; i++)
		{
			Y[i] = Y[i] + (dt / 6.0) * (Y1[i] + 2.0 * Y2[i] + 2.0 * Y3[i] + Y4[i]); // ���������� ������� �� ����� ����	
		}

		t = t + dt;							// ��������� ���
	}

	const int GetModelDim() const
	{
		return N;
	}

	//void GetDirect(const double dt, const double* Coords, T* Result) const
	//{
	//	F(dt, Coords, Result);
	//}

	void SetModelTime(const double t0)
	{
		t = t0;
	}
};


 