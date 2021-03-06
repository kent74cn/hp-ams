/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.container.conf 17798 2009-10-27 06:44:54Z magfr $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/table_container.h>
#include "cpqNicIfLogMapTable.h"
#include "cpqNicIfPhysAdapterTable.h"
#include <linux/rtnetlink.h>
#define SUPPORT_PREFIX_FLAGS 1

extern void     netsnmp_arch_ifphys_container_load(netsnmp_container *);
static void     _cache_free(netsnmp_cache * cache, void *magic);
static int      _cache_load(netsnmp_cache * cache, void *vmagic);

static int cpqnic_iflink_listen(void);

const oid       cpqNicIfPhysAdapterTable_oid[] =
        { 1, 3, 6, 1, 4, 1, 232, 18, 2, 3, 1 };
const size_t    cpqNicIfPhysAdapterTable_oid_len =
        OID_LENGTH(cpqNicIfPhysAdapterTable_oid);

void cpqNicIfPhysAdapterTable_cache_reload();
void cpqNicIfPhysAdapterTable_cache_remove();
extern void cpqNicIfLogMapTable_cache_reload();
extern void cpqNicIfLogMapTable_cache_remove();
/* taken from netlink patch for if-mib */
static int cpqnic_netlink_listen(unsigned subscriptions)
{
    struct sockaddr_nl localaddrinfo;
    int fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    if (fd < 0) {
       snmp_log(LOG_ERR, "cpqnic_netlink_listen: Cannot create socket.\n");
        return -1;
    }

    memset(&localaddrinfo, 0, sizeof(struct sockaddr_nl));

    localaddrinfo.nl_family = AF_NETLINK;
    localaddrinfo.nl_groups = subscriptions;

    if (bind(fd, (struct sockaddr*)&localaddrinfo, sizeof(localaddrinfo)) < 0) {
        snmp_log(LOG_ERR,"cpqnic_netlink_listen: Bind failed.\n");
        close(fd);
        return -1;
    }

    return fd;
}

static void cpqnic_iflink_process(int fd, void *data) {
    int                status;
    char               buf[16384];
    struct nlmsghdr    *nlmp;
    struct ifinfomsg   *ifi;
    int                len, req_len, length;

    status = recv(fd, buf, sizeof(buf), 0);
    if (status < 0) {
        snmp_log(LOG_ERR,"cpqnic_iflink_listen: Receive failed.\n");
        return;
    }

    if (status == 0){
        DEBUGMSGTL(("access:interface:iflink", "End of File\n"));
        return;
    }

    for (nlmp = (struct nlmsghdr *)buf;
        status > sizeof(*nlmp);
        status -= NLMSG_ALIGN(len),
            nlmp = (struct nlmsghdr*)((char*)nlmp + NLMSG_ALIGN(len))) {
       len = nlmp->nlmsg_len;
        req_len = len - sizeof(*nlmp);

        if (req_len < 0 || len > status) {
            snmp_log(LOG_ERR,"cpqnic_iflink_listen: Error in length\n");
            return;
        }

        if (!NLMSG_OK(nlmp, status)) {
            DEBUGMSGTL(("access:interface:iflink", "NLMSG not OK\n"));
            return;
        }

        DEBUGMSGTL(("access:interface:iflink", "NLMSG type = %d\n", nlmp->nlmsg_type));

        if (nlmp->nlmsg_type == RTM_NEWLINK)  {
            ifi = NLMSG_DATA(nlmp);
            length = nlmp->nlmsg_len - NLMSG_LENGTH(sizeof(*ifi));

            if (length < 0) {
                DEBUGMSGTL(("access:interface:iflink", "wrong nlmsg length %d\n", length));
                return;
            }

            DEBUGMSGTL(("access:interface:iflink", "NLMSG index = %d\n", ifi->ifi_index));
            DEBUGMSGTL(("access:interface:iflink", "NLMSG flags = %x\n", ifi->ifi_flags));
            DEBUGMSGTL(("access:interface:iflink", "NLMSG mask = %x\n", ifi->ifi_change));
           /* Just request a refresh! */
           cpqNicIfLogMapTable_cache_reload(ifi->ifi_index);
           cpqNicIfPhysAdapterTable_cache_reload(ifi->ifi_index);
        } else if (nlmp->nlmsg_type == RTM_DELLINK) {
            ifi = NLMSG_DATA(nlmp);
            length = nlmp->nlmsg_len - NLMSG_LENGTH(sizeof(*ifi));

            if (length < 0) {
                DEBUGMSGTL(("access:interface:iflink", "wrong nlmsg length %d\n", length));
                return;
            }
            DEBUGMSGTL(("access:interface:iflink", "NLMSG index = %d\n", ifi->ifi_index));
            DEBUGMSGTL(("access:interface:iflink", "NLMSG flags = %x\n", ifi->ifi_flags));
            DEBUGMSGTL(("access:interface:iflink", "NLMSG mask = %x\n", ifi->ifi_change));

           cpqNicIfLogMapTable_cache_remove(ifi->ifi_index);
           cpqNicIfPhysAdapterTable_cache_remove(ifi->ifi_index);

        }
    }
}

