#include <stdlib.h>
#include "context.h"
#include "coroutine.h"


hlist_head g_coroutine_map[1024];
list_head  g_coroutine_list = LIST_HEAD_INIT(g_coroutine_list);
list_head  g_coroutine_ready_list = LIST_HEAD_INIT(g_coroutine_ready_list);

coroutine_ctx_t *g_coroutine_running_ctx;


static int
hash_int(int val)
{
  val = val * 0x9e370001UL;

  /* return val % 1024; */
  return val & ((1 << 10) - 1);
}


coroutine_ctx_t*
coroutine_get_ctx(coroutine_t cid)
{
  int index;
  coroutine_ctx_t *ctx;

  ctx = NULL;
  index = hash_int(cid);
  hlist_for_each_entry(ctx, &g_coroutine_map[index], hash) {
    if (ctx->cid == cid) {
      return ctx;
    }
  }

  return NULL;
}


void
coroutine_set_ctx(coroutine_t cid, coroutine_ctx_t *ctx)
{
  int index;

  ctx->cid = cid;
  index = hash_int(cid);
  hlist_add_head(&ctx->hash, &g_coroutine_map[index]);

  list_add(&ctx->list, &g_coroutine_list);
}

