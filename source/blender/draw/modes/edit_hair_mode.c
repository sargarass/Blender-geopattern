/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) Blender Foundation
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): Lukas Toenne
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file blender/draw/modes/edit_hair_mode.c
 *  \ingroup draw
 */

#include "DRW_engine.h"
#include "DRW_render.h"

#include "DNA_hair_types.h"

/* If builtin shaders are needed */
#include "GPU_shader.h"

#include "draw_common.h"

#include "draw_mode_engines.h"

/* If needed, contains all global/Theme colors
 * Add needed theme colors / values to DRW_globals_update() and update UBO
 * Not needed for constant color. */
extern struct GPUUniformBuffer *globals_ubo; /* draw_common.c */
extern struct GlobalsUboStorage ts; /* draw_common.c */

extern char datatoc_common_globals_lib_glsl[];
extern char datatoc_edit_hair_overlay_loosevert_vert_glsl[];
extern char datatoc_edit_hair_overlay_frag_glsl[];

extern char datatoc_gpu_shader_3D_vert_glsl[];
extern char datatoc_gpu_shader_uniform_color_frag_glsl[];
extern char datatoc_gpu_shader_point_uniform_color_frag_glsl[];

/* *********** LISTS *********** */
/* All lists are per viewport specific datas.
 * They are all free when viewport changes engines
 * or is free itself. Use EDIT_GROOM_engine_init() to
 * initialize most of them and EDIT_GROOM_cache_init()
 * for EDIT_GROOM_PassList */

typedef struct EDIT_HAIR_PassList {
	/* Declare all passes here and init them in
	 * EDIT_GROOM_cache_init().
	 * Only contains (DRWPass *) */
	struct DRWPass *wire_pass;
	struct DRWPass *vert_pass;
} EDIT_HAIR_PassList;

typedef struct EDIT_HAIR_FramebufferList {
	/* Contains all framebuffer objects needed by this engine.
	 * Only contains (GPUFrameBuffer *) */
	struct GPUFrameBuffer *fb;
} EDIT_HAIR_FramebufferList;

typedef struct EDIT_HAIR_TextureList {
	/* Contains all framebuffer textures / utility textures
	 * needed by this engine. Only viewport specific textures
	 * (not per object). Only contains (GPUTexture *) */
	struct GPUTexture *texture;
} EDIT_HAIR_TextureList;

typedef struct EDIT_HAIR_StorageList {
	/* Contains any other memory block that the engine needs.
	 * Only directly MEM_(m/c)allocN'ed blocks because they are
	 * free with MEM_freeN() when viewport is freed.
	 * (not per object) */
	struct CustomStruct *block;
	struct EDIT_HAIR_PrivateData *g_data;
} EDIT_HAIR_StorageList;

typedef struct EDIT_HAIR_Data {
	/* Struct returned by DRW_viewport_engine_data_ensure.
	 * If you don't use one of these, just make it a (void *) */
	// void *fbl;
	void *engine_type; /* Required */
	EDIT_HAIR_FramebufferList *fbl;
	EDIT_HAIR_TextureList *txl;
	EDIT_HAIR_PassList *psl;
	EDIT_HAIR_StorageList *stl;
} EDIT_HAIR_Data;

/* *********** STATIC *********** */

static struct {
	/* Custom shaders :
	 * Add sources to source/blender/draw/modes/shaders
	 * init in EDIT_GROOM_engine_init();
	 * free in EDIT_GROOM_engine_free(); */
	GPUShader *wire_sh;

	GPUShader *overlay_vert_sh;

} e_data = {NULL}; /* Engine data */

typedef struct EDIT_HAIR_PrivateData {
	/* This keeps the references of the shading groups for
	 * easy access in EDIT_GROOM_cache_populate() */
	DRWShadingGroup *wire_shgrp;
	DRWShadingGroup *vert_shgrp;
} EDIT_HAIR_PrivateData; /* Transient data */

/* *********** FUNCTIONS *********** */

