#include "gl.h"
#include "list.h"
#include "debug.h"
#include "../glx/hardext.h"
#include "init.h"
#include "matrix.h"

#define alloc_sublist(n, cap) \
    (GLfloat *)malloc(n * sizeof(GLfloat) * cap)

#define realloc_sublist(ref, n, cap) \
    if (ref)                         \
        ref = (GLfloat *)realloc(ref, n * sizeof(GLfloat) * cap)

renderlist_t *alloc_renderlist() {
    int a;

    renderlist_t *list = (renderlist_t *)malloc(sizeof(renderlist_t));
    memset(list, 0, sizeof(*list));
    list->cap = DEFAULT_RENDER_LIST_CAPACITY;
    list->matrix_val[0] = list->matrix_val[5] = list->matrix_val[10] = 
                          list->matrix_val[15] = 1.0f;
    list->lightmodelparam = GL_LIGHT_MODEL_AMBIENT;
    list->target_texture = GL_TEXTURE_2D;
    list->tmu = glstate->texture.active;

    memcpy(list->lastNormal, glstate->normal, 3*sizeof(GLfloat));
    memcpy(list->lastSecondaryColors, glstate->secondary, 3*sizeof(GLfloat));
    memcpy(list->lastColors, glstate->color, 4*sizeof(GLfloat));

    list->open = true;
    return list;
}

bool ispurerender_renderlist(renderlist_t *list) {
    // return true if renderlist contains only rendering command, no state changes
    if (list->calls.len)
        return false;
    if (list->matrix_op)
        return false;
    if (list->raster_op)
        return false;
    if (list->raster)
        return false;
    if (list->pushattribute)
        return false;
    if (list->popattribute)
        return false;
    if (list->material || list->colormat_face || list->light || list->lightmodel || list->texgen || list->texenv)
        return false;
    if (list->fog_op)
        return false;
    if (list->pointparam_op)
        return false;
    if (list->mode_init == 0)
        return false;
    if (list->set_texture || list->set_tmu)
        return false;
    
    return true;
}

bool isempty_renderlist(renderlist_t *list) {
    return (list->stage == STAGE_NONE);
}

int rendermode_dimensions(GLenum mode) {
    // return 1 for points, 2 for any lines, 3 for any triangles, 4 for any Quad and 5 for polygon
    switch (mode) {
        case GL_POINTS:
            return 1;
        case GL_LINES:
        case GL_LINE_LOOP:
        case GL_LINE_STRIP:
            return 2;
        case GL_TRIANGLES:
        case GL_TRIANGLE_FAN:
        case GL_TRIANGLE_STRIP:
            return 3;
        case GL_QUADS:
        case GL_QUAD_STRIP:
            return 4;
        case GL_POLYGON:
            return 5;
    }
    return 0;
}

bool islistscompatible_renderlist(renderlist_t *a, renderlist_t *b) {
    if (!globals4es.mergelist || !a)
        return false;
        
    // check if 2 "pure rendering" list are compatible for merge
    if (a->mode_init != b->mode_init) {
        int a_mode = rendermode_dimensions(a->mode_init);
        int b_mode = rendermode_dimensions(b->mode_init);
        if ((a_mode == 5) || (b_mode == 5))
            return false;       // Let's not merge polygons
        if ((a_mode == 0) || (b_mode == 0))
            return false;       // undetermined is not good
        if (a_mode == 4) a_mode = 3; // quads are handled as triangles
        if (b_mode == 4) b_mode = 3;
        if (a_mode != b_mode)
            return false;
    }
    if(!a->open || !b->open)
        return false;
/*    if ((a->indices==NULL) != (b->indices==NULL))
        return false;*/
    if (a->polygon_mode != b->polygon_mode)
        return false;
    if ((a->vert==NULL) != (b->vert==NULL))
        return false;
    if ((a->normal==NULL) != (b->normal==NULL))
        return false;
    if ((a->color==NULL) != (b->color==NULL))
        return false;
    if ((a->secondary==NULL) != (b->secondary==NULL))
        return false;
    // check the textures
    if(a->maxtex!=b->maxtex)
        return false;
    for (int i=0; i<a->maxtex; i++)
        if ((a->tex[i]==NULL) != (b->tex[i]==NULL))
            return false;
    if ((a->set_texture==b->set_texture) && ((a->texture != b->texture) || (a->target_texture != b->target_texture)))
        return false;
    if (!a->set_texture && b->set_texture)
        return false;
    // polygon mode
    if(a->polygon_mode!=b->polygon_mode)
        return false;
    if(a->post_color || b->post_color || a->post_normal || b->post_normal)
        return false;
        
    // Check the size of a list, if it"s too big, don't merge...
    if ((a->len+b->len)>30000)
        return false;
    if ((a->ilen+b->ilen)>30000)
        return false;
    
    return true;
}

void renderlist_createindices(renderlist_t *a, GLushort *indices, int count) {
    int ilen = a->len;
    for (int i = 0; i<ilen; i++) {
        indices[i] = i+count;
    }
}

#define vind(a) (((ind)?ind[(a)]:(a))+count)

void renderlist_lineloop_lines(renderlist_t *a, GLushort *indices, int count) {
    GLushort *ind = a->indices;
    int len = (ind)? a->ilen:a->len;
    int ilen = len*2;  // new size is 2* + return
    for (int i = 0; i<ilen-1; i++) {
        indices[i] = vind((i+1)/2);
    }
    // go back to initial point
    indices[ilen-1] = indices[0];
}

void renderlist_linestrip_lines(renderlist_t *a, GLushort *indices, int count) {
    GLushort *ind = a->indices;
    int len = (ind)? a->ilen:a->len;
    int ilen = len*2-2;  // new size is 2*
    if (ilen<0) ilen=0;
    for (int i = 0; i<ilen; i++) {
        indices[i] = vind((i+1)/2);
    }
}

void renderlist_trianglestrip_triangles(renderlist_t *a, GLushort *indices, int count) {
    GLushort *ind = a->indices;
    int len = (ind)? a->ilen:a->len;
    int ilen = (len-2)*3;  
    if (ilen<0) ilen=0;
    for (int i = 2; i<len; i++) {
        indices[(i-2)*3+(i%2)] = vind(i-2);
        indices[(i-2)*3+1-(i%2)] = vind(i-1);
        indices[(i-2)*3+2] = vind(i);
    }
}

void renderlist_trianglefan_triangles(renderlist_t *a, GLushort *indices, int count) {
    GLushort *ind = a->indices;
    int len = (ind)? a->ilen:a->len;
    int ilen = (len-2)*3;  
    if (ilen<0) ilen=0;
    for (int i = 2; i<len; i++) {
        indices[(i-2)*3+0] = vind(0);
        indices[(i-2)*3+1] = vind(i-1);
        indices[(i-2)*3+2] = vind(i);
    }
}

