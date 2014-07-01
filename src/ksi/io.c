#include <stdlib.h>
#include <string.h>

#include "internal.h"

typedef enum {
	KSI_IO_FILE,
	KSI_IO_MEM
} KSI_IO_Type;

struct KSI_RDR_st {
	/* Context for the reader. */
	KSI_CTX *ctx;

	/* Type of the reader (see #KSI_IO_Type) */
	int ioType;

	/* Union of inputs. */
	union {
		/* KSI_IO_FILE type input. */
		FILE *file;

		/* KSI_IO_MEM type input */
		struct {
			unsigned char *buffer;
			size_t buffer_length;

			/* Does the memory belong to this reader? */
			int ownCopy;
		} mem;
	} data;

	/* Offset of stream. */
	size_t offset;

	/* Indicates end of stream.
	 * \note This will be set after reading the stream. */
	int eof;
};

static KSI_RDR *newReader(KSI_CTX *ctx, KSI_IO_Type ioType) {
	KSI_RDR *rdr = NULL;
	rdr = KSI_new(KSI_RDR);
	if (rdr == NULL) goto cleanup;

	rdr->ctx = ctx;
	rdr->eof = 0;
	rdr->ioType = ioType;
	rdr->offset = 0;

cleanup:

	return rdr;
}

int KSI_RDR_getOffset(KSI_RDR *rdr, size_t *offset) {
	KSI_ERR err;
	KSI_PRE(&err, rdr != NULL) goto cleanup;
	KSI_PRE(&err, offset != NULL) goto cleanup;
	KSI_BEGIN(rdr->ctx, &err);

	*offset = rdr->offset;

	KSI_SUCCESS(&err);

cleanup:

	return KSI_RETURN(&err);
}

int KSI_RDR_fromStream(KSI_CTX *ctx, FILE *file, KSI_RDR **rdr) {
	KSI_ERR err;
	KSI_RDR *reader = NULL;

	KSI_PRE(&err, ctx != NULL) goto cleanup;
	KSI_PRE(&err, file != NULL) goto cleanup;

	KSI_BEGIN(ctx, &err);

	reader = newReader(ctx, KSI_IO_FILE);
	if (reader == NULL) {
		KSI_FAIL(&err, KSI_OUT_OF_MEMORY, NULL);
		goto cleanup;
	}

	reader->data.file = file;

	*rdr = reader;
	reader = NULL;

	KSI_SUCCESS(&err);

cleanup:

	KSI_RDR_close(reader);

	return KSI_RETURN(&err);
}

int KSI_RDR_fromFile(KSI_CTX *ctx, const char *fileName, const char *flags, KSI_RDR **rdr) {
	KSI_ERR err;
	KSI_RDR *reader = NULL;
	FILE *file = NULL;
	int res;

	KSI_BEGIN(ctx, &err);

	file = fopen(fileName, flags);
	if (file == NULL) {
		KSI_FAIL(&err, KSI_IO_ERROR, "Unable to open file");
		goto cleanup;
	}

	res = KSI_RDR_fromStream(ctx, file, &reader);
	KSI_CATCH(&err, res) goto cleanup;

	file = NULL;

	*rdr = reader;
	reader = NULL;

	KSI_SUCCESS(&err);

cleanup:

	if (file != NULL) fclose(file);
	KSI_RDR_close(reader);

	return KSI_RETURN(&err);
}


static int createReader_fromMem(KSI_CTX *ctx, unsigned char *buffer, const size_t buffer_length, int ownCopy, KSI_RDR **rdr) {
	KSI_ERR err;
	KSI_RDR *reader = NULL;

	KSI_BEGIN(ctx, &err);

	reader = newReader(ctx, KSI_IO_MEM);
	if (reader == NULL) {
		KSI_FAIL(&err, KSI_OUT_OF_MEMORY, NULL);
		goto cleanup;
	}

	reader->data.mem.buffer = buffer;

	reader->data.mem.buffer_length = buffer_length;
	reader->data.mem.ownCopy = ownCopy;

	*rdr = reader;
	reader = NULL;

	KSI_SUCCESS(&err);
cleanup:

	KSI_RDR_close(reader);

	return KSI_RETURN(&err);
}

int KSI_RDR_fromMem(KSI_CTX *ctx, const unsigned char *buffer, const size_t buffer_length, KSI_RDR **rdr) {
	KSI_ERR err;
	int res;
	unsigned char *buf = NULL;

	KSI_BEGIN(ctx, &err);

	buf = KSI_calloc(buffer_length, 1);
	if (buf == NULL) {
		KSI_FAIL(&err, KSI_OUT_OF_MEMORY, NULL);
		goto cleanup;
	}
	memcpy(buf, buffer, buffer_length);

	res = createReader_fromMem(ctx, buf, buffer_length, 1, rdr);
	KSI_CATCH(&err, res) goto cleanup;

	buf = NULL;

	KSI_SUCCESS(&err);

cleanup:

	KSI_free(buf);

	return KSI_RETURN(&err);
}

int KSI_RDR_fromSharedMem(KSI_CTX *ctx, unsigned char *buffer, const size_t buffer_length, KSI_RDR **rdr) {
	KSI_ERR err;
	int res;

	KSI_BEGIN(ctx, &err);

	res = createReader_fromMem(ctx, buffer, buffer_length, 0, rdr);
	KSI_CATCH(&err, res) goto cleanup;

	KSI_SUCCESS(&err);

cleanup:

	return KSI_RETURN(&err);
}


int KSI_RDR_isEOF(KSI_RDR *rdr) {
	return rdr->eof;
}