static int cpqnic_iflink_listen()
{
    struct {
       struct nlmsghdr nlh;
       struct rtgenmsg g;
    } req;
    int status;
    int fd = cpqnic_netlink_listen(RTNLGRP_LINK);
    if (fd < 0) return -1;

    memset(&req, 0, sizeof(req));
    req.nlh.nlmsg_len = sizeof(req);
    req.nlh.nlmsg_type = RTM_GETLINK;
    req.nlh.nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST;
    req.g.rtgen_family = AF_UNSPEC;

    status = send(fd, (void*)&req, sizeof(req), 0);
    if (status < 0) {
        snmp_log(LOG_ERR,"cpqnic_iflink_listen: send failed\n");
        close(fd);
        return -1;
    }

    if (register_readfd(fd, cpqnic_iflink_process, NULL) != 0) {
        snmp_log(LOG_ERR,"cpqnic_iflink_listen: error registering netlink socket\n");
        close(fd);
        return -1;
    }
    return 0;

}

/** Initializes the cpqNicIfPhysAdapterTable module */
void
init_cpqNicIfPhysAdapterTable(void)
{
    /*
     * here we initialize all the tables we're planning on supporting 
     */
    initialize_table_cpqNicIfPhysAdapterTable();
}

/** Initialize the cpqNicIfPhysAdapterTable table by defining its contents and how it's structured */
void
initialize_table_cpqNicIfPhysAdapterTable(void)
{
    netsnmp_handler_registration *reg = NULL;
    netsnmp_mib_handler *handler = NULL;
    netsnmp_container *container = NULL;
    netsnmp_table_registration_info *table_info = NULL;
    netsnmp_cache  *cache = NULL;

    int reg_tbl_ret = SNMPERR_SUCCESS;

    DEBUGMSGTL(("cpqNicIfPhysAdapterTable:init",
                "initializing table cpqNicIfPhysAdapterTable\n"));

    reg =
        netsnmp_create_handler_registration("cpqNicIfPhysAdapterTable",
                                            cpqNicIfPhysAdapterTable_handler,
                                            cpqNicIfPhysAdapterTable_oid,
                                            cpqNicIfPhysAdapterTable_oid_len,
                                            HANDLER_CAN_RONLY);
    if (NULL == reg) {
        snmp_log(LOG_ERR,
                 "error creating handler registration for cpqNicIfPhysAdapterTable\n");
        goto bail;
    }

    container =
        netsnmp_container_find("cpqNicIfPhysAdapterTable:table_container");
    if (NULL == container) {
        snmp_log(LOG_ERR,
                 "error creating container for cpqNicIfPhysAdapterTable\n");
        goto bail;
    }
    container->container_name = strdup("cpqNicIfPhysAdapterTable container");


    table_info = SNMP_MALLOC_TYPEDEF(netsnmp_table_registration_info);
    if (NULL == table_info) {
        snmp_log(LOG_ERR,
                 "error allocating table registration for cpqNicIfPhysAdapterTable\n");
        goto bail;
    }
    netsnmp_table_helper_add_indexes(table_info, ASN_INTEGER,   /* index: cpqNicIfPhysAdapterIndex */
                                     0);
    table_info->min_column = COLUMN_CPQNICIFPHYSADAPTERINDEX;
    table_info->max_column = COLUMN_CPQNICIFPHYSADAPTERVIRTUALPORTNUMBER;

    /*************************************************
     *
     * inject container_table helper
     */
    handler = netsnmp_container_table_handler_get(table_info, container,
                                                  TABLE_CONTAINER_KEY_NETSNMP_INDEX);
    if (NULL == handler) {
        snmp_log(LOG_ERR,
                 "error allocating table registration for cpqNicIfPhysAdapterTable\n");
        goto bail;
    }
    if (SNMPERR_SUCCESS != netsnmp_inject_handler(reg, handler)) {
        snmp_log(LOG_ERR,
                 "error injecting container_table handler for cpqNicIfPhysAdapterTable\n");
        goto bail;
    }
    handler = NULL;             /* reg has it, will reuse below */

    /*************************************************
     *
     * inject cache helper
     */
    cache = netsnmp_cache_create(30,    /* timeout in seconds */
                                 _cache_load, _cache_free,
                                 cpqNicIfPhysAdapterTable_oid,
                                 cpqNicIfPhysAdapterTable_oid_len);

    if (NULL == cache) {
        snmp_log(LOG_ERR,
                 "error creating cache for cpqNicIfPhysAdapterTable\n");
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
                 "error creating cache handler for cpqNicIfPhysAdapterTable\n");
        goto bail;
    }

    if (SNMPERR_SUCCESS != netsnmp_inject_handler(reg, handler)) {
        snmp_log(LOG_ERR,
                 "error injecting cache handler for cpqNicIfPhysAdapterTable\n");
        goto bail;
    }
    handler = NULL;             /* reg has it */

    /*
     * register the table
     */
    reg_tbl_ret = netsnmp_register_table(reg, table_info);
    if (reg_tbl_ret != SNMPERR_SUCCESS) {
        snmp_log(LOG_ERR,
                 "error registering table handler for cpqNicIfPhysAdapterTable\n");
        goto bail;
    }

    cpqnic_iflink_listen();

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
cpqNicIfPhysAdapterTable_entry *
cpqNicIfPhysAdapterTable_createEntry(netsnmp_container * container,
                                     oid cpqNicIfPhysAdapterIndex)
{
    cpqNicIfPhysAdapterTable_entry *entry;

    entry = SNMP_MALLOC_TYPEDEF(cpqNicIfPhysAdapterTable_entry);
    if (!entry)
        return NULL;

    entry->cpqNicIfPhysAdapterIndex = cpqNicIfPhysAdapterIndex;
    entry->oid_index.len = 1;
    entry->oid_index.oids = (oid *) &entry->cpqNicIfPhysAdapterIndex;

    return entry;
}

