/*
 * sseq microblx function block (autogenerated, don't edit)
 */

#include <ubx.h>

/* includes types and type metadata */
#include "../types/seq_config.h"
#include "../types/seq_config.h.hexarr"
// #include "../types/ptrig_period.h"
// #include "../types/ptrig_period.h.hexarr"
// #include "../types/ptrig_tstat.h"
// #include "../types/ptrig_tstat.h.hexarr"


/* block meta information */
char sseq_meta[] =
        " { doc='',"
        "   real-time=true,"
        "}";

/* declaration of block configuration */
ubx_config_t sseq_config[] = {
//         { .name="period", .type_name = "struct ptrig_period", .doc="" },
//         { .name="stacksize", .type_name = "size_t", .doc="" },
//         { .name="sched_priority", .type_name = "int", .doc="" },
//         { .name="sched_policy", .type_name = "char", .doc="" },
//         { .name="thread_name", .type_name = "char", .doc="" },
        { .name="seq_blocks", .type_name = "struct seq_config", .doc="" },
//         { .name="time_stats_enabled", .type_name = "int", .doc="" },
        { NULL },
};

/* declaration port block ports */
ubx_port_t sseq_ports[] = {
//         { .name="tstats", .out_type_name="struct ptrig_tstat", .out_data_len=1, .doc=""  },
        { NULL },
};

/* declare a struct port_cache */
struct sseq_port_cache {
        ubx_port_t* tstats;
};

/* declare a helper function to update the port cache this is necessary
 * because the port ptrs can change if ports are dynamically added or
 * removed. This function should hence be called after all
 * initialization is done, i.e. typically in 'start'
 */
static void update_port_cache(ubx_block_t *b, struct sseq_port_cache *pc)
{
        pc->tstats = ubx_port_get(b, "tstats");
}


/* for each port type, declare convenience functions to read/write from ports */
// def_write_fun(write_tstats, struct ptrig_tstat)

/* block operation forward declarations */
int sseq_init(ubx_block_t *b);
int sseq_start(ubx_block_t *b);
void sseq_stop(ubx_block_t *b);
void sseq_cleanup(ubx_block_t *b);
void sseq_step(ubx_block_t *b);


/* put everything together */
ubx_block_t sseq_block = {
        .name = "sseq",
        .type = BLOCK_TYPE_COMPUTATION,
        .meta_data = sseq_meta,
        .configs = sseq_config,
        .ports = sseq_ports,

        /* ops */
        .init = sseq_init,
        .start = sseq_start,
        .stop = sseq_stop,
        .cleanup = sseq_cleanup,
        .step = sseq_step,
};


/* sseq module init and cleanup functions */
int sseq_mod_init(ubx_node_info_t* ni)
{
        DBG(" ");
        int ret = -1;

        if(ubx_block_register(ni, &sseq_block) != 0)
                goto out;

        ret=0;
out:
        return ret;
}

void sseq_mod_cleanup(ubx_node_info_t *ni)
{
        DBG(" ");
        ubx_block_unregister(ni, "sseq");
}