void renderlist_quads_triangles(renderlist_t *a, GLushort *indices, int count) {
    GLushort *ind = a->indices;
    int len = (ind)? a->ilen:a->len;
    // len must be a multiple of 4 !
    len &= ~3;  // discard extra vertex...
    int ilen = len*3/2;
    for (int i=0, j=0; i+3<len; i+=4, j+=6) {
        indices[j+0] = vind(i+0);
        indices[j+1] = vind(i+1);
        indices[j+2] = vind(i+2);

        indices[j+3] = vind(i+0);
        indices[j+4] = vind(i+2);
        indices[j+5] = vind(i+3);
    }
}
#undef vind
#define vind(a) ((ind)?ind[(a)]:(a))
void renderlist_quads2triangles(renderlist_t *a) {
    GLushort *ind = a->indices;
    int len = (ind)? a->ilen:a->len;
    // len must be a multiple of 4 !
    len &= ~3;  // discard extra vertex...
    int ilen = len*3/2;
    a->indices = (GLushort*)malloc(ilen*sizeof(GLushort));
    for (int i=0, j=0; i+3<len; i+=4, j+=6) {
        a->indices[j+0] = vind(i+0);
        a->indices[j+1] = vind(i+1);
        a->indices[j+2] = vind(i+2);

        a->indices[j+3] = vind(i+0);
        a->indices[j+4] = vind(i+2);
        a->indices[j+5] = vind(i+3);
    }
    a->ilen = ilen;
    if ((ind) && (!a->shared_indices || ((*a->shared_indices)--)==0))  {free(ind); free(a->shared_indices);}
    a->mode = GL_TRIANGLES;
}
#undef vind

int renderlist_getindicesize(renderlist_t *a) {
    int ilen_a;
    switch (a->mode) {
        case GL_LINE_LOOP:
            ilen_a = ((a->indices)? a->ilen:a->len)*2;
            if (ilen_a<0) ilen_a=1; // special borked case...
            break;
        case GL_LINE_STRIP:
            ilen_a = (((a->indices)? a->ilen:a->len)*2)-2;
            if (ilen_a<0) ilen_a=1; // special borked case...
            break;
        case GL_QUAD_STRIP:
        case GL_TRIANGLE_STRIP:
            ilen_a = (((a->indices)? a->ilen:a->len)-2)*3;
            if (ilen_a<0) ilen_a=1; // special borked case...
            break;
        case GL_TRIANGLE_FAN:
        case GL_POLYGON:
            ilen_a = (((a->indices)? a->ilen:a->len)-2)*3;
            if (ilen_a<0) ilen_a=1; // special borked case...
            break;
        case GL_QUADS:
            ilen_a = ((((a->indices)? a->ilen:a->len)&~3)*3)/2;
            break;
        default:
            ilen_a = a->ilen;
            break;
    }
    return ilen_a;
}
void append_renderlist(renderlist_t *a, renderlist_t *b) {
    // append all draw elements of b in a
    
    // check the final indice size of a and b
    int ilen_a = renderlist_getindicesize(a);
    int ilen_b = renderlist_getindicesize(b);
    // lets append all the arrays
    unsigned long cap = a->cap;
    if (a->len + b->len >= cap) cap += b->cap + DEFAULT_RENDER_LIST_CAPACITY;
    if (a->shared_arrays && ((*a->shared_arrays)--)>0) {
        // Unshare if shared (shared array are not used for now)
        a->cap = cap;
        GLfloat *tmp;
        tmp = a->vert;
        if (tmp) {
            a->vert = alloc_sublist(4, cap);
            memcpy(a->vert, tmp, 4*a->len*sizeof(GLfloat));
        }
        tmp = a->normal;
        if (tmp) {
            a->normal = alloc_sublist(3, cap);
            memcpy(a->normal, tmp, 3*a->len*sizeof(GLfloat));
        }
        tmp = a->color;
        if (tmp) {
            a->color = alloc_sublist(4, cap);
            memcpy(a->color, tmp, 4*a->len*sizeof(GLfloat));
        }
        tmp = a->secondary;
        if (tmp) {
            a->secondary = alloc_sublist(4, cap);
            memcpy(a->secondary, tmp, 4*a->len*sizeof(GLfloat));
        }
        for (int i=0; i<a->maxtex; i++) {
            tmp = a->tex[i];
            if (tmp) {
                a->tex[i] = alloc_sublist(4, cap);
                memcpy(a->tex[i], tmp, 4*a->len*sizeof(GLfloat));
            }
        }
    } else {
        if (a->cap < cap) {
            a->cap = cap;
            realloc_sublist(a->vert, 4, cap);
            realloc_sublist(a->normal, 3, cap);
            realloc_sublist(a->color, 4, cap);
            realloc_sublist(a->secondary, 4, cap);
            for (int i=0; i<a->maxtex; i++)
               realloc_sublist(a->tex[i], 4, cap);
        }
    }
    if(a->shared_arrays && *a->shared_arrays==0) {free(a->shared_arrays); a->shared_arrays=0;}
    if (a->shared_indices && ((*a->shared_indices)--)>0) {
        if (a->indices) {
            GLushort* tmpi = a->indices;
            a->indice_cap = ((ilen_a)?ilen_a:a->len) + ((ilen_b)?ilen_b:b->len);
            if (a->indice_cap > 48) a->indice_cap = (a->indice_cap+511)&~511;
            a->indices = (GLushort*)malloc(a->indice_cap*sizeof(GLushort));
            memcpy(a->indices, tmpi, a->ilen*sizeof(GLushort));
        }
    } 
    if(a->shared_indices && *a->shared_indices==0) {free(a->shared_indices); a->shared_indices=0;}
    // append arrays
    if (a->vert) memcpy(a->vert+a->len*4, b->vert, b->len*4*sizeof(GLfloat));
    if (a->normal) memcpy(a->normal+a->len*3, b->normal, b->len*3*sizeof(GLfloat));
    if (a->color) memcpy(a->color+a->len*4, b->color, b->len*4*sizeof(GLfloat));
    if (a->secondary) memcpy(a->secondary+a->len*4, b->secondary, b->len*4*sizeof(GLfloat));
    for (int i=0; i<a->maxtex; i++)
        if (a->tex[i]) memcpy(a->tex[i]+a->len*4, b->tex[i], b->len*4*sizeof(GLfloat));
    // indices
    if (ilen_a + ilen_b)
    {
        // alloc or realloc a->indices first...
        int capindices = ((ilen_a)?ilen_a:a->len) + ((ilen_b)?ilen_b:b->len);
        if (capindices > 48) capindices = (capindices+511)&~511;
        #define alloc_a_indices                                      \
        newind=(GLushort*)malloc(capindices*sizeof(GLushort))
        #define copy_a_indices                                       \
        if (a->indices) free(a->indices);                            \
        a->indices = newind;                                         \
        a->indice_cap = capindices
        // check if "a" needs to be converted
        GLushort *newind=NULL;
        switch (a->mode) {
            case GL_LINE_LOOP:
                alloc_a_indices;
                renderlist_lineloop_lines(a, newind, 0);
                a->mode = GL_LINES;
                copy_a_indices;
                break;
            case GL_LINE_STRIP:
                alloc_a_indices;
                renderlist_linestrip_lines(a, newind, 0);
                a->mode = GL_LINES;
                copy_a_indices;
                break;
            case GL_QUAD_STRIP:
            case GL_TRIANGLE_STRIP:
                alloc_a_indices;
                renderlist_trianglestrip_triangles(a, newind, 0);
                a->mode = GL_TRIANGLES;
                copy_a_indices;
                break;
            case GL_TRIANGLE_FAN:
            case GL_POLYGON:
                alloc_a_indices;
                renderlist_trianglefan_triangles(a, newind, 0);
                a->mode = GL_TRIANGLES;
                copy_a_indices;
                break;
            case GL_QUADS:
                alloc_a_indices;
                renderlist_quads_triangles(a, newind, 0);
                a->mode = GL_TRIANGLES;
                copy_a_indices;
                break;
            default:
                if (!ilen_a) {
                    // no a->indices, must alloc and fill one
                    alloc_a_indices;
                    renderlist_createindices(a, newind, 0);
                    ilen_a = a->len;
                    copy_a_indices;
                } else {
                    // a->indices already exist, just check if need to adjust its size
                    if (a->indice_cap < capindices) {
                        a->indices = (GLushort*)realloc(a->indices, capindices*sizeof(GLushort));
                        a->indice_cap = capindices;
                    }
                }
                break;
        }
        #undef copy_a_indices
        #undef alloc_a_indices

        a->ilen = ilen_a;
        // then append b
        switch (b->mode) {
            case GL_LINE_LOOP:
                renderlist_lineloop_lines(b, a->indices + ilen_a, a->len);
                break;
            case GL_LINE_STRIP:
                renderlist_linestrip_lines(b, a->indices + ilen_a, a->len);
                break;
            case GL_QUAD_STRIP:
            case GL_TRIANGLE_STRIP:
                renderlist_trianglestrip_triangles(b, a->indices + ilen_a, a->len);
                break;
            case GL_TRIANGLE_FAN:
            case GL_POLYGON:
                renderlist_trianglefan_triangles(b, a->indices + ilen_a, a->len);
                break;
            case GL_QUADS:
                renderlist_quads_triangles(b, a->indices + ilen_a, a->len);
                break;
            default:
                // no transform here, just take (or create) the indice list as-is
                if (!b->ilen) {
                    // append a newly created indice list
                    renderlist_createindices(b, a->indices + ilen_a, a->len);
                    ilen_b = b->len;
                } else {
                    // append existing one
                    newind = a->indices+ilen_a;
                    for(int i=0; i<b->ilen; i++)
                        newind[i] = b->indices[i]+a->len;
                }
                break;
        }
    }
    // lenghts
    a->len += b->len;
    a->ilen += ilen_b;
    // copy the lastColors if needed
    if(b->lastColorsSet) {
        a->lastColorsSet = 1;
        memcpy(a->lastColors, b->lastColors, 4*sizeof(GLfloat));
    }
    //all done
    a->stage = STAGE_DRAW;  // just in case
    return;
}
void adjust_renderlist(renderlist_t *list);

