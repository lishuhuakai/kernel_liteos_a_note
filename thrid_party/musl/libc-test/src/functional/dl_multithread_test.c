/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <dlfcn.h>
#include <pthread.h>

#include "test.h"

const char* g_libPath = "libdl_multithread_test_dso.so";
static bool stop = false;

static void* dl_test()
{
	while (!stop) {
		void* handle = dlopen(g_libPath, RTLD_GLOBAL);
		if (!handle) {
			t_error("dlopen(name=%s, mode=%d) failed: %s\n", g_libPath, RTLD_GLOBAL, dlerror());
		}
		dlclose(handle);
	}
    return 0;
}

int main()
{
	pthread_t thd1;
	pthread_t thd2;
	pthread_t thd3;
	pthread_t thd4;
	pthread_t thd5;
	void *res;
	pthread_create(&thd1, NULL, dl_test, NULL);
	pthread_create(&thd2, NULL, dl_test, NULL);
	pthread_create(&thd3, NULL, dl_test, NULL);
	pthread_create(&thd4, NULL, dl_test, NULL);
	pthread_create(&thd5, NULL, dl_test, NULL);
	sleep(3);
	stop = true;
	pthread_join(thd1, &res);
	pthread_join(thd2, &res);
	pthread_join(thd3, &res);
	pthread_join(thd4, &res);
	pthread_join(thd5, &res);
}
