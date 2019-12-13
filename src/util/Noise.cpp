#include "Noise.h"

#include <numeric>
#include <random>
#include <algorithm>

Noise::Noise(unsigned int seed) : m_p()
{
	m_p.resize(256);
	std::iota(m_p.begin(), m_p.end(), 0);
	std::default_random_engine random(seed);
	std::shuffle(m_p.begin(), m_p.end(), random);
	m_p.insert(m_p.end(), m_p.begin(), m_p.end());
}

Noise::~Noise()
{
}

float Noise::noise(float x, float y) const
{
	int xx = (int)floorf(x) & 255;
	int yy = (int)floorf(y) & 255;
	x -= floorf(x);
	y -= floorf(y);
	float u = fade(x);
	float v = fade(y);
	return lerp(u, lerp(v, (float)m_p[m_p[xx] + yy], (float)m_p[m_p[xx] + yy + 1]), lerp(v, (float)m_p[m_p[xx + 1] + yy], (float)m_p[m_p[xx + 1] + yy + 1])) / 255.0f * 2.0f - 1.0f;
}

float Noise::noise(float x, float y, float z) const
{
	int xx = (int)floorf(x) & 255;
	int yy = (int)floorf(y) & 255;
	int zz = (int)floorf(z) & 255;
	x -= floorf(x);
	y -= floorf(y);
	z -= floorf(z);
	float u = fade(x);
	float v = fade(y);
	float w = fade(z);
	float ulerp00 = lerp(u, (float)m_p[m_p[m_p[zz] + yy] + xx], (float)m_p[m_p[m_p[zz] + yy] + xx + 1]);
	float ulerp01 = lerp(u, (float)m_p[m_p[m_p[zz] + yy + 1] + xx], (float)m_p[m_p[m_p[zz] + yy + 1] + xx + 1]);
	float ulerp10 = lerp(u, (float)m_p[m_p[m_p[zz + 1] + yy] + xx], (float)m_p[m_p[m_p[zz + 1] + yy] + xx + 1]);
	float ulerp11 = lerp(u, (float)m_p[m_p[m_p[zz + 1] + yy + 1] + xx], (float)m_p[m_p[m_p[zz + 1] + yy + 1] + xx + 1]);
	float vlerp0 = lerp(v, ulerp00, ulerp01);
	float vlerp1 = lerp(v, ulerp10, ulerp11);
	return lerp(w, vlerp0, vlerp1) / 255.0f * 2.0f - 1.0f;
}

float Noise::fade(float t)
{
	return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float Noise::lerp(float t, float a, float b)
{
	return a + t * (b - a);
}