renderlist_t *extend_renderlist(renderlist_t *list) {
    if ((list->prev!=NULL) && ispurerender_renderlist(list) && islistscompatible_renderlist(list->prev, list)) {
        // append list!
        append_renderlist(list->prev, list);
        renderlist_t *new = alloc_renderlist();
        new->prev = list->prev;
        list->prev->next = new;
        // just in case
        memcpy(new->lastNormal, list->lastNormal, 3*sizeof(GLfloat));
        memcpy(new->lastSecondaryColors, list->lastSecondaryColors, 3*sizeof(GLfloat));
        memcpy(new->lastColors, list->lastColors, 4*sizeof(GLfloat));
        new->lastColorsSet = list->lastColorsSet;
        // detach
        list->prev = NULL;
        // free list now
        free_renderlist(list);
        return new;
    } else {
        renderlist_t *new = alloc_renderlist();
        list->next = new;
        new->prev = list;
        new->tmu = list->tmu;
        // copy local state
        memcpy(new->lastNormal, list->lastNormal, 3*sizeof(GLfloat));
        memcpy(new->lastSecondaryColors, list->lastSecondaryColors, 3*sizeof(GLfloat));
        memcpy(new->lastColors, list->lastColors, 4*sizeof(GLfloat));
        new->lastColorsSet = list->lastColorsSet;
        return new;
    }
}
int Cap2BatchState(GLenum cap);

renderlist_t* append_calllist(renderlist_t *list, renderlist_t *a)
{
    // go to end of list
    while(list->next) list = list->next;
    while(a) {
        if(ispurerender_renderlist(a) && islistscompatible_renderlist(list, a)) {
            // append list!
            append_renderlist(list, a);
        } else {
            // create a new appended list
            renderlist_t *new = alloc_renderlist();
            // prepared shared stuff...
            if(a->len && !a->shared_arrays) {
                a->shared_arrays = (int*)malloc(sizeof(int));
                *a->shared_arrays = 0;
            }
            if(a->ilen && !a->shared_indices) {
                a->shared_indices = (int*)malloc(sizeof(int));
                *a->shared_indices = 0;
            }
            if(a->calls.len && !a->shared_calls) {
                a->shared_calls = (int*)malloc(sizeof(int));
                *a->shared_calls = 0;
            }
            // batch copy first
            memcpy(new, a, sizeof(renderlist_t));
            list->next = new;
            new->prev = list;
            // ok, now on new list
            list = new;
            // copy the many list arrays
            if(glstate->gl_batch) {
                if (list->popattribute) {
                    // invalidate all batchstate
                    for (int i=0; i<hardext.maxtex; i++) {
                        glstate->statebatch.bound_targ[i] = 0xffff;
                        glstate->statebatch.bound_tex[i] = 0xffffffff;
                    }
                    for (int i=0; i<ENABLED_LAST; i++) {
                        glstate->statebatch.enabled[i] = 3;   //undefined
                    }
                }
            }
            if (list->calls.len > 0) {
                ++(*list->shared_calls);
                /*
                list->calls.calls = (packed_call_t**)malloc(sizeof(packed_call_t*)*a->calls.cap);
                for (int i = 0; i < list->calls.len; i++) {
                    list->calls.calls[i] = glCopyPackedCall(a->calls.calls[i]);
                }*/
                // in case of batch mode, need to update the batchstate...
                if(glstate->gl_batch) {
                    for (int i = 0; i < list->calls.len; i++) {
                        packed_call_t *p = list->calls.calls[i];
                        if(p->func == &gl4es_glEnable) {
                            int wich_cap = Cap2BatchState(((glEnable_PACKED*)p)->args.a1);
                            if(wich_cap!=ENABLED_LAST) glstate->statebatch.enabled[wich_cap] = 1;
                        }
                        if(p->func == &gl4es_glDisable) {
                            int wich_cap = Cap2BatchState(((glDisable_PACKED*)p)->args.a1);
                            if(wich_cap!=ENABLED_LAST) glstate->statebatch.enabled[wich_cap] = 0;
                        }
                    }
                }
            }
            if(list->len) {
                ++(*list->shared_arrays);
            }
            if(list->ilen) {
                ++(*list->shared_indices);
            }
            #define PROCESS(W, T, C) if(list->W) { \
                    list->W = kh_init(W);   \
                    T *m, *m2;      \
                    khint_t k;      \
                    int ret;        \
                    kh_foreach_value(a->W, m,   \
                        k = kh_put(W, list->W, C, &ret);    \
                        m2= kh_value(list->W, k) = malloc(sizeof(T));   \
                        memcpy(m2, m, sizeof(T));           \
                    );       \
                }
            PROCESS(material, rendermaterial_t, m->pname);
            PROCESS(light, renderlight_t, m->pname | ((m->which-GL_LIGHT0)<<16));
            PROCESS(texgen, rendertexgen_t, m->pname | ((m->coord-GL_S)<<16));
            PROCESS(texenv, rendertexenv_t, m->pname | ((m->target)<<16));
            #undef PROCESS
            if (list->lightmodel) {
                list->lightmodel = (GLfloat*)malloc(4*sizeof(GLfloat));
                memcpy(list->lightmodel, a->lightmodel, 4*sizeof(GLfloat));
            }
            if (list->raster) {
                (*list->raster->shared)++;
            }
            // Update other batchstate states
            if(glstate->gl_batch) {
                if (list->set_tmu) {
                    glstate->statebatch.active_tex = GL_TEXTURE0 + list->tmu;
                    glstate->statebatch.active_tex_changed = 1;
                }
                if (list->set_texture) {
                    const int batch_activetex = glstate->statebatch.active_tex_changed?(glstate->statebatch.active_tex-GL_TEXTURE0):glstate->texture.active;
                    glstate->statebatch.bound_targ[batch_activetex] = list->target_texture;
                    glstate->statebatch.bound_tex[batch_activetex] = list->texture;
                }
            }
        }
        a = a->next;
    }
    return list;
}