/** remove a row from the table */
void
cpqNicIfPhysAdapterTable_removeEntry(netsnmp_container * container,
                                     cpqNicIfPhysAdapterTable_entry *
                                     entry)
{

    if (!entry)
        return;                 /* Nothing to remove */
    CONTAINER_REMOVE(container, entry);
    if (entry)
        SNMP_FREE(entry);       /* XXX - release any other internal resources */
}

/** remove a row from the table */
void
cpqNicIfPhysAdapterTable_freeEntry(cpqNicIfPhysAdapterTable_entry * entry)
{

    if (!entry)
        return;                 /* Nothing to remove */
    SNMP_FREE(entry);           /* XXX - release any other internal resources */
}

/** handles requests for the cpqNicIfPhysAdapterTable table */
int
cpqNicIfPhysAdapterTable_handler(netsnmp_mib_handler *handler,
                                 netsnmp_handler_registration *reginfo,
                                 netsnmp_agent_request_info *reqinfo,
                                 netsnmp_request_info *requests)
{

    netsnmp_request_info *request;
    netsnmp_table_request_info *table_info;
    cpqNicIfPhysAdapterTable_entry *table_entry;

    DEBUGMSGTL(("cpqNicIfPhysAdapterTable:handler",
                "Processing request (%d)\n", reqinfo->mode));

    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
    case MODE_GET:
        for (request = requests; request; request = request->next) {
            if (request->processed)
                continue;
            table_entry = (cpqNicIfPhysAdapterTable_entry *)
                netsnmp_container_table_extract_context(request);
            table_info = netsnmp_extract_table_info(request);
            if ((NULL == table_entry) || (NULL == table_info)) {
                snmp_log(LOG_ERR,
                         "could not extract table entry or info for cpqNicIfPhysAdapterTable\n");
                snmp_set_var_typed_value(request->requestvb,
                                         SNMP_ERR_GENERR, NULL, 0);
                continue;
            }

            switch (table_info->colnum) {
            case COLUMN_CPQNICIFPHYSADAPTERINDEX:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqNicIfPhysAdapterIndex);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERIFNUMBER:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                         table_entry->
                                         cpqNicIfPhysAdapterIfNumber,
                                         table_entry->
                                         cpqNicIfPhysAdapterIfNumber_len);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERROLE:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqNicIfPhysAdapterRole);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERMACADDRESS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                         table_entry->
                                         cpqNicIfPhysAdapterMACAddress,
                                         table_entry->
                                         cpqNicIfPhysAdapterMACAddress_len);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERSLOT:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqNicIfPhysAdapterSlot);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERIOADDR:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqNicIfPhysAdapterIoAddr);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERIRQ:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqNicIfPhysAdapterIrq);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERDMA:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqNicIfPhysAdapterDma);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERMEMADDR:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqNicIfPhysAdapterMemAddr);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERPORT:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqNicIfPhysAdapterPort);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERDUPLEXSTATE:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqNicIfPhysAdapterDuplexState);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERCONDITION:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqNicIfPhysAdapterCondition);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERSTATE:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqNicIfPhysAdapterState);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERSTATUS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqNicIfPhysAdapterStatus);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERSTATSVALID:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqNicIfPhysAdapterStatsValid);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERGOODTRANSMITS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_COUNTER,
                                           table_entry->
                                           cpqNicIfPhysAdapterGoodTransmits);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERGOODRECEIVES:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_COUNTER,
                                           table_entry->
                                           cpqNicIfPhysAdapterGoodReceives);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERBADTRANSMITS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_COUNTER,
                                           table_entry->
                                           cpqNicIfPhysAdapterBadTransmits);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERBADRECEIVES:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_COUNTER,
                                           table_entry->
                                           cpqNicIfPhysAdapterBadReceives);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERALIGNMENTERRORS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_COUNTER,
                                           table_entry->
                                           cpqNicIfPhysAdapterAlignmentErrors);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERFCSERRORS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_COUNTER,
                                           table_entry->
                                           cpqNicIfPhysAdapterFCSErrors);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERSINGLECOLLISIONFRAMES:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_COUNTER,
                                           table_entry->
                                           cpqNicIfPhysAdapterSingleCollisionFrames);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERMULTIPLECOLLISIONFRAMES:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_COUNTER,
                                           table_entry->
                                           cpqNicIfPhysAdapterMultipleCollisionFrames);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERDEFERREDTRANSMISSIONS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_COUNTER,
                                           table_entry->
                                           cpqNicIfPhysAdapterDeferredTransmissions);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERLATECOLLISIONS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_COUNTER,
                                           table_entry->
                                           cpqNicIfPhysAdapterLateCollisions);
                break;
            case COLUMN_CPQNICIFPHYSADAPTEREXCESSIVECOLLISIONS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_COUNTER,
                                           table_entry->
                                           cpqNicIfPhysAdapterExcessiveCollisions);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERINTERNALMACTRANSMITERRORS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_COUNTER,
                                           table_entry->
                                           cpqNicIfPhysAdapterInternalMacTransmitErrors);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERCARRIERSENSEERRORS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_COUNTER,
                                           table_entry->
                                           cpqNicIfPhysAdapterCarrierSenseErrors);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERFRAMETOOLONGS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_COUNTER,
                                           table_entry->
                                           cpqNicIfPhysAdapterFrameTooLongs);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERINTERNALMACRECEIVEERRORS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_COUNTER,
                                           table_entry->
                                           cpqNicIfPhysAdapterInternalMacReceiveErrors);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERHWLOCATION:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                         table_entry->
                                         cpqNicIfPhysAdapterHwLocation,
                                         table_entry->
                                         cpqNicIfPhysAdapterHwLocation_len);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERPARTNUMBER:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                         table_entry->
                                         cpqNicIfPhysAdapterPartNumber,
                                         table_entry->
                                         cpqNicIfPhysAdapterPartNumber_len);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERSPEED:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_GAUGE,
                                           table_entry->
                                           cpqNicIfPhysAdapterSpeed);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERCONFSPEEDDUPLEX:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqNicIfPhysAdapterConfSpeedDuplex);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERAGGREGATIONGID:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqNicIfPhysAdapterAggregationGID);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERSPEEDMBPS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_GAUGE,
                                           table_entry->
                                           cpqNicIfPhysAdapterSpeedMbps);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERINOCTETS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_COUNTER,
                                           table_entry->
                                           cpqNicIfPhysAdapterInOctets);
                break;
            case COLUMN_CPQNICIFPHYSADAPTEROUTOCTETS:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_COUNTER,
                                           table_entry->
                                           cpqNicIfPhysAdapterOutOctets);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERNAME:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                         table_entry->
                                         cpqNicIfPhysAdapterName,
                                         table_entry->
                                         cpqNicIfPhysAdapterName_len);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERIOBAYNO:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqNicIfPhysAdapterIoBayNo);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERFWVERSION:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                         table_entry->
                                         cpqNicIfPhysAdapterFWVersion,
                                         table_entry->
                                         cpqNicIfPhysAdapterFWVersion_len);
                break;
            case COLUMN_CPQNICIFPHYSADAPTERVIRTUALPORTNUMBER:
                if (!table_entry) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->
                                           cpqNicIfPhysAdapterVirtualPortNumber);
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

