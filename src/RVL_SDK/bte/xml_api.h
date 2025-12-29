#ifndef BTE_XML_API_H
#define BTE_XML_API_H

/*******************************************************************************
 * headers
 */

#include "data_types.h"

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef UINT16 tXML_STATUS;
enum
{
	XML_SUCCESS,
	XML_ERROR,
};

typedef UINT16 tXML_EVENT_TYPE;
enum
{
	XML_EVENT_TYPE_NONE,
	XML_EVENT_TYPE_TAG,
	XML_EVENT_TYPE_EMPTY_ELEM,
	XML_EVENT_TYPE_ATTR,
	XML_EVENT_TYPE_CH_DATA,
	XML_EVENT_TYPE_ETAG,
	XML_EVENT_TYPE_PART,
	XML_EVENT_TYPE_COPY,
	XML_EVENT_TYPE_QUERY, // query of what?
	XML_EVENT_TYPE_9, // preceded by setting of p_copy?
};

typedef UINT16 tXML_WS_OP;
enum
{
	// I don't know what exactly these represent
	XML_WS_OP_0,
	XML_WS_OP_1,
	XML_WS_OP_2,
};

typedef UINT8 tXML_CBACK(tXML_EVENT_TYPE, void *, void *);

typedef struct
{
	UINT8	*p_hdr;		// size 0x04, offset 0x00
	UINT8	*p_begin;	// size 0x04, offset 0x04
	UINT8	*p_end;		// size 0x04, offset 0x08
} tXML_OS; // size 0x0c

typedef struct
{
	UINT8	*p;		// size 0x04, offset 0x00
	UINT16	len;	// size 0x02, offset 0x04
	/* 2 bytes padding */
} tXML_BFR; // size 0x08

typedef struct
{
	UINT8	stack[7];	// size 0x07, offset 0x00
	UINT8	top;		// size 0x01, offset 0x07
	UINT16	event;		// size 0x02, offset 0x08
} tXML_STACK; // size 0x0a

typedef struct
{
	UINT8	*p_version;		// size 0x04, offset 0x00
	UINT8	*p_enc_name;	// size 0x04, offset 0x04
	UINT8	*p_standalone;	// size 0x04, offset 0x08
} tXML_DECL; // size 0x0c

typedef struct
{
	tXML_STACK	stack;			// size 0x08, offset 0x00
	/* 2 bytes padding */
	tXML_BFR	prefix;			// size 0x08, offset 0x0c
	tXML_BFR	name;			// size 0x08, offset 0x14
	UINT8		*p_last_stm;	// size 0x04, offset 0x1c
} tXML_TAG; // size 0x20

typedef struct
{
	tXML_STACK	stack;	// size 0x0a, offset 0x00
	BOOLEAN		end;	// size 0x01, offset 0x0a
	/* 1 byte padding */
} tXML_EMPTY_ELEM; // size 0x0c

typedef struct
{
	tXML_STACK	stack;	// size 0x0a. offset 0x00
	/* 2 bytes padding */
	tXML_BFR	prefix;	// size 0x08. offset 0x0c
	tXML_BFR	name;	// size 0x08. offset 0x14
	tXML_BFR	value;	// size 0x08. offset 0x1c
} tXML_ATTR; // size 0x24

typedef struct
{
	tXML_STACK	stack;	// size 0x0a, offset 0x00
	/* 2 bytes padding */
	tXML_BFR	value;	// size 0x08, offset 0x0c
	UINT8		last;	// size 0x01, offset 0x14
	/* 3 bytes padding */
} tXML_CH_DATA; // size 0x18

typedef struct
{
	tXML_STACK	stack;	// size 0x0a, offset 0x00
	/* 2 bytes padding */
	tXML_BFR	prefix;	// size 0x08, offset 0x0c
	tXML_BFR	name;	// size 0x08, offset 0x14
} tXML_ETAG; // size 0x1c

