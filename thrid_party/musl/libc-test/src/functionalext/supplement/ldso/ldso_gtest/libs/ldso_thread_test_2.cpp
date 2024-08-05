extern "C" void LdsoThreadTest1();
extern "C" void LdsoThreadTest();

extern "C" void LdsoThreadTest2()
{
    LdsoThreadTest1();
    LdsoThreadTest();
}