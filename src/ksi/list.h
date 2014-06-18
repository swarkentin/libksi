#ifndef KSI_LIST_H_
#define KSI_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct KSI_List_st KSI_List;

#define KSI_LIST(type) type##List

#define KSI_LIST_FN_NAME(type, name) type##List_##name
#define KSI_DEFINE_LIST(type) 													\
typedef struct type##_list_st KSI_LIST(type);									\
void KSI_LIST_FN_NAME(type, free)(KSI_LIST(type) *);							\
int KSI_LIST_FN_NAME(type, new)(KSI_CTX *, KSI_LIST(type) **);					\
int KSI_LIST_FN_NAME(type, append)(KSI_LIST(type) *, type *);					\
int KSI_LIST_FN_NAME(type, iter)(KSI_LIST(type) *);								\
int KSI_LIST_FN_NAME(type, next)(KSI_LIST(type) *, type **);					\
int KSI_LIST_FN_NAME(type, indexOf)(const KSI_LIST(type) *, const type *);		\
int KSI_LIST_FN_NAME(type, insertAt)(KSI_LIST(type) *, int, type *);			\
int KSI_LIST_FN_NAME(type, replaceAt)(KSI_LIST(type) *, int, type *);			\
int KSI_LIST_FN_NAME(type, remove)(KSI_LIST(type) *, int);						\
int KSI_LIST_FN_NAME(type, length)(const KSI_LIST(type) *);						\
int KSI_LIST_FN_NAME(type, elementAt)(const KSI_LIST(type) *, int pos, type **);		\
KSI_CTX *type##List_getCtx(const KSI_LIST(type) *o);							\



void KSI_List_free(KSI_List *list);
int KSI_List_new(void (*obj_free)(void *), KSI_List **list);
int KSI_List_append(KSI_List *list, void *obj);
int KSI_List_remove(KSI_List *list, unsigned int pos);
int KSI_List_iter(KSI_List *list);
int KSI_List_next(KSI_List *list, void **o);
int KSI_List_indexOf(const KSI_List *list, const void *o);
int KSI_List_insertAt(KSI_List *list, unsigned int pos, void *o);
int KSI_List_replaceAt(KSI_List *list, unsigned int pos, void *o);
int KSI_List_elementAt(const KSI_List *list, unsigned int pos, void **o);
int KSI_List_length(const KSI_List *list);


#ifdef __cplusplus
}
#endif

#endif /* KSI_LIST_H_ */