void free_renderlist(renderlist_t *list) {
	// test if list is NULL
	if (list == NULL)
		return;
    // we want the first list in the chain
    while (list->prev)
        list = list->prev;

    renderlist_t *next;
    do {
        if ((list->calls.len > 0) && (!list->shared_calls || ((*list->shared_calls)--)==0)) {
            if(list->shared_calls) free(list->shared_calls);
            for (int i = 0; i < list->calls.len; i++) {
                free(list->calls.calls[i]);
            }
            free(list->calls.calls);
        }
        int a;
        if (!list->shared_arrays || ((*list->shared_arrays)--)==0) {
            if (list->shared_arrays) free(list->shared_arrays);
            if (list->vert) free(list->vert);
            if (list->normal) free(list->normal);
            if (list->color) free(list->color);
            if (list->secondary) free(list->secondary);
            for (a=0; a<list->maxtex; a++)
                if (list->tex[a]) free(list->tex[a]);
        }
        if (!list->shared_indices || ((*list->shared_indices)--)==0) {
            if (list->shared_indices) free(list->shared_indices);
            if (list->indices)
                free(list->indices);
        }

        if (list->material) {
            rendermaterial_t *m;
            kh_foreach_value(list->material, m,
                free(m);
            )
            kh_destroy(material, list->material);
        }
        if (list->light) {
            renderlight_t *m;
            kh_foreach_value(list->light, m,
                free(m);
            )
            kh_destroy(light, list->light);
        }
        if (list->texgen) {
            rendertexgen_t *m;
            kh_foreach_value(list->texgen, m,
                free(m);
            )
            kh_destroy(texgen, list->texgen);
        }
        if (list->texenv) {
            rendertexenv_t *m;
            kh_foreach_value(list->texenv, m,
                free(m);
            )
            kh_destroy(texenv, list->texenv);
        }
        if (list->lightmodel)
			free(list->lightmodel);
			
        if (list->raster && !(*(list->raster->shared)--)) {
			if (list->raster->texture)
				gl4es_glDeleteTextures(1, &list->raster->texture);
			free(list->raster);
		}

        next = list->next;
        free(list);
    } while ((list = next));
}

void resize_renderlist(renderlist_t *list) {
    if (list->len >= list->cap) {
        list->cap += DEFAULT_RENDER_LIST_CAPACITY*4;
        realloc_sublist(list->vert, 4, list->cap);
        realloc_sublist(list->normal, 3, list->cap);
        realloc_sublist(list->color, 4, list->cap);
        realloc_sublist(list->secondary, 4, list->cap);
        for (int a=0; a<list->maxtex; a++)
           realloc_sublist(list->tex[a], 4, list->cap);
    }
}

void adjust_renderlist(renderlist_t *list) {
    if (! list->open)
        return;

    list->stage = STAGE_LAST;
    list->open = false;
    for (int a=0; a<list->maxtex; a++) {
        const GLint itarget = get_target(glstate->enable.texture[a]);
	    gltexture_t *bound = (itarget>=0)?glstate->texture.bound[a][itarget]:NULL;
        // in case of Texture bounding inside a list
        if (list->set_texture && (list->tmu == a))
            bound = gl4es_getTexture(list->target_texture, list->texture);
	    // GL_ARB_texture_rectangle
	    if ((list->tex[a]) && (itarget == ENABLED_TEXTURE_RECTANGLE) && (bound)) {
		    tex_coord_rect_arb(list->tex[a], list->len, bound->width, bound->height);
	    }
    }
}

renderlist_t* end_renderlist(renderlist_t *list) {
    if (!list || ! list->open)
        return list;

    adjust_renderlist(list);
    
    switch (list->mode) {
        case GL_QUADS:
			if (((list->indices) && (list->ilen==4)) || ((list->indices==NULL) && (list->len==4))) {
				list->mode = GL_TRIANGLE_FAN;
			} else {
                renderlist_quads2triangles(list);
			}
            break;
        case GL_POLYGON:
            list->mode = GL_TRIANGLE_FAN;
            break;
        case GL_QUAD_STRIP:
            list->mode = GL_TRIANGLE_STRIP;
            break;
    }
    if(list->prev && isempty_renderlist(list)) {
        renderlist_t *p = list;
        list = list->prev;
        list->next = NULL;
        p->prev = NULL;
        free_renderlist(p);
    }
    
    return list;
}

renderlist_t* recycle_renderlist(renderlist_t *list) {
    if(isempty_renderlist(list)) {
        renderlist_t* old=list;
        list = list->prev;
        old->prev = NULL;
        list->next = NULL;
        free_renderlist(old);
    }
    // check if pending color...
    if (list->post_color) {
        list->post_color = 0;
        rlColor4f(list, list->post_colors[0], list->post_colors[1], list->post_colors[2], list->post_colors[3]);
    }
    if (list->post_normal) {
        list->post_normal = 0;
        rlNormal3f(list, list->post_normals[0], list->post_normals[1], list->post_normals[2]);
    }
    // All done
    list->stage=STAGE_DRAW;

    return list;
}