void
cpqNicIfPhysAdapterTable_cache_remove(oid index)
{
    netsnmp_cache  *cpqNicIfPhysAdapterTable_cache = NULL;
    netsnmp_container *ifphysdev_container;
    netsnmp_iterator  *it;
    cpqNicIfPhysAdapterTable_entry* entry = NULL;

    DEBUGMSGTL(("internal:cpqNicIfPhysAdapterTable:_cache_reload", "triggered\n"));
    cpqNicIfPhysAdapterTable_cache = netsnmp_cache_find_by_oid(cpqNicIfPhysAdapterTable_oid,
                                            cpqNicIfPhysAdapterTable_oid_len);

    if (NULL != cpqNicIfPhysAdapterTable_cache) {
        ifphysdev_container = cpqNicIfPhysAdapterTable_cache->magic;
        it = CONTAINER_ITERATOR(ifphysdev_container);

        entry = ITERATOR_FIRST( it );
        while (entry != NULL ) {
            if (entry->cpqNicIfPhysAdapterIndex == index)
                break;
            entry = ITERATOR_NEXT( it );
        }
        ITERATOR_RELEASE( it );
        if (entry != NULL)
            cpqNicIfPhysAdapterTable_removeEntry(ifphysdev_container, entry);
    }
}

void
cpqNicIfPhysAdapterTable_cache_reload(oid index)
{
    netsnmp_cache  *cpqNicIfPhysAdapterTable_cache = NULL;
    netsnmp_container *ifphysdev_container;
    netsnmp_iterator  *it;
    cpqNicIfPhysAdapterTable_entry* entry = NULL;

    DEBUGMSGTL(("internal:cpqNicIfPhysAdapterTable:_cache_reload", "triggered\n"));
    cpqNicIfPhysAdapterTable_cache = netsnmp_cache_find_by_oid(cpqNicIfPhysAdapterTable_oid,
                                            cpqNicIfPhysAdapterTable_oid_len);

    if (NULL != cpqNicIfPhysAdapterTable_cache) {
        ifphysdev_container = cpqNicIfPhysAdapterTable_cache->magic;
        it = CONTAINER_ITERATOR(ifphysdev_container);

        entry = ITERATOR_FIRST( it );
        while (entry != NULL ) {
            if (entry->cpqNicIfPhysAdapterIndex == index)
                break;
            entry = ITERATOR_NEXT( it );
        }
        ITERATOR_RELEASE( it );
        if (entry != NULL)
            cpqNicIfPhysAdapter_reload_entry(entry);
    }
}

