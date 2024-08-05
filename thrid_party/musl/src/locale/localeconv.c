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

#include <locale.h>
#include <limits.h>
#include "libc.h"
#include <string.h>
#include "locale_impl.h"

static const struct lconv posix_lconv = {
	.decimal_point = ".",
	.thousands_sep = "",
	.grouping = "",
	.int_curr_symbol = "",
	.currency_symbol = "",
	.mon_decimal_point = "",
	.mon_thousands_sep = "",
	.mon_grouping = "",
	.positive_sign = "",
	.negative_sign = "",
	.int_frac_digits = CHAR_MAX,
	.frac_digits = CHAR_MAX,
	.p_cs_precedes = CHAR_MAX,
	.p_sep_by_space = CHAR_MAX,
	.n_cs_precedes = CHAR_MAX,
	.n_sep_by_space = CHAR_MAX,
	.p_sign_posn = CHAR_MAX,
	.n_sign_posn = CHAR_MAX,
	.int_p_cs_precedes = CHAR_MAX,
	.int_p_sep_by_space = CHAR_MAX,
	.int_n_cs_precedes = CHAR_MAX,
	.int_n_sep_by_space = CHAR_MAX,
	.int_p_sign_posn = CHAR_MAX,
	.int_n_sign_posn = CHAR_MAX,
};

#ifdef FEATURE_ICU_LOCALE
#define ICU_BUFFER_SIZE 16
#define CHAR_BUFFER_SIZE 8
#define INITIALIZE_ICURES_PTR(posix_lconv_ptr, lconv_icures_ptr, buffsize)             \
	do {                                                                               \
		memset(lconv_icures_ptr, 0, buffsize);                                         \
		strncpy(lconv_icures_ptr, posix_lconv_ptr, strlen(posix_lconv_ptr));           \
	} while (0)

static struct lconv g_lconv_icures;
static int g_localeconv_initialize = 1;

typedef enum {
	ICU_DECIMAL_POINT = 0,
	ICU_THOUSANDS_SEP = 1,
	ICU_NEGATIVE_SIGN = 6,
	ICU_POSITIVE_SIGN = 7,
	ICU_CURR_SYMBOL = 8,
	ICU_INT_CURR_SYMBOL = 9,
	ICU_MON_DECIMAL_POINT = 10,
	ICU_MON_THOUSANDS_SEP = 17,
} icu_getsymbol_type;

static void update_lconv_member(u_char *icu_symbol, void *fmt, char *lconv_member, icu_getsymbol_type type)
{
	int icu_status = 0;
	if (!g_icu_opt_func.unum_get_symbol) {
		return;
	}
	int len = g_icu_opt_func.unum_get_symbol(fmt, type, icu_symbol, ICU_BUFFER_SIZE, &icu_status);
	if (icu_status <= 0) {
		if (g_icu_opt_func.u_austrncpy) {
			g_icu_opt_func.u_austrncpy(lconv_member, icu_symbol, ICU_BUFFER_SIZE);
		}
	}
}

// refresh the lconv_icures, fills with the default value
static void refresh_lconv_icures(void)
{
	if (g_localeconv_initialize == 1) {
		memcpy(&g_lconv_icures, &posix_lconv, sizeof(struct lconv));
		g_lconv_icures.decimal_point = (char *)malloc(ICU_BUFFER_SIZE);
		g_lconv_icures.thousands_sep = (char *)malloc(ICU_BUFFER_SIZE);
		g_lconv_icures.int_curr_symbol = (char *)malloc(ICU_BUFFER_SIZE);
		g_lconv_icures.currency_symbol = (char *)malloc(ICU_BUFFER_SIZE);
		g_lconv_icures.mon_decimal_point = (char *)malloc(ICU_BUFFER_SIZE);
		g_lconv_icures.mon_thousands_sep = (char *)malloc(ICU_BUFFER_SIZE);
		g_lconv_icures.positive_sign = (char *)malloc(ICU_BUFFER_SIZE);
		g_lconv_icures.negative_sign = (char *)malloc(ICU_BUFFER_SIZE);
		g_localeconv_initialize = 0;
	}

	INITIALIZE_ICURES_PTR(posix_lconv.decimal_point, g_lconv_icures.decimal_point, ICU_BUFFER_SIZE);
	INITIALIZE_ICURES_PTR(posix_lconv.thousands_sep, g_lconv_icures.thousands_sep, ICU_BUFFER_SIZE);
	INITIALIZE_ICURES_PTR(posix_lconv.int_curr_symbol, g_lconv_icures.int_curr_symbol, ICU_BUFFER_SIZE);
	INITIALIZE_ICURES_PTR(posix_lconv.currency_symbol, g_lconv_icures.currency_symbol, ICU_BUFFER_SIZE);
	INITIALIZE_ICURES_PTR(posix_lconv.mon_decimal_point, g_lconv_icures.mon_decimal_point, ICU_BUFFER_SIZE);
	INITIALIZE_ICURES_PTR(posix_lconv.mon_thousands_sep, g_lconv_icures.mon_thousands_sep, ICU_BUFFER_SIZE);
	INITIALIZE_ICURES_PTR(posix_lconv.positive_sign, g_lconv_icures.positive_sign, ICU_BUFFER_SIZE);
	INITIALIZE_ICURES_PTR(posix_lconv.negative_sign, g_lconv_icures.negative_sign, ICU_BUFFER_SIZE);
}
#endif

