#include "xml_api.h"

/*******************************************************************************
 * headers
 */

#include <stdlib.h> // strtoul
#include <string.h>

#include "bt_trace.h"
#include "data_types.h"

/*******************************************************************************
 * macros
 */

#define XML_CHAR_IS_WS(ch)			\
	((ch) == ' ' || (ch) == '\t' || (ch) == '\r' || (ch) == '\n')
#define XML_CHAR_IS_NUL(ch)			((ch) == '\0')
#define XML_CHAR_IS_WS_OR_NUL(ch)	(XML_CHAR_IS_WS(ch) || XML_CHAR_IS_NUL(ch))

/*******************************************************************************
 * local function declarations
 */

static void xml_set_bfr(tXML_ST *p_st, UINT8 set);
static void xml_write_bfr(tXML_ST *p_st);
static void xml_incr_pars_res(tXML_ST *p_st, UINT16 new_res);
static void xml_read_char(tXML_ST *p_st);
static BOOLEAN xml_get_next(tXML_ST *p_st, tXML_WS_OP ws_op);
static BOOLEAN xml_find_ch(tXML_ST *p_st, UINT8 ch, tXML_WS_OP ws_op);
static BOOLEAN xml_find_ch_n(tXML_ST *p_st, UINT8 const *p_str);
static BOOLEAN xml_find_str(tXML_ST *p_st, UINT8 const *p_str);
static BOOLEAN xml_consume_str(tXML_ST *p_st, UINT8 const *p_str);
static void xml_resolve_refs(tXML_ST *p_st);
static void xml_remove_trail_ws(tXML_ST *p_st);
static BOOLEAN xml_name(tXML_ST *p_st);
static BOOLEAN xml_attributes(tXML_ST *p_st);
static BOOLEAN xml_elems(tXML_ST *p_st, BOOLEAN prev_ok);
static BOOLEAN xml_qm_elem(tXML_ST *p_st);
static BOOLEAN xml_ex_elem(tXML_ST *p_st, BOOLEAN prolog);
static BOOLEAN xml_tag_elem(tXML_ST *p_st);
static BOOLEAN xml_etag_elem(tXML_ST *p_st);

/*******************************************************************************
 * variables
 */

// .rodata
static UINT8 const xml_name_srch[] = ":=/> \t\n\r";

/*******************************************************************************
 * functions
 */

void XML_InitPars(tXML_ST *p_st, tXML_CBACK *xml_cback, void *p_usr_data)
{
	memset(p_st, 0, sizeof *p_st);

	p_st->cback			= xml_cback;
	p_st->p_usr_data	= p_usr_data;
}

