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

#ifdef FEATURE_ICU_LOCALE
#include <stdio.h>
#include <dlfcn.h>
#include <musl_log.h>
#include <string.h>
#include "locale_impl.h"

#define ICU_UC_SO "libhmicuuc.z.so"
#define ICU_I18N_SO "libhmicui18n.z.so"

static void *g_icuuc_handle = NULL;
static void *g_icui18n_handle = NULL;
hidden struct icu_opt_func g_icu_opt_func = { NULL };
static int dlopen_fail_flag = 0;

static void *get_icu_handle(icu_so_type type, const char *symbol_name)
{
	void *cur_handle;
	char *cur_so;
	if (type == ICU_UC) {
		cur_handle = g_icuuc_handle;
		cur_so = ICU_UC_SO;
	} else {
		cur_handle = g_icui18n_handle;
		cur_so = ICU_I18N_SO;
	}

	if (!cur_handle && !dlopen_fail_flag) {
		cur_handle = dlopen(cur_so, RTLD_LOCAL);
	}
	if (!cur_handle) {
		dlopen_fail_flag = 1;
		MUSL_LOGE("dlopen icu so for musl locale fail %{public}s", dlerror());
		return NULL;
	}
	return dlsym(cur_handle, symbol_name);
}

static char *get_icu_version_num()
{
	if (!(g_icu_opt_func.get_icu_version)) {
		g_icu_opt_func.get_icu_version = get_icu_handle(ICU_UC, ICU_GET_VERSION_NUM_SYMBOL);
	}
	if (g_icu_opt_func.get_icu_version) {
		return g_icu_opt_func.get_icu_version();
	} else {
		return "";
	}
}

void get_icu_symbol(icu_so_type type, void **icu_symbol_handle, const char *symbol_name)
{
	if (!(*icu_symbol_handle)) {
		char *icu_version = get_icu_version_num();
		char *valid_icu_symbol = malloc(strlen(symbol_name) + strlen(icu_version) + 2);
		sprintf(valid_icu_symbol, "%s_%s", symbol_name, icu_version);
		*icu_symbol_handle = get_icu_handle(type, valid_icu_symbol);
		free(valid_icu_symbol);
	}
}

void set_icu_directory()
{
	if (!(g_icu_opt_func.set_data_directory)) {
		g_icu_opt_func.set_data_directory = get_icu_handle(ICU_UC, ICU_SET_DATA_DIRECTORY_SYMBOL);
		if (g_icu_opt_func.set_data_directory) {
			g_icu_opt_func.set_data_directory();
		}
	}
}

/* ICU methods don't need charset for locale, process the given locale name */
void get_valid_icu_locale_name(const char *name, const char *icu_name, int icu_name_len)
{
	char *pos = memchr(name, '.', strlen(name));
	int valid_len;
	if (pos) {
		valid_len = pos - name;
	} else {
		valid_len = strlen(name);
	}
	if (icu_name_len > valid_len) {
		strncpy(icu_name, name, valid_len);
	}
}
#endif
