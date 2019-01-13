#ifndef HLSL_BRDF_DEFINES
#define HLSL_BRDF_DEFINES


//----------------------------------------------------------------------------------
// BRDF Type
//----------------------------------------------------------------------------------
#define BRDF_LAMBERT       0
#define BRDF_BLINN_PHONG   1
#define BRDF_COOK_TORRANCE 2

//----------------------------------------------------------------------------------
// Cook-Torrance Distribution Term
//----------------------------------------------------------------------------------
#define BRDF_D_BLINN_PHONG      0
#define BRDF_D_BECKMANN         1
#define BRDF_D_TROWBRIDGE_REITZ 2

//----------------------------------------------------------------------------------
// Cook-Torrance Fresnel Term
//----------------------------------------------------------------------------------
#define BRDF_F_NONE          0
#define BRDF_F_SCHLICK       1
#define BRDF_F_COOK_TORRANCE 2

//----------------------------------------------------------------------------------
// Cook-Torrance Visibility Term
//----------------------------------------------------------------------------------
#define BRDF_V_IMPLICIT      0
#define BRDF_V_NEUMANN       1
#define BRDF_V_COOK_TORRANCE 2
#define BRDF_V_KELEMEN       3
#define BRDF_V_SMITH_SCHLICK 4
#define BRDF_V_SMITH_GGX     5


#endif //HLSL_BRDF_DEFINES