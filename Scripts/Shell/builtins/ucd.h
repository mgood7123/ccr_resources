/*
 * Copyright (C) 2012-2017 Reece H. Dunn
 *
 * This file is part of ucd-tools.
 *
 * ucd-tools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ucd-tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ucd-tools.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "printfmacro.h"
#include "ucd-tools/config.h"
#include "ucd-tools/src/include/ucd/ucd.h"
#include "ucd-tools/src/case.c"
#include "ucd-tools/src/categories.c"
#include "ucd-tools/src/ctype.c"
#include "ucd-tools/src/proplist.c"
#include "ucd-tools/src/scripts.c"
#include "ucd-tools/src/tostring.c"

#include <locale.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>

#ifndef HAVE_ISWBLANK
static int iswblank(wint_t c)
{
	return iswspace(c) && !(c >= 0x0A && c <= 0x0D);
}
#endif

static void fput_utf8c(FILE *out, codepoint_t c)
{
	if (c < 0x80)
		fputc((uint8_t)c, out);
	else if (c < 0x800)
	{
		fputc(0xC0 | (c >> 6), out);
		fputc(0x80 + (c & 0x3F), out);
	}
	else if (c < 0x10000)
	{
		fputc(0xE0 | (c >> 12), out);
		fputc(0x80 + ((c >> 6) & 0x3F), out);
		fputc(0x80 + (c & 0x3F), out);
	}
	else if (c < 0x200000)
	{
		fputc(0xF0 | (c >> 18), out);
		fputc(0x80 + ((c >> 12) & 0x3F), out);
		fputc(0x80 + ((c >>  6) & 0x3F), out);
		fputc(0x80 + (c & 0x3F), out);
	}
}

static char * fput_utf8c_ret(codepoint_t c)
{
	if (c < 0x80) {
		char * code = malloc(1);
		code[0] = (uint8_t)c;
		code[1] = '\0';
		return code;
	}
	else if (c < 0x800)
	{
		char * code = malloc(2);
		code[0] = 0xC0 | (c >> 6);
		code[1] = 0x80 + (c & 0x3F);
		code[2] = '\0';
		return code;
	}
	else if (c < 0x10000)
	{
		char * code = malloc(3);
		code[0] = 0xE0 | (c >> 12);
		code[1] = 0x80 + ((c >> 6) & 0x3F);
		code[2] = 0x80 + (c & 0x3F);
		code[3] = '\0';
		return code;
	}
	else if (c < 0x200000)
	{
		char * code = malloc(4);
		code[0] = 0xF0 | (c >> 18);
		code[1] = 0x80 + ((c >> 12) & 0x3F);
		code[2] = 0x80 + ((c >>  6) & 0x3F);
		code[3] = 0x80 + (c & 0x3F);
		code[4] = '\0';
		return code;
	}
}


static int fget_utf8c(FILE *in, codepoint_t *c)
{
	int ch = EOF;
	if ((ch = fgetc(in)) == EOF) return 0;
	if ((uint8_t)ch < 0x80)
		*c = (uint8_t)ch;
	else switch ((uint8_t)ch & 0xF0)
	{
	default:
		*c = (uint8_t)ch & 0x1F;
		if ((ch = fgetc(in)) == EOF) return 0;
		*c = (*c << 6) + ((uint8_t)ch & 0x3F);
		break;
	case 0xE0:
		*c = (uint8_t)ch & 0x0F;
		if ((ch = fgetc(in)) == EOF) return 0;
		*c = (*c << 6) + ((uint8_t)ch & 0x3F);
		if ((ch = fgetc(in)) == EOF) return 0;
		*c = (*c << 6) + ((uint8_t)ch & 0x3F);
		break;
	case 0xF0:
		*c = (uint8_t)ch & 0x07;
		if ((ch = fgetc(in)) == EOF) return 0;
		*c = (*c << 6) + ((uint8_t)ch & 0x3F);
		if ((ch = fgetc(in)) == EOF) return 0;
		*c = (*c << 6) + ((uint8_t)ch & 0x3F);
		if ((ch = fgetc(in)) == EOF) return 0;
		*c = (*c << 6) + ((uint8_t)ch & 0x3F);
		break;
	}
	return 1;
}

static void uprintf_codepoint(FILE *out, codepoint_t c, char mode)
{
	switch (mode)
	{
	case 'c': /* character */
		switch (c)
		{
		case '\t': fputs("\\t", out);  break;
		case '\r': fputs("\\r", out);  break;
		case '\n': fputs("\\n", out);  break;
		default:   fput_utf8c(out, c); break;
		}
		break;
	case 'h': /* hexadecimal (lower) */
		fprintf(out, "%06x", c);
		break;
	case 'H': /* hexadecimal (upper) */
		fprintf(out, "%06X", c);
		break;
	}
}

