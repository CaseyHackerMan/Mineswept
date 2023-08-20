typedef struct vector {
    int x, y;
} Vector;

Vector* add_vec(Vector* v1, Vector* v2, Vector* res);
Vector* sub_vec(Vector* v1, Vector* v2, Vector* res);
Vector* mul_vec(Vector* v, float a, Vector* res);
Vector* div_vec(Vector* v, float a, Vector* res);