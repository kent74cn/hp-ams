/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.container.conf 17798 2009-10-27 06:44:54Z magfr $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/table_container.h>

static void     _cache_free(netsnmp_cache * cache, void *magic);
static int      _cache_load(netsnmp_cache * cache, void *vmagic);
extern void    netsnmp_arch_pci_init();

/** Initializes the cpqSePciMemoryTable module */
void
init_cpqSePciMemoryTable(void)
{
    /*
     * here we initialize all the tables we're planning on supporting 
     */
    initialize_table_cpqSePciMemoryTable();
}

/** Initialize the cpqSePciMemoryTable table by defining its contents and how it's structured */
void
initialize_table_cpqSePciMemoryTable(void)
{
    const oid       cpqSePciMemoryTable_oid[] =
        { 1, 3, 6, 1, 4, 1, 232, 1, 2, 13, 3 };
    const size_t    cpqSePciMemoryTable_oid_len =
        OID_LENGTH(cpqSePciMemoryTable_oid);
    netsnmp_handler_registration *reg = NULL;
    netsnmp_mib_handler *handler = NULL;
    netsnmp_container *container = NULL;
    netsnmp_table_registration_info *table_info = NULL;
    netsnmp_cache  *cache = NULL;

    int reg_tbl_ret = SNMPERR_SUCCESS;

    DEBUGMSGTL(("cpqSePciMemoryTable:init",
                "initializing table cpqSePciMemoryTable\n"));

    reg =
        netsnmp_create_handler_registration("cpqSePciMemoryTable",
                                            cpqSePciMemoryTable_handler,
                                            cpqSePciMemoryTable_oid,
                                            cpqSePciMemoryTable_oid_len,
                                            HANDLER_CAN_RONLY);
    if (NULL == reg) {
        snmp_log(LOG_ERR,
                 "error creating handler registration for cpqSePciMemoryTable\n");
        goto bail;
    }

    container =
        netsnmp_container_find("cpqSePciMemoryTable:table_container");
    if (NULL == container) {
        snmp_log(LOG_ERR,
                 "error creating container for cpqSePciMemoryTable\n");
        goto bail;
    }
    container->container_name = strdup("PciFuncTable container");

    table_info = SNMP_MALLOC_TYPEDEF(netsnmp_table_registration_info);
    if (NULL == table_info) {
        snmp_log(LOG_ERR,
                 "error allocating table registration for cpqSePciMemoryTable\n");
        goto bail;
    }
    netsnmp_table_helper_add_indexes(table_info, ASN_INTEGER,   /* index: cpqSePciMemoryBusNumberIndex */
                                     ASN_INTEGER,       /* index: cpqSePciMemoryDeviceNumberIndex */
                                     ASN_INTEGER,       /* index: cpqSePciMemoryFunctionIndex */
                                     ASN_INTEGER,       /* index: cpqSePciMemoryIndex */
                                     0);
    table_info->min_column = COLUMN_CPQSEPCIMEMORYBUSNUMBERINDEX;
    table_info->max_column = COLUMN_CPQSEPCIMEMORYHWLOCATION;

    /*************************************************
     *
     * inject container_table helper
     */
    handler = netsnmp_container_table_handler_get(table_info, container,
                                                  TABLE_CONTAINER_KEY_NETSNMP_INDEX);
    if (NULL == handler) {
        snmp_log(LOG_ERR,
                 "error allocating table registration for cpqSePciMemoryTable\n");
        goto bail;
    }
    if (SNMPERR_SUCCESS != netsnmp_inject_handler(reg, handler)) {
        snmp_log(LOG_ERR,
                 "error injecting container_table handler for cpqSePciMemoryTable\n");
        goto bail;
    }
    handler = NULL;             /* reg has it, will reuse below */

    /*************************************************
     *
     * inject cache helper
     */
    cache = netsnmp_cache_create(240,    /* timeout in seconds */
                                 _cache_load, _cache_free,
                                 cpqSePciMemoryTable_oid,
                                 cpqSePciMemoryTable_oid_len);

    if (NULL == cache) {
        snmp_log(LOG_ERR,
                 "error creating cache for cpqSePciMemoryTable\n");
        goto bail;
    }
    cache->flags = NETSNMP_CACHE_DONT_INVALIDATE_ON_SET;
    cache->magic = container;

    handler = netsnmp_cache_handler_get(cache);
    if (NULL == handler) {
        snmp_log(LOG_ERR,
                 "error creating cache handler for cpqSePciMemoryTable\n");
        goto bail;
    }

    if (SNMPERR_SUCCESS != netsnmp_inject_handler(reg, handler)) {
        snmp_log(LOG_ERR,
                 "error injecting cache handler for cpqSePciMemoryTable\n");
        goto bail;
    }
    handler = NULL;             /* reg has it */

    /*
     * register the table
     */
    reg_tbl_ret = netsnmp_register_table(reg, table_info);
    if (reg_tbl_ret != SNMPERR_SUCCESS) {
        snmp_log(LOG_ERR,
                 "error registering table handler for cpqSePciMemoryTable\n");
        goto bail;
    }

    /*
     * Initialise the contents of the table here
     */

    netsnmp_arch_pci_init();
    return;                     /* ok */

    /*
     * Some error occurred during registration. Clean up and bail.
     */
  bail:                        /* not ok */

    if (handler)
        netsnmp_handler_free(handler);

    if (cache)
        netsnmp_cache_free(cache);

    if (table_info)
        netsnmp_table_registration_info_free(table_info);

    if (container)
        CONTAINER_FREE(container);

    if (reg_tbl_ret == SNMPERR_SUCCESS)
        if (reg)
            netsnmp_handler_registration_free(reg);
}

