#include "RVector.h"

bool RV3Colider::ColisionSphereToPlane(const Sphere &sphere, const Plane &plane, RVector3 *coliPos)
{
	//•½–Ê‚Æ‹…‚Ì’†S‚Æ‚Ì‹——£‚ğ‹‚ß‚é
	float dist = dot(sphere.center, plane.normal) - plane.distance;
	//‹——£‚Ìâ‘Î’l‚ª”¼Œa‚æ‚è‘å‚«‚¯‚ê‚Î‚ ‚½‚Á‚Ä‚È‚¢
	if (fabsf(dist) > sphere.rad) { return false; }
	//‚ ‚½‚Á‚Ä‚¢‚éê‡‚Í‹^—Œğ“_‚ğŒvZ
	*coliPos = -dist * plane.normal + sphere.center;
	//Õ“Ë
	return true;
}

const RVector3 Rv3Ease::lerp(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return start * (1.0f - t) + end * t;
}

const RVector3 Rv3Ease::InQuad(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return start * (1.0f - t * t) + end * (float)pow(t, 2);
}

const RVector3 Rv3Ease::OutQuad(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return start * (1.0f - (1.0f - (1.0f - t) * (1.0f - t))) + end * (1.0f - (1.0f - t) * (1.0f - t));
}

const RVector3 Rv3Ease::InOutQuad(const RVector3 &s, const RVector3 &e, const float t)
{
	RVector3 start = s;
	RVector3 end = e;
	return t < 0.5f ? (start * (1.0f - t)) + end * (t * t) : start * (1.0f - t) + end * (1 - (float)pow(-2 * t + 2, 2) / 2);
}