UINT16 XML_MulParse(tXML_ST *p_st, tXML_OS *p_os)
{
	BOOLEAN found;
	UINT8 query;
	BOOLEAN partial = FALSE;
	BOOLEAN parse_ok = TRUE;
	int keep_size;
	UINT16 res = 0;
	UINT16 old_pars_res;

	p_st->curr_res = 1;
	memcpy(&p_st->xml_os, p_os, sizeof p_st->xml_os);

	old_pars_res = p_st->pars_res;
	p_st->pars_res = 0;

	p_st->prefix.len	= 0;
	p_st->name.len		= 0;
	p_st->value.len		= 0;

	p_st->p_last_stm	= NULL;
	p_st->p_copy		= NULL;

	XML_TRACE(DEBUG, "XML_MulParse status:%d, pars_res: %d, begin:%x, end:x%x",
	         p_st->status, old_pars_res, p_os->p_begin, p_os->p_end);

	if (p_st->status == 0)
	{
		p_st->p_cur = p_os->p_begin;

		XML_TRACE(DEBUG, "p_cur:x%x", p_st->p_cur);

		if (!xml_get_next(p_st, XML_WS_OP_0))
		{
			res = 2;
		}
		else
		{
			found = FALSE;

			while (p_st->curr_res > 0 && !found)
			{
				if (!xml_find_ch(p_st, '<', XML_WS_OP_0)
				    || !xml_get_next(p_st, XML_WS_OP_0))
				{
					res = 5;
					break;
				}

				if (p_st->next_token == '?')
				{
					parse_ok = xml_qm_elem(p_st);
				}
				else if (p_st->next_token == '!')
				{
					parse_ok = xml_ex_elem(p_st, TRUE);
				}
				else if (p_st->next_token == '/')
				{
					parse_ok = FALSE;

					if (!xml_get_next(p_st, XML_WS_OP_0))
					{
						res = 5;
						break;
					}
				}
				else
				{
					found = TRUE;
					parse_ok = TRUE;
				}

				if (!parse_ok)
					xml_incr_pars_res(p_st, 5);
			}
		}

		p_st->status = 1;
	}
	else if (old_pars_res != 4)
	{
		XML_TRACE(DEBUG, "p_st->last_bfr.p:x%x, p_st->used_last_bfr:%d",
		                 p_st->last_bfr.p, p_st->used_last_bfr);

		// NOTE: p_st->partial_st.last_bfr.p may be assumed to always be true
		if (p_st->partial_st.last_bfr.p && p_st->partial_st.used_last_bfr)
		{
			memcpy(p_st->last_bfr.p, p_st->partial_st.last_bfr.p,
			       p_st->partial_st.used_last_bfr);

			p_st->used_last_bfr = p_st->partial_st.used_last_bfr;
			p_st->last_bfr.p[p_st->partial_st.used_last_bfr] = '\0';
			p_st->event_data.part.parse =
				p_st->partial_st.event_data.part.parse;

			p_st->partial_st.used_last_bfr = 0;

			XML_TRACE(DEBUG, "retrieved PARTIAL data = [%s]\n",
			                 p_st->last_bfr.p);

			p_st->p_cur = p_st->last_bfr.p;

			xml_get_next(p_st, XML_WS_OP_0);

			p_st->event_data.copy.p_begin = p_st->xml_os.p_begin;
			p_st->event_data.copy.last.p = p_st->last_bfr.p;
			p_st->event_data.copy.last.len = p_st->used_last_bfr;

			(*p_st->cback)(XML_EVENT_TYPE_COPY, &p_st->event_data.copy,
			               p_st->p_usr_data);
		}
		else if (!p_st->used_last_bfr)
		{
			p_st->p_cur = p_os->p_begin;
			xml_get_next(p_st, XML_WS_OP_0);
		}
		else
		{
			return 3;
		}

		XML_TRACE(DEBUG, "p_st->p_cur:x%x", p_st->p_cur);
	}

	XML_TRACE(DEBUG, "XML_MulParse end while");

	if (res == 0 && p_st->curr_res > 0)
	{
		if (p_st->status == 1)
		{
			p_st->p_copy = p_st->p_cur - 1;

			parse_ok = xml_tag_elem(p_st);
		}

		XML_TRACE(DEBUG, "XML_MulParse exit xml_tag_elem");

		if (!parse_ok)
		{
			query = (*p_st->cback)(XML_EVENT_TYPE_QUERY,
			                       &p_st->event_data.stack, p_st->p_usr_data);

			if (query == 1)
				xml_incr_pars_res(p_st, 5);
			else
				partial = TRUE;
		}
		else
		{
			p_st->status = 2;
		}

		if (p_st->curr_res > 0 && parse_ok)
		{
			parse_ok = xml_elems(p_st, parse_ok);

			query = (*p_st->cback)(XML_EVENT_TYPE_QUERY,
			                       &p_st->event_data.stack, p_st->p_usr_data);

			if (!parse_ok || query == 0)
				partial = TRUE;
			else
				p_st->status = 3;
		}

		if (partial)
		{
			if (p_st->pars_res == 4)
			{
				p_st->p_cur = p_st->p_copy;
				p_st->event_data.part.parse = p_st->pars_res;
				p_st->event_data.part.p_keep = p_st->p_cur;

				XML_TRACE(DEBUG, "p_st->p_cur:x%x (last_stm)", p_st->p_cur);

				(*p_st->cback)(XML_EVENT_TYPE_PART, &p_st->event_data.part,
				               p_st->p_usr_data);
				xml_incr_pars_res(p_st, 6);
			}
			else if (p_st->last_bfr.p && p_st->p_copy > p_st->xml_os.p_begin
			         && p_st->p_copy < p_st->xml_os.p_end)
			{
				keep_size = p_st->xml_os.p_end - p_st->p_copy;

				if (keep_size < p_st->last_bfr.len)
				{
					/* NOTE: p_st->partial_st.last_bfr.p may be assumed to
					 * always be true
					 */
					if (p_st->partial_st.last_bfr.p)
					{
						XML_TRACE(DEBUG, "Store partial data\n");

						strncpy((char *)p_st->partial_st.last_bfr.p,
						        (char *)p_st->p_copy, keep_size);

						p_st->partial_st.used_last_bfr = keep_size;
						p_st->partial_st.last_bfr.p[keep_size] = '\0';

						p_st->partial_st.event_data.part.parse = p_st->pars_res;
						p_st->partial_st.event_data.part.p_keep =
							p_st->last_bfr.p;
					}
					else
					{
						XML_TRACE(DEBUG, "ERROR to store partial data");
					}

					(*p_st->cback)(XML_EVENT_TYPE_PART, &p_st->event_data.part,
					               p_st->p_usr_data);
					xml_incr_pars_res(p_st, 6);
				}
			}
		}
	}

	return p_st->pars_res;
}

