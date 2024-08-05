/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#ifdef OHOS_SOCKET_HOOK_ENABLE
#include "musl_socket.h"
#include <sys/socket.h>
#include "common_def.h"
#include "musl_socket_preinit_common.h"

int socket(int domain, int type, int protocol)
{
	volatile const struct SocketDispatchType* dispatch = get_socket_dispatch();
	if (__predict_false(dispatch != NULL)) {
		int ret = dispatch->socket(domain, type, protocol);
		return ret;
	}
	int result = MuslSocket(socket)(domain, type, protocol);
	return result;
}

#endif