void draw_renderlist(renderlist_t *list) {
    if (!list) return;
    // go to 1st...
    while (list->prev) list = list->prev;
    // ok, go on now, draw everything
//printf("draw_renderlist %p, gl_batch=%i, size=%i, mode=%s(%s), ilen=%d, next=%p, color=%p, secondarycolor=%p\n", list, glstate->gl_batch, list->len, PrintEnum(list->mode), PrintEnum(list->mode_init), list->ilen, list->next, list->color, list->secondary);
    LOAD_GLES(glDrawArrays);
    LOAD_GLES(glDrawElements);
#ifdef USE_ES2
    LOAD_GLES(glVertexAttribPointer);
#else
    LOAD_GLES(glVertexPointer);
    LOAD_GLES(glNormalPointer);
    LOAD_GLES(glColorPointer);
    LOAD_GLES(glTexCoordPointer);
#endif
    LOAD_GLES(glEnable);
    LOAD_GLES(glDisable);
    LOAD_GLES(glEnableClientState);
    LOAD_GLES(glDisableClientState);
    gl4es_glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

	GLfloat *final_colors;
	int old_tex;
    GLushort *indices;
    do {
        // close if needed!
        if (list->open)
            list = end_renderlist(list);
        // push/pop attributes
        if (list->pushattribute)
            gl4es_glPushAttrib(list->pushattribute);
        if (list->popattribute)
            gl4es_glPopAttrib();
        call_list_t *cl = &list->calls;
        if (cl->len > 0) {
            for (int i = 0; i < cl->len; i++) {
                glPackedCall(cl->calls[i]);
            }
        }
        if(list->render_op) {
            switch(list->render_op) {
                case 1: gl4es_glInitNames(); break;
                case 2: gl4es_glPopName(); break;
                case 3: gl4es_glPushName(list->render_arg); break;
                case 4: gl4es_glLoadName(list->render_arg); break;
            }
        }
        if (list->fog_op) {
            gl4es_glFogfv(GL_FOG_COLOR, list->fog_val);
        }
        if (list->pointparam_op) {
            switch (list->pointparam_op) {
                case 1: // GL_POINT_DISTANCE_ATTENUATION 
                    gl4es_glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION , list->pointparam_val);
                    break;
            }
        }
        if (list->matrix_op) {
            switch (list->matrix_op) {
                case 1: // load
                    gl4es_glLoadMatrixf(list->matrix_val);
                    break;
                case 2: // mult
                    gl4es_glMultMatrixf(list->matrix_val);
                    break;
            }
        }
        if (list->set_tmu) {
            gl4es_glActiveTexture(GL_TEXTURE0+list->tmu);
        }
	    if (list->set_texture) {
            gl4es_glBindTexture(list->target_texture, list->texture);
        }
        // raster
        old_tex = glstate->texture.active;
        if (list->raster_op) {
            if (list->raster_op==1) {
                gl4es_glRasterPos3f(list->raster_xyz[0], list->raster_xyz[1], list->raster_xyz[2]);
            } else if (list->raster_op==2) {
                gl4es_glWindowPos3f(list->raster_xyz[0], list->raster_xyz[1], list->raster_xyz[2]);
            } else if (list->raster_op==3) {
                gl4es_glPixelZoom(list->raster_xyz[0], list->raster_xyz[1]);
            } else if ((list->raster_op&0x10000) == 0x10000) {
                gl4es_glPixelTransferf(list->raster_op&0xFFFF, list->raster_xyz[0]);
            }
        }
        if (list->raster) {
            rasterlist_t * r = list->raster;
            //glBitmap(r->width, r->height, r->xorig, r->yorig, r->xmove, r->ymove, r->raster);
            render_raster_list(list->raster);
        }
			

        if (list->material) {
            khash_t(material) *map = list->material;
            rendermaterial_t *m;
            kh_foreach_value(map, m,
                switch (m->pname) {
                    case GL_SHININESS:
                        gl4es_glMaterialf(m->face,  m->pname, m->color[0]);
                        break;
                    default:
                        gl4es_glMaterialfv(m->face, m->pname, m->color);
                }
            )
        }
        if (list->colormat_face)
            gl4es_glColorMaterial(list->colormat_face, list->colormat_mode);
        if (list->light) {
            khash_t(light) *lig = list->light;
            renderlight_t *m;
            kh_foreach_value(lig, m,
                switch (m->pname) {
                    default:
                        gl4es_glLightfv(m->which, m->pname, m->color);
                }
            )
        }
        if (list->lightmodel) {
            gl4es_glLightModelfv(list->lightmodelparam, list->lightmodel);
        }

        if (list->linestipple_op) {
            gl4es_glLineStipple(list->linestipple_factor, list->linestipple_pattern);
        }
		
        if (list->texenv) {
            khash_t(texenv) *tgn = list->texenv;
            rendertexenv_t *m;
            kh_foreach_value(tgn, m,
                gl4es_glTexEnvfv(m->target, m->pname, m->params);
            )
        }

        if (list->texgen) {
            khash_t(texgen) *tgn = list->texgen;
            rendertexgen_t *m;
            kh_foreach_value(tgn, m,
                gl4es_glTexGenfv(m->coord, m->pname, m->color);
            )
        }
        
        if (list->polygon_mode) {
            gl4es_glPolygonMode(GL_FRONT_AND_BACK, list->polygon_mode);}

        if (! list->len)
            continue;
#ifdef USE_ES2
        if (list->vert) {
            gl4es_glEnableVertexAttribArray(0);
            gles_glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, list->vert);
        }
        gles_glDrawArrays(list->mode, 0, list->len);
