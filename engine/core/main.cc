#include "config.h"
#include <stdio.h>
#include <vector>
#include <string>


#include "util.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"

const char* programName = "Math library";
const char* s = "OK";
const char* f = "FAILED";

typedef unsigned TestId;
typedef unsigned Line;
typedef std::string Expression;
struct FailedTest {
    TestId id;
    Line line;
    Expression expr;
};
static TestId testId = 0;
static std::vector<FailedTest> failedTests;
#define VERIFY(RESULT) {  testId++; printf("#%0*u: %*s\n", 3, testId, 6, RESULT ? s : f); if (!(RESULT)) failedTests.push_back({testId, __LINE__, #RESULT}); }

int main()
{ 
    printf("\n\n--- %s test\n", programName);
    
    //------------------------------------------------------------------------
    const float E = 0.00001f;
    const vec3 E3(E, E, E);
    const vec4 E4(E, E, E, E);

    {
        printf("vec3:\n");

        // construction
        vec3 v0(1.0f, 2.0f, 3.0f);
        vec3 v1(4.0f, 3.0f, 2.0f);
        vec3 v2(v0);
        vec3 v3(v1);
        VERIFY(v0 == v2);
        VERIFY(v1 == v3);
        VERIFY(v0 != v1);
        VERIFY(v2 != v3);
        VERIFY(v0 == vec3(1.0f, 2.0f, 3.0f));

        // assignment
        v2 = v1;
        VERIFY(v2 == v1);
        v2 = v0;
        VERIFY(v2 == v0);

        // operators
        // vec4 operator-() const;
        v0 = -v0;
        VERIFY(v0 == vec3(-1.0f, -2.0f, -3.0f));
        v0 = -v0;
        VERIFY(v0 == v2);
        // void operator+=(const vec4& rhs);
        v2 += v3;
        VERIFY(v2 == vec3(5.0f, 5.0f, 5.0f));
        // void operator-=(const vec4& rhs);
        v2 -= v3;
        VERIFY(v2 == v0);
        // void operator*=(scalar s);
        v2 *= 2.0f;
        VERIFY(v2 == vec3(2.0f, 4.0f, 6.0f));
        // vec4 operator+(const vec4& rhs) const;
        v2 = v0 + v1;
        VERIFY(v2 == vec3(5.0f, 5.0f, 5.0f));
        // vec4 operator-(const vec4& rhs) const;
        v2 = v0 - v1;
        VERIFY(v2 == vec3(-3.0f, -1.0f, 1.0f));
        // vec4 operator*(scalar s) const;
        v2 = v0 * 2.0f;
        VERIFY(v2 == vec3(2.0f, 4.0f, 6.0f));

        // setting and getting content
        v2 = vec3(2.0f, 3.0f, 4.0f);
        VERIFY(v2.x == 2.0f);
        VERIFY(v2.y == 3.0f);
        VERIFY(v2.z == 4.0f);
        VERIFY(v2 == vec3(2.0f, 3.0f, 4.0f));
        v2.x = 1.0f;
        v2.y = 2.0f;
        v2.z = 3.0f;
        VERIFY(v2[0] == 1.0f);
        VERIFY(v2[1] == 2.0f);
        VERIFY(v2[2] == 3.0f);
        VERIFY(v2 == vec3(1.0f, 2.0f, 3.0f));
        v2.x = 5.0f;
        v2.y = 6.0f;
        v2.z = 7.0f;
        VERIFY(v2[0] == 5.0f);
        VERIFY(v2[1] == 6.0f);
        VERIFY(v2[2] == 7.0f);
        VERIFY(v2 == vec3(5.0f, 6.0f, 7.0f));

        // length
        v2 = vec3(1.0f, 2.0f, 3.0f);
        VERIFY(n_fequal(length(v2), 3.74165738f, 0.0001f));
        
        // cross
        v0 = vec3(1.0f, 0.0f, 0.0f);
        v1 = vec3(0.0f, 0.0f, 1.0f);
        v2 = cross(v0, v1);
        VERIFY(nearequal(v2, vec3(0.0f, -1.0f, 0.0f), E3));

        // dot
        v0 = vec3(1.0f, 0.0f, 0.0f);
        v1 = vec3(1.0f, 0.0f, 0.0f);    
        VERIFY(dot(v0, v1) == 1.0f);
        v1 = vec3(-1.0f, 0.0f, 0.0f);
        VERIFY(dot(v0, v1) == -1.0f);
        v1 = vec3(0.0f, 1.0f, 0.0f);
        VERIFY(dot(v0, v1) == 0.0f);

        // normalize
        v0 = vec3(2.5f, 0.0f, 0.0f);
        v1 = normalize(v0);
        VERIFY(v1 == vec3(1.0f, 0.0f, 0.0f));
    }

    //------------------------------------------------------------------------
    printf("vec4:\n");
    {
        // construction
        vec4 v0(1.0f, 2.0f, 3.0f, 4.0f);
        vec4 v1(4.0f, 3.0f, 2.0f, 1.0f);
        vec4 v2(v0);
        vec4 v3(v1);
        VERIFY(v0 == v2);
        VERIFY(v1 == v3);
        VERIFY(v0 != v1);
        VERIFY(v2 != v3);
        VERIFY(v0 == vec4(1.0f, 2.0f, 3.0f, 4.0));

        // assignment
        v2 = v1;
        VERIFY(v2 == v1);
        v2 = v0;
        VERIFY(v2 == v0);

        // operators
        // vec4 operator-() const;
        v0 = -v0;
        VERIFY(v0 == vec4(-1.0f, -2.0f, -3.0f, -4.0f));
        v0 = -v0;
        VERIFY(v0 == v2);
        // void operator+=(const vec4& rhs);
        v2 += v3;
        VERIFY(v2 == vec4(5.0f, 5.0f, 5.0f, 5.0f));
        // void operator-=(const vec4& rhs);
        v2 -= v3;
        VERIFY(v2 == v0);
        // void operator*=(scalar s);
        v2 *= 2.0f;
        VERIFY(v2 == vec4(2.0f, 4.0f, 6.0f, 8.0f));
        // vec4 operator+(const vec4& rhs) const;
        v2 = v0 + v1;
        VERIFY(v2 == vec4(5.0f, 5.0f, 5.0f, 5.0f));
        // vec4 operator-(const vec4& rhs) const;
        v2 = v0 - v1;
        VERIFY(v2 == vec4(-3.0f, -1.0f, 1.0f, 3.0f));
        // vec4 operator*(scalar s) const;
        v2 = v0 * 2.0f;
        VERIFY(v2 == vec4(2.0f, 4.0f, 6.0f, 8.0f));

        // setting and getting content
        v2 = vec4(2.0f, 3.0f, 4.0f, 5.0f);
        VERIFY(v2.x == 2.0f);
        VERIFY(v2.y == 3.0f);
        VERIFY(v2.z == 4.0f);
        VERIFY(v2.w == 5.0f);
        VERIFY(v2 == vec4(2.0f, 3.0f, 4.0f, 5.0f));
        v2.x = 1.0f;
        v2.y = 2.0f;
        v2.z = 3.0f;
        v2.w = 4.0f;
        VERIFY(v2[0] == 1.0f);
        VERIFY(v2[1] == 2.0f);
        VERIFY(v2[2] == 3.0f);
        VERIFY(v2[3] == 4.0f);
        VERIFY(v2 == vec4(1.0f, 2.0f, 3.0f, 4.0f));
        v2.x = 5.0f;
        v2.y = 6.0f;
        v2.z = 7.0f;
        v2.w = 8.0f;
        VERIFY(v2[0] == 5.0f);
        VERIFY(v2[1] == 6.0f);
        VERIFY(v2[2] == 7.0f);
        VERIFY(v2[3] == 8.0f);
        VERIFY(v2 == vec4(5.0f, 6.0f, 7.0f, 8.0f));
        
        // length
        v2 = vec4(1.0f, 2.0f, 3.0f, 5.0f);
        VERIFY(n_fequal(length(v2), 6.244997998f, 0.0001f));
        
        // dot
        v0 = vec4(1.0f, 0.0f, 0.0f, 0.0f);
        v1 = vec4(1.0f, 0.0f, 0.0f, 0.0f);    
        VERIFY(dot(v0, v1) == 1.0f);
        v1 = vec4(-1.0f, 0.0f, 0.0f, 0.0f);
        VERIFY(dot(v0, v1) == -1.0f);
        v1 = vec4(0.0f, 1.0f, 0.0f, 0.0f);
        VERIFY(dot(v0, v1) == 0.0f);

        // normalize
        v0 = vec4(2.5f, 0.0f, 0.0f, 0.0f);
        v1 = normalize(v0);
        VERIFY(v1 == vec4(1.0f, 0.0f, 0.0f, 0.0f));
    }
    //------------------------------------------------------------------------
    printf("mat4:\n");

    {

        const vec4 pOneTwoThree(1.0, 2.0, 3.0, 1.0);
        const vec3 vOneTwoThree(1.0, 2.0, 3.0);
        const mat4 trans123(vec4(1.0f, 0.0f, 0.0f, 0.0f),
                            vec4(0.0f, 1.0f, 0.0f, 0.0f),
                            vec4(0.0f, 0.0f, 1.0f, 0.0f),
                            pOneTwoThree);
        const mat4 identity;
        const mat4 rotOneX = rotationx(1.0f);
        const vec4 pZero(0.0, 0.0, 0.0, 1.0);
        const vec4 pOneX(1.0, 0.0, 0.0, 1.0);
        const vec4 pOneY(0.0, 1.0, 0.0, 1.0);
        vec4 result;

        // identity and construction
        mat4 m0(identity);
        VERIFY(m0 == mat4());
        VERIFY(m0 == identity);
        
        // multiply by identity
        m0 = trans123 * identity;
        VERIFY(m0 == trans123);

        // point transform by matrix
        result = trans123 * pZero;
        VERIFY((result == pOneTwoThree));

        // multiplication and multiplication order, transform point by matrix
        const mat4 mRotOneX_Trans123 = trans123 * rotOneX;
        VERIFY(matnearequal(mRotOneX_Trans123, // 73 Failed
                            mat4(vec4(1.0f,       0.0f,      0.0f, 0.0f),
                                 vec4(0.0f,  0.540302f, 0.841471f, 0.0f),
                                 vec4(0.0f, -0.841471f, 0.540302f, 0.0f),
                                 vec4(1.0f,       2.0f,      3.0f, 1.0f))));
        result = mRotOneX_Trans123 * pZero;
        VERIFY(nearequal(result, pOneTwoThree, E4));
        result = mRotOneX_Trans123 * pOneX;
        VERIFY(nearequal(result, vec4(2.0f, 2.0f, 3.0f, 1.0f), E4));
        result = mRotOneX_Trans123 * pOneY;
        VERIFY(nearequal(result, vec4(1.0f, 2.540302f, 3.841471f, 1.0f), E4));

        // inverse
        m0 = inverse(mRotOneX_Trans123);
        VERIFY(matnearequal(m0, mat4(vec4( 1.0f,       0.0f,       0.0f, 0.0f),
                                     vec4( 0.0f,  0.540302f, -0.841471f, 0.0f),
                                     vec4( 0.0f,  0.841471f,  0.540302f, 0.0f),
                                     vec4(-1.0f, -3.605018f,  0.062035f, 1.0f))));
        // transpose
        m0 = transpose(mRotOneX_Trans123);
        VERIFY(matnearequal(m0, mat4(vec4( 1.0f,       0.0f,       0.0f, 1.0f),
                                     vec4( 0.0f,  0.540302f, -0.841471f, 2.0f),
                                     vec4( 0.0f,  0.841471f,  0.540302f, 3.0f),
                                     vec4( 0.0f,       0.0f,       0.0f, 1.0f))));
        // rotations
        const mat4 rotX = rotationx(2.0f);
        VERIFY(matnearequal(rotX, mat4(vec4(1.000000f,  0.000000f,  0.000000f, 0.000000f),
                                       vec4(0.000000f, -0.416147f,  0.909297f, 0.000000f),
                                       vec4(0.000000f, -0.909297f, -0.416147f, 0.000000f),
                                       vec4(0.000000f,  0.000000f,  0.000000f, 1.000000f))));
        const mat4 rotY = rotationy(-1.7f);
        VERIFY(matnearequal(rotY, mat4(vec4(-0.128845f, 0.000000f,  0.991665f, 0.000000f),
                                       vec4( 0.000000f, 1.000000f,  0.000000f, 0.000000f),
                                       vec4(-0.991665f, 0.000000f, -0.128845f, 0.000000f),
                                       vec4( 0.000000f, 0.000000f,  0.000000f, 1.000000f))));
        const mat4 rotZ = rotationz(3.1f);
        VERIFY(matnearequal(rotZ, mat4(vec4(-0.999135f,  0.041581f, 0.000000f, 0.000000f),
                                       vec4(-0.041581f, -0.999135f, 0.000000f, 0.000000f),
                                       vec4( 0.000000f,  0.000000f, 1.000000f, 0.000000f),
                                       vec4( 0.000000f,  0.000000f, 0.000000f, 1.000000f))));
        const vec3 rotaxis = normalize(vec3(1.0f, 0.2f, 2.0f));
        const mat4 rot = rotationaxis(rotaxis, -2.53652f);
        VERIFY(matnearequal(rot, mat4(vec4(-0.460861f, -0.434427f,  0.773873f, 0.000000f),
                                      vec4( 0.579067f, -0.807997f, -0.108734f, 0.000000f),
                                      vec4( 0.672524f,  0.398013f,  0.623936f, 0.000000f),
                                      vec4( 0.000000f,  0.000000f,  0.000000f, 1.000000f))));
        
        // component-wise access
        mat4 m1;
        m0 = mat4(vec4(2.0f, 0.0f, 0.0f, 0.0f),
                  vec4(0.0f, 2.0f, 0.0f, 0.0f),
                  vec4(0.0f, 0.0f, 2.0f, 0.0f),
                  vec4(0.0f, 0.0f, 0.0f, 1.0f));
        vec4 value(2.0f, 0.0f, 0.0f, 0.0f);
        m1[0] = value;
        value = vec4(0.0f, 2.0f, 0.0f, 0.0f);
        m1[1] = value;
        value = vec4(0.0f, 0.0f, 2.0f, 0.0f);
        m1[2] = value;
        value = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        m1[3] = value;
        VERIFY(m0 == m1);
        VERIFY(m0[0] == vec4(2.0f, 0.0f, 0.0f, 0.0f));
        VERIFY(m0[1] == vec4(0.0f, 2.0f, 0.0f, 0.0f));
        VERIFY(m0[2] == vec4(0.0f, 0.0f, 2.0f, 0.0f));
        VERIFY(m0[3] == vec4(0.0f, 0.0f, 0.0f, 1.0f));

        // determinant
        const float det = determinant(mRotOneX_Trans123);
        VERIFY(n_fequal(1.0f, det, 0.0001f));

#ifdef TEST_VIEW_PERSPECTIVE
        const vec3 eye(3.0f, 2.0f, 10.0f);
        const vec3 at(3.0f, 2.0f, 2.0f);
        const vec3 up(0.0f, 1.0f, 0.0f);
#ifdef USE_LH
        // lookatlh
        mat4 tmp = lookat(eye, at, up);
        VERIFY(matnearequal(tmp, mat4(vec4(  -1.0f,  0.0f,  0.0f, 0.0f),
                                      vec4(  0.0f,  1.0f,  0.0f, 0.0f),
                                      vec4(  0.0f, 0.0f,  -1.0f, 0.0f),
                                      vec4(  3.0f, -2.0f, 10.0f, 1.0f))));
        // perspfovlh
        tmp = perspective(70.0f, 3.0f/4.0f, 0.1f, 50.0f);
        VERIFY(matnearequal(tmp, mat4(vec4( 2.814039f,     0.0f,      0.0f, 0.0f),
                                      vec4(      0.0f, 2.11053f,      0.0f, 0.0f),
                                      vec4(      0.0f,     0.0f, 1.004008f, 1.0f),
                                      vec4(      0.0f,     0.0f,  -0.2004008f, 0.0f))));        
#else
        // lookatrh
        mat4 tmp = lookat(eye, at, up);
        VERIFY(matnearequal(tmp, mat4(vec4(  1.0f,  0.0f,  0.0f, 0.0f),
                                      vec4(  0.0f,  1.0f,  0.0f, 0.0f),
                                      vec4(  0.0f,  0.0f,  1.0f, 0.0f),
                                      vec4(  -3.0f, -2.0f, -10.0f, 1.0f))));

        // perspfovrh
        tmp = perspective(70.0f, 3.0f/4.0f, 0.1f, 50.0f);
        VERIFY(matnearequal(tmp, mat4(vec4( 2.814039f,     0.0f,       0.0f,  0.0f),
                                      vec4(      0.0f, 2.11053f,       0.0f,  0.0f),
                                      vec4(      0.0f,     0.0f, -1.004008f, -1.0f),
                                      vec4(      0.0f,     0.0f,  -0.2004008f,   0.0f))));
#endif
#endif
    }

    //------------------------------------------------------------------------
    // Various tests between different types

	// transform (point and vector)
    mat4 m = mat4(vec4(1.0f, 0.0f, 0.0f, 0.0f),
                  vec4(0.0f, 1.0f, 0.0f, 0.0f),
                  vec4(0.0f, 0.0f, 1.0f, 0.0f),
                  vec4(1.0f, 2.0f, 3.0f, 1.0f));
    vec4 v0 = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    vec4 v1 = m * v0;
    VERIFY(v1 == vec4(2.0f, 2.0f, 3.0f, 1.0f));
    v0 = vec4(1.0f, 0.0f, 0.0f, 0.0f);
    v1 = m * v0;
    VERIFY(v0 == vec4(1.0f, 0.0f, 0.0f, 0.0f));
    const mat4 m0(vec4(1.0f, 0.0f, 0.0f, 0.0f),
                  vec4(0.0f, 1.0f, 0.0f, 0.0f),
                  vec4(0.0f, 0.0f, 1.0f, 0.0f),
                  vec4(1.0f, 2.0f, 3.0f, 1.0f));
	const vec4 in(0.0f, 0.0f, 0.0f, 1.0f);
	const vec4 out = m0 * in;
	VERIFY(nearequal(out, vec4(1.0f, 2.0f, 3.0f, 1.0f), E4));

    //------------------------------------------------------------------------
    printf("--- Done\n\n");
    if (failedTests.empty())
    {
        printf("--- %u/%u tests passed!\n\n", testId, testId);
        return 0;
    }
    for (auto t : failedTests)
        printf("Test #%u failed. Line %u, Expression: %s\n", t.id, t.line, t.expr.c_str());
    printf("--- %u/%u tests failed!\n\n", (unsigned)failedTests.size(), testId);
    return 1;
} 