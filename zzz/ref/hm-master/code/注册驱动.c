#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include "hdf_log.h"
#include "hdf_base.h"
#include "hdf_device_desc.h"

#define HDF_LOG_TAG "sample_driver"

#define SAMPLE_WRITE_READ 123

int32_t HdfSampleDriverDispatch(
    struct HdfDeviceObject *deviceObject, int id, struct HdfSBuf *data, struct HdfSBuf *reply)
{
    HDF_LOGE("%s: received cmd %d", __func__, id);
    if (id == SAMPLE_WRITE_READ) {
        const char *readData = HdfSbufReadString(data);
        if (readData != NULL) {
            HDF_LOGE("%s: read data is: %s", __func__, readData);
        }
        if (!HdfSbufWriteInt32(reply, INT32_MAX)) {
            HDF_LOGE("%s: reply int32 fail", __func__);
        }
        return HdfDeviceSendEvent(deviceObject, id, data);
    }
    return HDF_FAILURE;
}

void HdfSampleDriverRelease(struct HdfDeviceObject *deviceObject)
{
    // release resources here
    return;
}

int HdfSampleDriverBind(struct HdfDeviceObject *deviceObject)
{
    if (deviceObject == NULL) {
        return HDF_FAILURE;
    }
    static struct IDeviceIoService testService = {
        .Dispatch = HdfSampleDriverDispatch,
    };
    deviceObject->service = &testService;
    return HDF_SUCCESS;
}

int HdfSampleDriverInit(struct HdfDeviceObject *deviceObject)
{
    if (deviceObject == NULL) {
        HDF_LOGE("%s::ptr is null!", __func__);
        return HDF_FAILURE;
    }
    HDF_LOGE("Sample driver Init success");
    return HDF_SUCCESS;
}

struct HdfDriverEntry g_sampleDriverEntry = {
    .moduleVersion = 1,
    .moduleName = "sample_driver",
    .Bind = HdfSampleDriverBind,
    .Init = HdfSampleDriverInit,
    .Release = HdfSampleDriverRelease,
};

HDF_INIT(g_sampleDriverEntry);


//--------注册驱动  HDF_INIT(g_sampleDriverEntry);
//--------展开后    __attribute__((used)) g_sampleDriverEntryHdfEntry __attribute__((section(".hdf.driver"))) = (size_t)(&(g_sampleDriverEntry))  


//--------注册驱动  HDF_INIT(g_HelloDevEntry);
//--------展开后    
const size_t __attribute__((used)) g_HelloDevEntryHdfEntry __attribute__((section(".hdf.driver"))) = (size_t)(&(g_sampleDriverEntry))  