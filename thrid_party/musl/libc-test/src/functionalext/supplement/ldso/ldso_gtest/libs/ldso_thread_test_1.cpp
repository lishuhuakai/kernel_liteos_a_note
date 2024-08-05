namespace {

class ThreadTest {
public:
    explicit ThreadTest(bool* value) : localValue_(value) {}
    ~ThreadTest()
    {
        *localValue_ = true;
    }

private:
    bool* localValue_;
};

}; // namespace

extern "C" void SetThreadLocalValue01(bool* value)
{
    thread_local ThreadTest testValue(value);
}

extern "C" void LdsoThreadTest1() {}
