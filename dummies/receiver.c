#include "receiver.h"

/* define a structure for holding the block local state. By assigning an
 * instance of this struct to the block private_data pointer (see init), this
 * information becomes accessible within the hook functions.
 */
struct receiver_info
{
  /* this is to have fast access to ports for reading and writing, without
   * needing a hash table lookup */
  struct receiver_port_cache ports;
};

/* init */
int receiver_init(ubx_block_t *b)
{
        int ret = -1;
        struct receiver_info *inf;

        /* allocate memory for the block local state */
        if ((inf = (struct receiver_info*)calloc(1, sizeof(struct receiver_info)))==NULL) {
                ERR("receiver: failed to alloc memory");
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
int receiver_start(ubx_block_t *b)
{
  
  return 0;
}

/* stop */
void receiver_stop(ubx_block_t *b)
{
        /* struct receiver_info *inf = (struct receiver_info*) b->private_data; */
}

/* cleanup */
void receiver_cleanup(ubx_block_t *b)
{
  free(b->private_data);
}

/* step */
void receiver_step(ubx_block_t *b)
{
  printf("receiver: %s\n",b->name);

}

