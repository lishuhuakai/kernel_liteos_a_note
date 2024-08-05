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

#define _GNU_SOURCE
#include <stdlib.h>
#include <locale.h>
#ifdef FEATURE_ICU_LOCALE
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "locale_impl.h"
#endif

float strtof_l(const char *restrict s, char **restrict p, locale_t l)
{
	return strtof(s, p);
}

#ifdef FEATURE_ICU_LOCALE
void *icu_unum_open(char *icu_locale_name, int *cur_status)
{
	/* ICU function: unum_open, returns a format related to specific locale */
	get_icu_symbol(ICU_I18N, &(g_icu_opt_func.unum_open), ICU_UNUM_OPEN_SYMBOL);

	/* dlopen/dlsym fail */
	if (!g_icu_opt_func.unum_open) {
		*cur_status = DLSYM_ICU_FAIL;
		return NULL;
	}

	int icu_status = 0;
	void *fmt = g_icu_opt_func.unum_open(1, NULL, -1, icu_locale_name, NULL, &icu_status);
	/* icu_status is the error code from icu methods, positive error code means errors
	 * happening in icu, e.g. invalid input string or no locale source data
	 */
	if (icu_status > 0) {
		*cur_status = ICU_ERROR;
		return NULL;
	}
	return fmt;
}

void icu_unum_close(void *fmt)
{
	/* ICU function: unum_close, close the formate returned from unum_open */
	get_icu_symbol(ICU_I18N, &(g_icu_opt_func.unum_close), ICU_UNUM_CLOSE_SYMBOL);
	if (g_icu_opt_func.unum_close) {
		g_icu_opt_func.unum_close(fmt);
	}
}

static int icu_char_trans(char *src, u_char *des, int des_size)
{
	/* ICU function: u_strFromUTF8, transfer utf-8 string to Uchar* string which is
	 * the input format of icu parse methods
	 */
	get_icu_symbol(ICU_I18N, &(g_icu_opt_func.u_str_from_utf8), ICU_STR_FROM_UTF8_SYMBOL);
	if (!g_icu_opt_func.u_str_from_utf8) {
		return DLSYM_ICU_FAIL;
	}

	int icu_status = 0;
	g_icu_opt_func.u_str_from_utf8(des, des_size, NULL, src, -1, &icu_status);
	if (icu_status > 0) {
		return ICU_ERROR;
	}
	return DLSYM_ICU_SUCC;
}

double icu_parse_double(void *fmt, u_char *ustr, int32_t *parse_pos, int *cur_status)
{
	/* ICU function: unum_parseDouble, parse the given Uchar* string to double */
	get_icu_symbol(ICU_I18N, &(g_icu_opt_func.unum_parse_double), ICU_UNUM_PARSE_DOUBLE_SYMBOL);
	if (!g_icu_opt_func.unum_parse_double) {
		*cur_status = DLSYM_ICU_FAIL;
		return 0;
	}

	int32_t icu_status = 0;
	double res = g_icu_opt_func.unum_parse_double(fmt, ustr, -1, parse_pos, &icu_status);
	if (icu_status > 0) {
		*cur_status = ICU_ERROR;
		return 0;
	}
	
	return res;
}

static double icu_strtod_l(char *loc_name, char *s, int *cur_status, int *parse_pos)
{
	char *icu_locale_name = calloc(1, MAX_VALID_ICU_NAME_LEN);
	get_valid_icu_locale_name(loc_name, icu_locale_name, MAX_VALID_ICU_NAME_LEN);
	void *fmt = icu_unum_open(icu_locale_name, cur_status);
	free(icu_locale_name);
	if (*cur_status == DLSYM_ICU_FAIL || *cur_status == ICU_ERROR) {
		return 0;
	}

	int n = strlen(s);
	u_char *ustr = (u_char *)calloc((n + 1), sizeof(u_char));
	*cur_status = icu_char_trans(s, ustr, n);
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
#endif

double strtod_l(const char *restrict s, char **restrict p, locale_t l)
{
#ifdef FEATURE_ICU_LOCALE
	if (l && s && l->cat[LC_NUMERIC] && l->cat[LC_NUMERIC]->flag == ICU_VALID) {
		int cur_status = DLSYM_ICU_SUCC;
		int sign = 1;
		int parse_pos = 0;
		/* preprocess blank characters */
		char *tmp_s = (char *)s;
		while (tmp_s && isspace((unsigned char)*tmp_s)) {
			tmp_s++;
		}

		/* preprocess positiv/negative signs */
		if (*tmp_s == '+' || *tmp_s == '-') {
			sign -= 2 * (*tmp_s == '-');
			++tmp_s;
		}

		/* process inf/infinity */
		int i;
		for (i = 0; i < 8 && (*(tmp_s + i) | 32) == "infinity"[i]; i++) {}
		if (i == 3 || i == 8 || i > 3) {
			if (!p) return sign * INFINITY;
			if (i != 8) {
				*p = tmp_s + 3;
			} else {
				*p = tmp_s + i;
			}
			return sign * INFINITY;
		}

		double res = icu_strtod_l(l->cat[LC_NUMERIC]->name, tmp_s, &cur_status, &parse_pos);
		if (cur_status == DLSYM_ICU_SUCC || cur_status == ICU_ERROR) {
			if (p) *p = parse_pos ? tmp_s + parse_pos : (char *)s;
			return sign * res;
		}
	}
#endif
	return strtod(s, p);
}

long double strtold_l(const char *restrict s, char **restrict p, locale_t l)
{
	return strtold(s, p);
}

weak_alias(strtof_l, __strtof_l);
weak_alias(strtod_l, __strtod_l);
weak_alias(strtold_l, __strtold_l);