typedef struct
{
	tXML_STACK	stack;		// size 0x0a, offset 0x00
	/* 2 bytes padding */
	UINT8		*p_begin;	// size 0x04, offset 0x0c
	tXML_BFR	last;		// size 0x08, offset 0x10
} tXML_COPY; // size 0x18

typedef struct
{
	tXML_STACK	stack;		// size 0x0a, offset 0x00
	UINT16		parse;		// size 0x02, offset 0x0a
	UINT8		*p_keep;	// size 0x04, offset 0x0c
} tXML_PART; // size 0x10

typedef union
{
	tXML_STACK		stack;		// size 0x0a
	tXML_DECL		decl;		// size 0x0c
	tXML_TAG		tag;		// size 0x20
	tXML_EMPTY_ELEM	empty_elem;	// size 0x0c
	tXML_ATTR		attr;		// size 0x24
	tXML_CH_DATA	ch_data;	// size 0x18
	tXML_ETAG		etag;		// size 0x1c
	tXML_COPY		copy;		// size 0x18
	tXML_PART		part;		// size 0x10
} tXML_EVENT_DATA; // size 0x24

typedef struct
{
	UINT8	p[512];	// size 0x200, offset 0x000
	UINT16	len;	// size 0x002, offset 0x200
} tXML_BIG_BFR; // size 0x202

typedef struct
{
	tXML_BIG_BFR	last_bfr;		// size 0x202, offset 0x000
	UINT16			used_last_bfr;	// size 0x002, offset 0x202
	tXML_EVENT_DATA	event_data;		// size 0x024, offset 0x204
} tXML_PARTIAL_ST; // size 0x228

typedef struct
{
	tXML_OS			xml_os;			// size 0x00c, offset 0x000
	UINT8			*p_cur;			// size 0x004, offset 0x00c
	tXML_CBACK		*cback;			// size 0x004, offset 0x010
	void			*p_usr_data;	// size 0x004, offset 0x014
	tXML_EVENT_DATA	event_data;		// size 0x024, offset 0x018
	UINT8			*p_data_bfr;	// size 0x004, offset 0x03c
	UINT8			*p_last_stm;	// size 0x004, offset 0x040
	UINT8			*p_copy;		// size 0x004, offset 0x044
	tXML_BFR		last_bfr;		// size 0x008, offset 0x048
	tXML_BFR		value;			// size 0x008, offset 0x050
	tXML_BFR		prefix;			// size 0x008, offset 0x058
	tXML_BFR		name;			// size 0x008, offset 0x060
	INT16			curr_res;		// size 0x002, offset 0x068
	UINT16			pars_res;		// size 0x002, offset 0x06a
	UINT8			next_token;		// size 0x001, offset 0x06c
	BOOLEAN			skip_next_nl;	// size 0x001, offset 0x06d
	UINT16			used_last_bfr;	// size 0x002, offset 0x06e
	UINT8			status;			// size 0x001, offset 0x070
	/* 3 bytes padding */
	tXML_PARTIAL_ST	partial_st;		// size 0x228, offset 0x074
} tXML_ST; // size 0x29c

/*******************************************************************************
 * functions
 */

void XML_InitPars(tXML_ST *p_st, tXML_CBACK *xml_cback, void *p_usr_data);
tXML_STATUS XML_MulParse(tXML_ST *p_st, tXML_OS *p_os);
tXML_STATUS XML_BufAddTag(UINT8 **pp_buf, UINT8 const *prefix, UINT8 const *tag,
                          BOOLEAN start_tag, BOOLEAN has_attr);
tXML_STATUS XML_BufAddAttribute(UINT8 **pp_buf, UINT8 const *prefix,
                                UINT8 const *attr_name, UINT8 const *attr_value,
                                UINT8 last_attr);
tXML_STATUS XML_BufAddCharData(UINT8 **pp_buf, UINT8 const *charData);

#ifdef __cplusplus
	}
#endif

#endif // BTE_XML_API_H
