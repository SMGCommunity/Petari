#include "xml_api.h"

/*******************************************************************************
 * headers
 */

#include <stdio.h> // sprintf

#include "data_types.h"

/*******************************************************************************
 * functions
 */

tXML_STATUS XML_BufAddTag(UINT8 **pp_buf, UINT8 const *prefix, UINT8 const *tag,
                     BOOLEAN start_tag, BOOLEAN has_attr)
{
	tXML_STATUS status = XML_ERROR;
	int n;

	if (tag)
	{
		if (start_tag)
			n = sprintf((char *)*pp_buf, "<");
		else
			n = sprintf((char *)*pp_buf, "</");

		*pp_buf += n;

		if (prefix)
		{
			n = sprintf((char *)*pp_buf, "%s:", (char const *)prefix);
			*pp_buf += n;
		}

		n = sprintf((char *)*pp_buf, "%s", (char const *)tag);
		*pp_buf += n;

		if (!has_attr)
		{
			n = sprintf((char *)*pp_buf, ">");
			*pp_buf += n;

			if (!start_tag)
			{
				n = sprintf((char *)*pp_buf, "\n");
				*pp_buf += n;
			}
		}

		status = XML_SUCCESS;
	}

	return status;
}

tXML_STATUS XML_BufAddAttribute(UINT8 **pp_buf, UINT8 const *prefix,
                           UINT8 const *attr_name, UINT8 const *attr_value,
                           UINT8 last_attr)
{
	tXML_STATUS status = XML_ERROR;
	int n;

	if (attr_name && attr_value)
	{
		n = sprintf((char *)*pp_buf, " ");
		*pp_buf += n;

		if (prefix)
		{
			n = sprintf((char *)*pp_buf, "%s:", (char const *)prefix);
			*pp_buf += n;
		}

		n = sprintf((char *)*pp_buf, "%s=\"%s", (char const *)attr_name,
		            (char const *)attr_value);
		*pp_buf += n;

		switch (last_attr)
		{
		case 0:
			n = sprintf((char *)*pp_buf, "\"");
			break;

		case 1:
			n = sprintf((char *)*pp_buf, "\">\n");
			break;

		case 2:
			n = sprintf((char *)*pp_buf, "\"/>\n");
			break;

		default:
			n = 0;
			break;
		}

		*pp_buf += n;

		status = XML_SUCCESS;
	}
	else if (last_attr == 2)
	{
		n = sprintf((char *)*pp_buf, "/>\n");
		*pp_buf += n;

		status = XML_SUCCESS;
	}

	return status;
}

tXML_STATUS XML_BufAddCharData(UINT8 **pp_buf, UINT8 const *charData)
{
	tXML_STATUS status = XML_ERROR;
	int n;

	if (charData)
	{
		n = sprintf((char *)*pp_buf, "%s", (char const *)charData);
		*pp_buf += n;

		status = XML_SUCCESS;
	}

	// TODO: Where can this use go
	(void)pp_buf;

	return status;
}
