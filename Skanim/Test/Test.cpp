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

    root1->addChild(c1);
    root1->addChild(c2);
    c1->addChild(c3);
    c3->addChild(c4);
    c3->addChild(c5);
    c3->addChild(c6);
    c3->addChild(c7);

    Joint *root2 = new Joint(std::move(*root1));
    root2->setName(L"root2");

    wprintf(L"%s\n", c3->getChild(1)->getName().c_str());

    delete root1;
    delete root2;

    delete MemoryConfig::getGlobalAllocManager();

    return 0;
}

