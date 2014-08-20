#include "erl_nif.h"

static ErlNifResourceType* bulleterl_RESOURCE = NULL;

typedef struct
{
} bulleterl_handle;

// Prototypes
static ERL_NIF_TERM bulleterl_new(ErlNifEnv* env, int argc,
                                   const ERL_NIF_TERM argv[]);
static ERL_NIF_TERM bulleterl_myfunction(ErlNifEnv* env, int argc,
                                          const ERL_NIF_TERM argv[]);

static ErlNifFunc nif_funcs[] =
{
    {"new", 0, bulleterl_new},
    {"myfunction", 1, bulleterl_myfunction}
};

static ERL_NIF_TERM bulleterl_new(ErlNifEnv* env, int argc,
                                   const ERL_NIF_TERM argv[])
{
    bulleterl_handle* handle = enif_alloc_resource(bulleterl_RESOURCE,
                                                    sizeof(bulleterl_handle));
    ERL_NIF_TERM result = enif_make_resource(env, handle);
    enif_release_resource(handle);
    return enif_make_tuple2(env, enif_make_atom(env, "ok"), result);
}


static ERL_NIF_TERM bulleterl_myfunction(ErlNifEnv* env, int argc,
                                          const ERL_NIF_TERM argv[])
{
    return enif_make_atom(env, "ok");
}

static void bulleterl_resource_cleanup(ErlNifEnv* env, void* arg)
{
    /* Delete any dynamically allocated memory stored in bulleterl_handle */
    /* bulleterl_handle* handle = (bulleterl_handle*)arg; */
}

static int on_load(ErlNifEnv* env, void** priv_data, ERL_NIF_TERM load_info)
{
    ErlNifResourceFlags flags = ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER;
    ErlNifResourceType* rt = enif_open_resource_type(env, NULL,
                                                     "bulleterl_resource",
                                                     &bulleterl_resource_cleanup,
                                                     flags, NULL);
    if (rt == NULL)
        return -1;

    bulleterl_RESOURCE = rt;

    return 0;
}

ERL_NIF_INIT(bulleterl, nif_funcs, &on_load, NULL, NULL, NULL);