static void xml_set_bfr(tXML_ST *p_st, UINT8 set)
{
	switch (set)
	{
	case 1:
		p_st->name.p = p_st->p_cur - 1;
		p_st->p_data_bfr = p_st->name.p;

		p_st->name.len = 0;

		break;

	case 2:
		p_st->value.p = p_st->p_cur - 1;
		p_st->p_data_bfr = p_st->value.p;

		p_st->value.len = 0;

		break;

	default:
		p_st->p_data_bfr = NULL;
		break;
	}
}

static void xml_write_bfr(tXML_ST *p_st)
{
	if (!p_st->p_data_bfr)
		return;

	if (p_st->p_data_bfr == p_st->name.p)
		++p_st->name.len;
	else
		++p_st->value.len;
}

static void xml_incr_pars_res(tXML_ST *p_st, UINT16 new_res)
{
	if (new_res <= p_st->pars_res)
		return;

	switch (p_st->pars_res)
	{
	case 2:
	case 3:
	case 4:
		break;

	default:
		p_st->pars_res = new_res;
		break;
	}
}

static void xml_read_char(tXML_ST *p_st)
{
	UINT8 get_new = FALSE;

	if (p_st->p_cur >= p_st->last_bfr.p
	    && p_st->p_cur < p_st->last_bfr.p + p_st->used_last_bfr)
	{
		p_st->next_token = *p_st->p_cur;

		if (XML_CHAR_IS_NUL(p_st->next_token))
		{
			p_st->p_cur = p_st->xml_os.p_begin;
			p_st->last_bfr.p[0] = '\0';
			p_st->used_last_bfr = 0;

			get_new = TRUE;
		}
		else
		{
			p_st->p_cur = p_st->p_cur + 1;
			p_st->curr_res = 1;
		}
	}
	else
	{
		if (p_st->p_cur == p_st->last_bfr.p + p_st->used_last_bfr)
		{
			p_st->used_last_bfr = 0;
			p_st->p_cur = p_st->xml_os.p_begin;
		}

		get_new = TRUE;
	}

	if (get_new)
	{
		if (p_st->p_cur && p_st->p_cur < p_st->xml_os.p_end)
		{
			p_st->next_token = *p_st->p_cur;
			p_st->p_cur = p_st->p_cur + 1;
			p_st->curr_res = 1;
		}
		else
		{
			p_st->curr_res = 0;
		}
	}
}

