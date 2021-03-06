/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.table_data.conf 19302 2010-08-13 12:19:42Z dts12 $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/table_container.h>
#include "cpqHoSwVerTable.h"

extern int cpqhost_arch_swinst_container_load(netsnmp_container*);
extern int cpqhost_arch_cpqHoSwVer_container_load(netsnmp_container*);

static void     _cache_free(netsnmp_cache * cache, void *magic);
static int      _cache_load(netsnmp_cache * cache, void *vmagic);
const oid       cpqHoSwVerTable_oid[] =
        { 1, 3, 6, 1, 4, 1, 232, 11, 2, 7, 2 };
const size_t    cpqHoSwVerTable_oid_len =
        OID_LENGTH(cpqHoSwVerTable_oid);
/** Initializes the cpqHoSwVerTable module */
void
init_cpqHoSwVerTable(void)
{
    /*
     * here we initialize all the tables we're planning on supporting 
     */
    netsnmp_cpqHoSwVer_arch_init();
    initialize_table_cpqHoSwVerTable();
}


/** Initialize the cpqHoSwVerTable table by defining its contents and how it's structured */
void
initialize_table_cpqHoSwVerTable(void)
{
    netsnmp_handler_registration *reg = NULL;
    netsnmp_mib_handler *handler = NULL;
    netsnmp_container *container = NULL;
    netsnmp_table_registration_info *table_info = NULL;
    netsnmp_cache  *cache = NULL;

    int reg_tbl_ret = SNMPERR_SUCCESS;

    DEBUGMSGTL(("cpqHoSwVerTable:init",
                "initializing table cpqHoSwVerTable\n"));

    reg =
        netsnmp_create_handler_registration("cpqHoSwVerTable",
                                            cpqHoSwVerTable_handler,
                                            cpqHoSwVerTable_oid,
                                            cpqHoSwVerTable_oid_len,
                                            HANDLER_CAN_RONLY);
    if (NULL == reg) {
        snmp_log(LOG_ERR,
                 "error creating handler registration for cpqHoSwVerTable\n");
        goto bail;
    }

    container =
        netsnmp_container_find("cpqHoSwVerTable:table_container");
    if (NULL == container) {
        snmp_log(LOG_ERR,
                 "error creating container for cpqHoSwVerTable\n");
        goto bail;
    }
    container->container_name = strdup("cpqHoSwVerTable container");

    table_info = SNMP_MALLOC_TYPEDEF(netsnmp_table_registration_info);
    if (NULL == table_info) {
        snmp_log(LOG_ERR,
                 "error allocating table registration for cpqHoSwVerTable\n");
        goto bail;
    }

    netsnmp_table_helper_add_indexes(table_info, ASN_INTEGER,   /* index: cpqHoSwVerIndex */
                                     0);

    table_info->min_column = COLUMN_CPQHOSWVERINDEX;
    table_info->max_column = COLUMN_CPQHOSWVERVERSIONBINARY;

    /*************************************************
     *
     * inject container_table helper
     */
    handler = netsnmp_container_table_handler_get(table_info, container,
                                                  TABLE_CONTAINER_KEY_NETSNMP_INDEX);
    if (NULL == handler) {
        snmp_log(LOG_ERR,
                 "error allocating table registration for cpqHoSwVerTable\n");
        goto bail;
    }
    if (SNMPERR_SUCCESS != netsnmp_inject_handler(reg, handler)) {
        snmp_log(LOG_ERR,
                 "error injecting container_table handler for cpqHoSwVerTable\n");
        goto bail;
    }
    handler = NULL;             /* reg has it, will reuse below */

    /*************************************************
     *
     * inject cache helper
     */
    cache = netsnmp_cache_create(240,    /* timeout in seconds */
                                 _cache_load, _cache_free,
                                 cpqHoSwVerTable_oid,
                                 cpqHoSwVerTable_oid_len);

    if (NULL == cache) {
        snmp_log(LOG_ERR, "error creating cache for cpqHoSwVerTable\n");
        goto bail;
    }
    cache->flags = NETSNMP_CACHE_PRELOAD |
    //               NETSNMP_CACHE_DONT_FREE_EXPIRED |
    //               NETSNMP_CACHE_DONT_AUTO_RELEASE |
                   NETSNMP_CACHE_DONT_INVALIDATE_ON_SET;
    cache->magic = container;

    handler = netsnmp_cache_handler_get(cache);
    if (NULL == handler) {
        snmp_log(LOG_ERR,
                 "error creating cache handler for cpqHoSwVerTable\n");
        goto bail;
    }

    if (SNMPERR_SUCCESS != netsnmp_inject_handler(reg, handler)) {
        snmp_log(LOG_ERR,
                 "error injecting cache handler for cpqHoSwVerTable\n");
        goto bail;
    }
    handler = NULL;             /* reg has it */

    /*
     * register the table
     */
    reg_tbl_ret = netsnmp_register_table(reg, table_info);
    if (reg_tbl_ret != SNMPERR_SUCCESS) {
        snmp_log(LOG_ERR,
                 "error registering table handler for cpqHoSwVerTable\n");
        goto bail;
    }

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


    /*
     * Initialise the contents of the table here 
     */
}

/*
 * create a new row in the table 
 */
cpqHoSwVerTable_entry *
cpqHoSwVerTable_createEntry(netsnmp_container * container,
                                oid cpqHoSwVerIndex)
{
    cpqHoSwVerTable_entry *entry;

    entry = SNMP_MALLOC_TYPEDEF(cpqHoSwVerTable_entry);
    if (!entry)
        return NULL;

    entry->cpqHoSwVerIndex =cpqHoSwVerIndex;
    entry->oid_index.len = 1;
    entry->oid_index.oids = (oid *) &entry->cpqHoSwVerIndex;
    return entry;

}

