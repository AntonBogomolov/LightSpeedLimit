#pragma once
#include <cmath>
#include "SyncHelper.h"

class CTexture;

class CHeightMap
{
private:
	unsigned int width;
	unsigned int height;
	unsigned int dataLen; 
	float* data;

	mutable bool isCalcAverage;
	mutable float averageValue;
	mutable float minValue;
	mutable float maxValue;

private:
	
public:
	float& operator[](const unsigned int index)
	{
		//if (index >= 0 && index < dataLen)
		//{
			return data[index];
		//}
		//else
		//{
		//	return data[0];
		//}
	}

	void blurHeightMap(unsigned int blurSize)
	{
		float* oldData = new float[dataLen];
		memcpy(oldData, data, dataLen*sizeof(float));

		int pxCnt = 4 * blurSize;
		for (unsigned int y = 0; y < height; y++)
		{
			for (unsigned int x = 0; x < width; x++)
			{
				float sum = 0;
				for (unsigned int i = 1; i < blurSize; i++)
				{
					unsigned int brx = x + i;
					int blx = x - i;
					unsigned int buy = y + i;
					int bdy = y - i;

					if (brx >= width)  brx = width  - 1;
					if (buy >= height) buy = height - 1;
					if (blx <  0) blx = 0;
					if (bdy <  0) bdy = 0;
					
					sum += oldData[y*width + brx];
					sum += oldData[y*width + blx];
					sum += oldData[buy*width + x];
					sum += oldData[bdy*width + x];
				}
				data[y*width + x] = sum / pxCnt;
			}
		}

		delete [] oldData;
	}

	void calcAverageValues() const
	{
		averageValue = 0.0f;
		minValue = 1.0f;
		maxValue = -1.0f;
		for (unsigned int i = 0; i < dataLen; i++)
		{
			float value = data[i];
			averageValue += value;
			if (value > maxValue) maxValue = value;
			if (value < minValue) minValue = value;
		}
		averageValue /= dataLen;

		isCalcAverage = true;
	}
	
	const float* getDataPtr() const
	{
		return data;
	}

	void put(const unsigned int x, const unsigned int y, float value)
	{
		unsigned int place = y * width + x;
		if (place >= dataLen) return;

		data[place] = value;
	}

	const float get(const unsigned int x, const unsigned int y) const
	{
		unsigned int place = y * width + x;
		if (place >= dataLen) return data[0];

		return data[place];
	}

	const unsigned int getWidth() const
	{
		return width;
	}

	const unsigned int getHeight() const
	{
		return height;
	}

	const float getAverageValue() const
	{
		return averageValue;
	}

	const float getMaxValue() const
	{
		return maxValue;
	}

	const float getMinValue() const
	{
		return minValue;
	}

	const float getSlope(const unsigned int x, const unsigned int y) const
	{
		const float eps = 0.005f;

		unsigned int place = y * width + x;
		if (place >= dataLen) return 0.0f;
		if (x <= 1 || x >= width - 1 || y <= 1 || y >= height - 1) return 0.0f;

		float sum = 0.0f;
		sum += data[(y + 0) * width + x - 1];
		sum += data[(y + 0) * width + x + 1];
		sum += data[(y - 1) * width + x - 1];
		sum += data[(y - 1) * width + x + 0];
		sum += data[(y - 1) * width + x + 1];
		sum += data[(y + 1) * width + x - 1];
		sum += data[(y + 1) * width + x + 0];
		sum += data[(y + 1) * width + x + 1];

		float slope = data[y * width + x] - (sum / 8);		
		
		return slope;
	}

	const float getBorderValue(const float border) const 
	{
		if (!isCalcAverage) calcAverageValues();

		const float eps = 0.00001f;

		float borderValue = 0.0f;

		const float len = border - 0.5f;
		const float positiveLen = maxValue - averageValue;
		const float negativeLen = averageValue - minValue;

		borderValue = averageValue;
		if (len > 0.0f + eps) borderValue = averageValue + positiveLen * (len * 2.0f);
		if (len < 0.0f - eps) borderValue = averageValue + negativeLen * (len * 2.0f);
		
		return borderValue;
	}

	void pow(const float power)
	{
		for (unsigned int i = 0; i < dataLen; i++)
		{
			data[i] = powf(data[i], power);
		}
	}
	
	CHeightMap(const unsigned int width, const unsigned int height)
	{
		this->width  = width;
		this->height = height;

		dataLen = width * height;
		data = new float[dataLen];
		for (unsigned int i = 0; i < dataLen; i++)
		{
			data[i] = 0.0f;
		}
		isCalcAverage = false;
	}
	
	~CHeightMap()
	{
		delete[] data;
	}
};

class CTextureGenerator
{	
private:
	void diamondSquareStep(CHeightMap* heightMap, const unsigned int squareLen);
	void diamondStep(CHeightMap* heightMap, const unsigned int squareLen, const unsigned int midX, const unsigned int midY, const int mod = 0);
protected:
	virtual CTexture* generateTexture(const unsigned int width, const unsigned int height, const int bpp = 4) = 0;
	CHeightMap* startDiamondSquareGenerator(const unsigned int sideLen, const unsigned int seed);
public:
	CTextureGenerator();
	virtual ~CTextureGenerator();
};