#else
        if (list->vert) {
            gles_glEnableClientState(GL_VERTEX_ARRAY);
            gles_glVertexPointer(4, GL_FLOAT, 0, list->vert);
            glstate->clientstate.vertex_array = 1;
        } else {
            gles_glDisableClientState(GL_VERTEX_ARRAY);
            glstate->clientstate.vertex_array = false;
        }

        if (list->normal) {
            gles_glEnableClientState(GL_NORMAL_ARRAY);
            gles_glNormalPointer(GL_FLOAT, 0, list->normal);
            glstate->clientstate.normal_array = 1;
        } else {
            gles_glDisableClientState(GL_NORMAL_ARRAY);
            glstate->clientstate.normal_array = 0;
        }

        indices = list->indices;
           
		final_colors = NULL;
        if (list->color) {
            gles_glEnableClientState(GL_COLOR_ARRAY);
            glstate->clientstate.color_array = 1;
            if (glstate->enable.color_sum && (list->secondary)) {
				final_colors=(GLfloat*)malloc(list->len * 4 * sizeof(GLfloat));
				if (indices) {
					for (int i=0; i<list->ilen; i++)
						for (int j=0; j<4; j++) {
							const int k=indices[i]*4+j;
							final_colors[k]=list->color[k] + list->secondary[k];
						}
				 } else {
					for (int i=0; i<list->len*4; i++)
						final_colors[i]=list->color[i] + list->secondary[i];
				}
				gles_glColorPointer(4, GL_FLOAT, 0, final_colors);
			} else {
//printf("colors=%f, %f, %f, %f / %f, %f, %f, %f\n", list->color[0],list->color[1],list->color[2],list->color[3], list->color[4],list->color[5],list->color[6],list->color[7]);
				gles_glColorPointer(4, GL_FLOAT, 0, list->color);
            }
        } else {
            gles_glDisableClientState(GL_COLOR_ARRAY);
            glstate->clientstate.color_array = 0;
        }
        GLuint texture;
        bool stipple;
        stipple = false;
        if (! list->tex[0]) {
            // TODO: do we need to support GL_LINE_STRIP?
            if (list->mode == GL_LINES && glstate->enable.line_stipple) {
                stipple = true;
                gl4es_glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT);
                gl4es_glEnable(GL_BLEND);
                gl4es_glEnable(GL_TEXTURE_2D);
                gl4es_glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                list->tex[0] = gen_stipple_tex_coords(list->vert, list->len);
            } 
        }
        static GLfloat *texgened[MAX_TEX] = {0};
        static int texgenedsz[MAX_TEX] = {0};
        int use_texgen[MAX_TEX] = {0};
        old_tex = glstate->texture.client;
        GLuint cur_tex = old_tex;
        GLint needclean[MAX_TEX] = {0};
        #define TEXTURE(A) if (cur_tex!=A) {gl4es_glClientActiveTexture(A+GL_TEXTURE0); cur_tex=A;}
        #define RS(A, len) if(texgenedsz[A]<len) {free(texgened[A]); texgened[A]=malloc(4*sizeof(GLfloat)*len); texgenedsz[A]=len; } use_texgen[A]=1
        // cannot use list->maxtex because some TMU can be using TexGen or point sprites...
        for (int a=0; a<hardext.maxtex; a++) {
            if(glstate->enable.texture[a]) {
                const GLint itarget = get_target(glstate->enable.texture[a]);
                needclean[a]=0;
                use_texgen[a]=0;
                if ((glstate->enable.texgen_s[a] || glstate->enable.texgen_t[a] || glstate->enable.texgen_r[a]  || glstate->enable.texgen_q[a])) {
                    TEXTURE(a);
                    RS(a, list->len);
                    gen_tex_coords(list->vert, list->normal, &texgened[a], list->len, &needclean[a], a, (list->ilen<list->len)?indices:NULL, (list->ilen<list->len)?list->ilen:0);
                } else if ((list->tex[a]==NULL) && !(list->mode==GL_POINT && glstate->texture.pscoordreplace[a])) {
                    RS(a, list->len);
                    gen_tex_coords(list->vert, list->normal, &texgened[a], list->len, &needclean[a], a, (list->ilen<list->len)?indices:NULL, (list->ilen<list->len)?list->ilen:0);
                }
                // adjust the tex_coord now if needed, even on texgened ones
                gltexture_t *bound = glstate->texture.bound[a][itarget];
                if((list->tex[a] || (use_texgen[a] && !needclean[a])) && ((!(globals4es.texmat || glstate->texture_matrix[a]->identity)) || (bound) && ((bound->width != bound->nwidth) || (bound->height != bound->nheight)))) {
                    if(!use_texgen[a]) {
                        RS(a, list->len);
                        memcpy(texgened[a], list->tex[a], 4*sizeof(GLfloat)*list->len);
                    }
                    if (!(globals4es.texmat || glstate->texture_matrix[a]->identity))
                        tex_coord_matrix(texgened[a], list->len, getTexMat(a));
                    if ((bound) && ((bound->width != bound->nwidth) || (bound->height != bound->nheight))) {
                        tex_coord_npot(texgened[a], list->len, bound->width, bound->height, bound->nwidth, bound->nheight);
                    }
                }
            }
		    if ((list->tex[a] || (use_texgen[a] && !needclean[a]))/* && glstate->enable.texture[a]*/) {
                TEXTURE(a);
                if(!glstate->clientstate.tex_coord_array[a]) {
                    gles_glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                    glstate->clientstate.tex_coord_array[a] = 1;
                }
		        gles_glTexCoordPointer(4, GL_FLOAT, 0, (use_texgen[a])?texgened[a]:list->tex[a]);
		    } else {
                if (glstate->clientstate.tex_coord_array[a]) {
                    TEXTURE(a);
                    gles_glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                    glstate->clientstate.tex_coord_array[a] = 0;
                } 
//else if (!glstate->enable.texgen_s[a] && glstate->enable.texture[a]) printf("LIBGL: texture[%i] without TexCoord, mode=0x%04X (init=0x%04X), listlen=%i\n", a, list->mode, list->mode_init, list->len);
			    
		    }
            if (!IS_TEX2D(glstate->enable.texture[a]) && (IS_ANYTEX(glstate->enable.texture[a]))) {
                TEXTURE(a);
                gles_glEnable(GL_TEXTURE_2D);
            }
        }
        if (glstate->texture.client != old_tex) TEXTURE(old_tex);
        #undef RS
        #undef TEXTURE
        GLenum mode;
        mode = list->mode;
        if ((glstate->polygon_mode == GL_LINE) && (mode>=GL_TRIANGLES))
			mode = GL_LINES;
		if ((glstate->polygon_mode == GL_POINT) && (mode>=GL_TRIANGLES))
			mode = GL_POINTS;

        if (indices) {
            if (glstate->render_mode == GL_SELECT) {
                pointer_state_t vtx;
                vtx.pointer = list->vert;
                vtx.type = GL_FLOAT;
                vtx.size = 4;
                vtx.stride = 0;
                select_glDrawElements(&vtx, list->mode, list->ilen, GL_UNSIGNED_SHORT, indices);
            } else {
                if (glstate->polygon_mode == GL_LINE && list->mode_init>=GL_TRIANGLES) {
                    int n, s;
                    int ilen = list->ilen;
                    GLushort ind_line[ilen*4+2];
                    int k=0;
                    switch (list->mode_init) {
                        case GL_TRIANGLES:
                            if(ilen>2) {
                                // 1 triangle -> 3 lines
                                for (int i = 0; i<ilen-2; i+=3) {
                                    ind_line[k++] = indices[i+0]; ind_line[k++] = indices[i+1];
                                    ind_line[k++] = indices[i+1]; ind_line[k++] = indices[i+2];
                                    ind_line[k++] = indices[i+2]; ind_line[k++] = indices[i+0];
                                }
                            }
                            break;
                        case GL_TRIANGLE_STRIP:
                            // first 3 points a triangle, then a 2 lines per new point
                            if (ilen>2) {
                                ind_line[k++] = indices[0]; ind_line[k++] = indices[1];
                                for (int i = 2; i<ilen; i++) {
                                    ind_line[k++] = indices[i-2]; ind_line[k++] = indices[i];
                                    ind_line[k++] = indices[i-1]; ind_line[k++] = indices[i];
                                }
                            }
                            break;
                        case GL_TRIANGLE_FAN:
                            // first 3 points a triangle, then a 2 lines per new point too
                            if (ilen>2) {
                                ind_line[k++] = indices[0]; ind_line[k++] = indices[1];
                                for (int i = 2; i<ilen; i++) {
                                    ind_line[k++] = indices[0]; ind_line[k++] = indices[i];
                                    ind_line[k++] = indices[i-1]; ind_line[k++] = indices[i];
                                }
                            }
                            break;
                        case GL_QUADS:
                            if (ilen>3) {
                                // 4 lines per quads, but dest may already be a triangles list...
                                if (list->mode == GL_TRIANGLE_FAN) {
                                    // just 1 Quad
                                    for (int i=0; i<4; i++) {
                                        ind_line[k++] = indices[i+0]; ind_line[k++] = indices[(i+1)%4];
                                    }
                                } else {
                                    // list of triangles, 2 per quads...
                                    for (int i=0; i<ilen-5; i+=6) {
                                        ind_line[k++] = indices[i+0]; ind_line[k++] = indices[i+1];
                                        ind_line[k++] = indices[i+1]; ind_line[k++] = indices[i+2];
                                        ind_line[k++] = indices[i+2]; ind_line[k++] = indices[i+5];
                                        ind_line[k++] = indices[i+5]; ind_line[k++] = indices[i+0];
                                    }
                                }
                            }
                            break;
                        case GL_QUAD_STRIP:
                            // first 4 points is a quad, then 2 points per new quad
                            if (ilen>3) {
                                ind_line[k++] = indices[0]; ind_line[k++] = indices[1];
                                for (int i = 2; i<ilen-1; i+=2) {
                                    ind_line[k++] = indices[i-1]; ind_line[k++] = indices[i];
                                    ind_line[k++] = indices[i-2]; ind_line[k++] = indices[i+1];
                                    ind_line[k++] = indices[i+0]; ind_line[k++] = indices[i+1];
                                }
                            }
                            break;
                        case GL_POLYGON:
                            // if polygons have been merged, then info is lost...
                            if (ilen) {
                                ind_line[k++] = indices[0]; ind_line[k++] = indices[1];
                                for (int i = 1; i<ilen; i++) {
                                    ind_line[k++] = indices[i-1]; ind_line[k++] = indices[i];
                                }
                                ind_line[k++] = indices[ilen-1]; ind_line[k++] = indices[0];
                            }
                            break;
                    }
                    gles_glDrawElements(mode, k, GL_UNSIGNED_SHORT, ind_line);
                } else {
                    gles_glDrawElements(mode, list->ilen, GL_UNSIGNED_SHORT, indices);
                }
            }
        } else {
            if (glstate->render_mode == GL_SELECT) {	
                pointer_state_t vtx;
                vtx.pointer = list->vert;
                vtx.type = GL_FLOAT;
                vtx.size = 4;
                vtx.stride = 0;
                select_glDrawArrays(&vtx, list->mode, 0, list->len);
            } else {
                int len = list->len;
                if ((glstate->polygon_mode == GL_LINE) && (list->mode_init>=GL_TRIANGLES)) {
                    int n, s;
                    GLushort ind_line[len*4+2];
                    int k=0;
                    switch (list->mode_init) {
                        case GL_TRIANGLES:
                            // 1 triangle -> 3 lines
                            if(len>2) {
                                for (int i = 0; i<len-2; i+=3) {
                                    ind_line[k++] = i+0; ind_line[k++] = i+1;
                                    ind_line[k++] = i+1; ind_line[k++] = i+2;
                                    ind_line[k++] = i+2; ind_line[k++] = i+0;
                                }
                            }
                            break;
                        case GL_TRIANGLE_STRIP:
                            // first 3 points a triangle, then a 2 lines per new point
                            if (len>2) {
                                ind_line[k++] = 0; ind_line[k++] = 1;
                                for (int i = 2; i<len; i++) {
                                    ind_line[k++] = i-2; ind_line[k++] = i;
                                    ind_line[k++] = i-1; ind_line[k++] = i;
                                }
                            }
                            break;
                        case GL_TRIANGLE_FAN:
                            // first 3 points a triangle, then a 2 lines per new point too
                            if (len>2) {
                                ind_line[k++] = 0; ind_line[k++] = 1;
                                for (int i = 2; i<len; i++) {
                                    ind_line[k++] = 0; ind_line[k++] = i;
                                    ind_line[k++] = i-1; ind_line[k++] = i;
                                }
                            }
                            break;
                        case GL_QUADS:
                            if(len>3) {
                                // 4 lines per quads, QUAD without indices means 1 single quad
                                if (list->mode == GL_TRIANGLE_FAN) {
                                    // just 1 Quad
                                    for (int i=0; i<4; i++) {
                                        ind_line[k++] = i+0; ind_line[k++] = (i+1)%4;
                                    }
                                } else {
                                    // list of triangles, 2 per quads...
                                    for (int i=0; i<len-5; i+=6) {
                                        ind_line[k++] = i+0; ind_line[k++] = i+1;
                                        ind_line[k++] = i+1; ind_line[k++] = i+2;
                                        ind_line[k++] = i+2; ind_line[k++] = i+5;
                                        ind_line[k++] = i+5; ind_line[k++] = i+0;
                                    }
                                }
                            }
                            break;
                        case GL_QUAD_STRIP:
                            if(len>3) {
                                // first 4 points is a quad, then 2 points per new quad
                                if (len>2) {
                                    ind_line[k++] = 0; ind_line[k++] = 1;
                                }
                                for (int i = 2; i<len-1; i+=2) {
                                    ind_line[k++] = i-1; ind_line[k++] = i;
                                    ind_line[k++] = i-2; ind_line[k++] = i+1;
                                    ind_line[k++] = i+0; ind_line[k++] = i+1;
                                }
                            }
                            break;
                        case GL_POLYGON:
                            // if polygons have been merged, then info is lost...
                            if (len) {
                                ind_line[k++] = 0; ind_line[k++] = 1;
                                for (int i = 1; i<len; i++) {
                                    ind_line[k++] = i-1; ind_line[k++] = i;
                                }
                                ind_line[k++] = len-1; ind_line[k++] = 0;
                            }
                            break;
                    }
					gles_glDrawElements(mode, k, GL_UNSIGNED_SHORT, ind_line);
                } else {
                    gles_glDrawArrays(mode, 0, len);
                }
            }
        }
        #define TEXTURE(A) if (cur_tex!=A) {gl4es_glClientActiveTexture(A+GL_TEXTURE0); cur_tex=A;}
        for (int a=0; a<hardext.maxtex; a++) {
            if (needclean[a]) {
                TEXTURE(a);
                gen_tex_clean(needclean[a], a);
            }
            if (!IS_TEX2D(glstate->enable.texture[a]) && (IS_ANYTEX(glstate->enable.texture[a]))) {
                TEXTURE(a);
                gles_glDisable(GL_TEXTURE_2D);
            }
        }
        if (glstate->texture.client!=old_tex)
            TEXTURE(old_tex);
        #undef TEXTURE

		if (final_colors)
			free(final_colors);
        if (stipple) {
            gl4es_glPopAttrib();
        }