/** create a new row in the table */
cpqSePciMemoryTable_entry *
cpqSePciMemoryTable_createEntry(netsnmp_container * container,
                                oid cpqSePciMemoryBusNumberIndex,
                                oid cpqSePciMemoryDeviceNumberIndex,
                                oid cpqSePciMemoryFunctionIndex,
                                oid cpqSePciMemoryIndex)
{
    cpqSePciMemoryTable_entry *entry;

    entry = SNMP_MALLOC_TYPEDEF(cpqSePciMemoryTable_entry);
    if (!entry)
        return NULL;

    entry->cpqSePciMemoryBusNumberIndex = cpqSePciMemoryBusNumberIndex;
    entry->cpqSePciMemoryDeviceNumberIndex =
        cpqSePciMemoryDeviceNumberIndex;
    entry->cpqSePciMemoryFunctionIndex = cpqSePciMemoryFunctionIndex;
    entry->cpqSePciMemoryIndex = cpqSePciMemoryIndex;
    entry->oid_index.len = 4;
    entry->oid_index.oids = (oid *) &entry->cpqSePciMemoryBusNumberIndex;
    return entry;
}

/** remove a row from the table */
void
cpqSePciMemoryTable_removeEntry(netsnmp_container * container,
                                cpqSePciMemoryTable_entry * entry)
{

    if (!entry)
        return;                 /* Nothing to remove */
    CONTAINER_REMOVE(container, entry);
    if (entry)
        SNMP_FREE(entry);       /* XXX - release any other internal resources */
}

/** remove a row from the table */
void
cpqSePciMemoryTable_freeEntry(cpqSePciMemoryTable_entry * entry)
{

    if (!entry)
        return;                 /* Nothing to remove */
    SNMP_FREE(entry);           /* XXX - release any other internal resources */
}

