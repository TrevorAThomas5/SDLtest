#include <math.h>
#include <string>

using namespace std;

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
 * 4D data
 */
struct xyzw {
    float x, y, z, w;
};

/**
 * a 2x2 matrix
 * 
 * WARNING: STARTED AT 1 INSTEAD OF 0 OOPS
 */
struct mat2x2 {
    float m11, m12;
    float m21, m22;
};

/**
 * a 4x4 matrix
 */
class mat4x4 {
  public:
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;

    mat4x4() {
        m00 = m01 = m02 = m03 = 0;
        m10 = m11 = m12 = m13 = 0;
        m20 = m21 = m22 = m23 = 0;
        m30 = m31 = m32 = m33 = 0;
    }
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
 * multiplies a 2d vector by a 4x4 matrix,
 * returns a 3d vector
 */
static xyz matrixMultiplication4D(mat4x4* mat, xyzw vec) {
    xyz ret;
    ret.x = vec.x*mat->m00 + vec.y*mat->m01 + vec.z*mat->m02 + vec.w*mat->m03;
    ret.y = vec.x*mat->m10 + vec.y*mat->m11 + vec.z*mat->m12 + vec.w*mat->m13;
    ret.z = vec.x*mat->m20 + vec.y*mat->m21 + vec.z*mat->m22 + vec.w*mat->m23;
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



/**
 * print xy
 */
static string printXY(xy& vec) {
    string str = "";
    str += to_string(vec.x);
    str += "\t";
    str += to_string(vec.y);
    return str;
}


/**
 * print xyz
 */
static string printXYZ(xyz& vec) {
    string str = "";
    str += to_string(vec.x);
    str += "\t";
    str += to_string(vec.y);
    str += "\t";
    str += to_string(vec.z);
    return str;
}


/**
 * print mat4x4
 */
static string printMat4x4(mat4x4& mat) {
    string str = "";

    str += to_string(mat.m00);
    str += "\t";
    str += to_string(mat.m01);
    str += "\t";
    str += to_string(mat.m02);
    str += "\t";
    str += to_string(mat.m03);
    str += "\n";

    str += to_string(mat.m10);
    str += "\t";
    str += to_string(mat.m11);
    str += "\t";
    str += to_string(mat.m12);
    str += "\t";
    str += to_string(mat.m13);
    str += "\n";

    str += to_string(mat.m20);
    str += "\t";
    str += to_string(mat.m21);
    str += "\t";
    str += to_string(mat.m22);
    str += "\t";
    str += to_string(mat.m23);
    str += "\n";

    str += to_string(mat.m30);
    str += "\t";
    str += to_string(mat.m31);
    str += "\t";
    str += to_string(mat.m32);
    str += "\t";
    str += to_string(mat.m33);
    str += "\n";

    return str;
}