/* Init Textures, Framebuffers, Storage and Shaders.
 * It is called for every frames.
 * (Optional) */
static void EDIT_HAIR_engine_init(void *vedata)
{
	EDIT_HAIR_TextureList *txl = ((EDIT_HAIR_Data *)vedata)->txl;
	EDIT_HAIR_FramebufferList *fbl = ((EDIT_HAIR_Data *)vedata)->fbl;
	EDIT_HAIR_StorageList *stl = ((EDIT_HAIR_Data *)vedata)->stl;

	UNUSED_VARS(txl, fbl, stl);

	/* Init Framebuffers like this: order is attachment order (for color texs) */
	/*
	 * DRWFboTexture tex[2] = {{&txl->depth, DRW_TEX_DEPTH_24, 0},
	 *                         {&txl->color, DRW_TEX_RGBA_8, DRW_TEX_FILTER}};
	 */

	/* DRW_framebuffer_init takes care of checking if
	 * the framebuffer is valid and has the right size*/
	/*
	 * float *viewport_size = DRW_viewport_size_get();
	 * DRW_framebuffer_init(&fbl->occlude_wire_fb,
	 *                     (int)viewport_size[0], (int)viewport_size[1],
	 *                     tex, 2);
	 */

	if (!e_data.wire_sh) {
		e_data.wire_sh = GPU_shader_get_builtin_shader(GPU_SHADER_3D_SMOOTH_COLOR);
	}

	if (!e_data.overlay_vert_sh) {
		e_data.overlay_vert_sh = DRW_shader_create_with_lib(
		        datatoc_edit_hair_overlay_loosevert_vert_glsl, NULL,
		        datatoc_edit_hair_overlay_frag_glsl,
		        datatoc_common_globals_lib_glsl, NULL);
	}
}

/* Here init all passes and shading groups
 * Assume that all Passes are NULL */
static void EDIT_HAIR_cache_init(void *vedata)
{
	EDIT_HAIR_PassList *psl = ((EDIT_HAIR_Data *)vedata)->psl;
	EDIT_HAIR_StorageList *stl = ((EDIT_HAIR_Data *)vedata)->stl;

	if (!stl->g_data) {
		/* Alloc transient pointers */
		stl->g_data = MEM_mallocN(sizeof(*stl->g_data), __func__);
	}

	{
		psl->wire_pass = DRW_pass_create(
		        "Hair Wire",
		        DRW_STATE_WRITE_COLOR | DRW_STATE_WRITE_DEPTH | DRW_STATE_DEPTH_LESS | DRW_STATE_WIRE);
		stl->g_data->wire_shgrp = DRW_shgroup_create(e_data.wire_sh, psl->wire_pass);

		psl->vert_pass = DRW_pass_create(
		        "Hair Verts",
		        DRW_STATE_WRITE_COLOR | DRW_STATE_WRITE_DEPTH | DRW_STATE_POINT);
		stl->g_data->vert_shgrp = DRW_shgroup_create(e_data.overlay_vert_sh, psl->vert_pass);

		DRW_shgroup_uniform_block(stl->g_data->vert_shgrp, "globalsBlock", globals_ubo);
	}
}

/* Add geometry to shadingGroups. Execute for each objects */
static void EDIT_HAIR_cache_populate(void *vedata, Object *ob)
{
	EDIT_HAIR_PassList *psl = ((EDIT_HAIR_Data *)vedata)->psl;
	EDIT_HAIR_StorageList *stl = ((EDIT_HAIR_Data *)vedata)->stl;
	const DRWContextState *draw_ctx = DRW_context_state_get();
	Scene *scene = draw_ctx->scene;
	Object *obedit = draw_ctx->object_edit;
	HairEditSettings *editsettings = &scene->toolsettings->hair_edit_settings;

	UNUSED_VARS(psl);

	if (ob->type == OB_HAIR) {
		if (ob == obedit) {
			/* Get geometry cache */
			HairSystem *hsys = ob->data;
			struct GPUBatch *geom;

			geom = DRW_cache_hair_get_edit_strands(ob, hsys);
			DRW_shgroup_call_add(stl->g_data->wire_shgrp, geom, ob->obmat);

//			geom = DRW_cache_hair_get_verts(ob, hsys);
//			DRW_shgroup_call_add(stl->g_data->vert_shgrp, geom, ob->obmat);
		}
	}
}

