#include "sseq.h"

/* define a structure for holding the block local state. By assigning an
 * instance of this struct to the block private_data pointer (see init), this
 * information becomes accessible within the hook functions.
 */
struct sseq_info
{
  /* custom block local data here */
  struct seq_config *seq;
  unsigned int seq_len;
  unsigned int curr_blx;
  /* this is to have fast access to ports for reading and writing, without
   * needing a hash table lookup */
  struct sseq_port_cache ports;
};

/* init */
int sseq_init(ubx_block_t *b)
{
        int ret = -1;
        struct sseq_info *inf;

        /* allocate memory for the block local state */
        if ((inf = (struct sseq_info*)calloc(1, sizeof(struct sseq_info)))==NULL) {
                ERR("sseq: failed to alloc memory");
                ret=EOUTOFMEM;
                goto out;
        }
        b->private_data=inf;
        update_port_cache(b, &inf->ports);
        ret=0;
out:
        return ret;
}

/* start */
int sseq_start(ubx_block_t *b)
{
  struct sseq_info *inf;
  inf = (struct sseq_info*) b->private_data;
  ubx_data_t* seq_blx_list;
  
  seq_blx_list = ubx_config_get_data(b, "seq_blocks");
  
  inf->seq = seq_blx_list->data;
  inf->seq_len = seq_blx_list->len;
  
  inf->curr_blx = 0;
  
  return 0;
}

/* stop */
void sseq_stop(ubx_block_t *b)
{
        /* struct sseq_info *inf = (struct sseq_info*) b->private_data; */
}

/* cleanup */
void sseq_cleanup(ubx_block_t *b)
{
  free(b->private_data);
}

/* step */
void sseq_step(ubx_block_t *b)
{
  struct sseq_info *inf;
  inf = (struct sseq_info*) b->private_data;
  int steps,i=-1;
  
  
  /* Implementation all cycle */
  /*
  for(i=0; i<inf->seq_len; i++) 
  {
    //printf("step %d",i);
    for(steps=0; steps<inf->seq[i].num_steps; steps++)
    {
      ubx_cblock_step(inf->seq[i].b);
//       if(ubx_cblock_step(inf->seq[i].b)!=0)
        //goto out;
//         sseq_stop(b);
    }
  }
  */
  
  /* Implementation single step */
  ubx_cblock_step(inf->seq[inf->curr_blx].b);
  inf->curr_blx++;
  if(! (inf->curr_blx < inf->seq_len ) )
    inf->curr_blx = 0;

}

