
typedef struct MemoryContextData *MemoryContext;

typedef struct MemoryContextData
{
	MemoryContext parent;		/* NULL if no parent (toplevel context) */
	MemoryContext firstchild;	/* head of linked list of children */
	MemoryContext prevchild;	/* previous child of same parent */
	MemoryContext nextchild;	/* next child of same parent */
	const char *name;			/* context name (just for debugging) */
	const char *ident;			/* context ID if any (just for debugging) */
} MemoryContextData;

