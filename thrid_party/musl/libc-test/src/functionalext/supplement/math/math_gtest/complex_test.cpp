#include <gtest/gtest.h>

#include <math.h>
#include <fenv.h>
#include <float.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

#include "include/complex.h"

#define M_PI_2L 1.570796326794896619231321691639751442L

using namespace testing::ext;

class ComplexTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
* @tc.name: cabs_001
* @tc.desc: This test verifies whether the return value is 0 when both the real and imaginary parts of the passed
            parameters are 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cabs_001, TestSize.Level1)
{
    double complex a = 0 + 0 * I;
    EXPECT_EQ(0.0, cabs(a));
}

/**
* @tc.name: cabsf_001
* @tc.desc: This test verifies whether the return value is 0 when both the real and imaginary parts of the passed
            parameters are 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cabsf_001, TestSize.Level1)
{
    float complex a = 0 + 0 * I;
    EXPECT_EQ(0.0, cabsf(a));
}

/**
* @tc.name: cabsl_001
* @tc.desc: This test verifies whether the return value is 0 when both the real and imaginary parts of the passed
            parameters are 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cabsl_001, TestSize.Level1)
{
    long double complex a = 0 + 0 * I;
    EXPECT_EQ(0.0, cabsl(a));
}

/**
* @tc.name: cacos_001
* @tc.desc: Test whether the cacos result is M_PI_2 when the parameter is 0.0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cacos_001, TestSize.Level1)
{
    EXPECT_EQ(M_PI_2, cacos(0.0));
}

/**
* @tc.name: cacosf_001
* @tc.desc: Test whether the cacosf result is M_PI_2 when the parameter is 0.0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cacosf_001, TestSize.Level1)
{
    EXPECT_EQ(static_cast<float>(M_PI_2), cacosf(0.0));
}

/**
* @tc.name: cacosl_001
* @tc.desc: Test whether the cacosl result is M_PI_2L when the parameter is 0.0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cacosl_001, TestSize.Level1)
{
    EXPECT_EQ(M_PI_2L, cacosl(0.0));
}

/**
* @tc.name: cacosh_001
* @tc.desc: Test whether the cacosh result is 0.0 when the parameter is 1.0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cacosh_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, cacosh(1.0));
}

/**
* @tc.name: cacoshl_001
* @tc.desc: Test whether the cacosh result is 0.0 when the parameter is 1.0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cacoshl_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, cacoshl(1.0));
}

/**
* @tc.name: cacoshf_001
* @tc.desc: Test whether the cacosh result is 0.0 when the parameter is 1.0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cacoshf_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, cacoshf(1.0));
}

/**
* @tc.name: carg_001
* @tc.desc: Test whether the carg result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, carg_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, carg(0));
}

/**
* @tc.name: cargf_001
* @tc.desc: Test whether the cargf result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cargf_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, cargf(0));
}

/**
* @tc.name: cargl_001
* @tc.desc: Test whether the cargl result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cargl_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, cargl(0));
}

/**
* @tc.name: casin_001
* @tc.desc: Test whether the casin result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, casin_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, casin(0));
}

/**
* @tc.name: casinf_001
* @tc.desc: Test whether the casinf result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, casinf_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, casinf(0));
}

/**
* @tc.name: casinl_001
* @tc.desc: Test whether the casinl result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, casinl_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, casinl(0));
}

/**
* @tc.name: casinh_001
* @tc.desc: Test whether the casinh result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, casinh_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, casinh(0));
}

/**
* @tc.name: casinhf_001
* @tc.desc: Test whether the casinhf result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, casinhf_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, casinhf(0));
}

/**
* @tc.name: casinhl_001
* @tc.desc: Test whether the casin result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, casinhl_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, casinhl(0));
}

/**
* @tc.name: catan_001
* @tc.desc: Test whether the catan result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, catan_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, catan(0));
}

/**
* @tc.name: catanf_001
* @tc.desc: Test whether the catanf result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, catanf_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, catanf(0));
}

/**
* @tc.name: catanl_001
* @tc.desc: Test whether the catanl result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, catanl_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, catanl(0));
}

/**
* @tc.name: catanh_001
* @tc.desc: Test whether the catanh result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, catanh_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, catanh(0));
}

/**
* @tc.name: catanhf_001
* @tc.desc: Test whether the catanhf result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, catanhf_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, catanhf(0));
}

/**
* @tc.name: catanhl_001
* @tc.desc: Test whether the catanhl result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, catanhl_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, catanhl(0));
}

/**
* @tc.name: ccos_001
* @tc.desc: Test whether the ccos result is 1.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ccos_001, TestSize.Level1)
{
    EXPECT_EQ(1.0, ccos(0));
}

/**
* @tc.name: ccosf_001
* @tc.desc: Test whether the ccosf result is 1.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ccosf_001, TestSize.Level1)
{
    EXPECT_EQ(1.0, ccosf(0));
}

/**
* @tc.name: ccosl_001
* @tc.desc: Test whether the ccosl result is 1.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ccosl_001, TestSize.Level1)
{
    EXPECT_EQ(1.0, ccosl(0));
}

/**
* @tc.name: ccosh_001
* @tc.desc: Test whether the ccosh result is 1.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ccosh_001, TestSize.Level1)
{
    EXPECT_EQ(1.0, ccosh(0));
}

/**
* @tc.name: ccoshf_001
* @tc.desc: Test whether the ccoshf result is 1.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ccoshf_001, TestSize.Level1)
{
    EXPECT_EQ(1.0, ccoshf(0));
}

/**
* @tc.name: ccoshl_001
* @tc.desc: Test whether the ccoshl result is 1.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ccoshl_001, TestSize.Level1)
{
    EXPECT_EQ(1.0, ccoshl(0));
}

/**
* @tc.name: cexp_001
* @tc.desc: Test whether the cexp result is 1.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cexp_001, TestSize.Level1)
{
    EXPECT_EQ(1.0, cexp(0));
}

/**
* @tc.name: cexpf_001
* @tc.desc: Test whether the cexpf result is 1.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cexpf_001, TestSize.Level1)
{
    EXPECT_EQ(1.0, cexpf(0));
}

/**
* @tc.name: cexpl_001
* @tc.desc: Test whether the cexpl result is 1.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cexpl_001, TestSize.Level1)
{
    EXPECT_EQ(1.0, cexpl(0));
}

/**
* @tc.name: cimag_001
* @tc.desc: Test whether the cimag result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cimag_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, cimag(0));
}

/**
* @tc.name: cimagf_001
* @tc.desc: Test whether the cimagf result is 0.0f when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cimagf_001, TestSize.Level1)
{
    EXPECT_EQ(0.0f, cimagf(0));
}

/**
* @tc.name: cimagl_001
* @tc.desc: Test whether the cimagl result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cimagl_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, cimagl(0));
}

/**
* @tc.name: clog_001
* @tc.desc: Test whether the clog result is 0.0 when the parameter is 1.0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, clog_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, clog(1.0));
}

/**
* @tc.name: clogf_001
* @tc.desc: Test whether the clogf result is 0.0f when the parameter is 1.0f.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, clogf_001, TestSize.Level1)
{
    EXPECT_EQ(0.0f, clogf(1.0f));
}

/**
* @tc.name: clogl_001
* @tc.desc: Test whether the clogl result is 0.0L when the parameter is 1.0L.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, clogl_001, TestSize.Level1)
{
    EXPECT_EQ(0.0L, clogl(1.0L));
}

/**
* @tc.name: conj_001
* @tc.desc: Test whether the conj result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, conj_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, conj(0));
}

/**
* @tc.name: conjf_001
* @tc.desc: Test whether the conjf result is 0.0f when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, conjf_001, TestSize.Level1)
{
    EXPECT_EQ(0.0f, conjf(0));
}

/**
* @tc.name: conjl_001
* @tc.desc: Test whether the conjl result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, conjl_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, conjl(0));
}

/**
* @tc.name: cpowf_001
* @tc.desc: Test whether the cpowf result is 2.0f when the parameter is 3.0f.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cpowf_001, TestSize.Level1)
{
    EXPECT_EQ(8.0f, cpowf(2.0f, 3.0f));
}

/**
* @tc.name: cproj_001
* @tc.desc: Test whether the cproj result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cproj_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, cproj(0));
}

/**
* @tc.name: cprojf_001
* @tc.desc: Test whether the cprojf result is 0.0f when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cprojf_001, TestSize.Level1)
{
    EXPECT_EQ(0.0f, cprojf(0));
}

/**
* @tc.name: cprojl_001
* @tc.desc: Test whether the cprojl result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, cprojl_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, cprojl(0));
}

/**
* @tc.name: creal_001
* @tc.desc: Test whether the creal result is 2.0 when the parameter is 2.0 + 3.0I.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, creal_001, TestSize.Level1)
{
    EXPECT_EQ(2.0, creal(2.0 + 3.0I));
}

/**
* @tc.name: crealf_001
* @tc.desc: Test whether the crealf result is 2.0f when the parameter is 2.0 + 3.0fI.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, crealf_001, TestSize.Level1)
{
    EXPECT_EQ(2.0f, crealf(2.0f + 3.0fI));
}

/**
* @tc.name: creall_001
* @tc.desc: Test whether the creall result is 2.0L when the parameter is 2.0 + 3.0LI.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, creall_001, TestSize.Level1)
{
    EXPECT_EQ(2.0, creall(2.0L + 3.0LI));
}

/**
* @tc.name: csin_001
* @tc.desc: Test whether the csin result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, csin_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, csin(0));
}

/**
* @tc.name: csinf_001
* @tc.desc: Test whether the csinf result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, csinf_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, csinf(0));
}

/**
* @tc.name: csinl_001
* @tc.desc: Test whether the csinl result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, csinl_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, csinl(0));
}

/**
* @tc.name: csinh_001
* @tc.desc: Test whether the csinh result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, csinh_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, csinh(0));
}

/**
* @tc.name: csinhf_001
* @tc.desc: Test whether the csinhf result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, csinhf_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, csinhf(0));
}

/**
* @tc.name: csinhl_001
* @tc.desc: Test whether the csinhl result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, csinhl_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, csinhl(0));
}

/**
* @tc.name: csqrt_001
* @tc.desc: Test whether the csqrt result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, csqrt_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, csqrt(0));
}

/**
* @tc.name: csqrtf_001
* @tc.desc: Test whether the csqrtf result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, csqrtf_001, TestSize.Level1)
{
    EXPECT_EQ(0.0f, csqrtf(0));
}

/**
* @tc.name: csqrtl_001
* @tc.desc: Test whether the csqrtl result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, csqrtl_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, csqrtl(0));
}

/**
* @tc.name: ctan_001
* @tc.desc: Test whether the ctan result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ctan_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, ctan(0));
}

/**
* @tc.name: ctanf_001
* @tc.desc: Test whether the ctanf result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ctanf_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, ctanf(0));
}

/**
* @tc.name: ctanl_001
* @tc.desc: Test whether the ctanl result is 0.0 when the parameter is 0.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ctanl_001, TestSize.Level1)
{
    EXPECT_EQ(0.0, ctanl(0));
}

/**
* @tc.name: ctanh_001
* @tc.desc: When the input value is a valid value, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ctanh_001, TestSize.Level1)
{
    EXPECT_TRUE(ctanh(0) == 0.0);
}

/**
* @tc.name: ctanh_002
* @tc.desc: This test verifies whether the ctanh function returns the expected result when processing a specific type of
            input, with the real part being NaN and the imaginary part being zero.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ctanh_002, TestSize.Level1)
{
    double complex tan_result = ctanh(nan("") + 0i);
    EXPECT_TRUE(isnan(creal(tan_result)));
    EXPECT_TRUE(cimag(tan_result) == 0.0);
}

/**
* @tc.name: ctanh_003
* @tc.desc: This test verifies whether the ctanh function returns the expected result when processing a specific type of
            input, where both the real and imaginary parts are NaN.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ctanh_003, TestSize.Level1)
{
    double complex tan_result = ctanh(nan("") + 2.0i);
    EXPECT_TRUE(isnan(creal(tan_result)));
    EXPECT_TRUE(isnan(cimag(tan_result)));
}

/**
* @tc.name: ctanh_004
* @tc.desc: This test verifies whether the ctanh function returns the expected result when processing a specific type of
            input, where both the real and imaginary parts are NaN.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ctanh_004, TestSize.Level1)
{
    double complex tan_result = ctanh(nan("") + nan("") * I);
    EXPECT_TRUE(isnan(creal(tan_result)));
    EXPECT_TRUE(isnan(cimag(tan_result)));
}

/**
* @tc.name: ctanhf_001
* @tc.desc: When the input value is a valid value, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ctanhf_001, TestSize.Level1)
{
    EXPECT_TRUE(ctanhf(0.0f) == 0.0f);
}

/**
* @tc.name: ctanhf_002
* @tc.desc: This test verifies whether the ctanh function returns the expected result when processing a specific type of
            input, with the real part being NaN and the imaginary part being zero.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ctanhf_002, TestSize.Level1)
{
    float complex tan_result = ctanhf(nanf("") + 0.0fi);
    EXPECT_TRUE(isnan(crealf(tan_result)));
    EXPECT_TRUE(cimagf(tan_result) == 0.0f);
}

/**
* @tc.name: ctanhf_003
* @tc.desc: This test verifies whether the ctanh function returns the expected result when processing a specific type of
            input, where both the real and imaginary parts are NaN.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ctanhf_003, TestSize.Level1)
{
    float complex tan_result = ctanhf(nanf("") + nanf("") * I);
    EXPECT_TRUE(isnan(crealf(tan_result)));
    EXPECT_TRUE(isnan(cimagf(tan_result)));
}

/**
* @tc.name: ctanhf_004
* @tc.desc: This test verifies whether the ctanh function returns the expected result when processing a specific type of
            input, where both the real and imaginary parts are NaN.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ctanhf_004, TestSize.Level1)
{
    float complex tan_result = ctanhf(nan("") + nan("") * I);
    EXPECT_TRUE(isnan(creal(tan_result)));
    EXPECT_TRUE(isnan(cimag(tan_result)));
}

/**
* @tc.name: ctanhl_001
* @tc.desc: Test whether the ctanhl result is 0.0 when the parameter is 0,And test whether
*           the result is 0 when the virtual and real parts are not the same number.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ctanhl_001, TestSize.Level1)
{
    EXPECT_TRUE(ctanhl(0.0L) == 0.0L);
}

/**
* @tc.name: ctanhl_002
* @tc.desc: This test verifies whether the ctanh function returns the expected result when processing a specific type of
            input, with the real part being NaN and the imaginary part being zero.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ctanhl_002, TestSize.Level1)
{
    long double complex tan_result = ctanhl(nanl("") + 0.0Li);
    EXPECT_TRUE(isnan(creall(tan_result)));
}

/**
* @tc.name: ctanhl_003
* @tc.desc: This test verifies whether the ctanh function returns the expected result when processing a specific type of
            input, where both the real and imaginary parts are NaN.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ctanhl_003, TestSize.Level1)
{
    long double complex tan_result = ctanhl(nanl("") + 2.0Li);
    EXPECT_TRUE(isnan(creall(tan_result)));
    EXPECT_TRUE(isnan(cimagl(tan_result)));
}

/**
* @tc.name: ctanhl_004
* @tc.desc: This test verifies whether the ctanh function returns the expected result when processing a specific type of
            input, where both the real and imaginary parts are NaN.
* @tc.type: FUNC
*/
HWTEST_F(ComplexTest, ctanhl_004, TestSize.Level1)
{
    long double complex tan_result = ctanhl(nanl("") + nanl("") * I);
    EXPECT_TRUE(isnan(creall(tan_result)));
    EXPECT_TRUE(isnan(cimagl(tan_result)));
}