#include "hdf_log.h"
#include "osal_mem.h"
#include "hdf_io_service_if.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char **argv)
{

	if (argc != 2)
	{
		printf("Usage: %s <service | /dev/hello>\n", argv[0]);
		return -1;
	}

	if (strcmp(argv[1], "service") == 0)
	{
		char *serviceName = "hello_service";
		int ret;	
		int val;
		
	    struct HdfIoService *service = HdfIoServiceBind(serviceName, 0);
	    if (service == NULL) {
	        printf("Failed to get service %s", serviceName);
	        return 0;
	    }

	    struct HdfSBuf *data = HdfSBufObtainDefaultSize();
	    if (data == NULL) {
	        printf("Failed to obtain data");
	        return HDF_FAILURE;
	    }

	    struct HdfSBuf *reply = HdfSBufObtainDefaultSize();
	    if (reply == NULL) {
	        printf("Failed to obtain replay");
	        return HDF_FAILURE;
	    }

             // 传入数据 data  传出数据 reply
	    ret = service->dispatcher->Dispatch(&service->object, 0, data, reply);
	    if (ret != HDF_SUCCESS) {
	        printf("Failed to send service call");
	    }

		ret = HdfSbufReadInt32(reply, &val);
		printf("read val from %s: 0x%x\n", serviceName, val);
		
	    HdfSBufRecycle(data);
	    HdfSBufRecycle(reply);
		return ret;
	}
	else
	{
		int fd = open(argv[1], O_RDWR);
		int val;
		int ret;
		
		if (fd < 0) {
			printf("can not open %s\n", argv[1]);
			return -1;
		}

		ret = read(fd, &val, 4);
		if (ret != 4) {
			printf("can not read %s, ret = %d\n", argv[1], ret);
			return -1;
		}

		printf("read val from device %s: 0x%x\n", argv[1], val);
		return 0;
	}
}

