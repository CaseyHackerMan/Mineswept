#include "graphics.h"

Vector* add_vec(Vector* v1, Vector* v2, Vector* res) {
    res->x = v1->x + v2->x;
    res->y = v1->y + v2->y;
    return res;
}

Vector* sub_vec(Vector* v1, Vector* v2, Vector* res) {
    res->x = v1->x - v2->x;
    res->y = v1->y - v2->y;
    return res;
}

Vector* mul_vec(Vector* v, float a, Vector* res) {
    res->x = v->x*a;
    res->y = v->y*a;
    return res;
}

Vector* div_vec(Vector* v, float a, Vector* res) {
    res->x = v->x/a;
    res->y = v->y/a;
    return res;
}