struct lconv *localeconv(void)
{
#ifdef FEATURE_ICU_LOCALE
	if ((libc.global_locale.cat[LC_NUMERIC] && libc.global_locale.cat[LC_NUMERIC]->flag == ICU_VALID) ||
		(libc.global_locale.cat[LC_MONETARY] && libc.global_locale.cat[LC_MONETARY]->flag == ICU_VALID)) {
		int cur_status = 0;
		refresh_lconv_icures();

		/* ICU function: unum_getSymbol, return specific information, e.g. currency symbol */
		get_icu_symbol(ICU_I18N, &(g_icu_opt_func.unum_get_symbol), ICU_UNUM_GET_SYMBOL_SYMBOL);
		/* ICU function: u_austrncpy, transfer result from unum_getSymbol to utf-8 string */
		get_icu_symbol(ICU_UC, &(g_icu_opt_func.u_austrncpy), ICU_AUSTRNCPY_SYMBOL);

		if (!(g_icu_opt_func.unum_get_symbol) || !(g_icu_opt_func.u_austrncpy)) {
			return (void *)&g_lconv_icures;
		}
		
		u_char icu_symbol[ICU_BUFFER_SIZE] = { 0 };
		void *fmt = NULL;

		if (libc.global_locale.cat[LC_NUMERIC]) {
			char *icu_locale_name_num = calloc(1, MAX_VALID_ICU_NAME_LEN);
			get_valid_icu_locale_name(libc.global_locale.cat[LC_NUMERIC]->name, icu_locale_name_num,
				MAX_VALID_ICU_NAME_LEN);
			fmt = icu_unum_open(icu_locale_name_num, &cur_status);
			free(icu_locale_name_num);
			if (cur_status == 0) {
				memset(icu_symbol, 0, ICU_BUFFER_SIZE);
				update_lconv_member(icu_symbol, fmt, g_lconv_icures.decimal_point, ICU_DECIMAL_POINT);
				memset(icu_symbol, 0, ICU_BUFFER_SIZE);
				update_lconv_member(icu_symbol, fmt, g_lconv_icures.thousands_sep, ICU_THOUSANDS_SEP);
				icu_unum_close(fmt);
			}
		}
		
		if (libc.global_locale.cat[LC_MONETARY]) {
			char *icu_locale_name_mon = calloc(1, MAX_VALID_ICU_NAME_LEN);
			get_valid_icu_locale_name(libc.global_locale.cat[LC_MONETARY]->name, icu_locale_name_mon,
				MAX_VALID_ICU_NAME_LEN);
			fmt = icu_unum_open(icu_locale_name_mon, &cur_status);
			free(icu_locale_name_mon);
			if (cur_status == 0) {
				memset(icu_symbol, 0, ICU_BUFFER_SIZE);
				update_lconv_member(icu_symbol, fmt, g_lconv_icures.int_curr_symbol, ICU_INT_CURR_SYMBOL);
				memset(icu_symbol, 0, ICU_BUFFER_SIZE);
				update_lconv_member(icu_symbol, fmt, g_lconv_icures.currency_symbol, ICU_CURR_SYMBOL);
				memset(icu_symbol, 0, ICU_BUFFER_SIZE);
				update_lconv_member(icu_symbol, fmt, g_lconv_icures.mon_decimal_point, ICU_MON_DECIMAL_POINT);
				memset(icu_symbol, 0, ICU_BUFFER_SIZE);
				update_lconv_member(icu_symbol, fmt, g_lconv_icures.mon_thousands_sep, ICU_MON_THOUSANDS_SEP);
				memset(icu_symbol, 0, ICU_BUFFER_SIZE);
				update_lconv_member(icu_symbol, fmt, g_lconv_icures.positive_sign, ICU_POSITIVE_SIGN);
				memset(icu_symbol, 0, ICU_BUFFER_SIZE);
				update_lconv_member(icu_symbol, fmt, g_lconv_icures.negative_sign, ICU_NEGATIVE_SIGN);
				icu_unum_close(fmt);
			}
		}
		return (void *)&g_lconv_icures;
	}
#endif
	return (void *)&posix_lconv;
}
