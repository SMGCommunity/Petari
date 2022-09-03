typedef struct DestructorChain {
    struct DestructorChain* next;
    void* dtor;
    void* obj;
} DestructorChain;


DestructorChain* __global_destructor_chain;

void* __register_global_object(void *pObj, void *pDtor, void *pReg) {
    DestructorChain* chain = pReg;
    chain->next = __global_destructor_chain;
    chain->dtor = pDtor;
    chain->obj = pObj;
    __global_destructor_chain = pReg;
    return pObj;
}

void __destroy_global_chain(void) {
    DestructorChain* chain;
    void* obj;

    while ((chain = __global_destructor_chain) != 0) {
        __global_destructor_chain = chain->next;
        obj = chain->obj;
        (((void (*)(void *,short))chain->dtor)(obj,-1));
    }
}

__declspec(section ".dtors") 
static void* const __destroy_global_chain_reference = __destroy_global_chain;