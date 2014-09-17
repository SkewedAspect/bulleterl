#include "erl_nif.h"

#include "BulletEngine.h"


static ErlNifResourceType* bulleterlType = NULL;


namespace bulleterl
{

	static ERL_NIF_TERM new_(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
	{
		BulletEngine* engine = new (enif_alloc_resource(bulleterlType, sizeof(BulletEngine))) BulletEngine();
		ERL_NIF_TERM result = enif_make_resource(env, engine);
		enif_release_resource(engine);

		return enif_make_tuple2(env, enif_make_atom(env, "bulleterl"), result);
	} // end new_


	static ERL_NIF_TERM myfunction(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
	{
		return enif_make_atom(env, "ok");
	} // end myfunction

	static void cleanup(ErlNifEnv* env, void* obj)
	{
		BulletEngine* handle = (BulletEngine*) obj;
		handle->~BulletEngine();
	} // end cleanup

	static int on_load(ErlNifEnv* env, void** priv_data, ERL_NIF_TERM load_info)
	{
		ErlNifResourceFlags flags = ErlNifResourceFlags(ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER);

		ErlNifResourceType* rt;

		rt = enif_open_resource_type(env, NULL, "bulleterl", &bulleterl::cleanup, flags, NULL);
		if(rt == NULL) { return -1; }
		bulleterlType = rt;

		return 0;
	} // end on_load

}; // end bulleterl


static ErlNifFunc nif_funcs[] = {
    {"new", 0, bulleterl::new_},
    {"myfunction", 1, bulleterl::myfunction}
}; // end nif_funcs

ERL_NIF_INIT(bulleterl, nif_funcs, &bulleterl::on_load, NULL, NULL, NULL);