static void uprintf_is(FILE *out, codepoint_t c, char mode)
{
	switch (mode)
	{
	case 'A': /* alpha-numeric */
		fputc(iswalnum(c) ? '1' : '0', out);
		break;
	case 'a': /* alpha */
		fputc(iswalpha(c) ? '1' : '0', out);
		break;
	case 'b': /* blank */
		fputc(iswblank(c) ? '1' : '0', out);
		break;
	case 'c': /* control */
		fputc(iswcntrl(c) ? '1' : '0', out);
		break;
	case 'd': /* numeric */
		fputc(iswdigit(c) ? '1' : '0', out);
		break;
	case 'g': /* glyph */
		fputc(iswgraph(c) ? '1' : '0', out);
		break;
	case 'l': /* lower case */
		fputc(iswlower(c) ? '1' : '0', out);
		break;
	case 'P': /* printable */
		fputc(iswprint(c) ? '1' : '0', out);
		break;
	case 'p': /* punctuation */
		fputc(iswpunct(c) ? '1' : '0', out);
		break;
	case 's': /* whitespace */
		fputc(iswspace(c) ? '1' : '0', out);
		break;
	case 'u': /* upper case */
		fputc(iswupper(c) ? '1' : '0', out);
		break;
	case 'x': /* xdigit */
		fputc(iswxdigit(c) ? '1' : '0', out);
		break;
	}
}

static void uprintf(FILE *out, codepoint_t c, const char *format)
{
	while (*format) switch (*format)
	{
	case '%':
		switch (*++format)
		{
		case 'c': /* category */
			fputs(ucd_get_category_string(ucd_lookup_category(c)), out);
			break;
		case 'C': /* category group */
			fputs(ucd_get_category_group_string(ucd_lookup_category_group(c)), out);
			break;
		case 'p': /* codepoint */
			uprintf_codepoint(out, c, *++format);
			break;
		case 'P': /* properties */
			fprintf(out, "%016llx", ucd_properties(c, ucd_lookup_category(c)));
			break;
		case 'i': /* is* */
			uprintf_is(out, c, *++format);
			break;
		case 'L': /* lowercase */
			uprintf_codepoint(out, towlower(c), *++format);
			break;
		case 's': /* script */
			fputs(ucd_get_script_string(ucd_lookup_script(c)), out);
			break;
		case 'T': /* titlecase */
			uprintf_codepoint(out, ucd_totitle(c), *++format);
			break;
		case 'U': /* uppercase */
			uprintf_codepoint(out, towupper(c), *++format);
			break;
		}
		++format;
		break;
	case '\\':
		switch (*++format) {
		case 0:
			break;
		case 't':
			fputc('\t', out);
			++format;
			break;
		case 'r':
			fputc('\r', out);
			++format;
			break;
		case 'n':
			fputc('\n', out);
			++format;
			break;
		default:
			fputc(*format, out);
			++format;
			break;
		}
		break;
	default:
		fputc(*format, out);
		++format;
		break;
	}
}

static void print_file(FILE *in, const char *format)
{
	codepoint_t c = 0;
	while (fget_utf8c(in, &c))
		uprintf(stdout, c, format ? format : "%pc\t%pH\t%s\t%c\t%Uc\t%Lc\t%Tc\t%is\n");
}

int ucd(void)
{
	codepoint_t c;
	for (c = 0; c <= 0x10FFFF; ++c)
		uprintf(stdout, c, "%pc\t%pH\t%s\t%c\t%Uc\t%Lc\t%Tc\t%is\n");
	return 0;
}
