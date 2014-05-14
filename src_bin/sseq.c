/* Standalone C-only system (no scripting layer).
 *   This source has been autogenerated
 *     from a usc system description.
 *    Autogenerated with generate_capp tool.
 */

#include <ubx.h>

/* Additional header files for types */
#include <types/seq_config.h>


#define WEBIF_PORT      "8888"
int main(int argc, char **argv)
{
  int len, ret=EXIT_FAILURE;
  ubx_node_info_t ni;
//   char* tmp_char;
  /* initalize the node */
  ubx_node_init(&ni, "sseq");
  ubx_block_t *webif;
    ubx_data_t *d;
  ubx_block_t *dummy_1_2;
  ubx_block_t *sseq_1_1;
  ubx_block_t *dummy_2_3;

  /* Load Modules */
  if(ubx_module_load(&ni, "/home/haianos/enea_microblx/microblx/std_types/stdtypes/stdtypes.so") != 0)
    goto out;
  
  if(ubx_module_load(&ni, "/home/haianos/enea_microblx/install/lib/ubx/types/ubx_std_sched_types.so") != 0)
    goto out;
  
  if(ubx_module_load(&ni, "/home/haianos/enea_microblx/microblx/std_blocks/lfds_buffers/lfds_cyclic.so") != 0)
    goto out;
  
  if(ubx_module_load(&ni, "/home/haianos/enea_microblx/microblx/std_blocks/logging/file_logger.so") != 0)
    goto out;
  
  if(ubx_module_load(&ni, "/home/haianos/enea_microblx/install/lib/ubx/blocks/schedmod.so") != 0)
    goto out;
  
  if(ubx_module_load(&ni, "/home/haianos/enea_microblx/install/lib/ubx/blocks/dummiesmod.so") != 0)
    goto out;
  

  /* load the web-interface block */
  if(ubx_module_load(&ni, "/home/haianos/enea_microblx/microblx/std_blocks/webif/webif.so") != 0)
    goto out;
    
  printf("All modules have been loaded!\n");
  
  /* Create blocks */
  /* create sseq_1 block, instance of sseq */
  if((sseq_1_1 = ubx_block_create(&ni, "sseq", "sseq_1_1"))==NULL)
    goto out;
    
  /* create dummy_1 block, instance of receiver */
  if((dummy_1_2 = ubx_block_create(&ni, "receiver", "dummy_1_2"))==NULL)
    goto out;
    
  /* create dummy_2 block, instance of receiver */
  if((dummy_2_3 = ubx_block_create(&ni, "receiver", "dummy_2_3"))==NULL)
    goto out;
    
  /* create a webserver block */
  if((webif = ubx_block_create(&ni, "webif/webif", "webif1"))==NULL)
    goto out;
  
  printf("All modules have been created!\n");
  
  /* Configure blocks */
    /* configuration of sseq_1_1  */
        
  d = ubx_config_get_data(sseq_1_1,"seq_blocks");
  ubx_data_resize(d,2);
    memcpy(d->data,
    (struct seq_config []){
      { .b=dummy_1_2, .num_steps=2, .measure=0},
      { .b=dummy_2_3, .num_steps=1, .measure=0},
      { NULL },
    },    
    sizeof(struct seq_config)*2
  );




  /* Configure port of webserver block
  * this gets the ubx_data_t pointer */
  d = ubx_config_get_data(webif, "port");
  len = strlen(WEBIF_PORT)+1;
    
  /* resize the char array as necessary and copy the port string */
  ubx_data_resize(d, len);
  strncpy(d->data, WEBIF_PORT, len);
  

  /* INIT and START the BLOCKS */
  if(ubx_block_init(dummy_1_2) != 0) {
    ERR("failed to init dummy_1_2");
    goto out;
  }
  if(ubx_block_init(sseq_1_1) != 0) {
    ERR("failed to init sseq_1_1");
    goto out;
  }
  if(ubx_block_init(dummy_2_3) != 0) {
    ERR("failed to init dummy_2_3");
    goto out;
  }
  printf("All blocks have been initialized!\n");

  if(ubx_block_start(dummy_1_2) != 0) {
    ERR("failed to start dummy_1_2");
    goto out;
  }
  if(ubx_block_start(sseq_1_1) != 0) {
    ERR("failed to start sseq_1_1");
    goto out;
  }
  if(ubx_block_start(dummy_2_3) != 0) {
    ERR("failed to start dummy_2_3");
    goto out;
  }
  printf("All blocks have been started!\n");
  
  /*        web interface       */
  if(ubx_block_init(webif) != 0) {
    ERR("failed to init webif");
    goto out;
  }

  if(ubx_block_start(webif) != 0) {
    ERR("failed to start webif");
    goto out;
  }
  printf("webif block lauched on port %s\n", WEBIF_PORT);

  ubx_block_t *sched = ubx_block_get(&ni,"sseq_1_1");
  if( sched )
  {
    while(1)
    {
      sched->step(sched);
      sleep(1);
    }
  }
  else
    printf("Scheduler Entry point at 4(schedname) not found");
    


  ret=EXIT_SUCCESS;
out:
  /* this cleans up all blocks and unloads all modules */
  ubx_node_cleanup(&ni);
  exit(ret);  
}