static BOOLEAN xml_get_next(tXML_ST *p_st, tXML_WS_OP ws_op)
{
	xml_write_bfr(p_st);

	do
	{
		xml_read_char(p_st);
	} while (ws_op == XML_WS_OP_1 && XML_CHAR_IS_WS_OR_NUL(p_st->next_token)
	         && p_st->curr_res > 0);

	if (p_st->curr_res > 0 && p_st->next_token == '\n' && p_st->skip_next_nl)
		xml_read_char(p_st);

	p_st->skip_next_nl = FALSE;

	if (p_st->curr_res <= 0)
	{
		p_st->next_token = '\0';
		return FALSE;
	}

	if (p_st->next_token == '\r')
	{
		p_st->next_token = '\n';
		p_st->skip_next_nl = TRUE;
	}

	if (ws_op == XML_WS_OP_2
	    && (p_st->next_token == '\n' || p_st->next_token == '\t'))
	{
		p_st->next_token = ' ';
	}

	return TRUE;
}

static BOOLEAN xml_find_ch(tXML_ST *p_st, UINT8 ch, tXML_WS_OP ws_op)
{
	while (p_st->curr_res > 0 && p_st->next_token != ch)
		xml_get_next(p_st, ws_op);

	return !(p_st->curr_res <= 0);
}

static BOOLEAN xml_find_ch_n(tXML_ST *p_st, UINT8 const *p_str)
{
	UINT8 const *p_tmp;

	while (p_st->curr_res > 0)
	{
		for (p_tmp = p_str; *p_tmp != '\0'; ++p_tmp)
		{
			if (p_st->next_token == *p_tmp)
				return TRUE;
		}

		xml_get_next(p_st, XML_WS_OP_0);
	}

	return FALSE;
}

static BOOLEAN xml_find_str(tXML_ST *p_st, UINT8 const *p_str)
{
	UINT8 const *p_tmp = p_str;

	while (*p_tmp != '\0' && p_st->curr_res > 0)
	{
		for (p_tmp = p_str; *p_tmp != '\0' && p_st->curr_res > 0; ++p_tmp)
		{
			if (p_st->next_token != *p_tmp)
				break;

			xml_get_next(p_st, XML_WS_OP_0);
		}

		if (p_tmp == p_str && p_st->curr_res > 0)
			xml_get_next(p_st, XML_WS_OP_0);
	}

	return *p_tmp == '\0';
}

static BOOLEAN xml_consume_str(tXML_ST *p_st, UINT8 const *p_str)
{
	do
	{
		if (p_st->next_token != *p_str)
			return FALSE;

		++p_str;

		if (!xml_get_next(p_st, XML_WS_OP_0))
			return FALSE;
	} while (*p_str != '\0');

	return TRUE;
}

static void xml_resolve_refs(tXML_ST *p_st)
{
	UINT8 *p_srch = p_st->value.p;
	UINT8 *p_am;
	UINT8 *p_sc;
	UINT8 *p_start;
	UINT8 *p_tmp;
	unsigned long ch_code;
	unsigned long tmp_code;
	INT8 i;
	UINT8 resolved;
	UINT16 len_left = p_st->value.len;

	do
	{
		p_start = p_srch;

		p_am = (UINT8 *)strchr((char *)p_srch, '&');
		p_sc = p_am ? (UINT8 *)strchr((char *)p_am, ';') : NULL;

		if (p_sc > p_start + len_left)
			p_sc = NULL;

		if (!p_am || !p_sc)
			continue;

		resolved = 0;

		p_tmp = p_am + 1;
		*p_sc = '\0';

		if (p_tmp[0] == '#')
		{
			if (p_tmp[1] == 'x')
			{
				p_tmp[0] = '0';
			}
			else
			{
				++p_tmp;

				while (*p_tmp == '0')
					++p_tmp;
			}

			ch_code = strtoul((char *)p_tmp, NULL, 0);

			i = 3;

			// do the byteswap match
			while (i >= 0 && ch_code >> (8 * i) == 0)
				--i;

			for (p_tmp = p_am; i >= 0; --i)
			{
				tmp_code = ch_code << (8 * (3 - i));

				*p_tmp++ = tmp_code >> 24;
			}

			resolved = 1;
		}
		else if (p_tmp < p_sc)
		{
			resolved = 1;

			if (strcmp((char *)p_tmp, "lt") == 0)
				*p_am = '<';
			else if (strcmp((char *)p_tmp, "gt") == 0)
				*p_am = '>';
			else if (strcmp((char *)p_tmp, "amp") == 0)
				*p_am = '?'; // not '&'?
			else if (strcmp((char *)p_tmp, "apos") == 0)
				*p_am = '\'';
			else if (strcmp((char *)p_tmp, "quot") == 0)
				*p_am = '"';
			else
				resolved = 0;
		}

		if (resolved)
		{
			p_srch = p_tmp;
			++p_sc;

			while (*p_sc != '\0')
				*p_tmp++ = *p_sc++;
		}
		else
		{
			*p_sc = ';';
			p_srch = p_sc + 1;
		}

	} while (*p_srch != '\0' && p_am && p_sc);
}