/* Optional: Post-cache_populate callback */
static void EDIT_HAIR_cache_finish(void *vedata)
{
	EDIT_HAIR_PassList *psl = ((EDIT_HAIR_Data *)vedata)->psl;
	EDIT_HAIR_StorageList *stl = ((EDIT_HAIR_Data *)vedata)->stl;

	/* Do something here! dependant on the objects gathered */
	UNUSED_VARS(psl, stl);
}

/* Draw time ! Control rendering pipeline from here */
static void EDIT_HAIR_draw_scene(void *vedata)
{
	EDIT_HAIR_PassList *psl = ((EDIT_HAIR_Data *)vedata)->psl;
	EDIT_HAIR_FramebufferList *fbl = ((EDIT_HAIR_Data *)vedata)->fbl;

	/* Default framebuffer and texture */
	DefaultFramebufferList *dfbl = DRW_viewport_framebuffer_list_get();
	DefaultTextureList *dtxl = DRW_viewport_texture_list_get();

	UNUSED_VARS(fbl, dtxl);

	MULTISAMPLE_SYNC_ENABLE(dfbl, dtxl)

	/* Show / hide entire passes, swap framebuffers ... whatever you fancy */
	/*
	 * DRW_framebuffer_texture_detach(dtxl->depth);
	 * DRW_framebuffer_bind(fbl->custom_fb);
	 * DRW_draw_pass(psl->pass);
	 * DRW_framebuffer_texture_attach(dfbl->default_fb, dtxl->depth, 0, 0);
	 * DRW_framebuffer_bind(dfbl->default_fb);
	 */

	/* ... or just render passes on default framebuffer. */
	DRW_draw_pass(psl->wire_pass);
	DRW_draw_pass(psl->vert_pass);

	MULTISAMPLE_SYNC_DISABLE(dfbl, dtxl)

	/* If you changed framebuffer, double check you rebind
	 * the default one with its textures attached before finishing */
}

/* Cleanup when destroying the engine.
 * This is not per viewport ! only when quitting blender.
 * Mostly used for freeing shaders */
static void EDIT_HAIR_engine_free(void)
{
	// Currently built-in, dont free
	DRW_SHADER_FREE_SAFE(e_data.overlay_vert_sh);
}

/* Create collection settings here.
 *
 * Be sure to add this function there :
 * source/blender/draw/DRW_engine.h
 * source/blender/blenkernel/intern/layer.c
 * source/blenderplayer/bad_level_call_stubs/stubs.c
 *
 * And relevant collection settings to :
 * source/blender/makesrna/intern/rna_scene.c
 * source/blender/blenkernel/intern/layer.c
 */
#if 0
void EDIT_HAIR_collection_settings_create(CollectionEngineSettings *ces)
{
	BLI_assert(ces);
	// BKE_collection_engine_property_add_int(ces, "my_bool_prop", false);
	// BKE_collection_engine_property_add_int(ces, "my_int_prop", 0);
	// BKE_collection_engine_property_add_float(ces, "my_float_prop", 0.0f);
}
#endif

static const DrawEngineDataSize EDIT_HAIR_data_size = DRW_VIEWPORT_DATA_SIZE(EDIT_HAIR_Data);

DrawEngineType draw_engine_edit_hair_type = {
	NULL, NULL,
	N_("EditHairMode"),
	&EDIT_HAIR_data_size,
	&EDIT_HAIR_engine_init,
	&EDIT_HAIR_engine_free,
	&EDIT_HAIR_cache_init,
	&EDIT_HAIR_cache_populate,
	&EDIT_HAIR_cache_finish,
	NULL, /* draw_background but not needed by mode engines */
	&EDIT_HAIR_draw_scene,
	NULL,
};