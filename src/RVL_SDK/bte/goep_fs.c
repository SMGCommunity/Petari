#include "goep_fs.h"

/*******************************************************************************
 * headers
 */

#include <string.h> // memset

#include "gki_target.h" // PPC_TASK
#include "data_types.h"

#include "gki.h"

/*******************************************************************************
 * functions
 */

void GOEP_OpenRsp(tGOEP_FD fd, tGOEP_STATUS status, UINT32 file_size,
                  UINT16 event_id)
{
	tGOEP_OPEN_RSP *p_evt_msg;
	UINT16 size = sizeof *p_evt_msg;

	p_evt_msg = GKI_getbuf(size);
	if (!p_evt_msg)
		return;

	memset(&p_evt_msg->hdr, 0, sizeof p_evt_msg->hdr);
	p_evt_msg->hdr.event	= event_id;
	p_evt_msg->fd			= fd;
	p_evt_msg->status		= status;
	p_evt_msg->file_size	= file_size;

	GKI_send_msg(PPC_TASK, TASK_MBOX_0, p_evt_msg);
}

void GOEP_ReadRsp(tGOEP_FD fd, tGOEP_STATUS status, UINT16 bytes_read,
                  UINT16 event_id)
{
	tGOEP_READ_RSP *p_evt_msg;
	UINT16 size = sizeof *p_evt_msg;

	p_evt_msg = GKI_getbuf(size);
	if (!p_evt_msg)
		return;

	memset(&p_evt_msg->hdr, 0, sizeof p_evt_msg->hdr);
	p_evt_msg->hdr.event	= event_id;
	p_evt_msg->fd			= fd;
	p_evt_msg->status		= status;
	p_evt_msg->bytes_read	= bytes_read;

	GKI_send_msg(PPC_TASK, TASK_MBOX_0, p_evt_msg);
}

void GOEP_WriteRsp(tGOEP_FD fd, tGOEP_STATUS status, UINT16 event_id)
{
	tGOEP_WRITE_RSP *p_evt_msg;
	UINT16 size = sizeof *p_evt_msg;

	p_evt_msg = GKI_getbuf(size);
	if (!p_evt_msg)
		return;

	memset(&p_evt_msg->hdr, 0, sizeof p_evt_msg->hdr);
	p_evt_msg->hdr.event	= event_id;
	p_evt_msg->fd			= fd;
	p_evt_msg->status		= status;

	GKI_send_msg(PPC_TASK, TASK_MBOX_0, p_evt_msg);
}

void GOEP_DirentryRsp(tGOEP_STATUS status, UINT16 event_id)
{
	tGOEP_DIRENTRY_RSP *p_evt_msg;
	UINT16 size = sizeof *p_evt_msg;

	p_evt_msg = GKI_getbuf(size);
	if (!p_evt_msg)
		return;

	memset(&p_evt_msg->hdr, 0, sizeof p_evt_msg->hdr);
	p_evt_msg->hdr.event	= event_id;
	p_evt_msg->status		= status;

	GKI_send_msg(PPC_TASK, TASK_MBOX_0, p_evt_msg);
}