static void xml_remove_trail_ws(tXML_ST *p_st)
{
	UINT16 xx;

	if (p_st->value.p)
	{
		xx = p_st->value.len;

		while (xx && XML_CHAR_IS_WS_OR_NUL(p_st->value.p[xx - 1]))
			--xx;

		p_st->value.len = xx;
	}
}

static BOOLEAN xml_name(tXML_ST *p_st)
{
	BOOLEAN found = FALSE;

	p_st->prefix.p = NULL;
	p_st->prefix.len = 0;

	xml_set_bfr(p_st, 1);
	xml_find_ch_n(p_st, xml_name_srch);

	if (p_st->curr_res > 0 && p_st->next_token == ':')
	{
		if (p_st->name.len)
		{
			found = TRUE;
			p_st->prefix.p = p_st->name.p;
			p_st->prefix.len = p_st->name.len;
		}

		xml_get_next(p_st, XML_WS_OP_0);
		xml_set_bfr(p_st, 1);

		if (p_st->curr_res > 0)
			xml_find_ch_n(p_st, xml_name_srch + 1);
	}

	// this personally is a first for me to see
	found = found || p_st->name.len;

	if (found)
		xml_set_bfr(p_st, 0);

	return found;
}

static BOOLEAN xml_attributes(tXML_ST *p_st)
{
	UINT8 cb_ret = 1;
	UINT8 q_ch;

	XML_TRACE(DEBUG, "[xml_parse] xml_attributes: res= %d", p_st->pars_res);

	while (cb_ret)
	{
		if (XML_CHAR_IS_WS_OR_NUL(p_st->next_token)
		    && !xml_get_next(p_st, XML_WS_OP_1))
		{
			return FALSE;
		}

		if (p_st->next_token == '=')
			return FALSE;

		if (p_st->next_token == '>' || p_st->next_token == '/')
			return TRUE;

		if (!xml_name(p_st) || p_st->curr_res <= 0)
			return FALSE;

		if (XML_CHAR_IS_WS_OR_NUL(p_st->next_token)
		    && !xml_get_next(p_st, XML_WS_OP_1))
		{
			return FALSE;
		}

		if (p_st->next_token != '=')
			return FALSE;

		if (!xml_get_next(p_st, XML_WS_OP_1))
			return FALSE;

		if (p_st->next_token != '\'' && p_st->next_token != '"')
			return FALSE;

		q_ch = p_st->next_token;

		if (!xml_get_next(p_st, XML_WS_OP_0))
			return FALSE;

		xml_set_bfr(p_st, 2);

		if (!xml_find_ch(p_st, q_ch, XML_WS_OP_2))
			return FALSE;

		xml_set_bfr(p_st, 0);
		xml_resolve_refs(p_st);

		p_st->event_data.attr.prefix.p = p_st->prefix.p;
		p_st->event_data.attr.prefix.len = p_st->prefix.len;

		p_st->event_data.attr.name.p = p_st->name.p;
		p_st->event_data.attr.name.len = p_st->name.len;

		p_st->event_data.attr.value.p = p_st->value.p;
		p_st->event_data.attr.value.len = p_st->value.len;

		p_st->value.len = 0;

		cb_ret = (*p_st->cback)(XML_EVENT_TYPE_ATTR, &p_st->event_data.attr,
		                        p_st->p_usr_data);

		if (cb_ret == 0)
		{
			xml_incr_pars_res(p_st, 4);
			return FALSE;
		}

		if (!xml_get_next(p_st, XML_WS_OP_0))
			return FALSE;
	}

	return p_st->next_token == '>' || p_st->next_token == '/';
}