static int readFromFile(KSI_RDR *rdr, unsigned char *buffer, const size_t size, unsigned int *readCount) {
	KSI_ERR err;
	int count;

	KSI_PRE(&err, rdr != NULL) goto cleanup;
	KSI_PRE(&err, buffer != NULL) goto cleanup;
	KSI_PRE(&err, readCount != NULL) goto cleanup;

	KSI_BEGIN(rdr->ctx, &err);
	count = fread(buffer, 1, size, rdr->data.file);
	/* Update metadata. */
	rdr->offset += count;
	rdr->eof = feof(rdr->data.file);

	*readCount = count;

	KSI_SUCCESS(&err);

cleanup:

	return KSI_RETURN(&err);
}

static int readFromMem(KSI_RDR *rdr, unsigned char *buffer, const size_t size, unsigned int *readCount) {
	KSI_ERR err;
	unsigned int count;

	KSI_PRE(&err, rdr != NULL) goto cleanup;
	KSI_PRE(&err, buffer != NULL) goto cleanup;
	KSI_PRE(&err, readCount != NULL) goto cleanup;
	KSI_BEGIN(rdr->ctx, &err);

	/* Max bytes still to read. */
	count = rdr->data.mem.buffer_length - rdr->offset;

	/* Update if requested for less. */
	if (count > size) count = size;

	memcpy(buffer, rdr->data.mem.buffer + rdr->offset, count);

	/* Update metadata */
	rdr->offset += count;
	rdr->eof = (rdr->offset == rdr->data.mem.buffer_length);

	if (readCount != NULL) *readCount = count;

	KSI_SUCCESS(&err);

cleanup:

	return KSI_RETURN(&err);
}


int KSI_RDR_read_ex(KSI_RDR *rdr, unsigned char *buffer, const size_t bufferLength, unsigned int *readCount)  {
	KSI_ERR err;
	int res;

	KSI_BEGIN(rdr->ctx, &err);

	switch (rdr->ioType) {
		case KSI_IO_FILE:
			res = readFromFile(rdr, buffer, bufferLength, readCount);
			break;
		case KSI_IO_MEM:
			res = readFromMem(rdr, buffer, bufferLength, readCount);
			break;
		default:
			KSI_FAIL(&err, KSI_UNKNOWN_ERROR, "Unsupported KSI IO TYPE");
			goto cleanup;
	}

	if (res != KSI_OK) {
		KSI_FAIL(&err, res, NULL);
		goto cleanup;
	}

	KSI_SUCCESS(&err);

cleanup:

	return KSI_RETURN(&err);
}

int KSI_RDR_read_ptr(KSI_RDR *rdr, unsigned char **ptr, const size_t len, unsigned int *readCount) {
	KSI_ERR err;
	unsigned char *p = NULL;
	int count = 0;

	KSI_PRE(&err, rdr != NULL) goto cleanup;
	KSI_PRE(&err, ptr != NULL) goto cleanup;
	KSI_PRE(&err, readCount != NULL) goto cleanup;
	KSI_BEGIN(rdr->ctx, &err);

	switch (rdr->ioType) {
		case KSI_IO_FILE:
			break;
		case KSI_IO_MEM:
			if (rdr->offset < rdr->data.mem.buffer_length) {
				p = rdr->data.mem.buffer + rdr->offset;
				count = len;
				if (rdr->offset + count > rdr->data.mem.buffer_length) {
					count = rdr->data.mem.buffer_length - rdr->offset;
					rdr->eof = 1;
				}

				rdr->offset += count;

				*ptr = p;
				*readCount = count;
			} else {
				rdr->eof = 1;
				*readCount = 0;
			}
			break;
		default:
			KSI_FAIL(&err, KSI_UNKNOWN_ERROR, "Unsupported KSI IO TYPE");
			goto cleanup;
	}

	KSI_SUCCESS(&err);

cleanup:

	return KSI_RETURN(&err);
}

void KSI_RDR_close(KSI_RDR *rdr)  {
	KSI_CTX *ctx = NULL;

	if (rdr == NULL) return;

	ctx = rdr->ctx;
	rdr->ctx = NULL;

	switch (rdr->ioType) {
		case KSI_IO_FILE:
			if (rdr->data.file != NULL) {
				if (fclose(rdr->data.file)) {
					rdr->data.file = NULL;
					KSI_LOG_warn(ctx, "Unable to close log file.");
				}
			}
			rdr->data.file = NULL;
			break;
		case KSI_IO_MEM:
			if (rdr->data.mem.ownCopy) {
				KSI_free(rdr->data.mem.buffer);
				rdr->data.mem.buffer = NULL;
			}
			break;
		default:
			KSI_LOG_warn(ctx, "Unsupported KSI IO-type - possible MEMORY LEAK");
	}

	KSI_free(rdr);
}

int KSI_RDR_verifyEnd(KSI_RDR *rdr) {
	KSI_ERR err;
	int res;
	unsigned char *buf = NULL;
	unsigned int buf_len = 0;

	KSI_PRE(&err, rdr != NULL) goto cleanup;
	KSI_BEGIN(rdr->ctx, &err);

	res = KSI_RDR_read_ptr(rdr, &buf, 1, &buf_len);
	if (res != KSI_OK || buf != NULL) {
		KSI_FAIL(&err, KSI_INVALID_FORMAT, NULL);
		goto cleanup;
	}

	KSI_SUCCESS(&err);

cleanup:

	KSI_nofree(buf);

	return KSI_RETURN(&err);

}

KSI_IMPLEMENT_GET_CTX(KSI_RDR);
