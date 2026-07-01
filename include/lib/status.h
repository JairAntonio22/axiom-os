#ifndef STATUS_H
#define STATUS_H

typedef enum {
	OK,
	ACCEPTED,
	EINVAL,
	ENOMEM,
	EBUSY,
	EFULL,
	ETIMEOUT,
	EIO,
	ENOTFOUND,
	ENOTSUPPORTED,
	EEXIST,
	ESTATE,
	EPERM
} status_code;

#endif
