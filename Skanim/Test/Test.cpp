#include "stdafx.h"
#include "skanim.h"

using namespace Skanim;

int main()
{
    MemoryConfig::setGlobalAllocManager(new DefaultAllocManager);

    Joint *root1 = new Joint(L"root1", 0);
    Joint *c1 = new Joint(L"c1", 0);
    Joint *c2 = new Joint(L"c2", 0);
    Joint *c3 = new Joint(L"c3", 0);
    Joint *c4 = new Joint(L"c4", 0);
    Joint *c5 = new Joint(L"c5", 0);
    Joint *c6 = new Joint(L"c6", 0);
    Joint *c7 = new Joint(L"c7", 0);
    Joint *c8 = new Joint(L"c8", 0);
    Joint *c9 = new Joint(L"c9", 0);

    Skeleton *skeleton = new Skeleton(root1);

    root1->addChild(c1);
    root1->addChild(c2);
    c1->addChild(c3);
    c3->addChild(c4);
    c3->addChild(c5);
    c3->addChild(c6);
    c6->addChild(c9);
    c3->addChild(c7);
    c1->addChild(c8);

    Skeleton::JointPtrIterator iterator = skeleton->getRootIterator();

    while (iterator.hasNext()) {
        Joint *p_joint = *iterator.current();
        wprintf(L"%s\n", p_joint->getName().c_str());
        iterator.moveNext();
    }

    Joint *p_joint = skeleton->findJoint(L"c9");

    Pose *pose = new Pose(5);
    (*pose)[0] = Transform(1.0f, Quaternion(Vector3(0.0f, 1.0f, 0.0f), Math::HALF_PI()), Vector3(1.0f, 10.0f, -10.0f));
    (*pose)[1] = Transform(1.0f, Quaternion(Vector3(1.0f, 0.0f, 0.0f), Math::HALF_PI()), Vector3(7.0f, -2.0f, -10.0f));

    skeleton->setPose(*pose);

    delete pose;

    delete skeleton;

    delete MemoryConfig::getGlobalAllocManager();

    return 0;
}