#endif
        if(list->post_color) gl4es_glColor4fv(list->post_colors);
        if(list->post_normal) gl4es_glNormal3fv(list->post_normals);
    } while ((list = list->next));
    gl4es_glPopClientAttrib();
}

// gl function wrappers

void FASTMATH rlVertex4f(renderlist_t *list, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    if (list->vert == NULL) {
        list->vert = alloc_sublist(4, list->cap);
    } else {
        resize_renderlist(list);
    }

    if (list->normal) {
        GLfloat * const normal = list->normal + (list->len * 3);
        memcpy(normal, list->lastNormal, sizeof(GLfloat) * 3);
    }

    if (list->color) {
        GLfloat * const color = list->color + (list->len * 4);
        memcpy(color, glstate->color, sizeof(GLfloat) * 4);
    }

    if (list->secondary) {
        GLfloat * const secondary = list->secondary + (list->len * 4);
        memcpy(secondary, glstate->secondary, sizeof(GLfloat) * 4);
    }

    for (int a=0; a<list->maxtex; a++) {
		if (list->tex[a]) {
			GLfloat * const tex = list->tex[a] + (list->len * 4);
			memcpy(tex, glstate->texcoord[a], sizeof(GLfloat) * 4);
		}
    }

    GLfloat * const vert = list->vert + (list->len++ * 4);
    vert[0] = x; vert[1] = y; vert[2] = z; vert[3] = w;
}

void rlNormal3f(renderlist_t *list, GLfloat x, GLfloat y, GLfloat z) {
    if (list->normal == NULL) {
        list->normal = alloc_sublist(3, list->cap);
        // catch up
        int i;
        if (list->len) for (i = 0; i < list->len; i++) {
            GLfloat *normal = (list->normal + (i * 3));
            memcpy(normal, list->lastNormal, sizeof(GLfloat) * 3);
        }
    }
    
    GLfloat *normal = list->lastNormal;
    normal[0] = x; normal[1] = y; normal[2] = z;
}

void rlColor4f(renderlist_t *list, GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    if (list->color == NULL) {
        list->lastColorsSet = 1;
        list->color = alloc_sublist(4, list->cap);
        // catch up
        int i;
        if (list->len) for (i = 0; i < list->len; i++) {
            GLfloat *color = (list->color + (i * 4));
            memcpy(color,list->lastColors, sizeof(GLfloat) * 4);
        }/* else {
            GLfloat *color = list->color;
            color[0] = r; color[1] = g; color[2] = b; color[3] = a;
        }*/
    }

    GLfloat *color = glstate->color;
    color[0] = r; color[1] = g; color[2] = b; color[3] = a;
    memcpy(list->lastColors, color, 4*sizeof(GLfloat));
}

void rlSecondary3f(renderlist_t *list, GLfloat r, GLfloat g, GLfloat b) {
    if (list->secondary == NULL) {
        list->secondary = alloc_sublist(4, list->cap);
        // catch up
        int i;
        if (list->len) for (i = 0; i < list->len; i++) {
            GLfloat *secondary = (list->secondary + (i * 4));
            memcpy(secondary, list->lastSecondaryColors, sizeof(GLfloat) * 4);
        }
    }

    GLfloat *color = glstate->secondary;
    color[0] = r; color[1] = g; color[2] = b; color[3] = 0.0f;
}

