// Basic 32-bit float type used by our ISA
typedef float float32;

// 4-component vector
typedef struct {
    float32 x, y, z, w;
} vec4;

// 4×4 transform matrix (row-major)
typedef struct {
    float32 m[4][4];
} mat4;

// Vertex shader entry point (equivalent to vs_main in the ISA doc)
void vertex_shader(vec4* input_vertex, mat4* transform_matrix, vec4* output_vertex) {

    // Load vertex components
    float32 x = input_vertex->x;
    float32 y = input_vertex->y;
    float32 z = input_vertex->z;
    float32 w = input_vertex->w;

    // Matrix × vector multiply
    output_vertex->x =
        transform_matrix->m[0][0] * x +
        transform_matrix->m[0][1] * y +
        transform_matrix->m[0][2] * z +
        transform_matrix->m[0][3] * w;

    output_vertex->y =
        transform_matrix->m[1][0] * x +
        transform_matrix->m[1][1] * y +
        transform_matrix->m[1][2] * z +
        transform_matrix->m[1][3] * w;

    output_vertex->z =
        transform_matrix->m[2][0] * x +
        transform_matrix->m[2][1] * y +
        transform_matrix->m[2][2] * z +
        transform_matrix->m[2][3] * w;

    output_vertex->w =
        transform_matrix->m[3][0] * x +
        transform_matrix->m[3][1] * y +
        transform_matrix->m[3][2] * z +
        transform_matrix->m[3][3] * w;
}

// Simple main for CPU-side compilation
int main() {
    vec4 in_v = {1.0f, 0.5f, 0.5f, 1.0f};
    vec4 out_v;

    // Identity transform
    mat4 mat = {{
        {1,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {1,0,0,0}
    }};

    vertex_shader(&in_v, &mat, &out_v);
    return 0;
}
