// TODO: glIsEnabled(), glGetMap()
// TODO: GL_AUTO_NORMAL

#include "eval.h"
#include "math/eval.h"

static inline map_state_t **get_map_pointer(GLenum target) {
    switch (target) {
        case GL_MAP1_COLOR_4:         return &glstate->map1.color4;
        case GL_MAP1_INDEX:           return &glstate->map1.index;
        case GL_MAP1_TEXTURE_COORD_1: return &glstate->map1.texture1;
        case GL_MAP1_TEXTURE_COORD_2: return &glstate->map1.texture2;
        case GL_MAP1_TEXTURE_COORD_3: return &glstate->map1.texture3;
        case GL_MAP1_TEXTURE_COORD_4: return &glstate->map1.texture4;
        case GL_MAP1_VERTEX_3:        return &glstate->map1.vertex3;
        case GL_MAP1_VERTEX_4:        return &glstate->map1.vertex4;
        case GL_MAP2_COLOR_4:         return &glstate->map2.color4;
        case GL_MAP2_INDEX:           return &glstate->map2.index;
        case GL_MAP2_TEXTURE_COORD_1: return &glstate->map2.texture1;
        case GL_MAP2_TEXTURE_COORD_2: return &glstate->map2.texture2;
        case GL_MAP2_TEXTURE_COORD_3: return &glstate->map2.texture3;
        case GL_MAP2_TEXTURE_COORD_4: return &glstate->map2.texture4;
        case GL_MAP2_VERTEX_3:        return &glstate->map2.vertex3;
        case GL_MAP2_VERTEX_4:        return &glstate->map2.vertex4;
        default:
            printf("libGL: unknown glMap target 0x%x\n", target);
    }
    return NULL;
}