/** handles requests for the cpqSePciMemoryTable table */
int
cpqSePciMemoryTable_handler(netsnmp_mib_handler *handler,
                            netsnmp_handler_registration *reginfo,
                            netsnmp_agent_request_info *reqinfo,
                            netsnmp_request_info *requests)
{

    netsnmp_request_info *request;
    netsnmp_table_request_info *table_info;
    cpqSePciMemoryTable_entry *table_entry;

    DEBUGMSGTL(("cpqSePciMemoryTable:handler", "Processing request (%d)\n",
                reqinfo->mode));

    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
    case MODE_GET:
        for (request = requests; request; request = request->next) {
            if (request->processed)
                continue;
            table_entry = (cpqSePciMemoryTable_entry *)
                netsnmp_container_table_extract_context(request);
            table_info = netsnmp_extract_table_info(request);
            if ((NULL == table_entry) || (NULL == table_info)) {
                snmp_log(LOG_ERR,
                         "could not extract table entry or info for cpqSePciMemoryTable\n");
                snmp_set_var_typed_value(request->requestvb,
                                         SNMP_ERR_GENERR, NULL, 0);
                continue;
            }

            switch (table_info->colnum) {
            case COLUMN_CPQSEPCIMEMORYBUSNUMBERINDEX:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqSePciMemoryBusNumberIndex);
                break;
            case COLUMN_CPQSEPCIMEMORYDEVICENUMBERINDEX:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqSePciMemoryDeviceNumberIndex);
                break;
            case COLUMN_CPQSEPCIMEMORYFUNCTIONINDEX:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqSePciMemoryFunctionIndex);
                break;
            case COLUMN_CPQSEPCIMEMORYINDEX:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqSePciMemoryIndex);
                break;
            case COLUMN_CPQSEPCIMEMORYBASEADDR:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqSePciMemoryBaseAddr);
                break;
            case COLUMN_CPQSEPCIMEMORYTYPE:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqSePciMemoryType);
                break;
            case COLUMN_CPQSEPCIMEMORYSIZE:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqSePciMemorySize);
                break;
            case COLUMN_CPQSEPCIMEMORYHWLOCATION:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                         table_entry->
                                         cpqSePciMemoryHwLocation,
                                         table_entry->
                                         cpqSePciMemoryHwLocation_len);
                break;
            default:
                netsnmp_set_request_error(reqinfo, request,
                                          SNMP_NOSUCHOBJECT);
                break;
            }
        }
        break;

    }
    return SNMP_ERR_NOERROR;
}

/**
 * @internal
 */
static int
_cache_load(netsnmp_cache * cache, void *vmagic)
{
    netsnmp_container *container;

    DEBUGMSGTL(("internal:cpqSePciMemoryTable:_cache_load", "called\n"));

    if ((NULL == cache) || (NULL == cache->magic)) {
        snmp_log(LOG_ERR,
                 "invalid cache for cpqSePciMemoryTable_cache_load\n");
        return -1;
    }
    container = (netsnmp_container *) cache->magic;

    /** should only be called for an invalid or expired cache */
    netsnmp_assert((0 == cache->valid) || (1 == cache->expired));

    /*
     * load cache here (or call function to do it)
     */
    netsnmp_arch_pcimem_container_load(container);

    return 0;
}                               /* _cache_load */

/**
 * @Internal
 */
/** remove a row from the table */
static void
cpqSePciMemoryTable_freeEntry_cb(cpqSePciMemoryTable_entry * entry,
                                 void *magic)
{

    cpqSePciMemoryTable_freeEntry(entry);
}

/**
 * @internal
 */
static void
_cache_free(netsnmp_cache * cache, void *magic)
{
    netsnmp_container *container;

    DEBUGMSGTL(("internal:cpqSePciMemoryTable:_cache_free", "called\n"));

    if ((NULL == cache) || (NULL == cache->magic)) {
        snmp_log(LOG_ERR,
                 "invalid cache in cpqSePciMemoryTable_cache_free\n");
        return;
    }
    container = (netsnmp_container *) cache->magic;

    /*
     * empty (but don't free) cache here
     */
    CONTAINER_CLEAR(container,
                    (netsnmp_container_obj_func *)
                    cpqSePciMemoryTable_freeEntry_cb, NULL);
}                               /* _cache_free */
