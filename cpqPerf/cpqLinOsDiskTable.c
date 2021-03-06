/*
 * Note: this file originally auto-generated by mib2c using
 *  $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/table_container.h>
#include "cpqLinOsDiskTable.h"

static void     _cache_free(netsnmp_cache * cache, void *magic);
static int      _cache_load(netsnmp_cache * cache, void *vmagic);

const oid       cpqLinOsDiskTable_oid[] =
    { 1, 3, 6, 1, 4, 1, 232, 23, 2, 7, 2 };
const size_t    cpqLinOsDiskTable_oid_len =
    OID_LENGTH(cpqLinOsDiskTable_oid);

/** Initializes the cpqLinOsDiskTable module */
void
init_cpqLinOsDiskTable(void)
{
    /*
     * here we initialize all the tables we're planning on supporting 
     */
    initialize_table_cpqLinOsDiskTable();
}

/** Initialize the cpqLinOsDiskTable table by defining its contents and how it's structured */
void
initialize_table_cpqLinOsDiskTable(void)
{
    netsnmp_handler_registration *reg = NULL;
    netsnmp_mib_handler *handler = NULL;
    netsnmp_cache  *cache = NULL;
    netsnmp_container *container = NULL;
    netsnmp_table_registration_info *table_info = NULL;

    DEBUGMSGTL(("cpqLinOsDiskTable:init",
                "initializing table cpqLinOsDiskTable\n"));

    reg =
        netsnmp_create_handler_registration("cpqLinOsDiskTable",
                                            cpqLinOsDiskTable_handler,
                                            cpqLinOsDiskTable_oid,
                                            cpqLinOsDiskTable_oid_len,
                                            HANDLER_CAN_RONLY);
    if (NULL == reg) {
        snmp_log(LOG_ERR,
                 "error creating handler registration for cpqLinOsDiskTable\n");
        goto bail;
    }

    container =
        netsnmp_container_find("cpqLinOsDiskTable:table_container");
    if (NULL == container) {
        snmp_log(LOG_ERR,
                 "error creating container for cpqLinOsDiskTable\n");
        goto bail;
    }
    container->container_name = strdup("cpqLinOsDiskTable container");

    table_info = SNMP_MALLOC_TYPEDEF(netsnmp_table_registration_info);
    if (NULL == table_info) {
        snmp_log(LOG_ERR,
                 "error allocating table registration for cpqLinOsDiskTable\n");
        goto bail;
    }
    netsnmp_table_helper_add_indexes(table_info, 
                                     ASN_INTEGER,   /* index: cpqLinOsDiskMajorIndex */
                                     ASN_INTEGER,       /* index: cpqLinOsDiskMinorIndex */
                                     0);

    table_info->min_column = COLUMN_CPQLINOSDISKMAJORINDEX;
    table_info->max_column = COLUMN_CPQLINOSDISKREADDURATIONMSPERIOS;

    /*************************************************
     *
     * inject container_table helper
     */
    handler = netsnmp_container_table_handler_get(table_info, container,
                                                  TABLE_CONTAINER_KEY_NETSNMP_INDEX);
    if (NULL == handler) {
        snmp_log(LOG_ERR,
                 "error allocating table registration for cpqLinOsDiskTable\n");
        goto bail;
    }
    if (SNMPERR_SUCCESS != netsnmp_inject_handler(reg, handler)) {
        snmp_log(LOG_ERR,
                 "error injecting container_table handler for cpqLinOsDiskTable\n");
        goto bail;
    }
    handler = NULL;             /* reg has it, will reuse below */

    /*************************************************
     *
     * inject cache helper
     */
    cache = netsnmp_cache_create(5,    /* timeout in seconds */
                                 _cache_load, _cache_free,
                                 cpqLinOsDiskTable_oid,
                                 cpqLinOsDiskTable_oid_len);

    if (NULL == cache) {
        snmp_log(LOG_ERR, "error creating cache for cpqLinOsDiskTable\n");
        goto bail;
    }
    cache->flags = NETSNMP_CACHE_PRELOAD |
                   NETSNMP_CACHE_DONT_FREE_EXPIRED |
                   NETSNMP_CACHE_DONT_AUTO_RELEASE |
                   NETSNMP_CACHE_DONT_FREE_BEFORE_LOAD |
                   NETSNMP_CACHE_DONT_INVALIDATE_ON_SET;

    cache->magic = container;

    handler = netsnmp_cache_handler_get(cache);
    if (NULL == handler) {
        snmp_log(LOG_ERR,
                 "error creating cache handler for cpqLinOsDiskTable\n");
        goto bail;
    }

    if (SNMPERR_SUCCESS != netsnmp_inject_handler(reg, handler)) {
        snmp_log(LOG_ERR,
                 "error injecting cache handler for cpqLinOsDiskTable\n");
        goto bail;
    }
    handler = NULL;             /* reg has it */

    /*
     * register the table
     */
    if (SNMPERR_SUCCESS != netsnmp_register_table(reg, table_info)) {
        snmp_log(LOG_ERR,
                 "error registering table handler for cpqLinOsDiskTable\n");
        reg = NULL;             /* it was freed inside netsnmp_register_table */
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

    if (reg)
        netsnmp_handler_registration_free(reg);
}

/** create a new row in the table */
cpqLinOsDiskTable_entry *
cpqLinOsDiskTable_createEntry(netsnmp_container * container,
                              oid cpqLinOsDiskMajorIndex,
                              oid cpqLinOsDiskMinorIndex)
{
    cpqLinOsDiskTable_entry *entry;

    entry = SNMP_MALLOC_TYPEDEF(cpqLinOsDiskTable_entry);
    if (!entry)
        return NULL;

    entry->cpqLinOsDiskMajorIndex = cpqLinOsDiskMajorIndex;
    entry->cpqLinOsDiskMinorIndex = cpqLinOsDiskMinorIndex;
    entry->oid_index.len = 2;
    entry->oid_index.oids = (oid *) &entry->cpqLinOsDiskMajorIndex;
    return entry;
}

/** remove a row from the table */
void
cpqLinOsDiskTable_removeEntry(netsnmp_container * container,
                              cpqLinOsDiskTable_entry * entry)
{

    if (!entry)
        return;                 /* Nothing to remove */
    CONTAINER_REMOVE(container, entry);
    if (entry)
        SNMP_FREE(entry);       /* XXX - release any other internal resources */
}

/** remove a row from the table */
void
cpqLinOsDiskTable_freeEntry(cpqLinOsDiskTable_entry * entry)
{

    if (!entry)
        return;                 /* Nothing to remove */
    SNMP_FREE(entry);           /* XXX - release any other internal resources */
}

/** handles requests for the cpqLinOsDiskTable table */
int
cpqLinOsDiskTable_handler(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info *reqinfo,
                          netsnmp_request_info *requests)
{

    netsnmp_request_info *request;
    netsnmp_table_request_info *table_info;
    netsnmp_container *container;
    cpqLinOsDiskTable_entry *table_entry;

    DEBUGMSGTL(("cpqLinOsDiskTable:handler", "Processing request (%d)\n",
                reqinfo->mode));

    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
    case MODE_GET:
        for (request = requests; request; request = request->next) {
            if (request->processed)
                continue;
            table_entry = (cpqLinOsDiskTable_entry *)
                netsnmp_container_table_extract_context(request);
            table_info = netsnmp_extract_table_info(request);
            if ((NULL == table_entry) || (NULL == table_info)) {
                snmp_log(LOG_ERR,
                         "could not extract table entry or info for cpqLinOsDiskTable\n");
                snmp_set_var_typed_value(request->requestvb,
                                         SNMP_ERR_GENERR, NULL, 0);
                continue;
            }

            switch (table_info->colnum) {
            case COLUMN_CPQLINOSDISKMAJORINDEX:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqLinOsDiskMajorIndex);
                break;
            case COLUMN_CPQLINOSDISKMINORINDEX:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqLinOsDiskMinorIndex);
                break;
            case COLUMN_CPQLINOSDISKNAME:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                         table_entry->cpqLinOsDiskName,
                                         table_entry->
                                         cpqLinOsDiskName_len);
                break;
            case COLUMN_CPQLINOSDISKSCSIINDEX:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqLinOsDiskScsiIndex);
                break;
            case COLUMN_CPQLINOSDISKWRITEIOS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqLinOsDiskWriteIos);
                break;
            case COLUMN_CPQLINOSDISKWRITEMERGES:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqLinOsDiskWriteMerges);
                break;
            case COLUMN_CPQLINOSDISKWRITESECTORS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqLinOsDiskWriteSectors);
                break;
            case COLUMN_CPQLINOSDISKWRITEDURATIONMS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqLinOsDiskWriteDurationMs);
                break;
            case COLUMN_CPQLINOSDISKWRITEIOSPERSEC:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqLinOsDiskWriteIosPerSec);
                break;
            case COLUMN_CPQLINOSDISKWRITESECTORSPERSEC:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqLinOsDiskWriteSectorsPerSec);
                break;
            case COLUMN_CPQLINOSDISKWRITEDURATIONMSPERIOS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqLinOsDiskWriteDurationMsPerIos);
                break;
            case COLUMN_CPQLINOSDISKREADIOS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqLinOsDiskReadIos);
                break;
            case COLUMN_CPQLINOSDISKREADMERGES:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqLinOsDiskReadMerges);
                break;
            case COLUMN_CPQLINOSDISKREADSECTORS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqLinOsDiskReadSectors);
                break;
            case COLUMN_CPQLINOSDISKREADDURATIONMS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqLinOsDiskReadDurationMs);
                break;
            case COLUMN_CPQLINOSDISKREADIOSPERSEC:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqLinOsDiskReadIosPerSec);
                break;
            case COLUMN_CPQLINOSDISKREADSECTORSPERSEC:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqLinOsDiskReadSectorsPerSec);
                break;
            case COLUMN_CPQLINOSDISKREADDURATIONMSPERIOS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqLinOsDiskReadDurationMsPerIos);
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

    DEBUGMSGTL(("internal:cpqLinOsDiskTable:_cache_load", "called\n"));

    if ((NULL == cache) || (NULL == cache->magic)) {
        snmp_log(LOG_ERR,
                 "invalid cache for cpqLinOsDiskTable_cache_load\n");
        return -1;
    }
    container = (netsnmp_container *) cache->magic;

    /** should only be called for an invalid or expired cache */
    netsnmp_assert((0 == cache->valid) || (1 == cache->expired));

    /*
     * load cache here (or call function to do it)
     */
    netsnmp_arch_linosdiskio_container_load(container);

    return 0;
}                               /* _cache_load */

/**
 * @Internal
 */
/** remove a row from the table */
static void
cpqLinOsDiskTable_freeEntry_cb(cpqLinOsDiskTable_entry * entry,
                               void *magic)
{
    cpqLinOsDiskTable_freeEntry(entry);
}

/**
 * @internal
 */
static void
_cache_free(netsnmp_cache * cache, void *magic)
{
    netsnmp_container *container;

    DEBUGMSGTL(("internal:cpqLinOsDiskTable:_cache_free", "called\n"));

    if ((NULL == cache) || (NULL == cache->magic)) {
        snmp_log(LOG_ERR,
                 "invalid cache in cpqLinOsDiskTable_cache_free\n");
        return;
    }

}                               /* _cache_free */
