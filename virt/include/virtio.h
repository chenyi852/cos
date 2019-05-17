#ifndef _COS_VIRTIO_H
#define _COS_VIRTIO_H

#include "types.h"

#define QUEUE_SIZE	16

struct virtq_desc {
	/* Address (guest-physical). */
	u64 addr;
	/* Length. */
	u32 len;
	/* This marks a buffer as continuing via the next field. */
#define VIRTQ_DESC_F_NEXT 1
	/* This marks a buffer as device write-only (otherwise device read-only). */
#define VIRTQ_DESC_F_WRITE 2
	/* This means the buffer contains a list of buffer descriptors. */
#define VIRTQ_DESC_F_INDIRECT 4
	/* The flags as indicated above. */
	u16 flags;
	/* Next field if flags & NEXT */
	u16 next;
};

struct virtq_avail {
#define VIRTQ_AVAIL_F_NO_INTERRUPT 1
	u16 flags;
	u16 idx;
	u16 ring[QUEUE_SIZE];
	u16 used_event; /* Only if VIRTIO_F_EVENT_IDX */
};

/* u32 is used here for ids for padding reasons. */
struct virtq_used_elem {
	/* Index of start of used descriptor chain. */
	u32 id;
	/* Total length of the descriptor chain which was used (written to) */
	u32 len;
};

struct virtq_used {
#define VIRTQ_USED_F_NO_NOTIFY 1
	u16 flags;
	u16 idx;
	struct virtq_used_elem ring[QUEUE_SIZE];
	u16 avail_event; /* Only if VIRTIO_F_EVENT_IDX */
};

struct virtq {
	// The actual descriptors (16 bytes each)
	struct virtq_desc desc[ QUEUE_SIZE ];
	// A ring of available descriptor heads with free-running index.
	struct virtq_avail avail;
	// Padding to the next Queue Align boundary.
	u8 pad[16];
	// A ring of used descriptor heads with free-running index.
	struct virtq_used used;
};

#endif