/**
 * @internal
 */
static int
_cache_load(netsnmp_cache * cache, void *vmagic)
{
    netsnmp_container *container;

    DEBUGMSGTL(("internal:cpqNicIfPhysAdapterTable:_cache_load",
                "called\n"));

    if ((NULL == cache) || (NULL == cache->magic)) {
        snmp_log(LOG_ERR,
                 "invalid cache for cpqNicIfPhysAdapterTable_cache_load\n");
        return -1;
    }
    container = (netsnmp_container *) cache->magic;

    /** should only be called for an invalid or expired cache */
    netsnmp_assert((0 == cache->valid) || (1 == cache->expired));

    /*
     * load cache here (or call function to do it)
     */
    netsnmp_arch_ifphys_container_load(container);

    return 0;
}                               /* _cache_load */

/**
 * @Internal
 */
/** remove a row from the table */
static void
cpqNicIfPhysAdapterTable_freeEntry_cb(cpqNicIfPhysAdapterTable_entry *
                                      entry, void *magic)
{

    cpqNicIfPhysAdapterTable_freeEntry(entry);
}

/**
 * @internal
 */
static void
_cache_free(netsnmp_cache * cache, void *magic)
{
    netsnmp_container *container;

    DEBUGMSGTL(("internal:cpqNicIfPhysAdapterTable:_cache_free",
                "called\n"));

    if ((NULL == cache) || (NULL == cache->magic)) {
        snmp_log(LOG_ERR,
                 "invalid cache in cpqNicIfPhysAdapterTable_cache_free\n");
        return;
    }
    container = (netsnmp_container *) cache->magic;

    /*
     * empty (but don't free) cache here
     */
    CONTAINER_CLEAR(container,
                    (netsnmp_container_obj_func *)
                    cpqNicIfPhysAdapterTable_freeEntry_cb, NULL);
}                               /* _cache_free */
