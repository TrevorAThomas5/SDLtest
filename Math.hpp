#include <math.h>

/**
 * 2D data
 */
struct xy {
    float x, y;
};


/**
 * 3D data
 */
struct xyz {
    float x, y, z;
};


/**
 * a 2x2 matrix
 */
struct mat2x2 {
    float m11, m12;
    float m21, m22;
};

/**
 * multiplies a 2d vector by a 2x2 matrix
 */
static xy matrixMultiplication(mat2x2* mat, xy vec) {
    xy ret;
    ret.x = vec.x*mat->m11 + vec.y*mat->m12; 
    ret.y = vec.x*mat->m21 + vec.y*mat->m22; 
    return ret;
}


/**
 * updates the rotation matrix with a new angle theta
 */ 
static void updateRotationMatrix(mat2x2* rot, float theta) {
    rot->m11 = cosf(theta);
    rot->m12 = -sinf(theta);
    rot->m21 = sinf(theta);
    rot->m22 = cosf(theta);
}