#pragma once

#include "../../tools/includes/glm/glm.hpp"

/**
*	Класс служит для численного решения дифференциальных уравнений
*/
template <typename T>
class CRungeKutta
{
private:
	int N;						/**<размерность системы		*/
    
	T* YY;					/**< внутренние переменные	*/
	T* Y1;					/**< внутренние переменные	*/
	T* Y2;					/**< внутренние переменные	*/
	T* Y3;					/**< внутренние переменные	*/
	T* Y4;					/**< внутренние переменные	*/
	double t;					/**< текущее время			*/
	double dt;

protected:
	T* Y;					/**< Y[0] - само решение, Y[i] - i-тая производная решения */
public:
	CRungeKutta(const int N)
	{
		this->N = N;				// сохранить размерность системы

		if (N < 1)
		{
			this->N = -1;			// если размерность меньше единицы, то установить флаг ошибки
			return;					// и выйти из конструктора
		}

		Y = new T[N];		// создать вектор решения
		YY = new T[N];		// и внутренних решений
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

		F(t, Y, Y1);							// рассчитать Y1
		for (i = 0; i < N; i++)
		{
			YY[i] = Y[i] + Y1[i] * (dt / 2.0);
		}
		F(t + dt / 2.0, YY, Y2);				// рассчитать Y2
		for (i = 0; i < N; i++)
		{
			YY[i] = Y[i] + Y2[i] * (dt / 2.0);
		}
		F(t + dt / 2.0, YY, Y3);				// рассчитать Y3

		for (i = 0; i < N; i++)
		{
			YY[i] = Y[i] + Y3[i] * dt;
		}
		F(t + dt, YY, Y4);					 // рассчитать Y4

		for (i = 0; i < N; i++)
		{
			Y[i] = Y[i] + (dt / 6.0) * (Y1[i] + 2.0 * Y2[i] + 2.0 * Y3[i] + Y4[i]); // рассчитать решение на новом шаге	
		}

		t = t + dt;							// увеличить шаг
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


 