#define set_map_coords(n)         \
    map->n._1 = n##1;             \
    map->n._2 = n##2;             \
    map->n.d = 1.0/(n##2 - n##1); \
    map->n.stride = n##stride;    \
    map->n.order = n##order;

#define case_state(dims, magic, name)                           \
    case magic: {                                               \
        map->width = get_map_width(magic);                      \
        map_statef_t *m = (map_statef_t *)glstate->map##dims.name; \
        if (m) {                                                \
            if (m->free)                                        \
                free((void *)m->points);                        \
            free(m);                                            \
        }                                                       \
        glstate->map##dims.name = (map_state_t *)map;              \
        break;                                                  \
    }

#define map_switch(dims)                                                \
    switch (target) {                                                   \
        case_state(dims, GL_MAP##dims##_COLOR_4, color4);               \
        case_state(dims, GL_MAP##dims##_INDEX, index);                  \
        case_state(dims, GL_MAP##dims##_NORMAL, normal);                \
        case_state(dims, GL_MAP##dims##_TEXTURE_COORD_1, texture1);     \
        case_state(dims, GL_MAP##dims##_TEXTURE_COORD_2, texture2);     \
        case_state(dims, GL_MAP##dims##_TEXTURE_COORD_3, texture3);     \
        case_state(dims, GL_MAP##dims##_TEXTURE_COORD_4, texture4);     \
        case_state(dims, GL_MAP##dims##_VERTEX_3, vertex3);             \
        case_state(dims, GL_MAP##dims##_VERTEX_4, vertex4);             \
    }

void gl4es_glMap1d(GLenum target, GLdouble u1, GLdouble u2,
             GLint ustride, GLint uorder, const GLdouble *points) {
    noerrorShim();
    map_statef_t *map = malloc(sizeof(map_statef_t));
    map->type = GL_FLOAT; map->dims = 1; map->free = true;
    set_map_coords(u);
    map_switch(1);
    map->points = copy_eval_double(target, ustride, uorder, 0, 1, points);
}

void gl4es_glMap1f(GLenum target, GLfloat u1, GLfloat u2,
             GLint ustride, GLint uorder, const GLfloat *points) {
    noerrorShim();
    map_statef_t *map = malloc(sizeof(map_statef_t));
    map->type = GL_FLOAT; map->dims = 1; map->free = false;
    set_map_coords(u);
    map_switch(1);
    map->points = points;
}

void gl4es_glMap2d(GLenum target, GLdouble u1, GLdouble u2,
             GLint ustride, GLint uorder, GLdouble v1, GLdouble v2,
             GLint vstride, GLint vorder, const GLdouble *points) {
    noerrorShim();
    map_statef_t *map = malloc(sizeof(map_statef_t));
    map->type = GL_FLOAT; map->dims = 2; map->free = true;
    set_map_coords(u);
    set_map_coords(v);
    map_switch(2);
    map->points = copy_eval_double(target, ustride, uorder, vstride, vorder, points);
}

void gl4es_glMap2f(GLenum target, GLfloat u1, GLfloat u2,
             GLint ustride, GLint uorder, GLfloat v1, GLfloat v2,
             GLint vstride, GLint vorder, const GLfloat *points) {
    noerrorShim();
    map_statef_t *map = malloc(sizeof(map_statef_t));
    map->type = GL_FLOAT; map->dims = 2; map->free = false;
    set_map_coords(u);
    set_map_coords(v);
    map_switch(2);
    map->points = points;
}

#undef set_map_coords
#undef case_state
#undef map_switch

#define p_map(d, name, func, code) {                  \
    map_state_t *_map = glstate->map##d.name;          \
    if (_map) {                                       \
        if (_map->type == GL_DOUBLE) {                \
            map_stated_t *map = (map_stated_t *)_map; \
            printf("double: not implemented\n");      \
        } else if (_map->type == GL_FLOAT) {          \
            map_statef_t *map = (map_statef_t *)_map; \
            GLfloat out[4];                           \
            code                                      \
            gl4es_##func##v(out);                    \
        }                                             \
    }}

#define iter_maps(d, code)                  \
    p_map(d, color4, glColor4f, code);      \
    p_map(d, index, glIndexf, code);        \
    p_map(d, normal, glNormal3f, code);     \
    p_map(d, texture1, glTexCoord1f, code); \
    p_map(d, texture2, glTexCoord2f, code); \
    p_map(d, texture3, glTexCoord3f, code); \
    p_map(d, texture4, glTexCoord4f, code); \
    p_map(d, vertex3, glVertex3f, code);    \
    p_map(d, vertex4, glVertex4f, code);

void gl4es_glEvalCoord1f(GLfloat u) {
    noerrorShim();
    iter_maps(1,
        GLfloat uu = (u - map->u._1) * map->u.d;
        _math_horner_bezier_curve(map->points, out, uu, map->width, map->u.order);
    )
}

void gl4es_glEvalCoord2f(GLfloat u, GLfloat v) {
    noerrorShim();
    iter_maps(2,
        GLfloat uu = (u - map->u._1) * map->u.d;
        GLfloat vv = (v - map->v._1) * map->v.d;
        // TODO: GL_AUTONORMAL

        _math_horner_bezier_surf((GLfloat *)map->points, out, uu, vv,
                                 map->width, map->u.order, map->v.order);
    )
}

#undef p_map
#undef iter_maps

void gl4es_glMapGrid1f(GLint un, GLfloat u1, GLfloat u2) {
    noerrorShim();
    // TODO: double support?
    map_statef_t *map;
    if (! glstate->map_grid)
        glstate->map_grid = malloc(sizeof(map_statef_t));

    map = (map_statef_t *)glstate->map_grid;
    map->dims = 1;
    map->u.n = un;
    map->u._1 = u1;
    map->u._2 = u2;
}

void gl4es_glMapGrid2f(GLint un, GLfloat u1, GLfloat u2,
                 GLint vn, GLfloat v1, GLfloat v2) {
    noerrorShim();
    // TODO: double support?
    map_statef_t *map;
    if (! glstate->map_grid)
        glstate->map_grid = malloc(sizeof(map_statef_t));

    map = (map_statef_t *)glstate->map_grid;
    map->dims = 2;
    map->u.n = un;
    map->u._1 = u1;
    map->u._2 = u2;
    map->v.n = vn;
    map->v._1 = v1;
    map->v._2 = v2;
}

static inline GLenum eval_mesh_prep(map_statef_t **map, GLenum mode) {
    if (glstate->map2.vertex4) {
        *map = (map_statef_t *)glstate->map2.vertex4;
    } else if (glstate->map2.vertex3) {
        *map = (map_statef_t *)glstate->map2.vertex3;
    } else {
        return 0;
    }

    if ((*map)->type == GL_DOUBLE) {
        printf("libGL: GL_DOUBLE map not implemented\n");
        return 0;
    }

    switch (mode) {
        case GL_POINT: return GL_POINTS;
        case GL_LINE: return GL_LINE_STRIP;
        case GL_FILL: return GL_TRIANGLE_STRIP;
        case 0: return 1;
        default:
            printf("unknown glEvalMesh mode: %x\n", mode);
            return 0;
    }
}

void gl4es_glEvalMesh1(GLenum mode, GLint i1, GLint i2) {
    noerrorShim();
    map_statef_t *map;
    GLenum renderMode = eval_mesh_prep(&map, mode);
    if (! renderMode)
        return;

    GLfloat u, du, u1;
    du = map->u.d;
    GLint i;
    gl4es_glBegin(renderMode);
    for (u = u1, i = i1; i <= i2; i++, u += du) {
        gl4es_glEvalCoord1f(u);
    }
    gl4es_glEnd();
}

void gl4es_glEvalMesh2(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2) {
    noerrorShim();
    map_statef_t *map;
    GLenum renderMode = eval_mesh_prep(&map, mode);
    if (! renderMode)
        return;

    GLfloat u, du, u1, v, dv, v1;
    du = map->u.d;
    dv = map->v.d;
    GLint i, j;
    gl4es_glBegin(renderMode);
    for (v = v1, j = j1; j <= j2; j++, v += dv) {
        for (u = u1, i = i1; i <= i2; i++, u += du) {
            gl4es_glEvalCoord2f(u, v);
            if (mode == GL_FILL)
                gl4es_glEvalCoord2f(u, v + dv);
        }
    }
    gl4es_glEnd();
    if (mode == GL_LINE) {
        gl4es_glBegin(renderMode);
        for (u = u1, i = i1; i <= i2; i++, u += du) {
            for (v = v1, j = j1; j <= j2; j++, v += dv) {
                gl4es_glEvalCoord2f(u, v);
            }
        }
        gl4es_glEnd();
    }
}

void gl4es_glEvalPoint1(GLint i) {
    map_statef_t *map;
    if (eval_mesh_prep(&map, 0))
        gl4es_glEvalCoord1f(i + map->u.d);
}

void gl4es_glEvalPoint2(GLint i, GLint j) {
    map_statef_t *map;
    if (eval_mesh_prep(&map, 0))
        gl4es_glEvalCoord2f(i + map->u.d, j + map->v.d);
}

#define GL_GET_MAP(t, type)                                        \
void gl4es_glGetMap##t##v(GLenum target, GLenum query, type *v) { \
    noerrorShim();                                                 \
    map_statef_t *map = *(map_statef_t **)get_map_pointer(target); \
    if (map) {                                                     \
        switch (query) {                                           \
            case GL_COEFF: {                                       \
                const GLfloat *points = map->points;               \
                for (int i = 0; i < map->u.order; i++) {           \
                    if (map->dims == 2) {                          \
                        for (int j = 0; j < map->v.order; j++) {   \
                            *v++ = *points++;                      \
                        }                                          \
                    } else {                                       \
                        *v++ = *points++;                          \
                    }                                              \
                }                                                  \
                return;                                            \
            }                                                      \
            case GL_ORDER:                                         \
                *v++ = map->u.order;                               \
                if (map->dims == 2)                                \
                    *v++ = map->v.order;                           \
                return;                                            \
            case GL_DOMAIN:                                        \
                *v++ = map->u._1;                                  \
                *v++ = map->u._2;                                  \
                if (map->dims == 2) {                              \
                    *v++ = map->u._1;                              \
                    *v++ = map->u._2;                              \
                }                                                  \
                return;                                            \
        }                                                          \
    }                                                              \
}

GL_GET_MAP(i, GLint)
GL_GET_MAP(f, GLfloat)
GL_GET_MAP(d, GLdouble)

#undef GL_GET_MAP

//Direct wrapper
void glMap1d(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points) AliasExport("gl4es_glMap1d");
void glMap1f(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points) AliasExport("gl4es_glMap1f");
void glMap2d(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points) AliasExport("gl4es_glMap2d");
void glMap2f(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points) AliasExport("gl4es_glMap2f");
void glEvalCoord1f(GLfloat u) AliasExport("gl4es_glEvalCoord1f");
void glEvalCoord2f(GLfloat u, GLfloat v) AliasExport("gl4es_glEvalCoord2f");
void glEvalMesh1(GLenum mode, GLint i1, GLint i2) AliasExport("gl4es_glEvalMesh1");
void glEvalMesh2(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2) AliasExport("gl4es_glEvalMesh2");
void glEvalPoint1(GLint i) AliasExport("gl4es_glEvalPoint1");
void glEvalPoint2(GLint i, GLint j) AliasExport("gl4es_glEvalPoint2");
void glMapGrid1f(GLint un, GLfloat u1, GLfloat u2) AliasExport("gl4es_glMapGrid1f");
void glMapGrid2f(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2) AliasExport("gl4es_glMapGrid2f");
void glGetMapdv(GLenum target, GLenum query, GLdouble *v) AliasExport("gl4es_glGetMapdv");
void glGetMapfv(GLenum target, GLenum query, GLfloat *v) AliasExport("gl4es_glGetMapfv");
void glGetMapiv(GLenum target, GLenum query, GLint *v) AliasExport("gl4es_glGetMapiv");