#ifdef NOT_NEEDED
/*
 * remove a row from the table 
 */
void
cpqHoSwVerTable_removeEntry(netsnmp_container * container,
				cpqHoSwVerTable_entry *entry)
{

    if (!entry)
        return;                 /* Nothing to remove */
    CONTAINER_REMOVE(container, entry);
    if (entry)
        SNMP_FREE(entry);       /* XXX - release any other internal resources */
}
#endif

/** free a row from the table */
void
cpqHoSwVerTable_freeEntry(cpqHoSwVerTable_entry * entry)
{

    if (!entry)
        return;                 /* Nothing to remove */
    SNMP_FREE(entry);           /* XXX - release any other internal resources */
}

/** handles requests for the cpqHoSwVerTable table */
int
cpqHoSwVerTable_handler(netsnmp_mib_handler *handler,
                            netsnmp_handler_registration *reginfo,
                            netsnmp_agent_request_info *reqinfo,
                            netsnmp_request_info *requests)
{

    netsnmp_request_info *request;
    netsnmp_table_request_info *table_info;
    cpqHoSwVerTable_entry *table_entry;

    DEBUGMSGTL(("cpqHoSwVerTable:handler", "Processing request (%d)\n",
                reqinfo->mode));

    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
    case MODE_GET:
        DEBUGMSGTL(("cpqHoSwVerTable:handler", "Processing Get request\n"));
        for (request = requests; request; request = request->next) {
            if (request->processed)
                continue;
            table_entry = (cpqHoSwVerTable_entry *)
                 netsnmp_container_table_extract_context(request);
            table_info = netsnmp_extract_table_info(request);
            if ((NULL == table_entry) || (NULL == table_info)) {
                DEBUGMSGTL(("cpqHoSwVerTable:handler", "table_entry=%p, table_info=%p\n", table_entry, table_info));

                snmp_log(LOG_ERR,
                         "could not extract table entry or info for cpqHoSwVerTable\n");
                snmp_set_var_typed_value(request->requestvb,
                                         SNMP_ERR_GENERR, NULL, 0);
                continue;
            }

            DEBUGMSGTL(("cpqHoSwVerTable:handler", "table_info->colnum=%d\n", table_info->colnum));
            switch (table_info->colnum) {
            case COLUMN_CPQHOSWVERINDEX:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqHoSwVerIndex);
                break;
            case COLUMN_CPQHOSWVERSTATUS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqHoSwVerStatus);
                break;
            case COLUMN_CPQHOSWVERTYPE:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqHoSwVerType);
                break;
            case COLUMN_CPQHOSWVERNAME:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                         table_entry->cpqHoSwVerName,
                                         table_entry-> cpqHoSwVerName_len);
                break;
            case COLUMN_CPQHOSWVERDESCRIPTION:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                         table_entry->cpqHoSwVerDescription,
                                         table_entry->
                                                   cpqHoSwVerDescription_len);
                break;
            case COLUMN_CPQHOSWVERDATE:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                         table_entry->cpqHoSwVerDate,
                                         table_entry->
                                         cpqHoSwVerDate_len);
                break;
            case COLUMN_CPQHOSWVERLOCATION:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                         table_entry->cpqHoSwVerLocation,
                                         table_entry->
                                                   cpqHoSwVerLocation_len);
                break;
            case COLUMN_CPQHOSWVERVERSION:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                         table_entry->
                                         cpqHoSwVerVersion,
                                         table_entry->
                                         cpqHoSwVerVersion_len);
                break;
            case COLUMN_CPQHOSWVERVERSIONBINARY:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                         table_entry->
                                         cpqHoSwVerVersionBinary,
                                         table_entry->
                                         cpqHoSwVerVersionBinary_len);
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

    DEBUGMSGTL(("internal:cpqHoSwVerTable:_cache_load", "called\n"));

    if ((NULL == cache) || (NULL == cache->magic)) {
        snmp_log(LOG_ERR,
                 "invalid cache for cpqHoSwVerTable_cache_load\n");
        return -1;
    }
    container = (netsnmp_container *) cache->magic;

    /** should only be called for an invalid or expired cache */
    netsnmp_assert((0 == cache->valid) || (1 == cache->expired));

    /*
     * load cache here (or call function to do it)
     */
    cpqhost_arch_cpqHoSwVer_container_load(container);
    return 0;
}                               /* _cache_load */

/**
 * @Internal
 */
/** remove a row from the table */
static void
cpqHoSwVerTable_freeEntry_cb(cpqHoSwVerTable_entry * entry,
                                void *magic)
{

    cpqHoSwVerTable_freeEntry(entry);
}

/**
 * @internal
 */
static void
_cache_free(netsnmp_cache * cache, void *magic)
{
    netsnmp_container *container;

    DEBUGMSGTL(("internal:cpqHoSwVerTable:_cache_free", "called\n"));

    if ((NULL == cache) || (NULL == cache->magic)) {
        snmp_log(LOG_ERR,
                 "invalid cache in cpqHoSwVerTable_cache_free\n");
        return;
    }
    container = (netsnmp_container *) cache->magic;

    /*
     * empty (but don't free) cache here
     */
    CONTAINER_CLEAR(container,
                    (netsnmp_container_obj_func *)
                    cpqHoSwVerTable_freeEntry_cb, NULL);
}                               /* _cache_free */