static BOOLEAN xml_elems(tXML_ST *p_st, BOOLEAN prev_ok)
{
	BOOLEAN tag_found;
	UINT8 cb_ret = 1;

	while (p_st->curr_res > 0 && prev_ok)
	{
		if (XML_CHAR_IS_WS_OR_NUL(p_st->next_token)
		    && !xml_get_next(p_st, XML_WS_OP_1))
			return TRUE;

		tag_found = p_st->next_token == '<';

		if (!tag_found)
		{
			if (prev_ok)
				xml_set_bfr(p_st, 2);
			else
				xml_set_bfr(p_st, 0);

			tag_found = xml_find_ch(p_st, '<', XML_WS_OP_0);

			if (prev_ok)
			{
				xml_remove_trail_ws(p_st);

				if (p_st->value.len)
				{
					xml_resolve_refs(p_st);

					p_st->event_data.ch_data.value.p = p_st->value.p;
					p_st->event_data.ch_data.value.len = p_st->value.len;
					p_st->event_data.ch_data.last = 1;

					p_st->value.len = 0;

					cb_ret = (*p_st->cback)(XML_EVENT_TYPE_CH_DATA,
					                        &p_st->event_data.ch_data,
					                        p_st->p_usr_data);

					if (cb_ret == 0)
					{
						xml_incr_pars_res(p_st, 4);
						return FALSE;
					}
				}

				xml_set_bfr(p_st, 0);
			}

			if (!tag_found)
				return prev_ok;
		}
		else
		{
			p_st->p_last_stm = p_st->p_cur - 1;

			if (p_st->p_cur)
				p_st->p_copy = p_st->p_last_stm;

			(*p_st->cback)(XML_EVENT_TYPE_9, &p_st->event_data.stack,
			               p_st->p_usr_data);
		}

		if (!xml_get_next(p_st, XML_WS_OP_0))
			return FALSE;

		if (p_st->next_token == '?')
			prev_ok = xml_qm_elem(p_st);
		else if (p_st->next_token == '!')
			prev_ok = xml_ex_elem(p_st, 0);
		else if (p_st->next_token == '/')
			prev_ok = xml_etag_elem(p_st);
		else
			prev_ok = xml_tag_elem(p_st);

		if (!prev_ok)
			xml_incr_pars_res(p_st, 5);
	}

	XML_TRACE(DEBUG, "xml_elems prev_ok:%d", prev_ok);

	return prev_ok;
}

static BOOLEAN xml_qm_elem(tXML_ST *p_st)
{
	if (!xml_get_next(p_st, XML_WS_OP_0))
		return FALSE;

	if (!xml_find_str(p_st, (UINT8 const *)"?>"))
		return FALSE;

	xml_incr_pars_res(p_st, 1);

	return TRUE;
}

