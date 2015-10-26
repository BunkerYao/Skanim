#include "stdafx.h"
#include "skanim.h"

using namespace Skanim;

int main()
{
    Vector3 axis1(12.0f, -55.0f, 3.1f);
    Vector3 axis2(1.0f, -77.0f, 3.2f);
    axis1.normalize();
    axis2.normalize();
    Quaternion qa(axis1, Math::PI() * 0.25f);
    Quaternion qb(axis2, Math::PI() * 1.34f);

    Transform trs1(1.0f, qa, Vector3(1.0f, -4.0f, 10.0f));
    Transform trs2(2.0f, qb, Vector3(-32.0f, 12.0f, 1.0f));

    Transform trs3 = Transform::combine(trs1, trs2);
    wprintf(L"%f %s %s\n", trs3.getScale(), trs3.getRotation().toString().c_str(), trs3.getTranslation().toString().c_str());

    MatrixUA4 m1 = trs1.toMatrix();
    MatrixUA4 m2 = trs2.toMatrix();
    trs3 = Transform::fromMatrix(m1 * m2);
    wprintf(L"%f %s %s\n", trs3.getScale(), trs3.getRotation().toString().c_str(), trs3.getTranslation().toString().c_str());

    return 0;
}

