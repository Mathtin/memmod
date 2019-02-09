#include "StrQueue_Static.h"
#include <stdlib.h>
#include <string.h>

typedef void* PTR;

typedef struct {
	char Str[MAXLINELENGTH + 1];
	PTR Next;
} StrQueueEl, *pStrQueueEl;
	
typedef struct {
	pStrQueueEl Start, End;
} StrQueue, *pStrQueue;


STRQUEUE_EXP PTR StrQueue_New() {
	pStrQueue p = (pStrQueue)malloc(sizeof(StrQueue));
	p->Start = NULL;
	p->End = NULL;
	return (PTR)p;
}

STRQUEUE_EXP void StrQueue_Delete(PTR p) {
	pStrQueue q = (pStrQueue)p;
	pStrQueueEl tmp, el = q->End;
	while (el) {
		tmp = el;
		el = (pStrQueueEl)el->Next;
		free(tmp);
	}
	free(p);
}
	
STRQUEUE_EXP void StrQueue_Push(PTR p, const char* str, size_t len) {
	pStrQueue q = (pStrQueue)p;
	if (len > MAXLINELENGTH) {
		len = MAXLINELENGTH;
	}
	pStrQueueEl el = (pStrQueueEl)malloc(sizeof(StrQueueEl));
	el->Next = NULL;
	memcpy(el->Str, str, len);
	el->Str[len] = '\0';
	if (q->Start) {
		q->Start->Next = el;
	} else {
		q->End = el;
	}
	q->Start = el;
}
	
STRQUEUE_EXP void StrQueue_Pop(PTR p, char* Buff, size_t size) {
	pStrQueue q = (pStrQueue)p;
	pStrQueueEl el = (pStrQueueEl)q->End;
	if (!el) {
		return;
	}
	if (size > MAXLINELENGTH + 1) {
		size = MAXLINELENGTH + 1;
	}
	memcpy(Buff, el->Str, size);
	q->End = (pStrQueueEl)el->Next;
	free(el);
	if (!q->End) {
		q->Start = NULL;
	}
}
	
STRQUEUE_EXP size_t StrQueue_Len(PTR p) {
	pStrQueue q = (pStrQueue)p;
	pStrQueueEl el = (pStrQueueEl)q->End;
	size_t len = 0;
	while (el) {
		el = (pStrQueueEl)el->Next;
		len++;
	}
	return len;
}

STRQUEUE_EXP int StrQueue_Empty(PTR p) {
	pStrQueue q = (pStrQueue)p;
	return q->End == NULL;
}

