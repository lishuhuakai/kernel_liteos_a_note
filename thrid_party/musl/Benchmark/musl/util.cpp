/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "util.h"
#if defined __APPLE__
#include "malloc/malloc.h"
#else
#include "malloc.h"
#endif

#include <err.h>
#include <cmath>
#include <sched.h>
#include <cstdio>
#include <cwchar>
#include <cstdlib>

void OpenTcache()
{
#if not defined __APPLE__
    mallopt(M_OHOS_CONFIG, M_TCACHE_PERFORMANCE_MODE);
    mallopt(M_OHOS_CONFIG, M_ENABLE_OPT_TCACHE);
    mallopt(M_SET_THREAD_CACHE, M_THREAD_CACHE_ENABLE);
    mallopt(M_DELAYED_FREE, M_DELAYED_FREE_ENABLE);
#endif
}
char* AlignUpMemoy(char* origPtr, size_t alignment)
{
    if ((alignment & (alignment - 1)) != 0) {
        perror("alignment is not a power of two.");
    }

    uintptr_t ptr = reinterpret_cast<uintptr_t>(origPtr);
    if (alignment > 0) {
        ptr = ((ptr + (alignment - 1)) & (~(alignment - 1)));
    }

    return reinterpret_cast<char*>(ptr);
}

char* GetAlignedPtr(std::vector<char>* buf, size_t alignment, size_t nbytes)
{
    buf->resize(nbytes + 2 * alignment);
    return AlignUpMemoy(buf->data(), alignment);
}

wchar_t* GetAlignedPtr(std::vector<wchar_t>* buf, size_t alignment, size_t nchars)
{
    buf->resize(nchars + ceil((2 * alignment) / sizeof(wchar_t)));
    return reinterpret_cast<wchar_t*>(AlignUpMemoy(reinterpret_cast<char*>(buf->data()), alignment));
}

char* GetAlignedPtrFilled(std::vector<char>* buf, size_t alignment, size_t nbytes, char fillByte)
{
    char* bufAligned = GetAlignedPtr(buf, alignment, nbytes);
    memset(bufAligned, fillByte, nbytes);
    return bufAligned;
}

wchar_t* GetAlignedPtrFilled(std::vector<wchar_t>* buf, size_t alignment, size_t nbytes, wchar_t fillByte)
{
    wchar_t* bufAligned = GetAlignedPtr(buf, alignment, nbytes);
    wmemset(bufAligned, fillByte, nbytes);
    return bufAligned;
}

char* ReadJsonFile(const char *fileName)
{
    FILE *jsonFile = nullptr;
    char *contentBuffer = nullptr;
    long jsonFileLength = 0;
    size_t readFileContent = 0;

    jsonFile = fopen(fileName, "rb");
    if (jsonFile == nullptr) {
        return nullptr;
    }

    /* get the length */
    if (fseek(jsonFile, 0, SEEK_END) != 0) {
        fclose(jsonFile);
        return nullptr;
    }
    jsonFileLength = ftell(jsonFile);
    if (jsonFileLength < 0) {
        fclose(jsonFile);
        return nullptr;
    }
    if (fseek(jsonFile, 0, SEEK_SET) != 0) {
        fclose(jsonFile);
        return nullptr;
    }

    contentBuffer = (char*)malloc((size_t)jsonFileLength + sizeof(""));
    if (contentBuffer == nullptr) {
        fclose(jsonFile);
        return nullptr;
    }

    /* read the json file into memory */
    readFileContent = fread(contentBuffer, sizeof(char), (size_t)jsonFileLength, jsonFile);
    if ((long)readFileContent != jsonFileLength) {
        free(contentBuffer);
        contentBuffer = nullptr;
        fclose(jsonFile);
        return contentBuffer;
    }
    contentBuffer[readFileContent] = '\0';

    return contentBuffer;
}
