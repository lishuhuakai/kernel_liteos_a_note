#include <dlfcn.h>
#include <iostream>

#define SO_CLOSE_RECURSIVE "libdlclose_recursive.so"

extern "C" {
int Add(int a, int b)
{
    return a + b;
}
}

class BarIns {
public:
	BarIns()
	{
            handle = dlopen(SO_CLOSE_RECURSIVE, RTLD_LOCAL);
            if (!handle)
            {
                std::cerr << "dlopen(name=" << SO_CLOSE_RECURSIVE \
                    << ",mode=" << RTLD_LOCAL \
                    << ",failed: " << dlerror() << std::endl;
            }
            std::cerr << "open: " << SO_CLOSE_RECURSIVE << " successfully" << std::endl;
	}

        ~BarIns()
        {
	    if (handle != nullptr)
	    {
                dlclose(handle);
            }
            std::cerr << "close: " << SO_CLOSE_RECURSIVE << " successfully" << std::endl;
	}

	static BarIns& GetInstance()
	{
		return bi;
	}

private:
	static BarIns bi;
	void *handle {nullptr};
};

BarIns BarIns::bi;
