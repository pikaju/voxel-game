#include "FrustumCuller.h"


FrustumCuller::FrustumCuller() : m_planes()
{
}

FrustumCuller::~FrustumCuller()
{
}

void FrustumCuller::extract(const vt::Mat4f& m)
{
	m_planes[0] = Plane(m.get(0, 3) + m.get(0, 0), m.get(1, 3) + m.get(1, 0), m.get(2, 3) + m.get(2, 0), m.get(3, 3) + m.get(3, 0));
	m_planes[1] = Plane(m.get(0, 3) - m.get(0, 0), m.get(1, 3) - m.get(1, 0), m.get(2, 3) - m.get(2, 0), m.get(3, 3) - m.get(3, 0));
	m_planes[2] = Plane(m.get(0, 3) + m.get(0, 1), m.get(1, 3) + m.get(1, 1), m.get(2, 3) + m.get(2, 1), m.get(3, 3) + m.get(3, 1));
	m_planes[3] = Plane(m.get(0, 3) - m.get(0, 1), m.get(1, 3) - m.get(1, 1), m.get(2, 3) - m.get(2, 1), m.get(3, 3) - m.get(3, 1));
	m_planes[4] = Plane(m.get(0, 3) + m.get(0, 2), m.get(1, 3) + m.get(1, 2), m.get(2, 3) + m.get(2, 2), m.get(3, 3) + m.get(3, 2));
	m_planes[5] = Plane(m.get(0, 3) - m.get(0, 2), m.get(1, 3) - m.get(1, 2), m.get(2, 3) - m.get(2, 2), m.get(3, 3) - m.get(3, 2));
}

bool FrustumCuller::sphereInFrustum(const vt::Vec3f& position, float radius) const
{
	for (unsigned int i = 0; i < 6; i++) {
		if (m_planes[i].m_a * position.x + m_planes[i].m_b * position.y + m_planes[i].m_c * position.z + m_planes[i].m_d <= -radius) {
			return false;
		}
	}
	return true;
}