void rlMaterialfv(renderlist_t *list, GLenum face, GLenum pname, const GLfloat * params) {
    rendermaterial_t *m;
    khash_t(material) *map;
    khint_t k;
    int ret;
    if (! list->material) {
        list->material = map = kh_init(material);
        // segfaults if we don't do a single put
        kh_put(material, map, 1, &ret);
        kh_del(material, map, 1);
    } else {
        map = list->material;
    }

    // TODO: currently puts all faces in the same map
    k = kh_get(material, map, pname);
    if (k == kh_end(map)) {
        k = kh_put(material, map, pname, &ret);
        m = kh_value(map, k) = malloc(sizeof(rendermaterial_t));
    } else {
        m = kh_value(map, k);
    }

    m->face = face;
    m->pname = pname;
    m->color[0] = params[0];
    m->color[1] = params[1];
    m->color[2] = params[2];
    m->color[3] = params[3];
}

void rlLightfv(renderlist_t *list, GLenum which, GLenum pname, const GLfloat * params) {
    renderlight_t *m;
    khash_t(light) *map;
    khint_t k;
    int ret;
    if (! list->light) {
        list->light = map = kh_init(light);
        // segfaults if we don't do a single put
        kh_put(light, map, 1, &ret);
        kh_del(light, map, 1);
    } else {
        map = list->light;
    }

	int key = pname | ((which-GL_LIGHT0)<<16);
    k = kh_get(light, map, key);
    if (k == kh_end(map)) {
        k = kh_put(light, map, key, &ret);
        m = kh_value(map, k) = malloc(sizeof(renderlight_t));
    } else {
        m = kh_value(map, k);
    }

    m->which = which;
    m->pname = pname;
    m->color[0] = params[0];
    m->color[1] = params[1];
    m->color[2] = params[2];
    m->color[3] = params[3];
}

void rlTexGenfv(renderlist_t *list, GLenum coord, GLenum pname, const GLfloat * params) {
    rendertexgen_t *m;
    khash_t(texgen) *map;
    khint_t k;
    int ret;
    if (! list->texgen) {
        list->texgen = map = kh_init(texgen);
        // segfaults if we don't do a single put
        kh_put(texgen, map, 1, &ret);
        kh_del(texgen, map, 1);
    } else {
        map = list->texgen;
    }

	int key = pname | ((coord-GL_S)<<16);
    k = kh_get(texgen, map, key);
    if (k == kh_end(map)) {
        k = kh_put(texgen, map, key, &ret);
        m = kh_value(map, k) = malloc(sizeof(rendertexgen_t));
    } else {
        m = kh_value(map, k);
    }

    m->coord = coord;
    m->pname = pname;
    memcpy(m->color, params, 4*sizeof(GLfloat));
}

void rlMultiTexCoord4f(renderlist_t *list, GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q) {
    const int tmu = target - GL_TEXTURE0;
    if (list->tex[tmu] == NULL) {
        if (list->maxtex<tmu+1) list->maxtex = tmu+1;
        list->tex[tmu] = alloc_sublist(4, list->cap);
        // catch up
        GLfloat *tex = list->tex[tmu];
        if (list->len) for (int i = 0; i < list->len; i++) {
            memcpy(tex, glstate->texcoord[tmu], sizeof(GLfloat) * 4);
            tex += 4;
        }
    }
    GLfloat *tex = glstate->texcoord[tmu];
    tex[0] = s; tex[1] = t;
    tex[2] = r; tex[3] = q;
}

void rlActiveTexture(renderlist_t *list, GLenum texture ) {
    list->set_tmu = true;
    list->tmu = texture - GL_TEXTURE0;
    if(list->maxtex < list->tmu+1) list->maxtex = list->tmu+1;
}

void rlBindTexture(renderlist_t *list, GLenum target, GLuint texture) {
    list->texture = texture;
    list->target_texture = target;
    list->set_texture = true;
}

void rlRasterOp(renderlist_t *list, int op, GLfloat x, GLfloat y, GLfloat z) {
    list->raster_op = op;
    list->raster_xyz[0] = x;
    list->raster_xyz[1] = y;
    list->raster_xyz[2] = z;
}

void rlFogOp(renderlist_t *list, int op, const GLfloat* v) {
    int n = 1;
    if (op==GL_FOG_COLOR) n = 4;
    list->fog_op = op;
    list->fog_val[0] = v[0];
    if (n>1) list->fog_val[1] = v[1];
    if (n>2) list->fog_val[2] = v[2];
    if (n>3) list->fog_val[3] = v[3];
}

void rlPointParamOp(renderlist_t *list, int op, const GLfloat* v) {
    list->pointparam_op = op;
    list->pointparam_val[0] = v[0];
    list->pointparam_val[1] = v[1];
    list->pointparam_val[2] = v[2];
    list->pointparam_val[3] = v[3];
}

void rlPushCall(renderlist_t *list, packed_call_t *data) {
    call_list_t *cl = &list->calls;
    if (!cl->calls) {
        cl->cap = DEFAULT_CALL_LIST_CAPACITY;
        cl->calls = malloc(DEFAULT_CALL_LIST_CAPACITY * sizeof(void*));
    } else if (list->calls.len == list->calls.cap) {
        cl->cap += DEFAULT_CALL_LIST_CAPACITY;
        cl->calls = realloc(cl->calls, cl->cap * sizeof(void*));
    }
    cl->calls[cl->len++] = data;
}

renderlist_t* GetFirst(renderlist_t* list) {
    while(list->prev)
        list = list->prev;
    return list;
}

void rlTexEnvfv(renderlist_t *list, GLenum target, GLenum pname, const GLfloat * params) {
    int n = 1;
    switch(target) {
        case GL_POINT_SPRITE: n = 1; break;
        default:
            switch(pname) {
                case GL_TEXTURE_ENV_MODE: n=4; break;
                case GL_TEXTURE_ENV_COLOR: n=4; break;
                case GL_TEXTURE_LOD_BIAS: n=4; break;
            }
    }
    rendertexenv_t *m;
    khash_t(texenv) *map;
    khint_t k;
    int ret;
    if (! list->texenv) {
        list->texenv = map = kh_init(texenv);
        // segfaults if we don't do a single put
        kh_put(texenv, map, 1, &ret);
        kh_del(texenv, map, 1);
    } else {
        map = list->texenv;
    }

	int key = pname | ((target)<<16);
    k = kh_get(texenv, map, key);
    if (k == kh_end(map)) {
        k = kh_put(texenv, map, key, &ret);
        m = kh_value(map, k) = malloc(sizeof(rendertexenv_t));
    } else {
        m = kh_value(map, k);
    }

    m->target = target;
    m->pname = pname;
    memcpy(m->params, params, n*sizeof(GLfloat));
}
void rlTexEnviv(renderlist_t *list, GLenum target, GLenum pname, const GLint * params) {
    GLfloat fparams[4];
    int n = 1;
    switch(target) {
        case GL_POINT_SPRITE: n = 1; break;
        default:
            switch(pname) {
                case GL_TEXTURE_ENV_MODE: n=4; break;
                case GL_TEXTURE_ENV_COLOR: n=4; break;
                case GL_TEXTURE_LOD_BIAS: n=4; break;
            }
    }
    for (int i=0; i<n; i++) fparams[i] = params[i];
    rlTexEnvfv(list, target, pname, fparams);
}


#undef alloc_sublist
#undef realloc_sublist
