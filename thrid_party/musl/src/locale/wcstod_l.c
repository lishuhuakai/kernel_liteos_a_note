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
#define _GNU_SOURCE
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include "locale_impl.h"

static int icu_wchar_trans(wchar_t *src, u_char *des, int des_size)
{
	get_icu_symbol(ICU_I18N, &(g_icu_opt_func.u_str_from_utf32), ICU_STR_FROM_UTF32_SYMBOL);

	if (!g_icu_opt_func.u_str_from_utf32) {
		return DLSYM_ICU_FAIL;
	}

	int icu_status = 0;
	void *res_tmp = g_icu_opt_func.u_str_from_utf32(des, des_size, NULL, src, -1, &icu_status);
	if (icu_status > 0) {
		return ICU_ERROR;
	}
	return DLSYM_ICU_SUCC;
}

static double icu_wcstod_l(char *loc_name, wchar_t *s, int *cur_status, int *parse_pos)
{
	char *icu_locale_name = calloc(1, MAX_VALID_ICU_NAME_LEN);
	get_valid_icu_locale_name(loc_name, icu_locale_name, MAX_VALID_ICU_NAME_LEN);
	void *fmt = icu_unum_open(icu_locale_name, cur_status);
	free(icu_locale_name);
	if (*cur_status == DLSYM_ICU_FAIL || *cur_status == ICU_ERROR) {
		icu_unum_close(fmt);
		return 0;
	}

	int n = wcslen(s);
	u_char *ustr = (u_char *)calloc((n + 1), sizeof(u_char));
	*cur_status = icu_wchar_trans(s, ustr, n);
	if (*cur_status == DLSYM_ICU_FAIL || *cur_status == ICU_ERROR) {
		free(ustr);
		icu_unum_close(fmt);
		return 0;
	}

	double res = icu_parse_double(fmt, ustr, parse_pos, cur_status);
	if (*cur_status == DLSYM_ICU_FAIL || *cur_status == ICU_ERROR) {
		free(ustr);
		icu_unum_close(fmt);
		return 0;
	}
	free(ustr);
	icu_unum_close(fmt);
	return res;
}

double wcstod_l(const wchar_t *restrict s, wchar_t **restrict p, locale_t l)
{
	if (l && s && l->cat[LC_NUMERIC] && l->cat[LC_NUMERIC]->flag == ICU_VALID) {
		int cur_status = DLSYM_ICU_SUCC;
		int sign = 1;
		int parse_pos = 0;
		wchar_t *tmp_s = (wchar_t *)s;
		while (tmp_s && iswspace(*tmp_s)) {
			tmp_s++;
		}
		if (*tmp_s == L'+' || *tmp_s == L'-') {
			sign -= 2 * (*tmp_s == L'-');
			++tmp_s;
		}

		int i;
		for (i = 0; i < 8 && towlower(*(tmp_s + i)) == L"infinity"[i]; i++) {}
		if (i == 3 || i == 8 || i > 3) {
			if (!p) return sign * INFINITY;
			if (i != 8) {
				*p = tmp_s + 3;
			} else {
				*p = tmp_s + i;
			}
			return sign * INFINITY;
		}

		double res = icu_wcstod_l(l->cat[LC_NUMERIC]->name, tmp_s, &cur_status, &parse_pos);
		if (cur_status == DLSYM_ICU_SUCC || cur_status == ICU_ERROR) {
			if (p) *p = parse_pos ? tmp_s + parse_pos : (char *)s;
			return sign * res;
		}
	}

	return wcstod(s, p);
}
#endif