static BOOLEAN xml_ex_elem(tXML_ST *p_st, BOOLEAN prolog)
{
	UINT8 q_ch;

	if (!xml_get_next(p_st, XML_WS_OP_0))
		return FALSE;

	if (p_st->next_token == '-')
	{
		if (!xml_consume_str(p_st, (UINT8 const *)"--"))
			return FALSE;

		if (!xml_find_str(p_st, (UINT8 const *)"-->"))
			return FALSE;
	}
	else if (p_st->next_token == '[')
	{
		if (!xml_consume_str(p_st, (UINT8 const *)"[CDATA["))
			return FALSE;

		if (!xml_find_str(p_st, (UINT8 const *)"]]>"))
			return FALSE;

		xml_incr_pars_res(p_st, 1);
	}
	else if (prolog)
	{
		do
		{
			if (!xml_find_ch_n(p_st, (UINT8 const *)"<'\""))
				return FALSE;

			if (p_st->next_token == '\'' || p_st->next_token == '"')
			{
				q_ch = p_st->next_token;

				if (!xml_get_next(p_st, XML_WS_OP_0))
					return FALSE;

				if (!xml_find_ch(p_st, q_ch, XML_WS_OP_0))
					return FALSE;

				xml_get_next(p_st, XML_WS_OP_0);
			}
		} while (p_st->curr_res > 0 && p_st->next_token != '<');

		xml_incr_pars_res(p_st, 1);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

static BOOLEAN xml_tag_elem(tXML_ST *p_st)
{
	UINT8 cb_ret = 1;

	if (!xml_name(p_st))
		return FALSE;

	p_st->event_data.tag.prefix.p = p_st->prefix.p;
	p_st->event_data.tag.name.p = p_st->name.p;

	p_st->event_data.tag.prefix.len = p_st->prefix.len;
	p_st->event_data.tag.name.len = p_st->name.len;

	p_st->event_data.tag.p_last_stm = p_st->p_last_stm;

	cb_ret = (*p_st->cback)(XML_EVENT_TYPE_TAG, &p_st->event_data.tag,
	                        p_st->p_usr_data);

	if (cb_ret == 0)
	{
		xml_incr_pars_res(p_st, 4);
		return FALSE;
	}

	if (p_st->curr_res <= 0)
		return FALSE;

	if (XML_CHAR_IS_WS_OR_NUL(p_st->next_token) && !xml_attributes(p_st))
		return FALSE;

	p_st->event_data.empty_elem.end = p_st->next_token == '/';

	if (p_st->event_data.empty_elem.end && !xml_get_next(p_st, XML_WS_OP_0))
		return FALSE;

	if (p_st->next_token != '>')
		return FALSE;

	xml_get_next(p_st, XML_WS_OP_0);

	cb_ret = (*p_st->cback)(XML_EVENT_TYPE_EMPTY_ELEM,
	                        &p_st->event_data.empty_elem, p_st->p_usr_data);

	if (cb_ret == 0)
	{
		xml_incr_pars_res(p_st, 4);
		return FALSE;
	}

	p_st->p_copy = p_st->p_cur - 1;

	(*p_st->cback)(XML_EVENT_TYPE_9, &p_st->event_data.stack, p_st->p_usr_data);

	return TRUE;
}

static BOOLEAN xml_etag_elem(tXML_ST *p_st)
{
	UINT8 cb_ret = 1;

	if (!xml_get_next(p_st, XML_WS_OP_0))
		return FALSE;

	if (!xml_name(p_st))
		return FALSE;

	p_st->event_data.etag.prefix.p = p_st->prefix.p;
	p_st->event_data.etag.name.p = p_st->name.p;

	// why is it out of order
	p_st->event_data.etag.name.len = p_st->name.len;
	p_st->event_data.etag.prefix.len = p_st->prefix.len;

	cb_ret = (*p_st->cback)(XML_EVENT_TYPE_ETAG, &p_st->event_data,
	                        p_st->p_usr_data);

	if (cb_ret == 0)
	{
		xml_incr_pars_res(p_st, 4);
		return FALSE;
	}

	p_st->p_copy = p_st->prefix.p ? p_st->prefix.p - 2 : p_st->name.p - 2;

	(*p_st->cback)(XML_EVENT_TYPE_9, &p_st->event_data.stack, p_st->p_usr_data);

	if (p_st->curr_res <= 0)
		return FALSE;

	if (XML_CHAR_IS_WS_OR_NUL(p_st->next_token)
	    && !xml_get_next(p_st, XML_WS_OP_1))
		return FALSE;

	if (p_st->next_token != '>')
		return FALSE;

	xml_get_next(p_st, XML_WS_OP_0);

	return TRUE;
}
