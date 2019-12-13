#pragma once

#include <vt/math/Vec3.h>
#include <vt/math/Mat4.h>

struct Plane
{
	float m_a;
	float m_b;
	float m_c;
	float m_d;

	Plane(float a = 0.0f, float b = 0.0f, float c = 0.0f, float d = 0.0f) : m_a(a), m_b(b), m_c(c), m_d(d) { normalize(); }

	void normalize()
	{
		float l = sqrt(m_a * m_a + m_b * m_b + m_c * m_c);
		m_a /= l;
		m_b /= l;
		m_c /= l;
		m_d /= l;
	}
};

class FrustumCuller
{
public:
	FrustumCuller();
	~FrustumCuller();

	void extract(const vt::Mat4f& m);
	bool sphereInFrustum(const vt::Vec3f& position, float radius) const;
private:
	Plane m_planes[6];
};

