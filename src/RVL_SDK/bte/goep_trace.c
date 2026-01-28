#include "goep_api.h"

/*******************************************************************************
 * functions
 */

char const *GOEP_ErrorName(tGOEP_STATUS error)
{
	switch (error)
	{
	case GOEP_SUCCESS:
		return "GOEP_SUCCESS (0x00)";

	case GOEP_ERROR:
		return "GOEP_ERROR (0x01)";

	case GOEP_RESOURCES:
		return "GOEP_RESOURCES (0x02)";

	case GOEP_INVALID_PARAM:
		return "GOEP_INVALID_PARAM (0x03)";

	default:
		return "UNKNOWN GOEP ERROR";
	}
}

